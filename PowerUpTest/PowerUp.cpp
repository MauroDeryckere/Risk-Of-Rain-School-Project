#include "pch.h"
#include "PowerUp.h"
#include "Texture.h"
#include <iostream>

PowerUp::PowerUp(const Point2f& center, PowerUp::Type type) :
	m_Type{ type },
	m_Angle{},
	m_pTexture{ new Texture{ "Images/PowerUp.png" } },
	m_RotSpeed{360}
{
	m_Shape = Circlef{ center, m_pTexture->GetWidth() / 2 };
	m_TextClip = Rectf{ 0.f, m_pTexture->GetHeight() / 2 * int(type) ,m_pTexture->GetWidth(),m_pTexture->GetHeight() / 2};
}

PowerUp::~PowerUp()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void PowerUp::Update(float elapsedSec)
{
	m_Angle += m_RotSpeed * elapsedSec;
}

void PowerUp::Draw() const
{
	glPushMatrix();
		glTranslatef(m_Shape.center.x, m_Shape.center.y, 0);
		glRotatef(m_Angle, 0, 0, 1);
		glTranslatef(-m_Shape.radius, -m_Shape.radius, 0);

		m_pTexture->Draw(Rectf{0.f ,0.f , m_pTexture->GetWidth(), m_pTexture->GetHeight() / 2 }, m_TextClip);	
	glPopMatrix();
}

bool PowerUp::IsOverlapping(const Rectf& rect) const
{
	if (IsRectInCircle(rect, m_Shape))
	{
		return true;
	}
	return false;
}

float PowerUp::GetDistance(float xPointA, float yPointA, float xPointB, float yPointB) const
{
	float result{ 0 };
	float firstValue{ xPointB - xPointA };
	float secondValue{ yPointB - yPointA };
	result = sqrtf(powf(firstValue, 2) + powf(secondValue, 2));

	return result;

}
float PowerUp::GetDistance(const Point2f& pointA, const Point2f& pointB) const
{
	return GetDistance(pointA.x, pointA.y, pointB.x, pointB.y);
}

bool PowerUp::IsPointInCircle(float xPoint, float yPoint, float xCenterCircle, float yCenterCircle, float radiusCircle) const
{
	float distanceBetweenPointAndCircle{ GetDistance(xPoint, yPoint, xCenterCircle, yCenterCircle) };

	if (distanceBetweenPointAndCircle <= radiusCircle)
	{
		return true;
	}

	return false;

}
bool PowerUp::IsPointInCircle(const Point2f& point, const Circlef& circle) const
{
	return IsPointInCircle(point.x, point.y, circle.center.x, circle.center.y, circle.radius);
}
bool PowerUp::IsRectInCircle(const Rectf& rect, const Circlef& circle) const
{
	if (IsPointInCircle(Point2f{ rect.left,rect.bottom }, circle))
	{
		return true;
	}
	if (IsPointInCircle(Point2f{rect.left + rect.width ,rect.bottom }, circle))
	{
		return true;
	}
	if (IsPointInCircle(Point2f{ rect.left,rect.bottom + rect.height }, circle))
	{
		return true;
	}
	if (IsPointInCircle(Point2f{ rect.left + rect.height,rect.bottom + rect.height }, circle))
	{
		return true;
	}
	return false;
	
}

