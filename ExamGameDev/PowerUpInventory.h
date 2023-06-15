#pragma once
#include <vector>
#include <map>
class  BasePowerUp;
class  Player;
class  Level;
class  TextureManager;
class  Texture;

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
		TextureManager* m_pTextureManager;
		std::vector<BasePowerUp*> m_pPowerUps;
};

