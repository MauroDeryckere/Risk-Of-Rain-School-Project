#include "pch.h"
#include "LevelObject.h"

#include "Texture.h"
#include "TextureManager.h"
#include "utils.h"
#include <cassert>

LevelObject::LevelObject(const Rectf& shape, TextureManager* pTextureManager) :
	m_Shape{ shape },
	m_pTextureManager {pTextureManager},
	m_pTexture{pTextureManager->CreateTexture("Level/LevelObjectsSpriteSheet.png", "levelObjects")}
{

}

LevelObject::~LevelObject()
{
	m_pTextureManager->DeleteTexture("levelObjects");
}

const Rectf& LevelObject::GetShape() const
{
	return m_Shape;
}
