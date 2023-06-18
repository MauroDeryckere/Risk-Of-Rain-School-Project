#include "pch.h"
#include "PowerUpGenerator.h"
#include "TextureManager.h"

#include "BasePowerUp.h"

#include "HealthBoostPowerUp.h"
#include "MovementSpeedBoostPowerUp.h"

#include "ATGPowerUp.h"
#include "Texture.h"
#include "utils.h"

#include <cassert>
#include <iostream>

BasePowerUp* PowerUpGenerator::GeneratePowerUp(Chest::Rarity chestRarity, const Rectf& shape, TextureManager* pTextureManager)
{
    assert(chestRarity != Chest::Rarity::money);

    BasePowerUp* pPowerUp{nullptr};

	constexpr size_t commonSpawnChance{ 100 };
    constexpr size_t rareSpawnChance{ 20 };

    constexpr size_t min{ 1 };
    constexpr size_t max{ 100 };

    const size_t randomNumber{ size_t(utils::GetRandomNumber(min, max)) };

    switch (chestRarity)
    {
    case Chest::Rarity::common:

        if (randomNumber <= rareSpawnChance)
        {
            pPowerUp = GenerateRarePowerUp(shape, pTextureManager);
        }
        else if (randomNumber <= commonSpawnChance)
        {
            pPowerUp = GenerateCommonPowerUp(shape, pTextureManager);
        }
        else
        {
            std::cerr << "ERROR in generating a powerUp (chance)\n";
        }
        break;

    case Chest::Rarity::rare:
        pPowerUp = GenerateRarePowerUp(shape, pTextureManager);
        break;

    case Chest::Rarity::legendary:
        pPowerUp = GenerateLegendaryPowerUp(shape, pTextureManager);
        break;
    }

    return pPowerUp;
}

BasePowerUp* PowerUpGenerator::GenerateCommonPowerUp(const Rectf& shape, TextureManager* pTextureManager)
{
    BasePowerUp* pPowerUp{ nullptr };
    const Texture* pCommonPowerUpTexture{ pTextureManager->CreateTexture("PowerUps/CommonPowerUps.png", "commonPowerUp")};

    constexpr size_t min{ 1 };
    constexpr size_t max{ 2 };

    const size_t randomNumber{ size_t(utils::GetRandomNumber(min, max)) };

    const Point2f position{ shape.left + shape.width/2 - 68.f / 2,  shape.bottom};

    switch (randomNumber)
    {
    case 1:
        pPowerUp = new HealthBoostPowerUp{ position, pCommonPowerUpTexture };
        break;
    case 2:
        pPowerUp = new MovementSpeedBoostPowerUp{ position, pCommonPowerUpTexture };
        break;
    }

    return pPowerUp;
}

BasePowerUp* PowerUpGenerator::GenerateRarePowerUp(const Rectf& shape, TextureManager* pTextureManager)
{
    BasePowerUp* pPowerUp{ nullptr };
    const Texture* pRarePowerUpTexture{ pTextureManager->CreateTexture("PowerUps/RarePowerUps.png", "rarePowerUp")};

    const Point2f position{ shape.left + shape.width / 2 - 68.f / 2,  shape.bottom};

    constexpr size_t min{ 1 };
    constexpr size_t max{ 1 };

    const size_t randomNumber{ size_t(utils::GetRandomNumber(min, max)) };

    switch (randomNumber)
    {
    case 1:
        pPowerUp = new ATGPowerUp{ position, pRarePowerUpTexture };
        break;
    }

    return pPowerUp;
}

BasePowerUp* PowerUpGenerator::GenerateLegendaryPowerUp(const Rectf& shape, TextureManager* pTextureManager)
{
    BasePowerUp* pPowerUp{ nullptr };
    const Texture* pRarePowerUpTexture{ pTextureManager->CreateTexture("PowerUps/RarePowerUps.png", "legendaryPowerUps") };

    const Point2f position{ shape.left + shape.width / 2 - 68.f / 2,  shape.bottom };

    constexpr size_t min{ 1 };
    constexpr size_t max{ 1 };

    const size_t randomNumber{ size_t(utils::GetRandomNumber(min, max)) };

    switch (randomNumber)
    {
    case 1:
        pPowerUp = new ATGPowerUp{ position, pRarePowerUpTexture }; 
        break;
    }

    return pPowerUp;
}
