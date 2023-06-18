#include "pch.h"
#include "Chest.h"

#include "BasePowerUp.h"
#include "Texture.h"
#include "Player.h"
#include "Level.h"

#include "PowerUpManager.h"
#include "TimeObjectManager.h"
#include "StopwatchManager.h"
#include "TextureManager.h"

#include "utils.h"
#include <iostream>

Chest::Chest(const Point2f& bottomLeft, Rarity rarity, PowerUpManager* pPowerUpManager, Level* pLevel, TimeObjectManager* pTimeObjectManager, TextureManager* pTextureManager) :
	LevelObject{ Rectf{bottomLeft.x, bottomLeft.y, 0.f, 0.f}, pTextureManager },
	m_ChestType{ rarity },
	m_IsOpened{ false },
	m_pLevel{ pLevel },
	m_pPowerUp{ nullptr },
	m_pTimeObjectManager{ pTimeObjectManager },
	m_pTextureManager{ pTextureManager },

	m_pChestStopwatchManager{ m_pTimeObjectManager->CreateStopwatchManager() },
	m_pDeSpawnStopwatch{ m_pChestStopwatchManager->CreateStopwatch(5.f) },

	m_IsPlayerOverLapping{ false },

	m_CurrAniTime{ 0.f }
{
	InitializeChestSize();
	GeneratePowerUp(pPowerUpManager, pTextureManager);
	InitializeCostPrice();
}

Chest::~Chest()
{
	if (!m_IsOpened)
	{
		delete m_pPowerUp;
	}

	m_pTimeObjectManager->DeleteStopwatchManager(m_pChestStopwatchManager);
}

void Chest::Update(float elapsedSec, Player* pPlayer, const Uint8* pInput)
{
	m_IsPlayerOverLapping = utils::IsOverlapping(m_Shape, pPlayer->GetShape());
	if(m_IsPlayerOverLapping && pInput[SDL_SCANCODE_E])
	{
		Interact(pPlayer);
	}

	if (m_pDeSpawnStopwatch->IsTimeReached())
	{
		m_pLevel->RemoveChest(this);
		return;
	}

	if (m_IsOpened)
	{
		m_CurrAniTime += elapsedSec;

		if (m_CurrAniTime >= 3.f)
		{
			m_CostPrice = 0;
		}
	}
}

void Chest::Draw() const
{
	const Rectf sourceRect{ ChangeSourceRect() };
	m_pTexture->Draw(m_Shape, sourceRect);

	if (m_ChestType != Rarity::money && !m_IsOpened)
	{
		if (m_IsPlayerOverLapping)
		{
			float numWidth{};
			m_pTextureManager->DrawNumber(TextureManager::NumberTextures::_20PxWhiteDigits, Point2f{ m_Shape.left, m_Shape.bottom + m_Shape.height }, m_CostPrice, 0, numWidth);
			m_pTextureManager->DrawSymbol(TextureManager::NumberTextures::_20PxWhiteDigits, Point2f{ m_Shape.left + numWidth, m_Shape.bottom + m_Shape.height }, TextureManager::Symbols::Dollar, 0, numWidth);
		}
	}

	if (m_ChestType == Rarity::money && m_IsOpened)
	{
		DrawDroppedMoney();
	}
}

void Chest::Interact(Player* pPlayer)
{
	if (!m_IsOpened)
	{
		if (m_CostPrice > 0)
		{
			if (pPlayer->GetBalance() >= m_CostPrice)
			{
				pPlayer->UpdateBalance(-m_CostPrice);
				OpenChest(pPlayer);
				m_pDeSpawnStopwatch->Start();
			}
			else
			{
				//std::cout << "balance not high enough \n";
			}
		}
		else
		{
			pPlayer->UpdateBalance(-m_CostPrice);
			OpenChest(pPlayer);
			m_pDeSpawnStopwatch->Start();
		}
	}
	else
	{
		//std::cout << "chest already opened\n";
	}
}


void Chest::OpenChest(Player* pPlayer)
{
	m_IsOpened = true;

	if (m_pPowerUp)
	{
		m_pLevel->DropPowerUp(m_pPowerUp);
	}

	m_pPowerUp = nullptr;
}

void Chest::DrawDroppedMoney() const
{
	if (m_ChestType == Rarity::money)
	{
		if (m_CostPrice != 0)
		{
			constexpr float moneyAniDistance{ 8.f };
			const Point2f bottomLeft{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 + (m_CurrAniTime * moneyAniDistance) };

			float numWidth{};

			m_pTextureManager->DrawNumber(TextureManager::NumberTextures::_20PxGreenDigits, bottomLeft, -m_CostPrice, 0.f, numWidth);
			m_pTextureManager->DrawSymbol(TextureManager::NumberTextures::_20PxGreenDigits, Point2f{ bottomLeft.x + numWidth, bottomLeft.y }, TextureManager::Symbols::Dollar, 0.f);
		}
	}
}

void Chest::InitializeChestSize()
{
	float width{}, height{};

	switch (m_ChestType)
	{
	case Rarity::money:
		width = 11.f;
		height = 11.f;
		break;

	case Rarity::common:
		width = 15.f;
		height = 14.f;
		break;

	case Rarity::rare:
		width = 19.f;
		height = 16.f;
		break;

	case Rarity::legendary:
		width = 23.f;
		height = 16.f;
		break;
	}

	m_Shape.width = width;
	m_Shape.height = height;
}

void Chest::GeneratePowerUp(PowerUpManager* pPowerUpManager, TextureManager* pTextureManager)
{
	if (m_ChestType != Rarity::money)
	{
		m_pPowerUp = pPowerUpManager->GeneratePowerUp(m_ChestType, m_Shape, pTextureManager);
	}
	else
	{
		m_pPowerUp = nullptr;
	}
}

void Chest::InitializeCostPrice()
{
	switch (m_ChestType)
	{
	case Rarity::money:
		m_CostPrice = utils::GetRandomNumber(-25, -10);
		break;
	case Rarity::common:
		m_CostPrice = 18.f;
		break;
	case Rarity::rare:
		m_CostPrice = 35.f;
		break;
	case Rarity::legendary:
		m_CostPrice = 122.f;
		break;
	}
}

const Rectf& Chest::ChangeSourceRect() const
{
	Rectf sourceRect{};

	constexpr float offset{ 4.f };

	switch (m_ChestType)
	{
	case Rarity::money:
		sourceRect = Rectf{ offset, 135.f, m_Shape.width, m_Shape.height};
		break;
	case Rarity::common:
		sourceRect = Rectf{ offset, 121.f, m_Shape.width, m_Shape.height};
		break;
	case Rarity::rare:
		sourceRect = Rectf{ offset, 103.f, m_Shape.width, m_Shape.height};
		break;
	case Rarity::legendary:
		sourceRect = Rectf{ offset + m_IsOpened * offset + m_IsOpened * 23.f, 83.f, m_Shape.width, m_Shape.height };
		break;
	}
	return sourceRect;
}
