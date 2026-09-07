//
// Module: CelestialEngine/Engine/Modules/Core/Render/Camera/Controller
// File: CameraControllerDescriptor.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-07
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#pragma once

#ifndef CE_CORE_RENDER_CAMERA_CONTROLLER_CAMERACONTROLLERDESCRIPTOR_HPP
#define CE_CORE_RENDER_CAMERA_CONTROLLER_CAMERACONTROLLERDESCRIPTOR_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <glm/glm.hpp>

#include <type_traits>
#include <variant>


namespace CE::Core {

/**
 * @struct CameraLookSettings
 * @brief Shared mouse-look tuning, for every controller that turns the camera with the pointer
 * @details Sensitivity is expressed in degrees per pixel of mouse travel rather than as an opaque multiplier, so the
 *			same value gives the same rotation on any window size and any DPI - the controller must divide the raw
 *			delta by the framebuffer scale before applying it.
 *
 *			The pitch limits stop just short of the poles on purpose: at exactly +/-90 degrees the forward vector
 *			becomes parallel to the world up, LookAt loses the basis it builds the right vector from, and the view
 *			matrix degenerates.
 */
struct CE_CORE_API CameraLookSettings {
	/// Degrees of rotation per pixel of mouse travel.
	f32 sensitivity = 0.1_f32;

	/// Flips the vertical axis, for users who fly rather than aim.
	bool invertY = false;

	f32 minPitch = -89.0_f32;
	f32 maxPitch = 89.0_f32;
};

/**
 * @struct CameraMoveSettings
 * @brief Shared translation tuning, for every controller that walks or flies the camera
 * @details The speed is in world units per second and must be scaled by the frame delta, never applied per frame, or
 *			the camera moves faster on a faster machine.
 */
struct CE_CORE_API CameraMoveSettings {
	/// World units per second.
	f32 speed = 5.0_f32;

	/// Multiplier applied while the sprint modifier is held.
	f32 sprintMultiplier = 3.0_f32;

	/// Seconds the camera takes to reach the target velocity; 0 starts and stops instantly.
	f32 acceleration = 0.0_f32;
};

/**
 * @struct CameraBounds
 * @brief Optional world-space box the camera position is clamped into
 * @details Disabled by default because an unbounded camera is what an editor viewport wants; gameplay controllers
 *			(top-down, side-scroller) switch it on to keep the view inside the level.
 */
struct CE_CORE_API CameraBounds {
	bool enabled = false;
	glm::vec3 min{-100.0_f32};
	glm::vec3 max{100.0_f32};
};

/**
 * @struct CameraNoneControllerDescriptor
 * @brief Sentinel matching Types::CameraController::None - the camera has no controller attached
 * @details Also the first alternative of the variant below, so a default-constructed descriptor means "nothing drives
 *			this camera" rather than an arbitrary behaviour.
 */
struct CE_CORE_API CameraNoneControllerDescriptor {
	static constexpr auto TYPE = Types::CameraController::None;
};

/**
 * @struct CameraStaticControllerDescriptor
 * @brief A camera that never moves on its own
 * @details Deliberately empty: the pose comes from whoever placed the camera, and the controller only exists so the
 *			viewport always has one to talk to instead of special-casing a null pointer. Consumes no input, which is
 *			what makes it the right choice for a fixed cutscene angle or a security-camera view.
 */
struct CE_CORE_API CameraStaticControllerDescriptor {
	static constexpr auto TYPE = Types::CameraController::Static;
};

/**
 * @struct CameraFreeControllerDescriptor
 * @brief Unconstrained fly camera - the editor and debug default
 * @details Translates on all three axes in its own local basis and looks anywhere, ignoring gravity and collision.
 *			Holding lookButton to rotate is what lets the same pointer still click on the scene; a game that owns the
 *			whole window sets requireLookButton to false and rotates on every mouse motion.
 */
struct CE_CORE_API CameraFreeControllerDescriptor {
	static constexpr auto TYPE = Types::CameraController::Free;

	CameraMoveSettings move;
	CameraLookSettings look;

	/// Held to rotate the camera; ignored when requireLookButton is false.
	Types::MouseButtonCode lookButton = Types::MouseButtonCode::Right;
	bool requireLookButton = true;

	/// Multiplier applied to move.speed per wheel notch, so the wheel retunes the fly speed instead of dollying.
	f32 speedScrollStep = 1.1_f32;
	f32 minSpeed = 0.1_f32;
	f32 maxSpeed = 500.0_f32;
};

/**
 * @struct CameraOrbitControllerDescriptor
 * @brief Turntable camera that revolves around a pivot at a fixed distance
 * @details The asset-viewer and DCC camera: orbit rotates around the pivot, pan slides the pivot in the camera plane,
 *			zoom changes the distance.
 *
 *			Both the pan speed and the zoom step must be scaled by the current distance, otherwise the camera crawls
 *			when close to the pivot and jumps across the scene when far from it. The zoom is multiplicative for the
 *			same reason - a fixed additive step overshoots through the pivot as soon as the distance gets small.
 */
struct CE_CORE_API CameraOrbitControllerDescriptor {
	static constexpr auto TYPE = Types::CameraController::Orbit;

