#pragma once
#include "utils.h"
class Texture;

class Level final
{
	public:
		Level();
		~Level();
		void DrawBackground() const;
		void DrawForeground() const;
		void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
		bool IsOnGround(const Rectf& actorShape) const;

		Rectf GetBoundaries() const;

	private:
		const std::vector<Point2f> m_Vertices;
		const Texture* m_pBackgroundTexture;
		const Texture* m_pFenceTexture;
		const Point2f m_FenceBottomLeft;

		const Rectf m_Boundaries;
};