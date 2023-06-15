#include "pch.h"
#include "PowerUpGenerator.h"
#include "TextureManager.h"

#include "BasePowerUp.h"

#include "HealthBoostPowerUp.h"
#include "MovementSpeedBoostPowerUp.h"

#include "ATGPowerUp.h"

#include <cassert>
#include <iostream>

BasePowerUp* PowerUpGenerator::GeneratePowerUp(Chest::Rarity chestRarity, const Point2f& position, TextureManager* pTextureManager)
{
    assert(chestRarity != Chest::Rarity::money);

    BasePowerUp* pPowerUp{nullptr};

	constexpr float commonSpawnChance{ 100 };
    constexpr float rareSpawnChance{ 20 };

    constexpr unsigned int max{ 100 };
    constexpr unsigned int min{ 1 };

    const unsigned int randomNumber = rand() % (max - min + 1) + min;


    switch (chestRarity)
    {
    case Chest::Rarity::common:

        if (randomNumber <= rareSpawnChance)
        {
            pPowerUp = GenerateRarePowerUp(position, pTextureManager);
        }
        else if (randomNumber <= commonSpawnChance)
        {
            pPowerUp = GenerateCommonPowerUp(position, pTextureManager);
        }
        else
        {
            std::cerr << "ERROR in generating a powerUp (chance)\n";
        }
        break;

    case Chest::Rarity::rare:
        pPowerUp = GenerateRarePowerUp(position, pTextureManager);
        break;

    case Chest::Rarity::legendary:
        pPowerUp = GenerateLegendaryPowerUp(position, pTextureManager);
        break;
    }

    return pPowerUp;
}

BasePowerUp* PowerUpGenerator::GenerateCommonPowerUp(const Point2f& position, TextureManager* pTextureManager)
{
    BasePowerUp* pPowerUp{ nullptr };
    const Texture* pCommonPowerUpTexture{ pTextureManager->CreateTexture("PowerUps/CommonPowerUps.png", "commonPowerUp")};

    constexpr unsigned int max{ 2 };
    constexpr unsigned int min{ 1 };

    const unsigned int randomNumber = rand() % (max - min + 1) + min;

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

BasePowerUp* PowerUpGenerator::GenerateRarePowerUp(const Point2f& position, TextureManager* pTextureManager)
{
    BasePowerUp* pPowerUp{ nullptr };
    const Texture* pRarePowerUpTexture{ pTextureManager->CreateTexture("PowerUps/RarePowerUps.png", "rarePowerUp")};

    constexpr unsigned int max{ 1 };
    constexpr unsigned int min{ 1 };

    const unsigned int randomNumber = rand() % (max - min + 1) + min;

    switch (randomNumber)
    {
    case 1:
        pPowerUp = new ATGPowerUp{ position, pRarePowerUpTexture };
        break;
    }

    return pPowerUp;
}

BasePowerUp* PowerUpGenerator::GenerateLegendaryPowerUp(const Point2f& position, TextureManager* pTextureManager)
{
    BasePowerUp* pPowerUp{ nullptr };
    const Texture* pRarePowerUpTexture{ pTextureManager->CreateTexture("PowerUps/RarePowerUps.png", "legendaryPowerUps") }; //Change to legandary in future

    constexpr unsigned int max{ 1 };
    constexpr unsigned int min{ 1 };

    const unsigned int randomNumber = rand() % (max - min + 1) + min;

    switch (randomNumber)
    {
    case 1:
        pPowerUp = new ATGPowerUp{ position, pRarePowerUpTexture }; //Temp, legendary powerUps here in future
        break;
    }

    return pPowerUp;
}
