//
// Created by Diego S. Seabra on 31/01/23
//

#include "RaycastPanel.h"

#include <imgui.h>
#include <imgui/IconsFontAwesome6.h>

#include "../../input/Input.h"
#include "../../rendering/Camera.h"
#include "../EditorDraw.h"

namespace Feather
{
	void RaycastPanel::Draw()
	{
		ImGui::Begin(ICON_FA_BOLT " Raycast");

		auto mouseCoords = Input::GetMouseScreenPosition();
		EditorDraw::Vec2Control("Mouse", mouseCoords);

		auto normalizedCoords = Camera::ScreenToNormalizedDeviceCoords(mouseCoords);
		EditorDraw::Vec3Control("Normalized", normalizedCoords);

		auto clipCoords = Camera::NormalizedToClipCoords(normalizedCoords);
		EditorDraw::Vec4Control("Clip", clipCoords);

		auto eyeCoords = Camera::ClipToEyeCoords(clipCoords);
		EditorDraw::Vec4Control("Eye", eyeCoords);

		auto worldRay = Camera::EyeToWorldCoords(eyeCoords);
		EditorDraw::Vec3Control("World", worldRay);

		auto screenToWorld = Camera::ScreenToWorldSpace(mouseCoords);
		EditorDraw::Vec3Control("Screen->World", screenToWorld);

		ImGui::End();
	}
}
