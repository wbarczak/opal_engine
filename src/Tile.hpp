#pragma once

#include <cstdint>
#include <string>
#include <cmath>
#include <memory>
#include "Renderer.hpp"
#include "Components.hpp"

class Tile
{
public:

	Tile(const std::string& stringTextureId) :
		m_TextureId(Renderer::getNumericalId(stringTextureId)) {}

	Tile() :
		m_TextureId(Renderer::NO_TEXTURE) {}

	TextureId textureId() const { return m_TextureId; }
	bool isSolid() const { return m_TextureId != Renderer::NO_TEXTURE; }

private:

	TextureId m_TextureId;
};