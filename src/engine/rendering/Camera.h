//
// Created by Diego S. Seabra on 01/09/22.
//

#ifndef FEATHER_CAMERA_H
#define FEATHER_CAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../core/Time.h"

#include "Window.h"
#include "../common/Utils.h"

namespace Feather
{
	enum CameraMode
	{
		PERSPECTIVE,
		ORTOGRAPHIC
	};

	class Camera
	{
	public:
		static void Init(float aspect);
		static void Update();

		inline static CameraMode GetMode() { return mode; }
		inline static std::string GetModeStr() {
			if (mode == CameraMode::ORTOGRAPHIC)
				return "ortographic";
			else
				return "perspective";
		}
		inline static void SetMode(CameraMode newMode) {
			mode = newMode;
			UpdateProjection();
		}
		static void UpdateProjection(); // updates the camera projection based on the new mode (ortographic/perspective)

		static void SetOrtographicProjection(float left, float right, float top, float bottom, float near, float far);
		static void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

		static void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f));
		static void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f));
		static void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

		inline static glm::vec3 GetWorldPosition() { return worldPosition; }
		inline static glm::vec3* GetWorldPositionRef() { return &worldPosition; }
		inline static void SetWorldPosition(glm::vec3 position) { worldPosition = position; UpdateCameraPosition(); }
		inline static void UpdateCameraPosition() { SetViewDirection(worldPosition, worldDirection); }

		inline static void ResetWorldPosition() { SetWorldPosition(glm::vec3(3.5f, 2.f, -2.f)); };

		inline static glm::vec3 GetWorldDirection() { return worldDirection; }
		inline static glm::vec3* GetWorldDirectionRef() { return &worldDirection; }
		inline static void SetWorldDirection(glm::vec3 direction) { worldDirection = direction; }

		// controls whether the camera is being controlled by someone
		inline static bool HasControl() { return takeControl; }
		inline static bool takeControl = false;

		static const glm::mat4& GetView();
		static const glm::mat4 GetViewForGizmos();
		static const glm::mat4& GetProjection();

		// coordinate conversions
		[[deprecated]] static glm::vec3 ScreenToWorldSpace(glm::vec2 screenCoordinates);
		static glm::vec3 ScreenToNormalizedDeviceCoords(glm::vec2 screenCoords);
		static glm::vec4 NormalizedToClipCoords(glm::vec3 normalizedCoords);
		static glm::vec4 ClipToEyeCoords(glm::vec4 clipCoords);
		static glm::vec3 EyeToWorldCoords(glm::vec4 eyeCoords);
		//static glm::vec3

		// raycasting
		static glm::vec3 Raycast(glm::vec2 screenPoint);

		inline static float* GetOrtographicZoomRef() { return &ortographicZoom; }

		// TODO: Implement zNear and zFar in the camera class

		// movement
		inline static const float cameraSpeedBase = 2.f;
		inline static const float cameraSpeedAccelerated = 5.f;

		inline static float cameraMoveSpeed = cameraSpeedBase;
		inline static float cameraZoomSpeed = cameraSpeedBase;

		inline static void MoveRight()
		{
			worldPosition.x += cameraMoveSpeed * Time::Delta; // REVIEW: This should me multiplied by the engine delta time?
			UpdateCameraPosition();
		}
		inline static void MoveLeft()
		{
			worldPosition.x -= cameraMoveSpeed * Time::Delta; // REVIEW: This should me multiplied by the engine delta time?
			UpdateCameraPosition();
		}
		inline static void MoveUp()
		{
			worldPosition.y -= cameraMoveSpeed * Time::Delta; // REVIEW: This should me multiplied by the engine delta time?
			UpdateCameraPosition();
		}
		inline static void MoveDown()
		{
			worldPosition.y += cameraMoveSpeed * Time::Delta; // REVIEW: This should me multiplied by the engine delta time?
			UpdateCameraPosition();
		}

		// zoom
		inline static void ZoomIn()
		{
			worldPosition.z += cameraZoomSpeed * Time::Delta; // REVIEW: This should me multiplied by the engine delta time?
			UpdateCameraPosition();
		}

		inline static void ZoomOut()
		{
			worldPosition.z -= cameraZoomSpeed * Time::Delta; // REVIEW: This should me multiplied by the engine delta time?
			UpdateCameraPosition();
		}

	private:
		// TODO: Put the projection and view matrix in a single matrix
		inline static glm::mat4 projectionMatrix{ 1.f };
		inline static glm::mat4 viewMatrix{ 1.f };

		inline static glm::vec3 worldPosition = glm::vec3(0.f);
		inline static glm::vec3 worldDirection = glm::vec3(0.f, 0.f, 1.f);

		inline static CameraMode mode = CameraMode::ORTOGRAPHIC;

		inline static float aspectRatio = 0.f;
		inline static float ortographicZoom = 1.0f;
	};
}

#endif //FEATHER_CAMERA_H
