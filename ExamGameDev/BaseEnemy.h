#pragma once
#include "Vector2f.h"

class Level;
class Player;
class StopwatchManager;
class TimeObjectManager;
class SoundManager;
class TextureManager;

class BaseEnemy
{
	public:
		explicit BaseEnemy(const Rectf& shape, float movementSpeed, float jumpSpeed, unsigned int MaxHealth, unsigned int attackDamage, 
							TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);

		virtual ~BaseEnemy();

		BaseEnemy(const BaseEnemy&) = delete;
		BaseEnemy& operator=(const BaseEnemy&) = delete;
		BaseEnemy(BaseEnemy&&) = delete;
		BaseEnemy& operator=(BaseEnemy&&) = delete;

		virtual void Update(float elapsedSec, Level* pLevel, Player* pPlayer) = 0;
		virtual void Draw() const = 0;

		bool IsAlive() const;

		virtual void TakeDamage(unsigned int attackDamage);

		const Rectf& GetShape() const;
		
	protected:
		virtual void DrawHealthBar() const;

		//Not owned by BaseEnemy
		TimeObjectManager* m_pTimeObjectManager; 
		SoundManager* m_pSoundManager;
		TextureManager* m_pTextureManager;
		//---------

		StopwatchManager* m_pEnemyStopwatchManager;

		Rectf m_Shape;

		bool m_IsFacingLeft;
		bool m_HasAttacked;

		const float m_MovementSpeed;
		const float m_JumpSpeed;
		Vector2f m_Velocity; 
		const Vector2f m_Acceleration;

		unsigned int m_MaxHealth;
		unsigned int m_CurrentHealth; 
		unsigned int m_AttackDamage;
};
