//
// Created by Diego S. Seabra on 01/09/22.
//

#include "Camera.h"
#include <iostream>
#include "../input/Input.h"

namespace Feather
{
	void Camera::Init(float aspect)
	{
		Logger::Debug("Camera Init");
		// FIXME: Can't use logger here because of the "NOMINMAX" global fix

		aspectRatio = aspect;

		UpdateProjection();

		// Setting camera properties
		// using ortographic projection (updating every frame so it matches the window width and height)
		Camera::SetWorldPosition(glm::vec3(1.5f, 0.5f, -2.0f));
		//    Camera::SetWorldDirection(glm::vec3(0.f, 0.f, 1.f));
		Camera::SetViewDirection(Camera::GetWorldPosition(), Camera::GetWorldDirection()); // camera at "origin", pointing "forward" at the Z axis
	}

	void Camera::Update()
	{
		if (HasControl())
		{
			cameraMoveSpeed = Input::GetKeyboardKeyHold(Keys::LEFT_SHIFT) ? cameraSpeedAccelerated : cameraSpeedBase;
			cameraZoomSpeed = cameraMoveSpeed;

			if (Input::GetKeyboardKeyHold(Keys::W))
				MoveUp();

			if (Input::GetKeyboardKeyHold(Keys::A))
				MoveLeft();

			if (Input::GetKeyboardKeyHold(Keys::S))
				MoveDown();

			if (Input::GetKeyboardKeyHold(Keys::D))
				MoveRight();
		}
	}

	void Camera::UpdateProjection()
	{
		Logger::Debug("Changing camera projection");

		if (mode == CameraMode::ORTOGRAPHIC)
			Camera::SetOrtographicProjection(-aspectRatio, aspectRatio, -1, 1, -10, 10);

		if (mode == CameraMode::PERSPECTIVE)
			Camera::SetPerspectiveProjection(glm::radians(50.f), aspectRatio, 0.1f, 100.f);
	}

	void Camera::SetOrtographicProjection(float left, float right, float top, float bottom, float _near, float _far)
	{
		projectionMatrix = glm::mat4{ 1.0f };
		projectionMatrix[0][0] = ortographicZoom / (right - left);
		projectionMatrix[1][1] = ortographicZoom / (bottom - top);
		projectionMatrix[2][2] = 1.f / (_far - _near);
		projectionMatrix[3][0] = -(right + left) / (right - left);
		projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
		projectionMatrix[3][2] = -_near / (_far - _near);

		Logger::Debug("Camera is now ortographic");
	}

	void Camera::SetPerspectiveProjection(float fovy, float aspect, float _near, float _far)
	{
		assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tan(fovy / 2.f);
		projectionMatrix = glm::mat4{ 0.0f };
		projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
		projectionMatrix[1][1] = 1.f / (tanHalfFovy);
		projectionMatrix[2][2] = _far / (_far - _near);
		projectionMatrix[2][3] = 1.f;
		projectionMatrix[3][2] = -(_far * _near) / (_far - _near);

		Logger::Debug("Camera is now perspective");
	}

	const glm::mat4& Camera::GetProjection()
	{
		return projectionMatrix;
	}

	const glm::mat4& Camera::GetView()
	{
		return viewMatrix;
	}

	const glm::mat4 Camera::GetViewForGizmos()
	{
		auto newMatrix = glm::inverse(viewMatrix);
		newMatrix[3][0] *= -1;
		return newMatrix;
	}

	void Camera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
	{
		// constructing an orthonormal basis (unit length = 1, all orthogonal to each other, 90 degress angles)
		const glm::vec3 w{ glm::normalize(direction) };
		const glm::vec3 u{ glm::normalize(glm::cross(w, up)) };
		const glm::vec3 v{ glm::cross(w, u) };

		viewMatrix = glm::mat4{ 1.f };
		viewMatrix[0][0] = u.x;
		viewMatrix[1][0] = u.y;
		viewMatrix[2][0] = u.z;
		viewMatrix[0][1] = v.x;
		viewMatrix[1][1] = v.y;
		viewMatrix[2][1] = v.z;
		viewMatrix[0][2] = w.x;
		viewMatrix[1][2] = w.y;
		viewMatrix[2][2] = w.z;
		viewMatrix[3][0] = -glm::dot(u, position);
		viewMatrix[3][1] = -glm::dot(v, position);
		viewMatrix[3][2] = -glm::dot(w, position);
	}

	// locks the camera to a specific target
	void Camera::SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
	{
		SetViewDirection(position, target - position, up);
	}

	void Camera::SetViewYXZ(glm::vec3 position, glm::vec3 rotation)
	{
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
		const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
		const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
		viewMatrix = glm::mat4{ 1.f };
		viewMatrix[0][0] = u.x;
		viewMatrix[1][0] = u.y;
		viewMatrix[2][0] = u.z;
		viewMatrix[0][1] = v.x;
		viewMatrix[1][1] = v.y;
		viewMatrix[2][1] = v.z;
		viewMatrix[0][2] = w.x;
		viewMatrix[1][2] = w.y;
		viewMatrix[2][2] = w.z;
		viewMatrix[3][0] = -glm::dot(u, position);
		viewMatrix[3][1] = -glm::dot(v, position);
		viewMatrix[3][2] = -glm::dot(w, position);
	}

