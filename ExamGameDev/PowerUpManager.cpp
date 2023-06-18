#include "pch.h"
#include "PowerUpManager.h"
#include "PowerUpGenerator.h"

#include "BasePowerUp.h"
#include "Player.h"
#include "utils.h"

PowerUpManager::PowerUpManager():
	m_pPowerUps{},
	m_pPowerUpGenerator{ new PowerUpGenerator{} }
{

}

PowerUpManager::~PowerUpManager()
{
	for (size_t index{ 0 }; index < m_pPowerUps.size(); ++index)
	{
		delete m_pPowerUps[index];
	}

	delete m_pPowerUpGenerator;
}

void PowerUpManager::Update(Player* pPlayer, const Uint8* pInput)
{
	for (size_t index{ 0 }; index < m_pPowerUps.size(); ++index)
	{
		if (utils::IsOverlapping(pPlayer->GetShape(), m_pPowerUps[index]->GetShape()) && pInput[SDL_SCANCODE_R])
		{
			pPlayer->CollectPowerUp(m_pPowerUps[index]);

			m_pPowerUps[index] = nullptr;
			m_pPowerUps[index] = m_pPowerUps.back();
			m_pPowerUps.pop_back();
		}
	}
}

void PowerUpManager::Draw() const
{
	for (size_t index{ 0 }; index < m_pPowerUps.size(); ++index)
	{
		m_pPowerUps[index]->Draw();
	}
}

BasePowerUp* PowerUpManager::GeneratePowerUp(Chest::Rarity rarity, const Rectf& shape, TextureManager* pTextureManager)
{
	return m_pPowerUpGenerator->GeneratePowerUp(rarity, shape, pTextureManager);
}

void PowerUpManager::AddPowerUp(BasePowerUp* pPowerUp)
{
	if (pPowerUp)
	{
		m_pPowerUps.emplace_back(pPowerUp);
	}
}

void PowerUpManager::ClearAllPowerUps()
{
	for (size_t index{ 0 }; index < m_pPowerUps.size(); ++index)
	{
		delete m_pPowerUps[index];
	}

	m_pPowerUps.clear();
}
