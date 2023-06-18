#include "pch.h"
#include "BasePowerUp.h"

#include "Texture.h"

BasePowerUp::BasePowerUp(PowerUps powerUp, Chest::Rarity rarity, const Point2f& position, const Rectf& sourceRect, const Texture* pTexture):
	m_PowerUp{powerUp},
	m_Shape{ position.x, position.y, 68.f, 68.f },
	m_Rarity{ rarity },
	m_SourceRect{ sourceRect },
	m_pTexture{pTexture},
	m_Amount{ 0 }
{

}

BasePowerUp::~BasePowerUp()
{
	m_pTexture = nullptr;
}

void BasePowerUp::Draw() const
{
	m_pTexture->Draw(m_Shape, m_SourceRect);
}

const Rectf& BasePowerUp::GetShape() const
{
	return m_Shape;
}

size_t BasePowerUp::GetIndex() const
{
	return size_t(m_PowerUp);
}

void BasePowerUp::SetBottomLeft(const Point2f& bottomLeft)
{
	m_Shape.left = bottomLeft.x;
	m_Shape.bottom = bottomLeft.y;
}

void BasePowerUp::IncreasePowerUpAmount()
{
	++m_Amount;
}

const size_t BasePowerUp::GetPowerUpAmount() const
{
	return m_Amount;
}

