#include "pch.h"
#include "Level.h"

#include "Texture.h"

#include "Rope.h"
#include "LaunchPad.h"
#include "Chest.h"

#include "BasePowerUp.h"
#include "ATGMissile.h"

#include "Player.h"
#include "Parent.h"

#include "utils.h"
#include "SVGParser.h"
#include "SoundStream.h"
#include "SoundEffect.h"

#include "TextureManager.h"
#include "SoundManager.h"
#include "LevelObjectManager.h"
#include "EnemyManager.h"
#include "TimeObjectManager.h"
#include "StopwatchManager.h"

#include "LevelHitBullet.h"

#include <iostream>
#include <cassert>
#include <algorithm>


Level::Level(TextureManager* pTextureManager, SoundManager* pSoundManager, LevelObjectManager* pLevelObjectManager, EnemyManager* pEnemyManager, TimeObjectManager* pTimeObjectManager) :
	m_pTextureManager{ pTextureManager },
	m_pSoundManager{ pSoundManager },
	m_pLevelObjectManager{ pLevelObjectManager },
	m_pEnemyManager{ pEnemyManager },
	m_pTimeObjectManager{ pTimeObjectManager },

	m_pForeGroundTexture{ m_pTextureManager->CreateTexture("Level/ForeGround.png", "levelForeGround") },
	m_pStarsBackgroundTexture{ m_pTextureManager->CreateTexture("Level/StarsBackground.png", "levelStarsBackground") },
	m_pMountainsBackgroundTexture1{ m_pTextureManager->CreateTexture("Level/MountainsBackground1.png", "levelMountainsBackground1") },
	m_pMountainsBackgroundTexture2{ m_pTextureManager->CreateTexture("Level/MountainsBackground2.png", "levelMountainsBackground2") },
	m_pMoonTexture{ m_pTextureManager->CreateTexture("Level/Moon.png", "levelMoon") },

	m_Boundaries{ Rectf{ 0.f, 0.f, m_pForeGroundTexture->GetWidth(), m_pForeGroundTexture->GetHeight() } }
{
	m_pEnemyManager->SpawnParent(Point2f{ 180.f + 25.f + 25.f ,  2096 - 1727.f }, m_pTimeObjectManager, m_pSoundManager, m_pTextureManager); //temp, testing !!
	//m_pEnemyManager->SpawnSuicider(Point2f{ 180.f + 25.f + 25.f ,  2096 - 1727 + 10.f }, m_pTimeObjectManager, m_pSoundManager, m_pTextureManager);

	//SVG Map
	SVGParser::GetVerticesFromSvgFile("Level/Risk_of_Rain_map_water_Background.svg", m_Vertices);

	InitializeSounds();

	m_pLevelMusic->Play(true);
}

Level::~Level()
{
	m_pTextureManager->DeleteTexture("levelForeGround");
	m_pTextureManager->DeleteTexture("levelStarsBackground");
	m_pTextureManager->DeleteTexture("levelMountainsBackground1");
	m_pTextureManager->DeleteTexture("levelMountainsBackground2");
	m_pTextureManager->DeleteTexture("levelMoon");

	DeleteSounds();

	//Delete the powerups still on the level
	for (size_t index{ 0 }; index < m_pPowerUps.size(); ++index)
	{
		delete m_pPowerUps[index];
	}

	//Delete all ATGMissiles still on the level
	for (size_t index{ 0 }; index < m_pATGMissiles.size(); ++index)
	{
		delete m_pATGMissiles[index];
	}

	//Delete hitbullet obj
	for (size_t index{ 0 }; index < m_pLevelHitBullets.size(); ++index)
	{
		delete m_pLevelHitBullets[index];
	}
}

void Level::Update(float elapsedSec, Player* pPlayer, const Uint8* pInput)
{
	UpdatePowerUps(pPlayer, pInput);

	for (size_t index{ 0 }; index < m_pATGMissiles.size(); ++index)
	{
		m_pATGMissiles[index]->Update(this, elapsedSec);
	}
}

