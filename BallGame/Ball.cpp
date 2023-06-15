#include "pch.h"
#include "Ball.h"
#include "utils.h"


Ball::Ball(const Point2f& center, const Vector2f& velocity, const Color4f& color, float radius) :
	m_Center{ center },
	m_Velocity{ velocity },
	m_Color{ color },
	m_Radius{ radius }
{

}

void Ball::Update(float elapsedSeconds, const Rectf& borders)
{
	const float leftBorder{borders.left};
	const float bottomBorder{ borders.bottom };
	const float rightBorder{ borders.left + borders.width};
	const float topBorder{ borders.bottom + borders.height};

	m_Center.x += m_Velocity.x * elapsedSeconds;
	m_Center.y += m_Velocity.y * elapsedSeconds;

}

void Ball::Draw() const
{
	utils::SetColor(m_Color);
	utils::FillEllipse(m_Center, m_Radius, m_Radius);
}
