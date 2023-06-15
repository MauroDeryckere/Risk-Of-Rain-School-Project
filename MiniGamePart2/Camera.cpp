#include "pch.h"
#include "Camera.h"

Camera::Camera(float width, float height) :
	m_Width {width},
	m_Height{height},
	m_LevelBoundaries{ Rectf{0,0,width,height} }
{
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::Transform(const Rectf& target) const
{
	//position camera so that rect target = center (track);
	Point2f position{ Track(target) };

	//Then adjust camera to level boundaries (clamp) if out of bounds
	Clamp(position);

		//Draw blue rectangle at position with dimensions of camera
		//utils::SetColor(Color4f{ 0.f,0.f,1.f,1.f });
		//utils::DrawRect(position ,m_Width ,m_Height );
	glTranslatef(-position.x, -position.y, 0);
}

Point2f Camera::Track(const Rectf& target) const
{
	//TRACKS PLAYER RECT, center it around the player and then return the bottomleft position for that
	Point2f bottomLeft{};
	
	bottomLeft.x = (target.left + target.width / 2);
	bottomLeft.y = (target.bottom + target.height / 2);

	bottomLeft.x -= m_Width / 2;
	bottomLeft.y -= m_Height / 2;

	return bottomLeft;
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	Point2f bottomRightPos{ bottomLeftPos.x + m_Width ,bottomLeftPos.y},
			topLeftPos	  { bottomLeftPos.x, bottomLeftPos.y + m_Height},
			topRightPos	  { bottomRightPos.x, bottomRightPos.y + m_Height };

	//Correct camera position so that it doesn't leave level boundaries
	if (!utils::IsPointInRect(bottomLeftPos, m_LevelBoundaries))
	{
		if (bottomLeftPos.x < m_LevelBoundaries.left)
		{
			bottomLeftPos.x = m_LevelBoundaries.left;
		}

		if (bottomLeftPos.y < m_LevelBoundaries.bottom)
		{
			bottomLeftPos.y = m_LevelBoundaries.bottom;
		}
	}

	if (!utils::IsPointInRect(bottomRightPos, m_LevelBoundaries))
	{
		if (bottomLeftPos.x + m_Width > m_LevelBoundaries.left + m_LevelBoundaries.width )
		{
			bottomLeftPos.x = (m_LevelBoundaries.left + m_LevelBoundaries.width) - m_Width;
		}
		
		if (bottomLeftPos.y < m_LevelBoundaries.bottom)
		{
			bottomLeftPos.y = m_LevelBoundaries.bottom;
		}
	}

	if (!utils::IsPointInRect(topLeftPos, m_LevelBoundaries))
	{
		if (bottomLeftPos.x < m_LevelBoundaries.left)
		{
			bottomLeftPos.x = m_LevelBoundaries.left;
		}

		if (bottomLeftPos.y + m_Height > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
		{
			bottomLeftPos.y = (m_LevelBoundaries.bottom + m_LevelBoundaries.height) - m_Height;
		}
	}

	if (!utils::IsPointInRect(topRightPos, m_LevelBoundaries))
	{
		if (bottomLeftPos.x + m_Width > m_LevelBoundaries.left + m_LevelBoundaries.width)
		{
			bottomLeftPos.x = (m_LevelBoundaries.left + m_LevelBoundaries.width) - m_Width;
		}

		if (bottomLeftPos.y + m_Height > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
		{
			bottomLeftPos.y = (m_LevelBoundaries.bottom + m_LevelBoundaries.height) - m_Height;
		}
	}
}