void Level::Draw(const Point2f& cameraPos) const
{
	DrawBackground(cameraPos);
	DrawForeGround();

	DrawPowerUps();
	DrawATGMissiles();

	for (size_t index{ 0 }; index < m_pLevelHitBullets.size(); ++index)
	{
		m_pLevelHitBullets[index]->Draw();
	}
}

void Level::DrawBackground(const Point2f& cameraPos) const
{
	constexpr float xStarParFactor{ .95f };
	constexpr float yStarParFactor{ .97f };

	glPushMatrix();
		glTranslatef(cameraPos.x * xStarParFactor, cameraPos.y *  yStarParFactor, 0);
		m_pStarsBackgroundTexture->Draw();
	glPopMatrix();

	constexpr float xMoonParFactor{ 0.90f };
	constexpr float yMoonParFactor{ 0.92f };

	glPushMatrix();
		glTranslatef(cameraPos.x * xMoonParFactor, cameraPos.y * yMoonParFactor, 0);
		m_pMoonTexture->Draw();
	glPopMatrix();

	constexpr float xMountain1ParFactor{ .75f };
	constexpr float yMountain1ParFactor{ .77f };

	glPushMatrix();
		glTranslatef(cameraPos.x * xMountain1ParFactor, cameraPos.y * yMountain1ParFactor, 0);
		m_pMountainsBackgroundTexture1->Draw();
	glPopMatrix();

	constexpr float xMountain2ParFactor{ .55f };
	constexpr float yMountain2ParFactor{ .57f };

	glPushMatrix();
		glTranslatef(cameraPos.x * xMountain2ParFactor, cameraPos.y * yMountain2ParFactor, 0);
		m_pMountainsBackgroundTexture2->Draw();
	glPopMatrix();
}

void Level::DrawForeGround() const
{
	m_pForeGroundTexture->Draw();
}

void Level::DrawPowerUps() const
{
	for (size_t index{ 0 }; index < m_pPowerUps.size(); ++index)
	{
		m_pPowerUps[index]->Draw();
	}
}

void Level::DrawATGMissiles() const
{
	for (size_t index{ 0 }; index < m_pATGMissiles.size(); ++index)
	{
		m_pATGMissiles[index]->Draw();
	}
}

void Level::HandleCollision(Rectf& characterShape, Vector2f& characterVelocity) const
{
	HandleVerticalCollision(characterShape, characterVelocity);

	if (characterVelocity.x != 0.f)
	{
		HandleHorizontalCollision(characterShape, characterVelocity);
	}

	HandleOutOfBoundariesCollision(characterShape);
}
bool Level::IsOnGround(const Rectf& characterShape) const
{
	constexpr float offset{ 1.f };
	constexpr float smallestPlatformWidth{ 32.f };

	const float rayCastWidth{ characterShape.width / smallestPlatformWidth };

	utils::HitInfo hitInfoGround{};
		const Point2f leftSideOrigin1{ characterShape.left + offset, characterShape.bottom + characterShape.height / 2 };
		const Point2f leftSideOrigin2{ characterShape.left + offset, characterShape.bottom - offset};

		const Point2f rightSideOrigin1{ characterShape.left + characterShape.width - offset, leftSideOrigin1.y };
		const Point2f rightSideOrigin2{ characterShape.left + characterShape.width - offset, leftSideOrigin2.y };

	for (size_t index{ 0 }; index < m_Vertices.size(); ++index)
	{
		//Are edges on ground
		if (Raycast(m_Vertices[index], leftSideOrigin1, leftSideOrigin2, hitInfoGround)
		 || Raycast(m_Vertices[index], rightSideOrigin1, rightSideOrigin2, hitInfoGround)) 
		{
			return true;
		}

		else //No Edges are on ground; check rest of shape
		{
			if (characterShape.width > smallestPlatformWidth * 2)
			{
				float currentWidthToCheck{ smallestPlatformWidth };

				while (currentWidthToCheck <= characterShape.width)
				{
					const Point2f origin1{ characterShape.left + currentWidthToCheck, characterShape.bottom + characterShape.height / 2 };
					const Point2f origin2{ characterShape.left + currentWidthToCheck, characterShape.bottom - offset };

					if (Raycast(m_Vertices[index], origin1, origin2, hitInfoGround))
					{
						return true;
					}

					currentWidthToCheck += smallestPlatformWidth;
				}
			}
		}
	}

	return false;
}

