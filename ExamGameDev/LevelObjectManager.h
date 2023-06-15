#pragma once
#include <vector>

class LevelObject;
class Player;

class LevelObjectManager final
{
	public:
		LevelObjectManager() = default;
		~LevelObjectManager();

		LevelObjectManager(const LevelObjectManager&) = delete;
		LevelObjectManager& operator=(const LevelObjectManager&) = delete;
		LevelObjectManager(LevelObjectManager&&) = delete;
		LevelObjectManager& operator=(LevelObjectManager&&) = delete;

		void Update(float elapsedSec, Player* pPlayer, const Uint8* pInput);
		void Draw() const;

		bool CreateLevelObject(LevelObject* pLevelObject);
		bool DeleteLevelObject(LevelObject* pLevelObject);
		bool ClearAllLevelObjects();

	private:
		std::vector<LevelObject*> m_pLevelObjects;
};

