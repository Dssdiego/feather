//
// Created by Diego S. Seabra on 15/10/22.
//

#ifndef FEATHER_COMPONENTS_H
#define FEATHER_COMPONENTS_H

// REVIEW: Do we need to import all of this ahead of time? There's a better way to do this?
#include <glm/glm.hpp>
#include "../rendering/shapes/Shape.h"
#include "../common/structs.h"
#include "../common/Color.h"
#include "../entities/ECS.h"
#include "../input/Input.h"

// REVIEW: Surround components with Feather namespace?
using namespace Feather;

// entt tag to identity objects in the game
struct IdentityComponent
{
	std::string id = "";
	std::string name = "";
};

struct TransformComponent
{
	glm::vec3 position{ 0.f, 0.f, 0.f };
	glm::vec3 scale{ 1.f, 1.f, 1.f };
	glm::vec3 rotation{ 0.f, 0.f, 0.f }; // NOTE: In Degrees
	glm::vec3 offset{ 0.f, 0.f, 0.f };
};

// entt tag for atlas sprites
struct SpriteRendererComponent
{
	Color color = Color::White;
	std::string pipeline = "";
	std::string textureName = "";
	uint32_t tile = 0;
	bool visible = true;
};


// [future use] empty tag for entities that are controlled by the player with the joystick
struct JoystickComponent
{
	Keys keyUp;
};

// [future use] empty tag for entities that are controlled by an AI (like in a cutscene, for example)
struct AIComponent { };

// [future use] empty tag for entities that are controlled by a cutscene
struct CutsceneComponent { };

// entt tag for area component
struct AreaComponent
{
	glm::vec3 min = { 0.f, 0.f, 0.f };
	glm::vec3 max = { 0.f, 0.f, 0.f };
};

// entt tag for draggable objects
struct DraggableComponent
{
	bool dragging = false;
	glm::vec3 offset = { 0.f, 0.f, 0.f };
};

// entt tag for object with gravity
struct GravityComponent 
{
	float value = 1.f;
};

struct CollisionComponent
{
	float size = 1.f;
};

//
// DEPRECATED TAGS BELOW
//

struct WorldInfo // DEPRECATED
{
	uint32_t id = 0;
	std::string name = "";
	Color color = Color::White;
	std::string pipeline = "";
	Shape shape{ ShapeType::Quad };
};

struct IsometricTile
{
	std::string textureName;
	uint32_t atlasPosition = 0;
	glm::vec2 gridPosition = glm::vec2(0, 0);
	bool flippedX = false;
};

// entt tag component for animated sprites
struct AnimatedTileSprite
{
	glm::vec2 beginTile = glm::vec2(0, 0);
	glm::vec2 endTile = glm::vec2(0, 0);
	uint32_t currentFrame = 0;
};

// entt tag for tile sprites
struct TileSprite
{
	std::string textureName;
	uint32_t tile;
};

//
// DEPRECATED TAGS BELOW (END)
//

#endif //FEATHER_COMPONENTS_H
