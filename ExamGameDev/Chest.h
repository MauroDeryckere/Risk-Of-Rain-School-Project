#pragma once
#include "LevelObject.h"
#include "Stopwatch.h"

class BasePowerUp;
class Level;
class TextureManager;
class TimeObjectManager;
class PowerUpManager;
class StopwatchManager;

class Chest final:
	public LevelObject
{
	public:
		enum class Rarity
		{
			money = 1,
			common = 2,
			rare = 3,
			legendary = 4
		};

		Chest(const Point2f& bottomLeft, Rarity rarity, PowerUpManager* pPowerUpManager, Level* pLevel, TimeObjectManager* pTimeObjectManager, TextureManager* pTextureManager);
		virtual ~Chest() override;

		Chest(const Chest&) = delete;
		Chest& operator=(const Chest&) = delete;
		Chest(Chest&&) = delete;
		Chest& operator=(Chest&&) = delete;

		virtual void Update(float elapsedSec, Player* pPlayer, const Uint8* pInput) override;
		virtual void Draw() const override;

	private:
		const Rarity m_ChestType;
		bool m_IsOpened;

		Level* m_pLevel;

		BasePowerUp* m_pPowerUp;

		//Not owned by Chest
		TimeObjectManager* m_pTimeObjectManager;
		TextureManager* m_pTextureManager;
		//-------

		StopwatchManager* m_pChestStopwatchManager;
		Stopwatch* m_pDeSpawnStopwatch;

		int m_CostPrice;

		bool m_IsPlayerOverLapping;

		float m_CurrAniTime;

		void InitializeChestSize();
		void GeneratePowerUp(PowerUpManager* pPowerUpManager, TextureManager* pTextureManager);
		void InitializeCostPrice();

		const Rectf& ChangeSourceRect() const;

		void Interact(Player* pPlayer);
		void OpenChest(Player* pPlayer);

		void DrawDroppedMoney() const;
};

