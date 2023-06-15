#pragma once
class BasePowerUp;
class TextureManager;

#include "Chest.h"

class PowerUpGenerator final
{
	public:
		static BasePowerUp* GeneratePowerUp(Chest::Rarity chestRarity, const Point2f& position, TextureManager* pTextureManager);

	private:
		static BasePowerUp* GenerateCommonPowerUp(const Point2f& position, TextureManager* pTextureManager);
		static BasePowerUp* GenerateRarePowerUp(const Point2f& position, TextureManager* pTextureManager);
		static BasePowerUp* GenerateLegendaryPowerUp(const Point2f& position, TextureManager* pTextureManager);
};