bool Level::HandleBulletRayCast(const Rectf& characterShape, bool isFacingLeft, unsigned int attackRange, unsigned int attackDamage, unsigned int pierceAmount)
{
	const std::vector<BaseEnemy*>& enemiesArr{ m_pEnemyManager->GetEnemiesArr() };

	const float rightCharacterSide{characterShape.left + characterShape.width};

	utils::HitInfo hitInfoBullet{};
		const Point2f bulletOrigin{ (isFacingLeft)? 
										characterShape.left: 
										rightCharacterSide, 
									 characterShape.bottom + characterShape.height / 2 };

		const Point2f bulletEndRange{ (isFacingLeft)? 
										characterShape.left - attackRange: 
										rightCharacterSide + attackRange, 
									  bulletOrigin.y };

	std::vector<BaseEnemy*> hitEnemies;
	std::vector<float> enemyDistances;

	for (size_t index{ 0 }; index < enemiesArr.size(); ++index)
	{
		const bool isAlive{ enemiesArr[index]->IsAlive( ) };

		if (isAlive)
		{
			const Rectf& enemyShape{ enemiesArr[index]->GetShape() };
			const std::vector<Point2f> enemyVertices{ Point2f{enemyShape.left, enemyShape.bottom},
													  Point2f{enemyShape.left, enemyShape.bottom + enemyShape.height},
													  Point2f{enemyShape.left + enemyShape.width, enemyShape.bottom + enemyShape.height},
													  Point2f{enemyShape.left + enemyShape.width, enemyShape.bottom} };

			if (Raycast(enemyVertices, bulletOrigin, bulletEndRange, hitInfoBullet))
			{
				hitEnemies.emplace_back(enemiesArr[index]);
				enemyDistances.emplace_back(abs(enemyShape.left + enemyShape.width / 2 - bulletOrigin.x));
			}
		}
	}

	constexpr float MAXDISTANCE{5000.f};

	bool isLevelHit{ false };

	float levelHitDistance{ MAXDISTANCE }; 
	Point2f levelHitIntersectPoint{};

	for (size_t index{ 0 }; index < m_Vertices.size(); ++index)
	{
		if (Raycast(m_Vertices[index], bulletOrigin, bulletEndRange, hitInfoBullet))
		{
			isLevelHit = true;
			const float newLevelHitDistance = abs(hitInfoBullet.intersectPoint.x - bulletOrigin.x);

			if (newLevelHitDistance < levelHitDistance)
			{
				levelHitDistance = newLevelHitDistance;
				levelHitIntersectPoint = hitInfoBullet.intersectPoint;
			}
		}
	}


	bool hitEnemy{ false };

	for (size_t piercedEnemies{ 0 }; piercedEnemies < pierceAmount; ++piercedEnemies)
	{
		const size_t closestEnemyIndex = std::distance(enemyDistances.begin(),
													std::min_element(enemyDistances.begin(), enemyDistances.end()));

		if (isLevelHit)
		{
			if (!hitEnemies.empty())
			{
				if (levelHitDistance < enemyDistances[closestEnemyIndex])
				{
					m_pLevelHitBullets.emplace_back(new LevelHitBullet{ levelHitIntersectPoint, isFacingLeft, m_pTextureManager });

					std::cout << "Hit Level (distance)\n";
					return hitEnemy;
				}
				else
				{
					std::cout << "Hit Enemy (distance)\n";
					hitEnemies[closestEnemyIndex]->TakeDamage(attackDamage);

					hitEnemies[closestEnemyIndex] = nullptr;
					hitEnemies[closestEnemyIndex] = hitEnemies.back();
					hitEnemies.pop_back();

					enemyDistances[closestEnemyIndex] = enemyDistances.back();
					enemyDistances.pop_back();

					hitEnemy = true;
				}
			}
			else
			{
				m_pLevelHitBullets.emplace_back(new LevelHitBullet{ levelHitIntersectPoint, isFacingLeft, m_pTextureManager });

				std::cout << "Level hit \n";
				return hitEnemy;
			}
		}
		else
		{
			if (!hitEnemies.empty())
			{
				std::cout << "Hit Enemy (no level hit)\n";
				hitEnemies[closestEnemyIndex]->TakeDamage(attackDamage);

				hitEnemies[closestEnemyIndex] = nullptr;
				hitEnemies[closestEnemyIndex] = hitEnemies.back();
				hitEnemies.pop_back();

				enemyDistances[closestEnemyIndex] = enemyDistances.back();
				enemyDistances.pop_back();

				hitEnemy = true;
			}
			else
			{
				std::cout << "Nothing hit \n";
				return hitEnemy;
			}
		}
	}

	return hitEnemy;
}