	CameraLookSettings look;

	/// World-space point the camera revolves around; pan moves it, zoom leaves it alone.
	glm::vec3 pivot{0.0_f32};

	f32 distance = 10.0_f32;
	f32 minDistance = 0.5_f32;
	f32 maxDistance = 500.0_f32;

	/// Multiplier applied to the distance per wheel notch.
	f32 zoomStep = 1.1_f32;

	/// Scales the pan translation, on top of the mandatory scaling by the current distance.
	f32 panSensitivity = 1.0_f32;

	Types::MouseButtonCode orbitButton = Types::MouseButtonCode::Left;
	Types::MouseButtonCode panButton = Types::MouseButtonCode::Middle;

	/// Requires the Alt modifier for orbit and pan, as Blender, Maya and Unity all do, leaving the bare buttons free
	/// for selection and gizmo dragging.
	bool requireModifier = true;
};

/**
 * @struct CameraFirstPersonControllerDescriptor
 * @brief Eye-level camera driven like a character
 * @details Differs from the free camera in one decision: with constrainToGroundPlane the forward direction used for
 *			movement is the look direction flattened onto the horizontal plane, so looking at the floor and walking
 *			forward walks forward instead of burrowing. That single flag is what separates walking from flying.
 */
struct CE_CORE_API CameraFirstPersonControllerDescriptor {
	static constexpr auto TYPE = Types::CameraController::FirstPerson;

	CameraMoveSettings move;
	CameraLookSettings look;

	/// Vertical offset from the character's feet to the eyes, in world units.
	f32 eyeHeight = 1.7_f32;

	/// Projects the movement basis onto the horizontal plane: walking, not flying.
	bool constrainToGroundPlane = true;

	/// Hides the pointer and keeps it centred so the mouse can turn without ever hitting the window edge.
	bool captureCursor = true;
};

/**
 * @struct CameraThirdPersonControllerDescriptor
 * @brief Camera trailing a target on a spring arm
 * @details The arm is a rigid rod of armLength from the target, rotated by the mouse; damping is what makes it lag
 *			behind the target instead of being welded to it.
 *
 *			Collision is not optional in practice: without the sweep the arm passes through walls and the camera ends
 *			up inside geometry every time the target backs into a corner. The sweep uses collisionRadius so the near
 *			plane clears the surface it pulls in against, rather than clipping through it.
 */
struct CE_CORE_API CameraThirdPersonControllerDescriptor {
	static constexpr auto TYPE = Types::CameraController::ThirdPerson;

	CameraLookSettings look;

	/// Offset from the followed target's origin to the arm's anchor - normally head or shoulder height.
	glm::vec3 targetOffset{0.0_f32, 1.5_f32, 0.0_f32};

	f32 armLength = 4.0_f32;
	f32 minArmLength = 1.0_f32;
	f32 maxArmLength = 12.0_f32;
	f32 zoomStep = 1.1_f32;

	/// Higher values stiffen the spring; the controller converges with 1 - exp(-damping * deltaTime), which stays
	/// frame-rate independent where a bare lerp factor would not.
	f32 positionDamping = 10.0_f32;
	f32 rotationDamping = 15.0_f32;

	bool collisionEnabled = true;
	f32 collisionRadius = 0.25_f32;
};

/**
 * @struct CameraTopDownControllerDescriptor
 * @brief RTS-style camera looking down at a fixed tilt
 * @details Height and pitch are decoupled from each other: zooming changes how high the camera sits without changing
 *			the angle it looks at the ground with, which is what keeps an RTS view readable while zooming. Rotation is
 *			off by default because a fixed compass orientation is what makes a strategy map legible.
 */
struct CE_CORE_API CameraTopDownControllerDescriptor {
	static constexpr auto TYPE = Types::CameraController::TopDown;

	CameraMoveSettings move;
	CameraBounds bounds;

	f32 height = 20.0_f32;
	f32 minHeight = 5.0_f32;
	f32 maxHeight = 80.0_f32;
	f32 zoomStep = 1.1_f32;

	/// Fixed downward tilt, in degrees; -90 looks straight down.
	f32 pitch = -60.0_f32;
	f32 yaw = 0.0_f32;

	bool rotationEnabled = false;
	/// Degrees per second while the rotation input is held.
	f32 rotationSpeed = 90.0_f32;

