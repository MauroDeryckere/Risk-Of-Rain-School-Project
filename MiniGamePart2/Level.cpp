#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include "utils.h"

#include <iostream>
//TO DO: svg file for level

Level::Level():
	m_Vertices{ Point2f {0,0},Point2f {0,190}, Point2f {340,190}, Point2f {408,124}, Point2f {560,124}, Point2f {660,224}, Point2f {846,224}, Point2f {846,0}, Point2f {0,0} },
	m_pBackgroundTexture{ new Texture{"Images/background.png"}},
	m_pFenceTexture{ new Texture{"Images/fence.png"}},
	m_FenceBottomLeft{200,190},

	m_Boundaries{0,0,m_pBackgroundTexture->GetWidth(),m_pBackgroundTexture->GetHeight()}
{

}

Level::~Level()
{
	delete m_pBackgroundTexture;
	m_pBackgroundTexture = nullptr;

	delete m_pFenceTexture;
	m_pFenceTexture = nullptr;
}

void Level::DrawBackground() const
{
	m_pBackgroundTexture->Draw();
}

void Level::DrawForeground() const
{
	m_pFenceTexture->Draw(m_FenceBottomLeft);
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	//if actor <= bottom, change to y pos intersection, change y velocity to 0

	//vertical ray in the middle of actor
	Point2f origin1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };
	Point2f origin2{ actorShape.left + actorShape.width / 2, actorShape.bottom };

	utils::HitInfo hitInfo{}; //Raycast needs empty hitinfo to return multiple variables

	if (utils::Raycast(m_Vertices, origin1, origin2, hitInfo))
	{
		actorVelocity.y = 0.f;
		actorShape.bottom = hitInfo.intersectPoint.y;
	}
}

bool Level::IsOnGround(const Rectf& actorShape) const
{
	//if actor is on lvl, return true, else false
	// raycast vertical ray in middle of actor that is 1 pixel deeper than bottom

	//vertical ray in the middle of actor
	Point2f origin1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height - 1.f };
	Point2f origin2{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1.f };

	utils::HitInfo hitInfo{}; //Raycast needs empty hitinfo to return multiple variables

	return utils::Raycast(m_Vertices, origin1, origin2, hitInfo);
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}