void Level::DropPowerUp(BasePowerUp* pPowerUp)
{
	m_pPowerUps.emplace_back(pPowerUp);
}

void Level::RemoveChest(LevelObject* pLevelObject)
{
	m_pLevelObjectManager->DeleteLevelObject(pLevelObject);
}

BaseEnemy* Level::GetClosestByEnemy(const Rectf& shape)
{
	return m_pEnemyManager->GetClosestByEnemyPtr(shape);
}

bool Level::DeSpawnEnemy(BaseEnemy* pEnemy)
{
	return m_pEnemyManager->DeleteEnemy(pEnemy);
}

bool Level::SpawnATGMissile(const Point2f& position, unsigned int attackDamage)
{
	BaseEnemy* pTarget{ m_pEnemyManager->GetClosestByEnemyPtr(Rectf{position.x, position.y, 10.f, 10.f}) };
	if (!pTarget)
	{
		return false;
	}

	m_pATGMissiles.emplace_back(new ATGMissile{ position, m_pTextureManager, pTarget, attackDamage });
	m_pSpawnMissileSound->Play(0);

	return true;
}

void Level::DeSpawnATGMissile(ATGMissile* pMissile)
{
	for (size_t index = 0; index < m_pATGMissiles.size(); index++)
	{
		delete m_pATGMissiles[index];
		m_pATGMissiles[index] = m_pATGMissiles.back();
		m_pATGMissiles.pop_back();
	}
}

const Rectf& Level::GetBoundaries() const 
{
	return m_Boundaries;
}

void Level::InitializeSounds()
{
	m_pSpawnMissileSound = m_pSoundManager->CreateSoundEffect("SpawnMissile", "Sounds/SoundEffects/wMissileLaunch.ogg");
	m_pLevelMusic = m_pSoundManager->CreateSoundStream("LevelMusic", "Sounds/SoundStreams/musicStage10.ogg");
}

void Level::DeleteSounds()
{
	m_pSoundManager->DeleteSoundEffect("SpawnMissile");
	m_pSoundManager->DeleteSoundStream("LevelMusic");
}

void Level::UpdatePowerUps(Player* pPlayer, const Uint8* pInput)
{
	for (size_t index{ 0 }; index < m_pPowerUps.size(); ++index)
	{
		if (utils::IsOverlapping(pPlayer->GetShape(), m_pPowerUps[index]->GetShape()) && pInput[SDL_SCANCODE_R])
		{
			pPlayer->CollectPowerUp(m_pPowerUps[index]);

			m_pPowerUps[index] = nullptr;
			m_pPowerUps[index] = m_pPowerUps.back();
			m_pPowerUps.pop_back();
		}
	}
}

