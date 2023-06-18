#pragma once
#include "BasePowerUp.h"

class HealthBoostPowerUp final:
	public BasePowerUp
{
	public:
		explicit HealthBoostPowerUp(const Point2f& position, const Texture* pTexture);
		virtual ~HealthBoostPowerUp() override = default;

		HealthBoostPowerUp(const HealthBoostPowerUp&) = delete;
		HealthBoostPowerUp& operator=(const HealthBoostPowerUp&) = delete;
		HealthBoostPowerUp(HealthBoostPowerUp&&) = delete;
		HealthBoostPowerUp& operator=(HealthBoostPowerUp&&) = delete;

		virtual void Activate(Player* pPlayer, Level* pLevel) override;
		virtual void IncreasePowerUpAmount() override;

	private:
		const size_t m_HealthBoost;
		bool m_IsActivated;
};

