#pragma once
class BasePowerUp;
class TextureManager;

#include "Chest.h"

class PowerUpGenerator final
{
	public:
		BasePowerUp* GeneratePowerUp(Chest::Rarity chestRarity, const Rectf& shape, TextureManager* pTextureManager);

	private:
		BasePowerUp* GenerateCommonPowerUp(const Rectf& shape, TextureManager* pTextureManager);
		BasePowerUp* GenerateRarePowerUp(const Rectf& shape, TextureManager* pTextureManager);
		BasePowerUp* GenerateLegendaryPowerUp(const Rectf& shape, TextureManager* pTextureManager);
};

