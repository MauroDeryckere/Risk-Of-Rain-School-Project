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
class Game;

class Player final
{
    public:
        Player(const Point2f& playerPosition, TextureManager* pTextureManager, SoundManager* pSoundManager, TimeObjectManager* pTimeObjectManager);
		~Player();

        Player(const Player&) = delete;
        Player& operator=(const Player&) = delete;
        Player(Player&&) = delete;
        Player& operator=(Player&&) = delete;

        void Draw() const;

        //HUD
        void DrawInventory(float windowWidth, float windowheight) const;
        void DrawPlayerStatsHud(float windowWidth, float windowHeight) const;

		void Update(float elapsedSec, Level* pLevel, Game* pGame, const Uint8* pInput);
        const Rectf& GetShape() const;

        size_t GetBalance() const;
        void UpdateBalance(int balChange);

        bool CanFireATG() const;

		//Movement and interaction (with the level)
        void ClimbRope(const Rope* pRope);
        void StopClimbing();
        bool IsClimbing() const;
        void UseLaunchPad(const Point2f& bottomCenter, const Vector2f& velocity);

        bool IsOnGround() const;

        //-1 attackDamage == set hp to 1
        void TakeDamage(int attackDamage);

		//Powerups
        void CollectPowerUp(BasePowerUp* pPowerUp);

        void IncreaseMaxHealth(size_t healthBoost);
        void IncreaseMovementSpeed(float movementSpeedBoost);

    private:
        //Not owned by player
        TextureManager* m_pTextureManager;
        SoundManager* m_pSoundManager;
        TimeObjectManager* m_pTimeObjectManager;
        //------------------

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

        Point2f m_LastPosOnGround;
        Point2f m_LastPosOnRope;
        bool m_IsOnGround;

        size_t m_MaxHealth;
        size_t m_CurrentHealth;
        size_t m_AttackRange;
        size_t m_AttackDamage;

        bool m_IsInvulnerable;
        bool m_CanFireATG;

        PowerUpInventory* m_pInventory;
        size_t m_Balance;

        size_t m_JumpCounter;
        size_t m_MaxJumps;

        PlayerState m_PlayerState;
        PlayerState m_PreviousPlayerState;

        size_t m_CurrentWalkFrame;
        const size_t m_WalkFrames;

        size_t m_CurrentClimbFrame;
        const size_t m_ClimbFrames;

        size_t m_CurrentRollFrame;
        const size_t m_RollFrames;

        size_t m_CurrentDoubleTapFrame;
        const size_t m_DoubleTapFrames;

        size_t m_CurrentFullMetalJacketFrame;
        const size_t m_FulllMetalJacketFrames;
	
        size_t m_CurrentDeathFrame;
        const size_t m_DeathFrames;

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

        bool m_WonGame;

        const Texture* m_pHudTexture;
        bool m_WasClimbing;

        //Helper functions
        void InitializeStopwatches();
        void DeleteStopwatches();
        void InitializeSoundEffects();
        void DeleteSoundEffects();
    
        void HandleFallDamage();
        void HandleHealthRegen();

        void UpdatePlayerState(const Level* pLevel, const Uint8* pInput);
        void UpdatePlayerStateAfterAttackAbility(bool isJumpInput, bool isWalkInput, bool isRollInput);

        void HandleMovementKeyboardInput(const Uint8* pInput);
        void HandleMovement(float elapsedSec);
        void HandleAttackStates(Level* pLevel);

        void HandleStopwatches(Game* pGame, float elapsedSec);
        void StartStopwatches(Game* pGame);
        void ResetCooldownStopwatches(float elapsedSec);

        void ResetAnimationStopwatches();

        void ChangePlayerShape();
        const Rectf& ChangePlayerSourceRect() const;

        void DrawPlayer() const;
        void DrawBullet() const;

        //HUD
        void DrawAbilities(float windowWidth, float windowHeight, float hudTextureScale, const Rectf& hudTextureDstrect) const;
        void DrawHealth(float windowWidth, float windowHeight, float hudTextureScale, const Rectf& hudTextureDstrect) const;
};

