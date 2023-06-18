#include "pch.h"
#include "Hud.h"

#include "Player.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Timer.h"
#include "utils.h"
#include <iostream>

#define digTexture TextureManager::NumberTextures::_20PxWhiteDigits


Hud::Hud(float windowWidth, float windowHeight, const Timer* pGameTimer, const TextureManager* pTextureManager) :
	m_pTextureManager{ pTextureManager },
	m_pGameTimer{ pGameTimer },

	m_WindowWidth{windowWidth},
	m_WindowHeight{ windowHeight }

{
}


void Hud::Draw(const Player* pPlayer) const
{
	DrawBalance(pPlayer);
	DrawTime();
	DrawInventory(pPlayer);
	pPlayer->DrawPlayerStatsHud(m_WindowWidth, m_WindowHeight);
}

void Hud::DrawBalance(const Player* pPLayer) const
{	
	const float scale{ m_WindowHeight / 720.f };

	Point2f bottomLeft{ m_WindowWidth - 60.f * scale, m_WindowHeight - 60.f * scale };
	
	float numWidth{};

	m_pTextureManager->DrawNumber(digTexture, bottomLeft, pPLayer->GetBalance(), 0.f, numWidth);
	bottomLeft.x += numWidth;
	m_pTextureManager->DrawSymbol(digTexture, bottomLeft, TextureManager::Symbols::Dollar, 0.f);
}

void Hud::DrawTime() const
{
	const float scale{ m_WindowHeight / 720.f };

	Point2f bottomLeft{m_WindowWidth - 60.f * scale, m_WindowHeight - 30.f * scale };

	float numWidth{};

	float fullTimeSeconds{ m_pGameTimer->GetTime() };

	const size_t minutes{ size_t(fullTimeSeconds) / 60 };

	fullTimeSeconds -= minutes * 60;


	if (minutes == 0)
	{
		m_pTextureManager->DrawNumber(digTexture, bottomLeft, 0, 0.f, numWidth);
		bottomLeft.x += numWidth;
		m_pTextureManager->DrawNumber(digTexture, bottomLeft, 0, 0.f, numWidth);
		bottomLeft.x += numWidth;
	}
	else if (minutes < 10)
	{
		m_pTextureManager->DrawNumber(digTexture, bottomLeft, 0, 0.f, numWidth);
		bottomLeft.x += numWidth;
		m_pTextureManager->DrawNumber(digTexture, bottomLeft, minutes, 0.f, numWidth);
		bottomLeft.x += numWidth;
	}
	else
	{
		m_pTextureManager->DrawNumber(digTexture, bottomLeft, minutes, 0.f, numWidth);
		bottomLeft.x += numWidth;
	}


	m_pTextureManager->DrawSymbol(digTexture, bottomLeft, TextureManager::Symbols::Colon, 0.f, numWidth);
	bottomLeft.x += numWidth;

	if (fullTimeSeconds < 1.f)
	{
		m_pTextureManager->DrawNumber(digTexture, bottomLeft, 0, 0.f, numWidth);
		bottomLeft.x += numWidth;
		m_pTextureManager->DrawNumber(digTexture, bottomLeft, 0, 0.f, numWidth);
		bottomLeft.x += numWidth;
	}
	else if (fullTimeSeconds < 10.f)
	{
		m_pTextureManager->DrawNumber(digTexture, bottomLeft, 0, 0.f, numWidth);
		bottomLeft.x += numWidth;
		m_pTextureManager->DrawNumber(digTexture, bottomLeft, size_t(fullTimeSeconds), 0.f, numWidth);
		bottomLeft.x += numWidth;
	}
	else
	{
		m_pTextureManager->DrawNumber(digTexture, bottomLeft, size_t(fullTimeSeconds), 0.f, numWidth);
		bottomLeft.x += numWidth;
	}

	#undef digTexture
}

void Hud::DrawInventory(const Player* pPlayer) const
{
	pPlayer->DrawInventory(m_WindowWidth, m_WindowHeight);
}