	/// Pans when the pointer rests within edgeScrollMargin pixels of a window border.
	bool edgeScrollEnabled = true;
	f32 edgeScrollMargin = 16.0_f32;
};

/**
 * @struct CameraSideScrollerControllerDescriptor
 * @brief 2D follow camera confined to a plane
 * @details Built around the dead zone: the camera does not move at all while the target stays inside that box, and
 *			only then starts following. Following the target exactly makes a platformer nauseating, because every jump
 *			and every small correction shakes the whole screen.
 *
 *			Look-ahead pushes the framing in the direction of travel, so the player sees where they are going rather
 *			than where they have been.
 */
struct CE_CORE_API CameraSideScrollerControllerDescriptor {
	static constexpr auto TYPE = Types::CameraController::SideScroller;

	CameraBounds bounds;

	/// Constant framing offset from the target, in world units.
	glm::vec2 targetOffset{0.0_f32};

	/// Half-extents of the box the target may move in before the camera reacts.
	glm::vec2 deadZone{2.0_f32, 1.0_f32};

	f32 followDamping = 5.0_f32;

	/// Leads the target by this many world units in its direction of travel.
	f32 lookAheadDistance = 2.0_f32;
	f32 lookAheadDamping = 2.0_f32;

	/// Pins the camera to a constant height, the usual choice when the level scrolls horizontally only.
	bool lockVertical = false;

	/// Distance from the 2D plane the camera sits at; only the sign matters under an orthographic projection.
	f32 planeDistance = 10.0_f32;
};

/**
 * @struct CameraCinematicControllerDescriptor
 * @brief Camera playing back an authored path
 * @details Holds the playback state only. The path itself - the spline, or the list of keyframed poses - belongs in a
 *			resource this descriptor will reference once that type exists; keeping a handle out of it for now means the
 *			descriptor stays a trivially copyable POD that Config can deserialise.
 *
 *			With lookAtEnabled the camera keeps aiming at a fixed point while it travels, instead of orienting along
 *			the path tangent: the difference between circling a subject and riding a rollercoaster.
 */
struct CE_CORE_API CameraCinematicControllerDescriptor {
	static constexpr auto TYPE = Types::CameraController::Cinematic;

	/// Seconds the full path takes at a playback speed of 1.
	f32 duration = 5.0_f32;
	f32 playbackSpeed = 1.0_f32;

	/// Normalised position along the path to start from, in [0, 1].
	f32 startOffset = 0.0_f32;

	bool autoPlay = true;
	bool loop = false;

	/// Eases the first and last 10% of the path instead of starting and stopping abruptly.
	bool easeInOut = true;

	bool lookAtEnabled = false;
	glm::vec3 lookAtTarget{0.0_f32};
};

/**
 * @struct CameraScriptedControllerDescriptor
 * @brief Camera driven entirely from outside the engine's own input handling
 * @details The escape hatch: gameplay code, a tool, or a test writes the pose straight onto the camera every frame and
 *			this controller does nothing of its own. It exists so such a camera still has a controller in the slot, and
 *			so the viewport knows not to fight it for the input.
 *
 *			consumeEvents lets the script swallow the events it handles, keeping them away from whatever sits behind
 *			the camera in the layer stack.
 */
struct CE_CORE_API CameraScriptedControllerDescriptor {
	static constexpr auto TYPE = Types::CameraController::Scripted;

	bool consumeEvents = false;
};

/**
 * @struct CameraControllerDescriptor
 * @brief Tagged set of the parameters one controller is built from
 * @details The variant is the tag: the active alternative already says which controller to build, so there is no
 *			separate enum member that could drift out of sync with it. GetController() reads the constant each
 *			alternative carries, which keeps that mapping declared in one place - the descriptor itself.
 *
 *			CameraNoneController comes first so a default-constructed descriptor describes no controller at all.
 */
struct CE_CORE_API CameraControllerDescriptor {
	std::variant<
		CameraNoneControllerDescriptor,
		CameraStaticControllerDescriptor,
		CameraFreeControllerDescriptor,
		CameraOrbitControllerDescriptor,
		CameraFirstPersonControllerDescriptor,
		CameraThirdPersonControllerDescriptor,
		CameraTopDownControllerDescriptor,
		CameraSideScrollerControllerDescriptor,
		CameraCinematicControllerDescriptor,
		CameraScriptedControllerDescriptor
	> descriptor;

	/**
	 * @brief Gets the controller type the active alternative describes
	 */
	[[nodiscard]] Types::CameraController GetController() const {
		return std::visit([]<typename Descriptor>(const Descriptor&) { return std::decay_t<Descriptor>::TYPE; }, descriptor);
	}
};

}

#endif //CE_CORE_RENDER_CAMERA_CONTROLLER_CAMERACONTROLLERDESCRIPTOR_HPP
