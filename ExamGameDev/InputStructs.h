#pragma once
#include <iostream>

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

	Point2f rangePoint1;
	Point2f rangePoint2;
};
