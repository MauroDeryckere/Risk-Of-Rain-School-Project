#include "pch.h"
#include "Camera.h"

Camera::Camera(float width, float height) :
	m_Width{ width },
	m_Height{ height },
	m_LevelBoundaries{ Rectf{0,0,width,height} }
{
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::Transform(const Rectf& target)
{
	//position camera so that rect target = center (track);
	m_Position = Track(target);

	//Then adjust camera to level boundaries (clamp) if out of bounds
	Clamp(m_Position);
	glTranslatef(-m_Position.x, -m_Position.y, 0);
}

const Point2f& Camera::GetPosition() const
{
	return m_Position;
}

const Point2f& Camera::Track(const Rectf& target) const
{
	//TRACKS PLAYER RECT, center it around the player and then return the bottomleft position for that
	const Point2f bottomLeft{ (target.left + target.width / 2) - m_Width / 2, 
							  (target.bottom + target.height / 2) - m_Height / 2 };

	return bottomLeft;
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	const float	left{ bottomLeftPos.x },
				right{ bottomLeftPos.x + m_Width },
				top {bottomLeftPos.y + m_Height},
				bottom{bottomLeftPos.y};

	//Correct camera position so that it doesn't leave level boundaries
	if (left < m_LevelBoundaries.left)
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}
	if (right > m_LevelBoundaries.left + m_LevelBoundaries.width)
	{
		bottomLeftPos.x = m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width;
	}
	if (bottom < m_LevelBoundaries.bottom)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}
	if (top > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height;
	}
}
