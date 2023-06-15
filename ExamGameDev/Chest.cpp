#include "pch.h"
#include "Chest.h"

#include "BasePowerUp.h"
#include "Texture.h"
#include "Player.h"
#include "Level.h"

#include "PowerUpGenerator.h"
#include "TimeObjectManager.h"
#include "StopwatchManager.h"

#include "utils.h"
#include <iostream>

Chest::Chest(const Point2f& bottomLeft, Rarity rarity, Level* pLevel, TimeObjectManager* pTimeObjectManager, TextureManager* pTextureManager):
	LevelObject{ Rectf{bottomLeft.x, bottomLeft.y, 0.f, 0.f}, pTextureManager },
	m_ChestType{ rarity },
	m_IsOpened{ false },
	m_pLevel{ pLevel },
	m_pPowerUp{ nullptr },
	m_pTimeObjectManager{ pTimeObjectManager },

	m_pChestStopwatchManager{m_pTimeObjectManager->CreateStopwatchManager() },
	m_pDeSpawnStopwatch{ m_pChestStopwatchManager->CreateStopwatch(5.f) }
{
	InitializeChestSize();
	GeneratePowerUp(pTextureManager);
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
	if (utils::IsOverlapping(m_Shape, pPlayer->GetShape()) && pInput[SDL_SCANCODE_E]) 
	{
		Interact(pPlayer);
	}

	if (m_pDeSpawnStopwatch->IsTimeReached())
	{
		m_pLevel->RemoveChest(this);
		return;
	}
}

void Chest::Draw() const
{
	const Rectf sourceRect{ ChangeSourceRect() };
	m_pTexture->Draw(m_Shape, sourceRect);
}

void Chest::Interact(Player* pPlayer)
{
	if (!m_IsOpened)
	{
		OpenChest(pPlayer);
		m_pDeSpawnStopwatch->Start();
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

void Chest::GeneratePowerUp(TextureManager* pTextureManager)
{
	const Point2f powerUpPosition{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };

	if (m_ChestType != Rarity::money)
	{
		m_pPowerUp = PowerUpGenerator::GeneratePowerUp(m_ChestType, powerUpPosition, pTextureManager);
	}
	else
	{
		m_pPowerUp = nullptr;
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
