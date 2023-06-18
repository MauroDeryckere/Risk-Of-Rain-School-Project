#pragma once
#include "LevelObject.h"
#include "utils.h"
#include "Stopwatch.h"
class StopwatchManager;
class TimeObjectManager;


class LaunchPad final:
	public LevelObject
{
	public:
		LaunchPad(const Rectf& shape, float yVelocity, TimeObjectManager* pTimeObjectManager, TextureManager* pTextureManager);
		virtual ~LaunchPad() override;

		LaunchPad(const LaunchPad&) = delete;
		LaunchPad& operator=(const LaunchPad&) = delete;
		LaunchPad(LaunchPad&&) = delete;
		LaunchPad& operator=(LaunchPad&&) = delete;

		virtual void Draw() const override;
		virtual void Update(float elapsedSec, Player* pPlayer, const Uint8* pInput ) override;

	private:
		//Not owned by LaunchPad
		TimeObjectManager* m_pTimeObjectManager;
		//--------------------

		StopwatchManager* m_pLaunchPadStopwatchManager;

		const Vector2f m_VelocityBoost;

		Stopwatch* m_pLaunchPadStopwatch;
		size_t m_CurrentLaunchPadFrame;
		const size_t m_LaunchPadFrames;

		void ChangeLaunchPadFrame();
		void Interact(Player* pPlayer) const;
};