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
class PowerUpManager;
class BasePowerUp;
class ATGMissile;
class SoundStream;
class SoundEffect;

class Level final
{
public:
	Level(const Point2f& playerSpawnPos, PowerUpManager* pPowerUpManager, TextureManager* pTextureManager, SoundManager* pSoundManager, LevelObjectManager* pLevelObjectManager, EnemyManager* pEnemyManager, TimeObjectManager* pTimeObjectManager);
	~Level();

	Level(const Level&) = delete;
	Level& operator=(const Level&) = delete;
	Level(Level&&) = delete;
	Level& operator=(Level&&) = delete;

	void Update(float elapsedSec, Player* pPlayer, const Uint8* pInput);

	void Draw(const Point2f& cameraPos) const;

	void HandleCollision(Rectf& characterShape, Vector2f& characterVelocity, BaseEnemy* pEnemy, Player* pPlayer = nullptr, const Point2f& lastPosOnGround = Point2f{ 0.f,0.f }) const;
	bool IsIntersectingMapOnRope(const Rectf& characterShape) const;
	bool IsOnGround(const Rectf& characterShape) const;

	bool HandleBulletRayCast(Player* pPlayer, const Rectf& characterShape, bool isFacingLeft, size_t attackRange, size_t attackDamage, size_t pierceAmount = 1);

	void DropPowerUp(BasePowerUp* pPowerUp);
	void RemoveChest(LevelObject* pLevelObject);

	BaseEnemy* GetClosestByEnemy(const Rectf& shape);
	bool DeSpawnEnemy(BaseEnemy* pEnemy);

	bool SpawnATGMissile(const Point2f& position, size_t attackDamage);
	void DestroyATGMissile(ATGMissile* pMissile);

	const Rectf& GetBoundaries() const;
	const Point2f& GetSpawnPosition() const;

private:
	const Point2f m_PlayerSpawnPos;

	//Not owned by Level
	TextureManager* m_pTextureManager;
	SoundManager* m_pSoundManager;
	LevelObjectManager* m_pLevelObjectManager;
	EnemyManager* m_pEnemyManager;
	TimeObjectManager* m_pTimeObjectManager;
	PowerUpManager* m_pPowerUpManager;
	//-----------------

	std::vector<std::vector<Point2f>> m_Vertices;

	const Texture* m_pForeGroundTexture;
	const Texture* m_pStarsBackgroundTexture;
	const Texture* m_pMountainsBackgroundTexture1;
	const Texture* m_pMountainsBackgroundTexture2;
	const Texture* m_pMoonTexture;

	const Rectf m_Boundaries;

	std::vector<ATGMissile*> m_pATGMissiles;

	SoundEffect* m_pSpawnMissileSound;
	SoundStream* m_pLevelMusic;

	void InitializeSounds();
	void DeleteTextures();
	void DeleteSounds();

	void DrawBackground(const Point2f& cameraPos) const;
	void DrawForeGround() const;
	void DrawATGMissiles() const;

	void HandleHorizontalCollision(Rectf& characterShape, Vector2f& characterVelocity) const;
	void HandleVerticalCollision(Rectf& characterShape, Vector2f& characterVelocity) const;
	void HandleOutOfBoundariesCollision(Rectf& characterShape, Vector2f& characterVelocity, BaseEnemy* pEnemy, Player* pPlayer, const Point2f& lastPosOnGround) const;
};

