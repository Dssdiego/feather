//
// Created by Diego S. Seabra on 07/12/22.
//

#ifndef FEATHER_ASSETS_H
#define FEATHER_ASSETS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "../rendering/Texture.h"

struct TextureSet
{
    size_t id;
    std::string textureName;
    std::string texturePath;
    Texture texture;
    uint32_t tileWidth;
    uint32_t tileHeight;
    std::string samplerName;
};

class Assets
{
public:
    static void Init();
    static void Shutdown();

    static const uint32_t MAX_TEXTURES = 16;

    static uint32_t GetTextureId(const std::string& texName);
    static Texture* GetTexture(const std::string &texName);
    inline static Texture* GetDefaultTexture() { return GetTexture("solitaire_cards"); }

    static TextureSet* GetTextureSet(const std::string &texName);

    inline static std::vector<VkDescriptorImageInfo> GetDescriptors() { return descriptors; }

private:
    static void AddTexture(const std::string &texName, const std::string &texPath, uint32_t tileWidth, uint32_t tileHeight, const std::string &samplerName);
    static void ReadTexturesFromYAML();
    static void CreateTextures();
    static void DestroyTextures();

    // REVIEW: Do the same to shaders?
    // TODO: Do the same to texture samplers (we only need one for now)
    inline static std::vector<TextureSet> textures{}; // FIXME: Change this to mTextures
    inline static std::vector<VkDescriptorImageInfo> descriptors{}; // FIXME: Change this to mDescriptors

    inline static size_t texId = 0;
};


#endif //FEATHER_ASSETS_H
