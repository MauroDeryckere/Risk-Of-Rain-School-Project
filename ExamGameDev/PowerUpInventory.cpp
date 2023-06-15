#include "pch.h"

#include "PowerUpInventory.h"
#include "BasePowerUp.h"
#include "utils.h"
#include "TextureManager.h"
#include "PowerUpGenerator.h"

#include <iostream>

#include "Player.h"
#include "Texture.h"
#include "Level.h"

PowerUpInventory::PowerUpInventory(TextureManager* pTextureManager, size_t initSize) :
	m_pTextureManager{pTextureManager}
{
	m_pPowerUps.reserve(initSize);

	while (m_pPowerUps.size() < initSize)
	{
		m_pPowerUps.emplace_back(nullptr);
	}

	//tests
	for (size_t i = 1; i <= 100; i++)
	{
		AddPowerUp(PowerUpGenerator::GeneratePowerUp(Chest::Rarity::rare, Point2f{ 0.f,0.f }, m_pTextureManager));
	}
}

PowerUpInventory::~PowerUpInventory()
{
	for (size_t index{ 0 }; index < m_pPowerUps.size(); ++index)
	{
		delete m_pPowerUps[index];
	}
}

void PowerUpInventory::Draw(float windowWidth, float windowHeight) const
{
	if (m_pPowerUps.empty())
	{
		return;
	}

	Rectf powerUpShape{};

	for (size_t index{ 0 }; index < m_pPowerUps.size(); ++index)
	{
		if (m_pPowerUps[index] != nullptr)
		{
			powerUpShape = m_pPowerUps[index]->GetShape();
			break;
		}
	}

	constexpr float spaceBetwPowerUps{ 5.f };
	constexpr float textOffset{ 2.f };

	size_t drawnPowerUps{ 0 };

	glPushMatrix();
		glTranslatef(20.f, 20.f, 0.f);
		for (size_t index{ 0 }; index < m_pPowerUps.size(); ++index)
		{
			if (m_pPowerUps[index] != nullptr)
			{
				glPushMatrix();
					glTranslatef(drawnPowerUps * (powerUpShape.width + spaceBetwPowerUps) - spaceBetwPowerUps, 0, 0);
						m_pPowerUps[index]->Draw();
						m_pTextureManager->DrawNumber(TextureManager::NumberTextures::_20PxWhiteDigits, 
													  Point2f{ powerUpShape.left + textOffset, powerUpShape.bottom + textOffset},
													  m_pPowerUps[index]->GetPowerUpAmount(), 0.f);
				glPopMatrix();

				++drawnPowerUps;
			}
		}
	glPopMatrix();
}

bool PowerUpInventory::AddPowerUp(BasePowerUp* pPowerUp)
{
	if (pPowerUp == nullptr)
	{
		return false;
	}

	const size_t index = pPowerUp->GetIndex();

	if (m_pPowerUps.size() < index)
	{
		while (m_pPowerUps.size() < index)
		{
			m_pPowerUps.emplace_back(nullptr);
		}
	}

	if (m_pPowerUps[index] == nullptr)
	{
		m_pPowerUps[index] = pPowerUp;
	}
	else 
	{
		delete pPowerUp;
	}

	m_pPowerUps[index]->IncreasePowerUpAmount();
	m_pPowerUps[index]->SetBottomLeft(Point2f{ 0.f, 0.f });

	return true;
}

void PowerUpInventory::ActivatePowerUps(Player* pPlayer, Level* pLevel) const
{
	for (size_t index{ 0 }; index < m_pPowerUps.size(); ++index)
	{
		if (m_pPowerUps[index] != nullptr)
		{
			m_pPowerUps[index]->Activate(pPlayer, pLevel);
		}
	}
}