#pragma once
#include <iostream>
#include "EnemySpawner.h"

struct RopeInput final
{
	RopeInput() = default;

	friend std::istream& operator>>(std::istream& inputString, RopeInput& ropeData) //Need to overload the operator for each input struct
	{
		inputString >> ropeData.shape.left
					>> ropeData.shape.bottom
					>> ropeData.shape.width
					>> ropeData.shape.height;
		return inputString;
	}

	Rectf shape;
};

struct LaunchPadInput final
{
	LaunchPadInput() = default;

	friend std::istream& operator>>(std::istream& inputString, LaunchPadInput& launchPadData) //Need to overload the operator for each input struct
	{
		inputString >> launchPadData.shape.left
					>> launchPadData.shape.bottom
					>> launchPadData.shape.width
					>> launchPadData.shape.height
					>> launchPadData.yVelocity;
		return inputString;
	}

	Rectf shape;
	float yVelocity;
};
 
struct ChestInput final
{
	ChestInput() = default;

	friend std::istream& operator>>(std::istream& inputString, ChestInput& chestData) //Need to overload the operator for each input struct
	{
		inputString >> chestData.bottomLeft.x
					>> chestData.bottomLeft.y
					>> chestData.rarity;
		return inputString;
	}

	Point2f bottomLeft;
	unsigned int rarity;
};

struct EnemySpawnAreaInput final
{
	EnemySpawnAreaInput() = default;

	friend std::istream& operator>>(std::istream& inputString, EnemySpawnAreaInput& enemySpawnData) //Need to overload the operator for each input struct
	{
		inputString >> enemySpawnData.rangePoint0.x
					>> enemySpawnData.rangePoint0.y
					>> enemySpawnData.rangePoint1.x
					>> enemySpawnData.rangePoint1.y;

		enemySpawnData.canSpawnEnemy.resize(AMOUNT_OF_ENEMY_TYPES);

		for (size_t currEnemy {0}; currEnemy  < AMOUNT_OF_ENEMY_TYPES; ++currEnemy)
		{
			inputString >> enemySpawnData.canSpawnEnemy[currEnemy];
		}

		return inputString;
	}

	Point2f rangePoint0;
	Point2f rangePoint1;
	std::vector<size_t> canSpawnEnemy;
};
