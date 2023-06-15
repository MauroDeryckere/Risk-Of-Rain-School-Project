#pragma once
#include "BaseEnemy.h"
#include "Parent.h"

class TimeObjectManager;
class SoundManager;
class TextureManager;
class StopwatchManager;

class Stopwatch;
class Texture;
class SoundEffect;
class Player;

class Suicider final :
    public BaseEnemy
{
	public:
		Suicider(const Point2f& position, float movementSpeed, unsigned int health, unsigned int attackDamage, 
				 TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);
		virtual ~Suicider() override;

		Suicider(const Suicider&) = delete;
		Suicider& operator=(const Suicider&) = delete;
		Suicider(Suicider&&) = delete;
		Suicider& operator=(Suicider&&) = delete;

		virtual void Update(float elapsedSec, Level* pLevel, Player* pPlayer) override;
		virtual void Draw() const override;
		virtual void TakeDamage(unsigned int attackDamage) override;

		const Rectf ChangeSourceRect() const;

	private:
		Stopwatch* m_pSpawnStopwatch; 

		const Texture* m_pWalkTexture;
		Stopwatch* m_pWalkStopwatch; 
		unsigned int m_CurrentWalkFrame; 
		const unsigned int m_WalkFrames;

		const Texture* m_pAttackTexture; 
		unsigned int m_CurrentAttackFrame; 
		const unsigned int m_AttackFrames;

		Stopwatch* m_pAttackStopwatch;

		enum class SuiciderState 
		{
			spawning,
			walking,
			attacking
		};

		SuiciderState m_SuiciderState; 
		SuiciderState m_PreviousSuiciderState;

		Stopwatch* m_pDeSpawnStopwatch; 

		SoundEffect* m_pAttackSound; 
		SoundEffect* m_pHitSound; 

		void DrawSuicider() const;

		void ChangeEnemyState(Player* pPlayer);
		void ChangeShape();
		void HandleEnemyBehavior(Player* pPlayer, float elapsedSec);
		void Attack(Player* pPlayer);
		void HandleMovement(float elapsedSec);
		void UpdateAnimationFrame();
};
