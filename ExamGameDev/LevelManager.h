#pragma once
class Level;
class Player;
class TextureManager;
class SoundManager;
class LevelObjectManager;
class EnemyManager;
class TimeObjectManager;

#include <vector>

class LevelManager
{
	public:
		LevelManager();
		~LevelManager();

		LevelManager(const LevelManager&) = delete;
		LevelManager& operator=(const LevelManager&) = delete;
		LevelManager(LevelManager&&) = delete;
		LevelManager& operator=(LevelManager&&) = delete;

		void Update(float elapsedSec, Player* pPlayer, const Uint8* pInput);

		void Draw(const Point2f& cameraPos) const;

		bool InitializeStage1(TextureManager* pTextureManager, SoundManager* pSoundManager, TimeObjectManager* pTimeObjectManager);

		Level* GetCurrentLevel() const;

		bool DeleteCurrentLevel();

	private:
		LevelObjectManager* m_pLevelObjectManager;
		EnemyManager* m_pEnemyManager;

		Level* m_pCurrentLevel;

		bool LoadStage1LevelObjects(TextureManager* pTextureManager, TimeObjectManager* pTimeObjectManager);
};

