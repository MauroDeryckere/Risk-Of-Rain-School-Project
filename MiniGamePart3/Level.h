#pragma once
#include "Texture.h"
#include "utils.h"

class Platform;

class Level final
{
	public:
		Level();
		~Level();
		void DrawBackground() const;
		void DrawForeground() const;
		void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
		bool IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const;

		Rectf GetBoundaries() const;

		bool HasReachedEnd(const Rectf& actorShape) const;

	private:
		const Texture* m_pBackgroundTexture;
		const Texture* m_pFenceTexture;
		const Point2f m_FenceBottomLeft;

		const Rectf m_Boundaries;

		const Platform* m_pPlatform;

		const Texture m_EndSignTexture;
		const Rectf m_EndSignShape;

		std::vector<std::vector<Point2f>> m_Vertices;
};