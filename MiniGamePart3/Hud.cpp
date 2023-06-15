#include "pch.h"
#include "Hud.h"

#include "Texture.h"

Hud::Hud(const Point2f& topLeft, const size_t totalPowerUps) :
	m_TotalPowerUps{ totalPowerUps },
	m_HitPowerUps {0},

	m_pLeftTexture{ new Texture {"Images/HudLeft.png"} },
	m_pRightTexture{ new Texture {"Images/HudRight.png"} },
	m_pPowerUpTexture{ new Texture {"Images/HudPowerUp.png"} }
{
	m_BottomLeft = Point2f{ topLeft.x + 20.f, topLeft.y - m_pLeftTexture->GetHeight() - 20.f };
}

Hud::~Hud()
{
	delete m_pLeftTexture;
	m_pLeftTexture = nullptr;

	delete m_pRightTexture;
	m_pRightTexture = nullptr;

	delete m_pPowerUpTexture;
	m_pPowerUpTexture = nullptr;
}

void Hud::Draw() const
{
	m_pLeftTexture->Draw(m_BottomLeft);

	const float powerUpTextureWidth{ m_pPowerUpTexture->GetWidth() };
	const float powerUpTextureHeight{ m_pPowerUpTexture->GetHeight() };

	Rectf destRect{ m_BottomLeft.x + m_pLeftTexture->GetWidth(), m_BottomLeft.y, powerUpTextureWidth / 2, powerUpTextureHeight };
	Rectf sourceRect{ powerUpTextureWidth / 2, 0.f, powerUpTextureWidth / 2 , powerUpTextureHeight };

	for (size_t index = 1; index <= m_TotalPowerUps; index++)
	{
		if (index <= m_HitPowerUps)
		{
			sourceRect.left = 0.f;

		}
		else
		{
			sourceRect.left = powerUpTextureWidth / 2;
		}
		m_pPowerUpTexture->Draw(destRect, sourceRect);
		destRect.left += powerUpTextureWidth / 2;
	}

	m_pRightTexture->Draw(Point2f{ m_BottomLeft.x + m_pLeftTexture->GetWidth() + m_TotalPowerUps * powerUpTextureWidth / 2, m_BottomLeft.y });
}

void Hud::PowerUpHit()
{
	m_HitPowerUps++;
}
