#pragma once
class  BasePowerUp;
class  Player;
class  Level;
class  TextureManager;
class  Texture;

#include <vector>

class PowerUpInventory final
{
	public:
		PowerUpInventory(TextureManager* pTextureManager, size_t initSize);
		~PowerUpInventory();

		PowerUpInventory(const PowerUpInventory&) = delete;
		PowerUpInventory& operator=(const PowerUpInventory&) = delete;
		PowerUpInventory(PowerUpInventory&&) = delete;
		PowerUpInventory& operator=(PowerUpInventory&&) = delete;

		void Draw(float windowWidth, float windowheight) const;
		bool AddPowerUp(BasePowerUp* pPowerUp);

		void ActivatePowerUps(Player* pPlayer, Level* plevel) const;

	private:
		//Not owned by powwerUpInventory
		TextureManager* m_pTextureManager;
		//----------

		std::vector<BasePowerUp*> m_pPowerUps;
};

