//
// Created by Diego S. Seabra on 17/08/22.
//

#ifndef FEATHER_SPRITE_RENDER_SYSTEM_H
#define FEATHER_SPRITE_RENDER_SYSTEM_H

#include <iostream>
#include <vector>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "../components/Components.h"

#include "../rendering/SpriteAtlas.h"

namespace Feather
{
	struct SpriteRenderSystemImpl
	{
		SpriteRenderSystemImpl();
		~SpriteRenderSystemImpl();

		void RenderEntities(entt::registry* registry);

		glm::mat4 GetModelTransformation(TransformComponent transform);
	};

	class SpriteRenderSystem
	{
	public:
		static void Init();
		static void Shutdown();

		static void RenderEntities(entt::registry* registry);
	};
}

#endif //FEATHER_SPRITE_RENDER_SYSTEM_H
