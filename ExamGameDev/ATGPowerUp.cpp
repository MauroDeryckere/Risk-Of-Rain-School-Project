#include "pch.h"
#include "ATGPowerUp.h"
#include "Player.h"
#include "Level.h"

#include <iostream>

ATGPowerUp::ATGPowerUp(const Point2f& position, const Texture* pTexture):
	BasePowerUp { PowerUps::ATG, Chest::Rarity::rare, position, Rectf{ 71.f,35.f,34.f,34.f }, pTexture },
	m_ActivationChance{ 10 }
{

}

void ATGPowerUp::Activate(Player* pPlayer, Level* pLevel)
{
	constexpr size_t attackDamage{ 25 };

	bool isTriggered{ false };

	if (pPlayer->CanFireATG())
	{
		constexpr size_t min{ 1 };
		constexpr size_t max{ 100 };

		if (utils::GetRandomNumber(min, max) <= m_ActivationChance)
		{
			isTriggered = true;
		}
	}

	if (isTriggered)
	{
		const Rectf& playerShape{ pPlayer->GetShape() };

		if (pLevel->SpawnATGMissile(Point2f{ playerShape.left, playerShape.bottom + playerShape.height / 2 }, 25))
		{
			//std::cout << "ATG fired \n";
		}

	}
}
void ATGPowerUp::IncreasePowerUpAmount()
{
	m_Amount++;
	m_ActivationChance = std::min(int(m_ActivationChance+10), 100);
}
