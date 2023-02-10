//
// Created by Diego S. Seabra on 26/01/23
//

#ifndef FEATHER_ECS_H
#define FEATHER_ECS_H

#include <vector>
#include <entt/entt.hpp>

namespace Feather
{
	class ECS
	{
	public:
		static void Init();
		static void Shutdown();

		static entt::entity Create();
		static void Destroy(entt::entity e);
		static void DestroyAll();

		inline static entt::registry* GetRegistry() { return &registry; }

		static void Update();

		// templates
		template <typename T>
		static void AddComponent(entt::entity e, T t)
		{
			// we add the component if the entity doesn't have it yet
			// and we replace the component if the entity already has it
			registry.emplace_or_replace<T>(e, t);
		}

		template <typename T>
		static void RemoveComponent(entt::entity e, T t)
		{
			registry.erase<T>(e);
		}

	private:
		inline static std::vector<entt::entity> entities{};
		inline static entt::registry registry{};
	};
}

#endif //FEATHER_ECS_H
