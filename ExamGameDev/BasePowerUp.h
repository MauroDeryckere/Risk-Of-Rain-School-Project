#pragma once

class Player;
class Texture;
class Level;

#include "Chest.h"

class BasePowerUp
{
	public:
		enum class PowerUps
		{
			MovementSpeed,
			HealthBoost,
			ATG
		};


		BasePowerUp(PowerUps powerUp, Chest::Rarity rarity, const Point2f& position, const Rectf& sourceRect, const Texture* pTexture);
		virtual ~BasePowerUp();

		BasePowerUp(const BasePowerUp&) = delete;
		BasePowerUp& operator=(const BasePowerUp&) = delete;
		BasePowerUp(BasePowerUp&&) = delete;
		BasePowerUp& operator=(BasePowerUp&&) = delete;

		void Draw() const;
		virtual void Activate(Player* pPlayer, Level* pLevel) = 0;

		const Rectf& GetShape() const;
		//Index of lookuptable
		size_t GetIndex() const;

		//Set bottomLeft position (for inventory drawing)
		void SetBottomLeft(const Point2f& bottomLeft);

		virtual void IncreasePowerUpAmount();
		const size_t GetPowerUpAmount() const;

	protected: 
		size_t m_Amount;
		
	private:
		const PowerUps m_PowerUp;

		Rectf m_Shape;
		const Chest::Rarity m_Rarity;
		const Rectf m_SourceRect;
		const Texture* m_pTexture;
};

