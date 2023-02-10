//
// Created by Diego S. Seabra on 22/09/22.
//

#include "SpriteAtlas.h"

glm::vec4 SpriteAtlas::GetUVCoordinate(TextureSet* texSet, uint32_t line, uint32_t column)
{
    auto texSize = texSet->texture.Size();
    auto tileWidth = texSet->tileWidth;
    auto tileHeight = texSet->tileHeight;

    auto lineIncrement = (float) tileHeight / texSize.y;
    auto columnIncrement = (float) tileWidth / texSize.x;

    auto uvStartX = (float) column * columnIncrement;
    auto uvStartY = (float) line * lineIncrement;

    auto uvEndX = (float) (column + 1) * columnIncrement;
    auto uvEndY = (float) (line + 1) * lineIncrement;

    return {uvStartX, uvStartY, uvEndX, uvEndY};
}

std::vector<glm::vec2> SpriteAtlas::GetUVsCoords(TextureSet* texSet, uint32_t tileId, bool flippedX)
{
    auto texSize = texSet->texture.Size();
    auto tileWidth = texSet->tileWidth;
    auto tileHeight = texSet->tileHeight;

    uint32_t nCols = (uint32_t) texSize.x / (uint32_t) tileWidth;
    uint32_t nRows = (uint32_t) texSize.y / (uint32_t) tileHeight;

    assert(nRows > 0 && "Wrong number of rows. Check the tile division of your texture!");
    assert(nCols > 0 && "Wrong number of cols. Check the tile division of your texture!");

    auto col = tileId % nCols;
    auto row = (uint32_t) (tileId / nRows);

    auto lineIncrement = (float) tileHeight / texSize.y;
    auto columnIncrement = (float) tileWidth / texSize.x;

    auto uvStartX = (float) col * columnIncrement;
    auto uvStartY = (float) row * lineIncrement;

    auto uvEndX = (float) (col + 1) * columnIncrement;
    auto uvEndY = (float) (row + 1) * lineIncrement;

    glm::vec2 uv0 = { uvStartX, uvStartY };
    glm::vec2 uv1 = { uvEndX, uvStartY };
    glm::vec2 uv2 = { uvEndX, uvEndY };
    glm::vec2 uv3 = { uvStartX, uvEndY };

    std::vector<glm::vec2> result = {};

    if (flippedX)
        result = { uv1, uv0, uv3, uv2 };
    else
        result = { uv0, uv1, uv2, uv3 };

//    std::cout << "== Get UV Coords ==" << std::endl;
//    std::cout << "(" << uv0[0] << "," << uv0[1] << "), ("
//                     << uv1[0] << "," << uv1[1] << "), ("
//                     << uv2[0] << "," << uv2[1] << "), ("
//                     << uv3[0] << "," << uv3[1] << ")" << std::endl;

    return result;
}

