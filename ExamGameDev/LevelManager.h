#pragma once
class Level;
class Player;
class TextureManager;
class SoundManager;
class LevelObjectManager;
class EnemyManager;
class TimeObjectManager;
class PowerUpManager;
class Game;

#include <vector>

class LevelManager final
{
	public:
		LevelManager(Game* pGame, PowerUpManager* pPowerUpManager, TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);
		~LevelManager();

		LevelManager(const LevelManager&) = delete;
		LevelManager& operator=(const LevelManager&) = delete;
		LevelManager(LevelManager&&) = delete;
		LevelManager& operator=(LevelManager&&) = delete;

		void Update(float elapsedSec, Player* pPlayer, const Uint8* pInput);

		void Draw(const Point2f& cameraPos) const;

		bool InitializeStage1(TextureManager* pTextureManager, SoundManager* pSoundManager, TimeObjectManager* pTimeObjectManager);
		
		Level* GetCurrentLevel() const;
		const Rectf& GetLevelBoundaries() const;
		const Point2f& GetSpawnPosition() const;

		bool DeleteCurrentLevel();

		bool StartNextStage() const;
	private:
		//Not owned by LevelManager
		Game* m_pGame;
		PowerUpManager* m_pPowerUpManager;
		//--------

		LevelObjectManager* m_pLevelObjectManager;
		EnemyManager* m_pEnemyManager;

		Level* m_pCurrentLevel;

		bool LoadStage1LevelObjects(TextureManager* pTextureManager, TimeObjectManager* pTimeObjectManager);
};

