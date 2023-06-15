#pragma once
#include "utils.h"
#include "Stopwatch.h"

class Level;
class Texture;
class TextureManager;
class SoundManager;
class TimeObjectManager;
class StopwatchManager;
class PowerUpInventory;
class BasePowerUp;
class Stopwatch;
class Rope;
class SoundEffect;
class SoundStream;

class Player final
{
    public:
        explicit Player(const Point2f& playerPosition, TextureManager* pTextureManager, SoundManager* pSoundManager, TimeObjectManager* pTimeObjectManager);
		~Player();

        Player(const Player&) = delete;
        Player& operator=(const Player&) = delete;
        Player(Player&&) = delete;
        Player& operator=(Player&&) = delete;

        void Draw() const;
        void DrawInventory(float windowWidth, float windowheight) const;
        void DrawHealth(float windowWidth, float windowHeight) const;
        void DrawAbilities(float windowWidth, float windowHeight) const;

		void Update(float elapsedSec, Level* pLevel, const Uint8* pInput);
        const Rectf& GetShape() const;

        bool CanFireATG() const;

		//Movement and interaction (with the level)
        void ClimbRope(const Rope* pRope);
        void StopClimbing();
        void UseLaunchPad(const Vector2f& velocity);

        void TakeDamage(unsigned int attackDamage);

		//Powerups
        void CollectPowerUp(BasePowerUp* pPowerUp);

        void IncreaseMaxHealth(unsigned int healthBoost);
        void IncreaseMovementSpeed(float movementSpeedBoost);

    private:
        //Not owned by player
        TextureManager* m_pTextureManager;
        SoundManager* m_pSoundManager;
        TimeObjectManager* m_pTimeObjectManager;
        //

        StopwatchManager* m_pPlayerStopwatchManager;

        enum class PlayerState
        {
            idle,
            walking,
            jumping,
            climbing,
            rolling,
            doubleTap,
            fullMetalJacket,
            dying
        };

        Rectf m_Shape;
        const Texture* m_pTexture;
        bool m_IsFacingLeft;

		float m_HorSpeed;
        const float m_JumpSpeed;
        const float m_ClimbSpeed;
        Vector2f m_Velocity;
        const Vector2f m_Acceleration;

        unsigned int m_MaxHealth;
        unsigned int m_CurrentHealth;
        unsigned int m_AttackRange;
        unsigned int m_AttackDamage;

        bool m_IsInvulnerable;
        bool m_CanFireATG;

        PowerUpInventory* m_pInventory;

        unsigned int m_JumpCounter;
        unsigned int m_MaxJumps;

        PlayerState m_PlayerState;
        PlayerState m_PreviousPlayerState;

        unsigned int m_CurrentWalkFrame;
        const unsigned int m_WalkFrames;

        unsigned int m_CurrentClimbFrame;
        const unsigned int m_ClimbFrames;

        unsigned int m_CurrentRollFrame;
        const unsigned int m_RollFrames;

        unsigned int m_CurrentDoubleTapFrame;
        const unsigned int m_DoubleTapFrames; 

        unsigned int m_CurrentFullMetalJacketFrame;
        const unsigned int m_FulllMetalJacketFrames;
	
        unsigned int m_CurrentDeathFrame;
        const unsigned int m_DeathFrames;

        Stopwatch* m_pWalkStopwatch;
        Stopwatch* m_pClimbStopwatch;
        Stopwatch* m_pRollStopwatch;
        Stopwatch* m_pRollCooldownStopwatch;

        Stopwatch* m_pDoubleTapStopwatch;
        Stopwatch* m_pDoubleTapCooldownStopwatch;

        Stopwatch* m_FullMetalJacketStopwatch;
        Stopwatch* m_FullMetalJacketCooldownStopwatch;

        Stopwatch* m_pDyingStopwatch;
        Stopwatch* m_pHealthRegenStopwatch;
        Stopwatch* m_pNoDamageTakenStopwatch;

        SoundEffect* m_pAttackBulletSound;
        SoundEffect* m_pCollectPowerUp;

        //Helper functions
        void InitializeStopwatches();
        void DeleteStopwatches();
        void InitializeSounds();
        void DeleteSounds();
    
        void HandleHealthRegen();

        void ChangePlayerState(const Level* pLevel, const Uint8* pInput);

        void HandleMovementKeyboardInput(const Uint8* pInput);
        void HandleMovement(float elapsedSec);
        void HandleAttackStates(Level* pLevel);

        void StartStopwatches();
        void HandleStopwatches(float elapsedSec);
        void ResetCooldownStopwatches(float elapsedSec);

        void ResetAnimationStopwatches();

        void ChangeCharacterShape();
        const Rectf ChangeCharacterSourceRect() const;

        void DrawPlayer() const;
        void DrawBullet() const;
};

