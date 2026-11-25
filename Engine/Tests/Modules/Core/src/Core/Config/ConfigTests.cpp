//
// Module: CelestialEngine/Engine/Tests/Modules/Core/Config
// File: ConfigTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

#include <Core/Config/Config.hpp>

#include <Types/Render/Render.hpp>
#include <Types/Window/WindowProps.hpp>

#include <gtest/gtest.h>

#include <array>
#include <atomic>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

using CE::Core::Config;
using CE::Types::GraphicsApi;
using CE::Types::WindowApi;
using CE::Types::WindowProps;

namespace {

/**
 * @brief Test fixture for Config tests
 * @details Config is process-wide state, so each test starts by writing a known configuration.
 */
class ConfigTest: public ::testing::Test {
protected:
	void SetUp() override {
		Config::SetWindowProps(WindowProps{"ConfigTest", 800, 600, false, 0, GraphicsApi::OpenGL, WindowApi::GLFW});
	}
};

}

/**
 * @brief Test that the window properties written are the ones read back
 */
TEST_F(ConfigTest, SetWindowProps_StoresProperties) {
	const auto props = Config::GetWindowProps();

	EXPECT_EQ(props.title, "ConfigTest");
	EXPECT_EQ(props.width, 800);
	EXPECT_EQ(props.height, 600);
	EXPECT_FALSE(props.VSync);
	EXPECT_EQ(props.refreshRate, 0);
	EXPECT_EQ(props.graphicsApi, GraphicsApi::OpenGL);
	EXPECT_EQ(props.windowApi, WindowApi::GLFW);
}

/**
 * @brief Test that a second write replaces the stored properties
 */
TEST_F(ConfigTest, SetWindowProps_OverwritesPreviousProperties) {
	Config::SetWindowProps(WindowProps{"Replaced", 1920, 1080, true, 144, GraphicsApi::Metal, WindowApi::Cocoa});

	const auto props = Config::GetWindowProps();

	EXPECT_EQ(props.title, "Replaced");
	EXPECT_EQ(props.width, 1920);
	EXPECT_EQ(props.height, 1080);
	EXPECT_TRUE(props.VSync);
	EXPECT_EQ(props.refreshRate, 144);
	EXPECT_EQ(props.graphicsApi, GraphicsApi::Metal);
	EXPECT_EQ(props.windowApi, WindowApi::Cocoa);
}

/**
 * @brief Test that GetWindowProps hands out a copy, never a view of the stored properties
 * @details A reference would outlive the lock that guarded it. The copy also means a later write does not change what
 *			was already read.
 */
TEST_F(ConfigTest, GetWindowProps_ReturnsACopy) {
	static_assert(std::is_same_v<decltype(Config::GetWindowProps()), WindowProps>,
		"GetWindowProps must return by value");

	const auto before = Config::GetWindowProps();
	Config::SetWindowProps(WindowProps{"After", 1, 1, true, 0, GraphicsApi::Metal, WindowApi::Cocoa});

	EXPECT_EQ(before.title, "ConfigTest");
	EXPECT_EQ(before.width, 800);
}

/**
 * @brief Test that UpdateWindowProps changes only what the function changes
 */
TEST_F(ConfigTest, UpdateWindowProps_ChangesInPlace) {
	Config::UpdateWindowProps([](WindowProps& props) { props.VSync = not props.VSync; });

	const auto props = Config::GetWindowProps();
	EXPECT_TRUE(props.VSync);
	EXPECT_EQ(props.title, "ConfigTest");
	EXPECT_EQ(props.width, 800);
}

/**
 * @brief Test that UpdateWindowProps works with a callable passed as an lvalue and one that returns a value
 */
TEST_F(ConfigTest, UpdateWindowProps_AcceptsLvalueCallables) {
	unsigned int seenWidth = 0;
	auto widen = [&seenWidth](WindowProps& props) {
		seenWidth = props.width;
		props.width *= 2;
		return props.width;
	};

	Config::UpdateWindowProps(widen);
	Config::UpdateWindowProps(widen);

	EXPECT_EQ(seenWidth, 1600);
	EXPECT_EQ(Config::GetWindowProps().width, 3200);
}

