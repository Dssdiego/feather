//
// Created by Diego S. Seabra on 08/01/23
//

#include "Mouse.h"

namespace Feather
{
	void Mouse::Init()
	{
		LoadCursorsFromYAML();
		SetCursor(CursorMode::ARROW);
	}

	void Mouse::Update()
	{
		if (Input::IsMouseButtonPressed(MouseButton::LEFT))
			SetCursor(CursorMode::GRABBING);
		else
			SetCursor(CursorMode::ARROW);
	}

	void Mouse::Shutdown()
	{
		for (auto& c : cursors)
		{
			glfwDestroyCursor(c.cursor);
		}
	}

	void Mouse::SetCursor(CursorMode mode)
	{
		for (auto& c : cursors)
		{
			if (c.mode == mode)
			{
				glfwSetCursor(Window::GetWindow(), c.cursor);
			}
		}
	}

	void Mouse::LoadCursorsFromYAML()
	{
		YAML::Node cursorYAML = YAML::LoadFile("assets/cursors.yaml");
		auto curs = cursorYAML["cursors"];

		for (const auto& c : curs)
		{
			CursorData data =
			{
				c["name"].as<std::string>(),
				static_cast<CursorMode>(c["mode"].as<int>()),
				c["image"].as<std::string>(),
				glm::vec2(c["hotspot"][0].as<float>(), c["hotspot"][1].as<float>())
			};

			CreateCursor(data);
		}
	}

	void Mouse::CreateCursor(CursorData data)
	{
		// load the image in the memory
		GLFWimage image;
		image.width = 0;
		image.height = 0;
		image.pixels = stbi_load(data.textureName.c_str(), &image.width, &image.height, nullptr, STBI_rgb_alpha);

		if (!image.pixels)
		{
			throw std::runtime_error("failed to load cursor image");
		}

		auto cursor = glfwCreateCursor(
			&image,
			data.hotspot.x,
			data.hotspot.y
		);
		data.cursor = cursor;

		cursors.push_back(data);

		// we don't need the image anymore, cleaning it
		stbi_image_free(image.pixels);
	}
}
