#include "pch.h"
#include "Platform.h"

#include "Texture.h"

#include "utils.h"
#include <iostream>


Platform::Platform(const Point2f& bottomLeft) :
	m_pTexture{ new Texture{"platform.png"} },
	m_Shape{ Rectf{bottomLeft.x ,bottomLeft.y ,m_pTexture->GetWidth()/2 ,m_pTexture->GetHeight()/2}}
{
}

Platform::~Platform()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Platform::Draw() const
{
	const Rectf sourceRect{0.f,0.f,m_pTexture->GetWidth() ,m_pTexture->GetHeight() };
	const Rectf destRect{ m_Shape };

	m_pTexture->Draw(destRect,sourceRect);
}

void Platform::HandleCollission(Rectf& actorShape, Vector2f& actorVelocity) const
{
	if (actorVelocity.y < 0.f)
	{
		const std::vector<Point2f> vertices{ Point2f{m_Shape.left, m_Shape.bottom + m_Shape.height},
											 Point2f{m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height} };
		
		const Point2f origin1{ actorShape.left + actorShape.width / 2, actorShape.bottom  };
		const Point2f origin2{ actorShape.left + actorShape.width / 2, actorShape.bottom + 5.f };

		utils::HitInfo hitInfo{}; //Raycast needs empty hitinfo to return multiple variables

		if (utils::Raycast(vertices, origin1, origin2, hitInfo))
		{
			actorVelocity.y = 0.f;
			actorShape.bottom = m_Shape.bottom + m_Shape.height;
		}
	}

}

bool Platform::IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const
{
	if (actorVelocity.y <= 0.f)
	{
		if (actorShape.bottom == m_Shape.bottom + m_Shape.height)
		{
			return true;
		}
	}
	return false;
}
