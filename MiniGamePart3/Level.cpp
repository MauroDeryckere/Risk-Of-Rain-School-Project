#include "pch.h"
#include "Level.h"
#include "Platform.h"

#include "SVGParser.h"

#include <iostream>

//TO DO: svg file for level

Level::Level() :
	m_pBackgroundTexture{ new Texture{"Images/background.png"} },
	m_pFenceTexture{ new Texture{"Images/fence.png"} },
	m_FenceBottomLeft{ 200,190 },

	m_Boundaries{ 0,0,m_pBackgroundTexture->GetWidth(),m_pBackgroundTexture->GetHeight() },

	m_pPlatform{ new Platform{Point2f{30.f,300.f}} },

	m_EndSignTexture{ Texture{"Images/EndSign.png"} },
	m_EndSignShape{ Rectf{730.f, 224.f, m_EndSignTexture.GetWidth(), m_EndSignTexture.GetWidth()} }
{
	SVGParser::GetVerticesFromSvgFile("Images/background.svg", m_Vertices);
}

Level::~Level()
{
	delete m_pBackgroundTexture;
	m_pBackgroundTexture = nullptr;

	delete m_pFenceTexture;
	m_pFenceTexture = nullptr;

	delete m_pPlatform;
	m_pPlatform = nullptr;
}

void Level::DrawBackground() const
{
	m_pBackgroundTexture->Draw();
	m_pPlatform->Draw();
	m_EndSignTexture.Draw(m_EndSignShape, Rectf{0.f,0.f, m_EndSignTexture.GetWidth(), m_EndSignTexture.GetWidth() });
}

void Level::DrawForeground() const
{
	m_pFenceTexture->Draw(m_FenceBottomLeft);
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const 
{
	m_pPlatform->HandleCollission(actorShape, actorVelocity); //Platform collission

	//if actor <= bottom, change to y pos intersection, change y velocity to 0

	//vertical ray in the middle of actor
	const Point2f origin1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height };
	const Point2f origin2{ actorShape.left + actorShape.width / 2, actorShape.bottom };

	utils::HitInfo hitInfo{}; //Raycast needs empty hitinfo to return multiple variables

	if (utils::Raycast(m_Vertices[0], origin1, origin2, hitInfo))
	{
		actorVelocity.y = 0.f;
		actorShape.bottom = hitInfo.intersectPoint.y;
	}


	if (actorShape.left < m_Boundaries.left)
	{
		actorShape.left = m_Boundaries.left;
	}
	if (actorShape.left + actorShape.width > m_Boundaries.left + m_Boundaries.width)
	{
		actorShape.left = m_Boundaries.left + m_Boundaries.width - actorShape.width;
	}

}

bool Level::IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const
{
	//if actor is on lvl, return true, else false
	// raycast vertical ray in middle of actor that is 1 pixel deeper than bottom

	//vertical ray in the middle of actor
	Point2f origin1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height - 1.f };
	Point2f origin2{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1.f };

	utils::HitInfo hitInfo{}; //Raycast needs empty hitinfo to return multiple variables

	if (utils::Raycast(m_Vertices[0], origin1, origin2, hitInfo) || m_pPlatform->IsOnGround(actorShape, actorVelocity))
	{
		return true;
	}

	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

bool Level::HasReachedEnd(const Rectf& actorShape) const
{
	if (utils::IsOverlapping(actorShape, m_EndSignShape))
	{
		return true;
	}
	return false;
}
