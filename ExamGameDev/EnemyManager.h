#pragma once
#include <vector>

class BaseEnemy;
class TimeObjectManager;
class SoundManager;
class TextureManager;
class EnemySpawner;
class Level;
class Player;

class EnemyManager final
{
	public:
		EnemyManager(TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);
		~EnemyManager();

		EnemyManager(const EnemyManager&) = delete;
		EnemyManager& operator=(const EnemyManager&) = delete;
		EnemyManager(EnemyManager&&) = delete;
		EnemyManager& operator=(EnemyManager&&) = delete;

		void Update(float elapsedSec, Level* pLevel, Player* pPlayer);
		void Draw() const;

		void AddEnemy(BaseEnemy* pEnemy);
		bool DeleteEnemy(BaseEnemy* pEnemy);
		void ClearAllEnemies();

		void InitSpawnLocations(size_t stage, const Point2f& playerSpawnPos);
		//Clear the spawn areas when deleting the level
		void ClearSpawnLocations();

		const std::vector<BaseEnemy*>& GetEnemiesArr() const;
		BaseEnemy* GetClosestByEnemyPtr(const Rectf& shape) const;

		void SetMobCap(size_t newMobCap);
		size_t GetCurrentMobCap() const;

	private:
		std::vector<BaseEnemy*> m_pEnemies;
		EnemySpawner* m_pEnemySpawner;

		size_t m_MobCap;
};