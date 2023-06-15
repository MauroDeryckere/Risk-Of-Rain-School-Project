#include "pch.h"
#include "ATGMissile.h"
#include "TextureManager.h"
#include "Texture.h"
#include "BaseEnemy.h"

#include "utils.h"
#include "Level.h"

ATGMissile::ATGMissile(const Point2f& spawnPosition, TextureManager* pTextureManager, BaseEnemy* pTarget, unsigned int damage) :
	m_pTextureManager{ pTextureManager },
	m_pTexture{ m_pTextureManager->CreateTexture("PowerUps/ATGMissile.png", "ATGMissile") },
	m_Shape{ Rectf{spawnPosition.x, spawnPosition.y, 11.f/3*2 , 24.f /3*2} },
	m_Damage{ damage },

	m_pTarget{ pTarget },

	m_MissileState{ MissileState::upwards },

	m_t{ 0.f },
	m_RotationAngle{ 0.f }
{
	if (m_pTarget != nullptr)
	{
		InitializeBezierPoints();
	}
}

ATGMissile::~ATGMissile()
{
	m_pTextureManager->DeleteTexture("ATGMissile");
}

void ATGMissile::Update(Level* pLevel, float elapsedSec)
{
	const Rectf targetShape{ m_pTarget->GetShape() };

	m_t += elapsedSec;

	if (m_t >= 1.f)
	{
		m_t = .0f;
		ChangeMissileState(targetShape);
	}	

	if (!m_pTarget->IsAlive())
	{
		//retarget if a different enemy is on the map
		m_pTarget = pLevel->GetClosestByEnemy(m_Shape);
		if (!m_pTarget)
		{
			pLevel->DeSpawnATGMissile(this);
			return;
		}

		m_MissileState = MissileState::upwards;
		InitializeBezierPoints();
	}
	else
	{
		if (m_MissileState == MissileState::downwards && utils::IsOverlapping(m_Shape, targetShape))
		{
			m_MissileState = MissileState::despawn;
		}

		if (m_MissileState == MissileState::despawn)
		{
			m_pTarget->TakeDamage(m_Damage);
			pLevel->DeSpawnATGMissile(this);
			return;
		}
	}


	HandleBezierCalculations(targetShape);
}

void ATGMissile::Draw() const
{
	const Point2f centerObj{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };
	const Rectf srcRect{ 0.f,0.f,11.f,24.f };

	glPushMatrix();
		glTranslatef(centerObj.x, centerObj.y, 0);
		glRotatef(m_RotationAngle, 0, 0, 1);
		glTranslatef(-centerObj.x, -centerObj.y, 0);
			m_pTexture->Draw(m_Shape, srcRect);
	glPopMatrix();
}

void ATGMissile::InitializeBezierPoints()
{
	const Rectf targetShape{ m_pTarget->GetShape() };

	const Point2f centerTarget{ targetShape.left + targetShape.width / 2, 
								targetShape.bottom + targetShape.height / 2 };

	const float distanceEnemyMissile{ utils::GetDistance(m_Shape.left + m_Shape.width/2, m_Shape.bottom + m_Shape.height/2, 
														 centerTarget.x, centerTarget.y) };

	const int scalingFactor{ int(distanceEnemyMissile / 100.f) };


	const bool isOnLeftSide{ m_Shape.left > targetShape.left };

	//~10% chance to make a 'loop'
	const bool launchLeft{ (utils::GetRandomNumber(1, 100) % 10 == 0 ? !isOnLeftSide : isOnLeftSide) };

	constexpr int minContinuityPointDistance{ 30 };
	constexpr int maxContinuityPointDistance{ 50 };
	const float continuityPointDistance{ (launchLeft ? float(utils::GetRandomNumber(minContinuityPointDistance, maxContinuityPointDistance)) :
													  -float(utils::GetRandomNumber(minContinuityPointDistance, maxContinuityPointDistance)))};

	//Up
	constexpr int minLaunchHeight{ 200 };
	constexpr int maxLaunchHeight{ 350 };
	const float launchHeight{ float(utils::GetRandomNumber(minLaunchHeight, maxLaunchHeight)) };

	const int topHorDisDivValue1{ std::max(scalingFactor * 10, 15) };
	const int topHorDisDivValue2{ 35 };

	const float randomTopHorDis{ (topHorDisDivValue1 > topHorDisDivValue2 ? 
		 launchHeight / (utils::GetRandomNumber(topHorDisDivValue2, topHorDisDivValue1) / 10.f) :
		 launchHeight / (utils::GetRandomNumber(topHorDisDivValue1, topHorDisDivValue2) / 10.f)
		)};

	const float topHorDisplacement{ (launchLeft ? -randomTopHorDis : randomTopHorDis) };

	//Bezier
	const Point2f p0{ m_Shape.left + m_Shape.width / 2,
					  m_Shape.bottom },

				  p1{ p0.x, 
					  p0.y + (launchHeight - launchHeight / 10) },

				  p2{ p0.x + topHorDisplacement + continuityPointDistance, 
					  p0.y + launchHeight }, //1st order continuity point

				  p3{ p0.x + topHorDisplacement, 
					  p0.y + launchHeight }; //0th order continuity point

	m_upBez.p0 = p0;
	m_upBez.p1 = p1;
	m_upBez.p2 = p2;
	m_upBez.p3 = p3;

	//Down
	const float minCenterHorDisplacement{ launchHeight - launchHeight / utils::GetRandomNumber(2,6) };
	const float centerHorDisplacement{ (launchLeft ? -minCenterHorDisplacement : minCenterHorDisplacement) };

	//Bezier
	const Point2f q0{ p3 }, //0th order continuity point

			      q1{ q0.x - continuityPointDistance,
					  q0.y },  //1st order continuity point

				  q2{ q0.x + centerHorDisplacement, 
					  q0.y - (q0.y - targetShape.bottom) / 2 }, //Will be updated every frame

				  q3{ centerTarget }; //Will be updated every frame

	m_DownBez.p0 = q0;
	m_DownBez.p1 = q1;
	m_DownBez.p2 = q2;
	m_DownBez.p3 = q3;
}

void ATGMissile::ChangeMissileState(const Rectf& targetShape)
{
	switch (m_MissileState)
	{
	case ATGMissile::MissileState::upwards:
		m_MissileState = MissileState::downwards;
		break;
	case ATGMissile::MissileState::downwards:
		m_MissileState = MissileState::despawn;
		break;
	}
}

 void ATGMissile::HandleBezierCalculations(const Rectf& targetShape)
{	
	//adjust down bezPoints based on enemy position
	 const Point2f q0{ m_DownBez.p0 };
	 m_DownBez.p2.y = q0.y - (q0.y - targetShape.bottom + targetShape.height / 2) / 2;

 	 m_DownBez.p3.x = targetShape.left + targetShape.width / 2;
	 m_DownBez.p3.y = targetShape.bottom + targetShape.height / 2;

	//Calculate the Current object position and rotation angle
	if (m_MissileState == MissileState::upwards)
	{
		const Point2f objPos{ utils::PointOnCubicBezier(m_upBez , m_t)};

		m_Shape.left = objPos.x;
		m_Shape.bottom = objPos.y;

		m_RotationAngle = utils::AngleDCubicBezier(m_upBez, m_t) - 90;
	}
	else if(m_MissileState == MissileState::downwards)
	{
		const Point2f objPos{ utils::PointOnCubicBezier(m_DownBez , m_t)};

		m_Shape.left = objPos.x;
		m_Shape.bottom = objPos.y;

		m_RotationAngle = utils::AngleDCubicBezier(m_DownBez, m_t) - 90;
	}
}
