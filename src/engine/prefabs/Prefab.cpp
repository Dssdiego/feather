//
// Created by Diego S. Seabra on 03/01/23
//

#include "Prefab.h"

Prefab::Prefab() {}

void Prefab::ReadFromYAML(const std::string& filePath)
{
    YAML::Node prefabYAML = YAML::LoadFile(filePath);
    auto props = prefabYAML["prefab"];

    for (const auto &p : props)
    {
        
    }
}

