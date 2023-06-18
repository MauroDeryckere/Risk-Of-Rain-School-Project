#pragma once
class TextureManager;
class TimeObjectManager;
class SoundManager;

class EnemyManager;
class StopwatchManager;
class Stopwatch;

#include <vector>

constexpr size_t AMOUNT_OF_ENEMY_TYPES{ 2 };

class EnemySpawner final 
{
	public:
		enum class Enemies {
			parent = 0,
			suicider = 1
		};

		EnemySpawner(TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager, EnemyManager* pEnemyManager);
		~EnemySpawner();

		EnemySpawner(const EnemySpawner&) = delete;
		EnemySpawner& operator=(const EnemySpawner&) = delete;
		EnemySpawner(EnemySpawner&&) = delete;
		EnemySpawner& operator=(EnemySpawner&&) = delete;

		void Update(const Point2f& characterPos);

		bool SetSpawnableLocations(size_t stage, const Point2f& playerSpawnPos);
		bool ClearCurrentSpawnAbleLocations();

	private:
		//Not owned by EnemySapwner
		TimeObjectManager* m_pTimeObjectManager;
		SoundManager* m_pSoundManager;
		TextureManager* m_pTextureManager;

		EnemyManager* m_pEnemyManager;
		//-----------------------

		StopwatchManager* m_pSpawnStopwatchManager;
		Stopwatch* m_pSpawnStopwatch;

		std::vector<std::pair<const Point2f, const Point2f>> m_SpawnableRanges;
		std::vector<std::vector<bool>> m_SpawnableEnemies;

		void HandleSpawnCalc(const Point2f& characterPos, size_t mobCap, size_t amountOfEnemies); 

		bool SpawnEnemy(Enemies enemyType, const Point2f& position);

		bool SpawnParent(const Point2f& position);
		bool SpawnSuicider(const Point2f& position);
};