/**
 * @brief Test that UpdateWindowProps takes a const callable without casting its constness away
 */
TEST_F(ConfigTest, UpdateWindowProps_AcceptsConstCallables) {
	const auto setHeight = [](WindowProps& props) { props.height = 42; };

	Config::UpdateWindowProps(setHeight);

	EXPECT_EQ(Config::GetWindowProps().height, 42);
}

/**
 * @brief Test that an rvalue callable is invoked as an rvalue (it is forwarded, not called as an lvalue)
 */
TEST_F(ConfigTest, UpdateWindowProps_ForwardsRvalueCallables) {
	struct RvalueOnly {
		void operator()(WindowProps& props) && { props.title = "Called as an rvalue"; }
		void operator()(WindowProps&) & = delete;
	};

	Config::UpdateWindowProps(RvalueOnly{});

	EXPECT_EQ(Config::GetWindowProps().title, "Called as an rvalue");
}

/**
 * @brief Test that concurrent read-modify-writes through UpdateWindowProps lose no update
 * @details Each thread increments the width many times. Done as GetWindowProps + SetWindowProps, two threads could
 *			read the same value and both write value + 1; under UpdateWindowProps' single lock every increment lands.
 */
TEST_F(ConfigTest, UpdateWindowProps_ConcurrentIncrements_NoneLost) {
	Config::UpdateWindowProps([](WindowProps& props) { props.width = 0; });

	constexpr int threadCount = 8;
	constexpr int incrementsPerThread = 2'000;
	{
		std::vector<std::jthread> threads;
		threads.reserve(threadCount);
		for (int t = 0; t < threadCount; ++t) {
			threads.emplace_back([] {
				for (int i = 0; i < incrementsPerThread; ++i)
					Config::UpdateWindowProps([](WindowProps& props) { ++props.width; });
			});
		}
	}

	EXPECT_EQ(Config::GetWindowProps().width, static_cast<unsigned int>(threadCount * incrementsPerThread));
}

/**
 * @brief Test that readers running beside a writer always see a whole configuration, never half of one
 * @details The writer alternates between two configurations whose fields all agree with each other (the title encodes
 *			the width). A reader that copied while a write was halfway through would see a title from one and a width
 *			from the other. Run under ThreadSanitizer, this is also the test that reports a missing lock.
 */
TEST_F(ConfigTest, GetWindowProps_ConcurrentWithWrites_SeesConsistentProps) {
	const std::array configurations{
		// Titles longer than the small-string buffer, so a torn read of the string itself would be caught too.
		WindowProps{"Configuration with width 640, long enough to live on the heap", 640, 480, false, 0, GraphicsApi::OpenGL, WindowApi::GLFW},
		WindowProps{"Configuration with width 1920, long enough to live on the heap", 1920, 1080, true, 0, GraphicsApi::Metal, WindowApi::Cocoa},
	};
	Config::SetWindowProps(configurations[0]);

	std::atomic<bool> stop{false};
	std::atomic<int> torn{0};
	{
		constexpr int readerCount = 4;
		std::vector<std::jthread> readers;
		readers.reserve(readerCount);
		for (int r = 0; r < readerCount; ++r) {
			readers.emplace_back([&] {
				while (not stop.load(std::memory_order_relaxed)) {
					const auto props = Config::GetWindowProps();
					const auto& expected = props.width == 640 ? configurations[0] : configurations[1];
					if (props.title != expected.title or props.height != expected.height or props.VSync != expected.VSync)
						torn.fetch_add(1, std::memory_order_relaxed);
				}
			});
		}

		for (int i = 0; i < 5'000; ++i)
			Config::SetWindowProps(configurations[i % 2]);
		stop.store(true, std::memory_order_relaxed);
	}

	EXPECT_EQ(torn.load(), 0);
}
