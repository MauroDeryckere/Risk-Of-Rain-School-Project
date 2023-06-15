#pragma once

class Texture;

class Hud final
{
	public:
		Hud(const Point2f& topLeft, const size_t totalPowerUps);
		~Hud();
		void Draw() const;
		void PowerUpHit();

	private:
		Point2f m_BottomLeft;
		const size_t m_TotalPowerUps;
		size_t m_HitPowerUps;

		Texture* m_pLeftTexture;
		Texture* m_pRightTexture;
		Texture* m_pPowerUpTexture;
};

