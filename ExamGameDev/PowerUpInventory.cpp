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
		for (size_t inventoryIndex{ 0 }; inventoryIndex < m_pPowerUps.size(); ++inventoryIndex)
		{
			if (m_pPowerUps[inventoryIndex] != nullptr)
			{

				glPushMatrix();
					glTranslatef(drawnPowerUps * (powerUpShape.width + spaceBetwPowerUps) - spaceBetwPowerUps, 0, 0);
						utils::SetColor(0.f, 0.f, 0.f, .5f);
						utils::FillRect(powerUpShape);

						m_pPowerUps[inventoryIndex]->Draw();
						m_pTextureManager->DrawNumber(TextureManager::NumberTextures::_20PxWhiteDigits, 
													  Point2f{ powerUpShape.left + textOffset, powerUpShape.bottom + textOffset},
													  m_pPowerUps[inventoryIndex]->GetPowerUpAmount(), 0.f);
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

	const size_t inventoryIndex{ pPowerUp->GetIndex() };

	if (m_pPowerUps.size() < inventoryIndex)
	{
		while (m_pPowerUps.size() < inventoryIndex)
		{
			m_pPowerUps.emplace_back(nullptr);
		}
	}

	if (m_pPowerUps[inventoryIndex] == nullptr)
	{
		m_pPowerUps[inventoryIndex] = pPowerUp;
	}
	else 
	{
		delete pPowerUp;
	}

	m_pPowerUps[inventoryIndex]->IncreasePowerUpAmount();
	m_pPowerUps[inventoryIndex]->SetBottomLeft(Point2f{ 0.f, 0.f });

	return true;
}

void PowerUpInventory::ActivatePowerUps(Player* pPlayer, Level* pLevel) const
{
	for (size_t inventoryIndex{ 0 }; inventoryIndex < m_pPowerUps.size(); ++inventoryIndex)
	{
		if (m_pPowerUps[inventoryIndex] != nullptr) 
		{
			m_pPowerUps[inventoryIndex]->Activate(pPlayer, pLevel);
		}
	}
}