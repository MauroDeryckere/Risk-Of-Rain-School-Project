#include "pch.h"
#include "Hud.h"

#include "Player.h"
#include "Texture.h"
#include "SoundEffect.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Timer.h"
#include "utils.h"
#include <vector>
#include <iostream>

#define digTexture TextureManager::NumberTextures::_20PxWhiteDigits


Hud::Hud(float windowWidth, float windowHeight, const Timer* pGameTimer, SoundManager* pSoundManager, TextureManager* pTextureManager) :
	m_pTextureManager{ pTextureManager },
	m_pSoundManager{ pSoundManager },
	m_pGameTimer{ pGameTimer },

	m_WindowWidth{windowWidth},
	m_WindowHeight{ windowHeight }

{
	InitializeTextures();
}

Hud::~Hud()
{

}

void Hud::Draw(const Player* pPlayer) const
{
	DrawBalance();
	DrawTime();
	DrawDifficulty();
	DrawInventory(pPlayer);
	DrawAbilities(pPlayer);
	DrawHealth(pPlayer);
	DrawLevel();
}

void Hud::InitializeTextures()
{
	
}

void Hud::DrawBalance() const
{

}

void Hud::DrawTime() const
{
	//TODO: magic numbers

	Point2f bottomLeft{m_WindowWidth - 60.F, m_WindowHeight - 30.f};

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

void Hud::DrawDifficulty() const
{

}

void Hud::DrawInventory(const Player* pPlayer) const
{
	pPlayer->DrawInventory(m_WindowWidth, m_WindowHeight);
}

void Hud::DrawAbilities(const Player* pPlayer) const
{
	pPlayer->DrawAbilities(m_WindowWidth, m_WindowHeight);
}

void Hud::DrawHealth(const Player* pPlayer) const
{
	pPlayer->DrawHealth(m_WindowWidth, m_WindowHeight);
}

void Hud::DrawLevel() const
{
	constexpr float levelBarWidth{ 200.f };
	constexpr float levelBarHeight{ 10.f };

	//Experience
	utils::SetColor(Color4f{ 0.f,0.f,1.f,1.f });
	utils::FillRect(m_WindowWidth/2 - levelBarWidth/2, 20.f, levelBarWidth, levelBarHeight);

	//Background
	utils::SetColor(Color4f{ 0.f,0.f,0.f,.5f });
	utils::FillRect(m_WindowWidth / 2 - levelBarWidth / 2, 20.f, levelBarWidth, levelBarHeight);
}
