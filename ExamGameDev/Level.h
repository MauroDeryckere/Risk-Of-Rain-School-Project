#pragma once
#include "utils.h"

class Texture;
class Rope;
class LaunchPad;
class LevelObject;
class BaseEnemy;
class Player;
class TextureManager;
class SoundManager;
class LevelObjectManager;
class TimeObjectManager;
class StopwatchManager;
class EnemyManager;
class BasePowerUp;
class ATGMissile;
class SoundStream;
class SoundEffect;

class LevelHitBullet;

class Level final
{
	public:
		Level(TextureManager* pTextureManager, SoundManager* pSoundManager, LevelObjectManager* pLevelObjectManager, EnemyManager* pEnemyManager, TimeObjectManager* pTimeObjectManager);
		~Level();

		Level(const Level&) = delete;
		Level& operator=(const Level&) = delete;
		Level(Level&&) = delete;
		Level& operator=(Level&&) = delete;

		void Update(float elapsedSec, Player* pPlayer, const Uint8* pInput);

		void Draw(const Point2f& cameraPos) const;

		void HandleCollision(Rectf& characterShape, Vector2f& characterVelocity) const;
		bool IsIntersectingMapOnRope(const Rectf& characterShape) const;
		bool IsOnGround(const Rectf& characterShape) const;

		bool HandleBulletRayCast(const Rectf& characterShape, bool isFacingLeft, unsigned int attackRange, unsigned int attackDamage, unsigned int pierceAmount = 1);

		void DropPowerUp(BasePowerUp* pPowerUp);
		void RemoveChest(LevelObject* pLevelObject);

		BaseEnemy* GetClosestByEnemy(const Rectf& shape);
		bool DeSpawnEnemy(BaseEnemy* pEnemy);

		bool SpawnATGMissile(const Point2f& position, unsigned int attackDamage);
		void DeSpawnATGMissile(ATGMissile* pMissile);

		const Rectf& GetBoundaries() const;

	private:
		//Not owned by Level
		TextureManager* m_pTextureManager;
		SoundManager* m_pSoundManager;
		LevelObjectManager* m_pLevelObjectManager;
		EnemyManager* m_pEnemyManager;
		TimeObjectManager* m_pTimeObjectManager;
		//-----------------

		std::vector<std::vector<Point2f>> m_Vertices;

		const Texture* m_pForeGroundTexture;
		const Texture* m_pStarsBackgroundTexture;
		const Texture* m_pMountainsBackgroundTexture1;
		const Texture* m_pMountainsBackgroundTexture2;
		const Texture* m_pMoonTexture;

		const Rectf m_Boundaries;

		std::vector<BasePowerUp*> m_pPowerUps;

		//Turn into manager or base class in future
		std::vector<ATGMissile*> m_pATGMissiles;
		std::vector<LevelHitBullet*> m_pLevelHitBullets;

		SoundEffect* m_pSpawnMissileSound;
		SoundStream* m_pLevelMusic;

		void InitializeSounds();
		void DeleteSounds();

		void UpdatePowerUps(Player* pPlayer, const Uint8* pInput);

		void DrawBackground(const Point2f& cameraPos) const;
		void DrawForeGround() const;
		void DrawPowerUps() const;
		void DrawATGMissiles() const;

		void HandleHorizontalCollision(Rectf& characterShape, Vector2f& characterVelocity) const;
		void HandleVerticalCollision(Rectf& characterShape, Vector2f& characterVelocity) const;
		void HandleOutOfBoundariesCollision(Rectf& characterShape) const;
};

