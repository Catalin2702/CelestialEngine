//
// Created by Catalin Chirosca on 2026-02-24.
//

#include "Layers/ImGuiMetalLayer.hpp"

#include <imgui_impl_metal.h>


namespace CE::Layers {

ImGuiMetalLayer::ImGuiMetalLayer(): Layer("ImGuiMetalLayer") {}

void ImGuiMetalLayer::OnAttach() {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

void ImGuiMetalLayer::OnDetach() {
}

void ImGuiMetalLayer::OnUpdate() {
}

void ImGuiMetalLayer::OnEvent(Events::Event&) {
}

}
