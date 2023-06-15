#pragma once
#include "utils.h"
class Texture;

class Platform 
{
	public:
		Platform(const Point2f& bottomLeft);
		~Platform();
		void Draw() const;
		void HandleCollission(Rectf& actorShape, Vector2f& actorVelocity) const;
		bool IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const;

	private:
		const Texture* m_pTexture;
		Rectf m_Shape;
};

