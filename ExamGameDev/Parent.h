#pragma once
#include "BaseEnemy.h"

class TimeObjectManager;
class StopwatchManager;
class Stopwatch;

class Texture;
class SoundManager;
class TextureManager;
class SoundEffect;

class Parent final:
    public BaseEnemy
{
	public:
	    Parent(const Point2f& position, float movementSpeed, float jumpSpeed, size_t health, size_t attackDamage,
			TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);

	    virtual ~Parent() override;

		Parent(const Parent&) = delete;
		Parent& operator=(const Parent&) = delete;
		Parent(Parent&&) = delete;
		Parent& operator=(Parent&&) = delete;

		virtual void Update(float elapsedSec, Level* pLevel, Player* pPlayer) override;
		virtual void Draw() const override;
		virtual void TakeDamage(Player* pPlayer, size_t attackDamage) override;

	private:
		enum class ParentState 
		{
			spawning,
			idle,
			walking,
			attacking,
			dying
		};

		ParentState m_ParentState;

		const Texture* m_pTexture;

		Stopwatch* m_pSpawnStopwatch;
		size_t m_CurrentSpawnFrame;
		const size_t m_SpawnFrames;

		Stopwatch* m_pWalkStopwatch;
		size_t m_CurrentWalkFrame;
		const size_t m_WalkFrames;

		Stopwatch* m_pAttackStopwatch;
		size_t m_CurrentAttackFrame;
		const size_t m_AttackFrames;

		Stopwatch* m_pDyingStopwatch;
		size_t m_CurrentDyingFrame;
		const size_t m_DyingFrames;

		Stopwatch* m_pDeSpawnStopwatch;

		Stopwatch* m_pAttackCooldownStopwatch;

		SoundEffect* m_pDeathSoundEffect;
		SoundEffect* m_pAttackSoundEffect;
		SoundEffect* m_pHitSoundEffect;

		const Rectf& ChangeSourceRect() const;
		void UpdateShape();

		void UpdateEnemyState(Player* pPlayer);
		void UpdateEnemyBehavior(Player* pPlayer, float elapsedSec);
		void Attack(Player* pPlayer);
		void HandleMovement(float elapsedSec);

		void UpdateAnimationFrame();
		void ResetAnimationStopwatches();

		void UpdateSpawnAnimationFrame();
		void UpdateWalkAnimationFrame();
		void UpdateAttackAnimationFrame();
		void UpdateDyingAnimationFrame();
};

