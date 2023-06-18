#pragma once
#include "BasePowerUp.h"
class ATGPowerUp final:
    public BasePowerUp
{
	public:
		ATGPowerUp(const Point2f& position, const Texture* pTexture);
		virtual ~ATGPowerUp() override = default;

		ATGPowerUp(const ATGPowerUp&) = delete;
		ATGPowerUp& operator=(const ATGPowerUp&) = delete;
		ATGPowerUp(ATGPowerUp&&) = delete;
		ATGPowerUp& operator=(ATGPowerUp&&) = delete;

		virtual void Activate(Player* pPlayer, Level* pLevel) override;
		virtual void IncreasePowerUpAmount() override;

	private:
		size_t m_ActivationChance;
};

