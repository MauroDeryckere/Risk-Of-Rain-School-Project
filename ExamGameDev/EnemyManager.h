#pragma once
#include <vector>

class BaseEnemy;
class TimeObjectManager;
class SoundManager;
class TextureManager;
class Level;
class Player;

class EnemyManager final
{
	public:
		EnemyManager() = default;
		~EnemyManager();

		EnemyManager(const EnemyManager&) = delete;
		EnemyManager& operator=(const EnemyManager&) = delete;
		EnemyManager(EnemyManager&&) = delete;
		EnemyManager& operator=(EnemyManager&&) = delete;

		void Update(float elapsedSec, Level* pLevel, Player* pPlayer);
		void Draw() const;

		bool SpawnParent(const Point2f& position, TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);
		bool SpawnSuicider(const Point2f& position, TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);
		bool SpawnLemurian(const Point2f& position, TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);

		bool SetSpawnAreas(const std::string& filePath);

		bool DeleteEnemy(BaseEnemy* pEnemy);
		void ClearAllEnemies();

		const std::vector<BaseEnemy*>& GetEnemiesArr() const;
		BaseEnemy* GetClosestByEnemyPtr(const Rectf& shape) const;

	private:
		std::vector<BaseEnemy*> m_pEnemies;

		//EnemySpawner
		std::vector<bool> test;

		void AddEnemy(BaseEnemy* pEnemy);

};