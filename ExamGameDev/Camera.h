#pragma once
#include "utils.h"

class Camera final
{
	public:
		Camera(const float width, const float height);
		~Camera() = default;

		void SetLevelBoundaries(const Rectf& levelBoundaries);
		void Transform(const Rectf& target);

		const Point2f& GetPosition() const;

	private:
		const float m_Width;
		const float m_Height;
		Rectf m_LevelBoundaries;

		Point2f m_Position;

		const Point2f& Track(const Rectf& target) const;
		void Clamp(Point2f& bottomLeftPos) const;
};

