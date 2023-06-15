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
	    explicit Parent(const Point2f& position, float movementSpeed, float jumpSpeed, unsigned int health, unsigned int attackDamage, 
			TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);

	    virtual ~Parent() override;

		Parent(const Parent&) = delete;
		Parent& operator=(const Parent&) = delete;
		Parent(Parent&&) = delete;
		Parent& operator=(Parent&&) = delete;

		virtual void Update(float elapsedSec, Level* pLevel, Player* pPlayer) override;
		virtual void Draw() const override;
		virtual void TakeDamage(unsigned int attackDamage) override;

	private:
		enum class ParentState 
		{
			spawning,
			idle,
			jumping,
			walking,
			attacking,
			dying
		};

		ParentState m_ParentState;
		ParentState m_PreviousParentState;

		const Texture* m_pTexture;

		Stopwatch* m_pSpawnStopwatch;
		unsigned int m_CurrentSpawnFrame;
		const unsigned int m_SpawnFrames;

		Stopwatch* m_pWalkStopwatch;
		unsigned int m_CurrentWalkFrame;
		const unsigned int m_WalkFrames;

		Stopwatch* m_pAttackStopwatch;
		unsigned int m_CurrentAttackFrame;
		const unsigned int m_AttackFrames;

		Stopwatch* m_pDyingStopwatch;
		unsigned int m_CurrentDyingFrame;
		const unsigned int m_DyingFrames;

		Stopwatch* m_pDeSpawnStopwatch;

		Stopwatch* m_pAttackCooldownStopwatch;

		SoundEffect* m_pDeathSoundEffect;
		SoundEffect* m_pAttackSoundEffect;
		SoundEffect* m_pHitSoundEffect;

		const Rectf ChangeSourceRect() const;
		void ChangeShape();

		void ChangeEnemyState(Player* pPlayer);
		void HandleEnemyBehavior(Player* pPlayer, float elapsedSec);
		void Attack(Player* pPlayer);
		void Move(float elapsedSec);

		void ChangeAnimationFrame();
		void ResetAnimationStopwatches();

		void ChangeSpawnAnimationFrame();
		void ChangeWalkAnimationFrame();
		void ChangeAttackAnimationFrame();
		void ChangeDyingAnimationFrame();
};

