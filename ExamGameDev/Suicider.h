#pragma once
#include "BaseEnemy.h"

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
		Suicider(const Point2f& position, float movementSpeed, size_t health, size_t attackDamage,
				 TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);
		virtual ~Suicider() override;

		Suicider(const Suicider&) = delete;
		Suicider& operator=(const Suicider&) = delete;
		Suicider(Suicider&&) = delete;
		Suicider& operator=(Suicider&&) = delete;

		virtual void Update(float elapsedSec, Level* pLevel, Player* pPlayer) override;
		virtual void Draw() const override;
		virtual void TakeDamage(Player* pPlayer, size_t attackDamage) override;

		const Rectf& ChangeSourceRect() const;

	private:
		Stopwatch* m_pSpawnStopwatch; 

		const Texture* m_pWalkTexture;
		Stopwatch* m_pWalkStopwatch; 
		size_t m_CurrentWalkFrame;
		const size_t m_WalkFrames;

		const Texture* m_pAttackTexture; 
		size_t m_CurrentAttackFrame;
		const size_t m_AttackFrames;

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
		Stopwatch* m_pDieStopwatch;

		SoundEffect* m_pAttackSound; 
		SoundEffect* m_pHitSound; 

		void DrawSuicider() const;

		void UpdateEnemyState(Player* pPlayer);
		void UpdateShape();
		void UpdateEnemyBehavior(Player* pPlayer, float elapsedSec);
		void Attack(Player* pPlayer);
		void HandleMovement(float elapsedSec);
		void UpdateAnimationFrame();
};
