#pragma once
#include "BasePowerUp.h"
class MovementSpeedBoostPowerUp final:
    public BasePowerUp
{
	public:
		MovementSpeedBoostPowerUp(const Point2f& position, const Texture* pTexture);
		virtual ~MovementSpeedBoostPowerUp() override = default;

		MovementSpeedBoostPowerUp(const MovementSpeedBoostPowerUp&) = delete;
		MovementSpeedBoostPowerUp& operator=(const MovementSpeedBoostPowerUp&) = delete;
		MovementSpeedBoostPowerUp(MovementSpeedBoostPowerUp&&) = delete;
		MovementSpeedBoostPowerUp& operator=(MovementSpeedBoostPowerUp&&) = delete;

		virtual void Activate(Player* pPlayer, Level* pLevel) override;
		virtual void IncreasePowerUpAmount() override;

	private:
		const float m_MovementSpeedBoost;
		bool m_IsActivated;
};

