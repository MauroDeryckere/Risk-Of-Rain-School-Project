#pragma once
class BasePowerUp;
class PowerUpGenerator;
class Player;
class TextureManager;

#include <vector>
#include "Chest.h"

class PowerUpManager final
{
	public:
		PowerUpManager();
		~PowerUpManager();

		PowerUpManager(const PowerUpManager&) = delete;
		PowerUpManager& operator=(const PowerUpManager&) = delete;
		PowerUpManager(PowerUpManager&&) = delete;
		PowerUpManager& operator=(PowerUpManager&&) = delete;

		void Update(Player* pPlayer, const Uint8* pInput);
		void Draw() const;

		BasePowerUp* GeneratePowerUp(Chest::Rarity rarity, const Rectf& shape, TextureManager* pTextureManager);
		void AddPowerUp(BasePowerUp* pPowerUp);

		void ClearAllPowerUps();

	private:
		PowerUpGenerator* m_pPowerUpGenerator;

		//PowerUps in the level
		std::vector<BasePowerUp*> m_pPowerUps;
};