void Level::HandleHorizontalCollision(Rectf& characterShape, Vector2f& characterVelocity) const
{
	constexpr float offset{ 1.f };

	constexpr float smallestPlatformHeight{ 10.f };

	utils::HitInfo hitInfoX{};
		const Point2f bottomEdgeOrigin1{ characterShape.left, characterShape.bottom + offset };
		const Point2f bottomEdgeOrigin2{ characterShape.left + characterShape.width, bottomEdgeOrigin1.y };

		const Point2f topEdgeOrigin1{ bottomEdgeOrigin1.x, characterShape.bottom + characterShape.height - offset };
		const Point2f topEdgeOrigin2{ bottomEdgeOrigin2.x , topEdgeOrigin1.y };

	for (size_t index{ 0 }; index < m_Vertices.size(); ++index)
	{
		if (characterVelocity.x < 0.f) //Moving left
		{
			//Check of an edge intersects
			if (Raycast(m_Vertices[index], bottomEdgeOrigin1, bottomEdgeOrigin2, hitInfoX) 
			|| (Raycast(m_Vertices[index], topEdgeOrigin1, topEdgeOrigin2, hitInfoX)))
			{
				characterShape.left = hitInfoX.intersectPoint.x + 1.f;
				characterVelocity.x = 1.f;
			}
			else //No Edges intersect
			{
				if (characterShape.height > smallestPlatformHeight * 2)
				{
					float currentHeightToCheck{ smallestPlatformHeight };

					while (currentHeightToCheck <= characterShape.width)
					{
						const Point2f origin1{ characterShape.left, characterShape.bottom + currentHeightToCheck };
						const Point2f origin2{ characterShape.left + characterShape.width, origin1.y };

						if (Raycast(m_Vertices[index], origin1, origin2, hitInfoX))
						{
							characterShape.left = hitInfoX.intersectPoint.x + 1.f;
							characterVelocity.x = 1.f;
							return;
						}

						currentHeightToCheck += smallestPlatformHeight;
					}
				}
			}
		}

		else if (characterVelocity.x > 0.f) //Moving right
		{
			//Check if an edge intersects
			if (Raycast(m_Vertices[index], bottomEdgeOrigin1, bottomEdgeOrigin2, hitInfoX) 
			||  Raycast(m_Vertices[index], topEdgeOrigin1, topEdgeOrigin2, hitInfoX))
			{
				characterShape.left = hitInfoX.intersectPoint.x - characterShape.width - 1.f;
				characterVelocity.x = -1.f;
			}
			else //No edges intersect
			{
				if (characterShape.height > smallestPlatformHeight*2)
				{
					float currentHeightToCheck{ smallestPlatformHeight };

					while (currentHeightToCheck <= characterShape.width)
					{
						const Point2f origin1{ characterShape.left, characterShape.bottom + currentHeightToCheck };
						const Point2f origin2{ characterShape.left + characterShape.width, origin1.y };

						if (Raycast(m_Vertices[index], origin1, origin2, hitInfoX))
						{
							characterShape.left = hitInfoX.intersectPoint.x - characterShape.width - 1.f;
							characterVelocity.x = -1.f;
							return;
						}

						currentHeightToCheck += smallestPlatformHeight;
					}
				}
			}
		}
	}
}

