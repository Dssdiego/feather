//
// Created by Diego S. Seabra on 03/01/23
//

#ifndef FEATHER_PREFAB_H
#define FEATHER_PREFAB_H

#include <string>
#include <yaml.h>
#include <glm/vec2.hpp>

//struct PrefabProps
//{
//	std::string texture;
//	int tile;
//	glm::vec2 offset;
//	bool visible;
//};

class Prefab
{
public:
	Prefab();

	void ReadFromYAML(const std::string &filePath);

//private:
//	std::vector<Entity> entities;
};

#endif //FEATHER_PREFAB_H
