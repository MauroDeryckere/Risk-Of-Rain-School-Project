#include "pch.h"
#include "LaunchPad.h"
#include "StopwatchManager.h"
#include "TimeObjectManager.h"
#include "Stopwatch.h"
#include "Texture.h"

#include "utils.h"
#include <iostream>

#include "Player.h"

LaunchPad::LaunchPad(const Rectf& shape, float yVelocity, TimeObjectManager* pTimeObjectManager, TextureManager* pTextureManager):
	LevelObject{ shape, pTextureManager },
	m_pTimeObjectManager{ pTimeObjectManager },
	m_pLaunchPadStopwatchManager{ pTimeObjectManager->CreateStopwatchManager() },
	m_VelocityBoost{ Vector2f{0.f, yVelocity} },
	m_pLaunchPadStopwatch{ m_pLaunchPadStopwatchManager->CreateStopwatch(0.05f) },
	m_CurrentLaunchPadFrame { 0 },
	m_LaunchPadFrames{ 6 } 
{
	m_pLaunchPadStopwatch->Start();
}

LaunchPad::~LaunchPad()
{
	m_pTimeObjectManager->DeleteStopwatchManager(m_pLaunchPadStopwatchManager);
}

void LaunchPad::Draw() const
{
	const Rectf sourceRect {4.f + m_CurrentLaunchPadFrame * m_Shape.width + m_CurrentLaunchPadFrame * 4.f,
								  63.f ,
								  m_Shape.width,
								  m_Shape.height };

	m_pTexture->Draw(m_Shape, sourceRect);
}

void LaunchPad::Update(float elapsedSec, Player* pPlayer, const Uint8* pInput)
{
	if (utils::IsOverlapping(m_Shape, pPlayer->GetShape()))
	{
		Interact(pPlayer);
	}

	ChangeLaunchPadFrame();
}

void LaunchPad::ChangeLaunchPadFrame()
{
	if (m_pLaunchPadStopwatch->IsTimeReached())
	{
		++m_CurrentLaunchPadFrame %= m_LaunchPadFrames;
	}
}

void LaunchPad::Interact(Player* pPlayer) const
{
	pPlayer->UseLaunchPad(Point2f{m_Shape.left + m_Shape.width/2, m_Shape.bottom }, m_VelocityBoost);
}
