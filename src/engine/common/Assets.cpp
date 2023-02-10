//
// Created by Diego S. Seabra on 07/12/22.
//

#include "Assets.h"
#include "../rendering/Samplers.h"
#include <yaml.h>

//
// External
//

void Assets::Init()
{
    Logger::Info("Initializing assets...");

    ReadTexturesFromYAML();
    CreateTextures();
}

void Assets::Shutdown()
{
    DestroyTextures();
}

uint32_t Assets::GetTextureId(const std::string& texName)
{
    // FIXME: There is a more performant way to find the texture id by its name?
    for (size_t i = 0; i < textures.size(); i++)
    {
        if (textures[i].textureName == texName)
            return i;
    }

    // if we don't find the texture we are looking for, we return the first texture id (which is 0)
    return 0;
}

Texture* Assets::GetTexture(const std::string &texName)
{
    // FIXME: There is a more performant way to find the texture by its name?
    for (auto &t : textures)
    {
        if (t.textureName == texName)
            return &t.texture;
    }

    // if we don't find the texture we are looking for, we return the first texture of the array
    return &textures[0].texture;
}

TextureSet* Assets::GetTextureSet(const std::string& texName)
{
    // FIXME: There is a more performant way to find the texture set using its name?
    for (auto &t : textures)
    {
        if (t.textureName == texName)
            return &t;
    }

    // if we don't find the texture set we are looking for, we return the first texture of the array
    return nullptr;
}

//
// Internal
//

void Assets::AddTexture(const std::string &texName, const std::string &texPath, uint32_t tileWidth, uint32_t tileHeight, const std::string &samplerName)
{
    Logger::Debug("Adding texture " + texName);

    TextureSet textureSet
    {
        texId,
        texName,
        texPath,
        Texture(),
        tileWidth,
        tileHeight,
        samplerName
    };

    textures.push_back(textureSet);
    texId++;
}

void Assets::ReadTexturesFromYAML()
{
    YAML::Node texYAML = YAML::LoadFile("assets/textures.yaml");
    auto texs = texYAML["textures"];

    for (const auto& t : texs)
    {
        AddTexture(t["name"].as<std::string>(), 
            t["path"].as<std::string>(),
            t["tile_w"].as<int>(),
            t["tile_h"].as<int>(),
            t["sampler"].as<std::string>()
        );
    }
}

void Assets::CreateTextures()
{
    assert(!textures.empty() && "At least one texture should exist in the textures array");

    Logger::Debug("Creating textures");

    for (auto& t : textures)
    {
        t.texture.Create(t.texturePath, t.samplerName);
        descriptors.push_back(t.texture.DescriptorInfo());
    }
}

void Assets::DestroyTextures()
{
    Logger::Debug("Destroying textures");

    for (auto& t : textures)
    {
        t.texture.Destroy();
    }

    textures.clear();
    descriptors.clear();
}