	glm::vec3 Camera::ScreenToWorldSpace(glm::vec2 screenCoordinates)
	{
		auto viewProjection = viewMatrix * projectionMatrix;
		auto inverse = glm::inverse(viewProjection);

		auto windowSize = Window::GetSize();

		auto x = Utils::MapToRange((float)screenCoordinates.x, 0.f, (float)windowSize.width, -1.f, 1.f);
		auto y = Utils::MapToRange((float)screenCoordinates.y, 0.f, (float)windowSize.height, -1.f, 1.f);
		auto z = 1.f; // FIXME: This should get the camera zFar

		auto result = glm::vec4(x, y, z, 1.f);

		auto pos = result * inverse;

		// accounting for the camera's position

		// FIXME: Not working for perspective projections, only for orthographic projections
	//    pos.w = 1.0f / pos.w;
		pos.x += worldPosition.x;
		pos.y += worldPosition.y;
		//    pos.z *= worldPosition.z;

		//    std::stringstream ss;
		//    std::cout << "[Screen to World Space] X: " << result.x << " | Y: " << result.y << std::endl;
		//    std::cout << "[Screen to World Space >> POS] X: " << pos.x << " | Y: " << pos.y << std::endl;
		//    Logger::Debug(ss.str());

		return pos;
	}

	glm::vec3 Camera::ScreenToNormalizedDeviceCoords(glm::vec2 screenCoords)
	{
		auto windowSize = Window::GetSize();

		auto x = Utils::MapToRange((float)screenCoords.x, 0.f, (float)windowSize.width, -1.f, 1.f);
		auto y = Utils::MapToRange((float)screenCoords.y, 0.f, (float)windowSize.height, -1.f, 1.f);
		auto z = 1.f; // FIXME: This should get the camera zFar

		auto result = glm::vec3(x, y, z);

		/*std::cout << "[SCREEN] X: " << screenCoords.x
			<< " | Y: " << screenCoords.y
			<< std::endl;

		std::cout << "[NORMALIZED] X: " << result.x
			<< " | Y: " << result.y
			<< " | Z: " << result.z
			<< std::endl;*/

		return result;
	}

	glm::vec4 Camera::NormalizedToClipCoords(glm::vec3 normalizedCoords)
	{
		// REVIEW: Here the "Z" component should by negative or positive?
		auto result = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1.f, 1.f);

		/*std::cout << "[NORMALIZED] X: " << normalizedCoords.x
			<< " | Y: " << normalizedCoords.y
			<< " | Z: " << normalizedCoords.z
			<< std::endl;

		std::cout << "[CLIP] X: " << result.x
			<< " | Y: " << result.y
			<< " | Z: " << result.z
			<< " | W: " << result.w
			<< std::endl;*/

		return result;
	}

	// NOTE: we probably aren't using this
	glm::vec4 Camera::ClipToEyeCoords(glm::vec4 clipCoords)
	{
		auto inverted = glm::inverse(projectionMatrix) * clipCoords;
		auto result = glm::vec4(inverted.x, inverted.y, -1.f, 0.f); // "unprojecting" the "xy" part so that it becomes a vector, not a point

		/*std::cout << "[CLIP] X: " << clipCoords.x
			<< " | Y: " << clipCoords.y
			<< " | Z: " << clipCoords.z
			<< " | W: " << clipCoords.w
			<< std::endl;

		std::cout << "[EYE] X: " << result.x
			<< " | Y: " << result.y
			<< " | Z: " << result.z
			<< " | W: " << result.w
			<< std::endl;*/

		return result;
	}

	glm::vec3 Camera::EyeToWorldCoords(glm::vec4 eyeCoords)
	{
		auto inverted = glm::inverse(viewMatrix) * eyeCoords;
		auto result = glm::normalize(glm::vec3(inverted.x, inverted.y, inverted.z));

		/*std::cout << "[EYE] X: " << eyeCoords.x
			<< " | Y: " << eyeCoords.y
			<< " | Z: " << eyeCoords.z
			<< " | W: " << eyeCoords.w
			<< std::endl;

		std::cout << "[WORLD] X: " << result.x
			<< " | Y: " << result.y
			<< " | Z: " << result.z
			<< std::endl << std::endl;*/

		return result;
	}

	glm::vec3 Camera::Raycast(glm::vec2 screenPoint)
	{
		auto normalizedCoords = ScreenToNormalizedDeviceCoords(screenPoint);
		auto clipCoords = NormalizedToClipCoords(normalizedCoords);
		auto eyeCoords = ClipToEyeCoords(clipCoords);
		auto worldRay = EyeToWorldCoords(eyeCoords);
		return worldRay;
	}
}