void Level::HandleVerticalCollision(Rectf& characterShape, Vector2f& characterVelocity) const
{
	constexpr float smallestPlatformWidth{ 32.f };

	constexpr float offset{ 1.f };

	const float rayCastWidth { characterShape.width / smallestPlatformWidth };

	if (characterVelocity.y <= 0.f) //When falling
	{
		utils::HitInfo hitInfoY{};
			const Point2f leftEdgeOrigin1{ characterShape.left + 2*offset, characterShape.bottom + 5.f * -characterVelocity.y / 100 };
			const Point2f leftEdgeOrigin2{ characterShape.left + 2*offset, characterShape.bottom };

			const Point2f rightEdgeOrigin1{ characterShape.left + characterShape.width - offset, leftEdgeOrigin1.y };
			const Point2f rightEdgeOrigin2{ characterShape.left + characterShape.width - offset, leftEdgeOrigin2.y };

		for (size_t index{ 0 }; index < m_Vertices.size(); ++index)
		{
			if (Raycast(m_Vertices[index], leftEdgeOrigin1, leftEdgeOrigin2, hitInfoY))
			{
				characterShape.bottom = hitInfoY.intersectPoint.y;
				characterVelocity.y = 0.f;
			}
			else if (Raycast(m_Vertices[index], rightEdgeOrigin1, rightEdgeOrigin2, hitInfoY))
			{
				characterShape.bottom = hitInfoY.intersectPoint.y;
				characterVelocity.y = 0.f;
			}
			else //No Edges collide
			{
				if (characterShape.width > smallestPlatformWidth*2)
				{
					float currentWidthToCheck{ smallestPlatformWidth };

					while (currentWidthToCheck <= characterShape.width)
					{
						const Point2f origin1{ characterShape.left + currentWidthToCheck, characterShape.bottom + 5.f * -characterVelocity.y / 100 };
						const Point2f origin2{ characterShape.left + currentWidthToCheck, characterShape.bottom };

						if (Raycast(m_Vertices[index], origin1, origin2, hitInfoY))
						{
							characterShape.bottom = hitInfoY.intersectPoint.y;
							characterVelocity.y = 0.f;
							return;
						}

						currentWidthToCheck += smallestPlatformWidth;
					}
				}
			}
		}
	}
	else //When jumping
	{
		utils::HitInfo hitInfoY{};
			const Point2f leftEdgeOrigin1{ characterShape.left + offset, characterShape.bottom + characterShape.height - 5.f  }; 
			const Point2f leftEdgeOrigin2{ characterShape.left + offset, characterShape.bottom + characterShape.height };

			const Point2f rightEdgeOrigin1{ characterShape.left + characterShape.width - offset, leftEdgeOrigin1.y };
			const Point2f rightEdgeOrigin2{ characterShape.left + characterShape.width - offset, leftEdgeOrigin2.y };

		for (size_t index{ 0 }; index < m_Vertices.size(); ++index)
		{
			if (Raycast(m_Vertices[index], leftEdgeOrigin1, leftEdgeOrigin2, hitInfoY))
			{
				characterShape.bottom = hitInfoY.intersectPoint.y - characterShape.height - offset;
				characterVelocity.y = 0.f;
			}
			else if (Raycast(m_Vertices[index], rightEdgeOrigin1, rightEdgeOrigin2, hitInfoY))
			{
				characterShape.bottom = hitInfoY.intersectPoint.y - characterShape.height - offset;
				characterVelocity.y = 0.f;
			}
			else //No edges collide
			{
				if (characterShape.width > smallestPlatformWidth * 2)
				{
					float currentWidthToCheck{ smallestPlatformWidth };

					while (currentWidthToCheck <= characterShape.width)
					{
						const Point2f origin1{ characterShape.left + currentWidthToCheck, characterShape.bottom + characterShape.height - 5.f };
						const Point2f origin2{ origin1.x, characterShape.bottom + characterShape.height };

						if (Raycast(m_Vertices[index], origin1, origin2, hitInfoY))
						{
							characterShape.bottom = hitInfoY.intersectPoint.y;
							characterVelocity.y = 0.f;
							return;
						}

						currentWidthToCheck += smallestPlatformWidth;
					}
				}
			}
		}
	}
}

void Level::HandleOutOfBoundariesCollision(Rectf& characterShape) const
{
	if (characterShape.left + characterShape.width < m_Boundaries.left || characterShape.left > m_Boundaries.left + m_Boundaries.width)
	{
		characterShape.left = 500.f;
		characterShape.bottom = 500.f;
	}
	if (characterShape.bottom + characterShape.height < m_Boundaries.bottom)
	{
		characterShape.left = 500.f;
		characterShape.bottom = 500.f;
			return;
	}
	if (characterShape.bottom > m_Boundaries.bottom + m_Boundaries.height)
	{
		characterShape.left = 500.f;
		characterShape.bottom = 500.f;
	}
}

bool Level::IsIntersectingMapOnRope(const Rectf& characterShape) const
{
	for (size_t index{ 0 }; index < m_Vertices.size(); ++index)
	{
		if (utils::IsOverLappingPolygon(m_Vertices[index], characterShape))
		{
			return true;
		}
	}
	return false;
}
