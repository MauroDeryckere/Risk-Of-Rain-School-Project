#pragma once
#include "Vector2f.h"

class Level;
class Player;
class StopwatchManager;
class TimeObjectManager;
class SoundManager;
class TextureManager;
class Stopwatch;

class BaseEnemy
{
	public:
		BaseEnemy(const Rectf& shape, float movementSpeed, float jumpSpeed, size_t, size_t attackDamage, 
				  TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);

		virtual ~BaseEnemy();

		BaseEnemy(const BaseEnemy&) = delete;
		BaseEnemy& operator=(const BaseEnemy&) = delete;
		BaseEnemy(BaseEnemy&&) = delete;
		BaseEnemy& operator=(BaseEnemy&&) = delete;

		virtual void Update(float elapsedSec, Level* pLevel, Player* pPlayer);
		virtual void Draw() const = 0;

		bool IsAlive() const;

		virtual void TakeDamage(Player* pPlayer, size_t attackDamage);

		const Rectf& GetShape() const;
		
	protected:
		virtual void DrawHealthBar() const;
		void DrawDroppedMoney() const;

		//Not owned by BaseEnemy
		TimeObjectManager* m_pTimeObjectManager; 
		SoundManager* m_pSoundManager;

		TextureManager* m_pTextureManager;
		//---------

		//Manages all stopwatches required for the enemy, deletes all used stopwatches once enemy == destroyed
		StopwatchManager* m_pEnemyStopwatchManager;

		Rectf m_Shape;

		bool m_IsFacingLeft;
		bool m_HasAttacked;

		const float m_MovementSpeed;
		const float m_JumpSpeed;
		Vector2f m_Velocity; 
		const Vector2f m_Acceleration;

		size_t m_MaxHealth;
		size_t m_CurrentHealth;
		size_t m_AttackDamage;

	private:
		size_t m_DroppedMoney;
		Stopwatch* m_pMoneyAniStopwatch;
};
