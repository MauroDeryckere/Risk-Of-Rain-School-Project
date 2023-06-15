#pragma once
#include <vector>
constexpr size_t AMOUNT_OF_ENEMY_TYPES{ 2 };

class EnemySpawner
{
	public:
		enum class Enemies {
			parent = 0,
			suicider = 1
		};

		EnemySpawner();
		~EnemySpawner() = default;

	private:
		std::vector<std::pair<Point2f, Point2f>> m_SpawnableRanges;
		std::vector<std::vector<bool>> m_SpawnableEnemies;
};

