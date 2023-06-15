#include "pch.h"
#include "LevelHitBullet.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Stopwatch.h"

#include "utils.h"

LevelHitBullet::LevelHitBullet(const Point2f& intersectPoint, bool isFacingLeft, TextureManager* pTextureManager) :
	m_Shape{intersectPoint.x , intersectPoint.y, 122.f, 94.f},
	m_pTexture{},

	m_IsFacingLeft{isFacingLeft},

	m_CurrentFrame{0},
	m_Frames{8},

	m_pStopwatch{ new Stopwatch{1.f} }
{
}

LevelHitBullet::~LevelHitBullet()
{
	delete m_pStopwatch;
}

void LevelHitBullet::Draw() const
{
	utils::SetColor(Color4f{ 1.f,0.f,0.f,1.f });
	utils::FillEllipse(m_Shape.left, m_Shape.bottom,5, 5);


}