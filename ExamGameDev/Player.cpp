#include "pch.h"
#include "Player.h"

#include "Texture.h"
#include <iostream>

#include "TextureManager.h"
#include "SoundManager.h"
#include "Level.h"
#include "PowerUpInventory.h"
#include "TimeObjectManager.h"
#include "StopwatchManager.h"
#include "Stopwatch.h"
#include "Rope.h"
#include "SoundEffect.h"
#include "SoundStream.h"

#define digTexture TextureManager::NumberTextures::_20PxWhiteDigits

Player::Player(const Point2f& playerPosition, TextureManager* pTextureManager, SoundManager* pSoundManager, TimeObjectManager* pTimeObjectManager) :
	m_pTextureManager{ pTextureManager },
	m_pSoundManager{ pSoundManager },
	m_pTimeObjectManager{ pTimeObjectManager },
	m_pPlayerStopwatchManager{ m_pTimeObjectManager->CreateStopwatchManager() },

	m_Shape{ Rectf {playerPosition.x, playerPosition.y, 16.f, 24.f} },
	m_pTexture{ m_pTextureManager->CreateTexture("Character/CharacterSpriteSheetFull.png", "player") },
	m_IsFacingLeft{ false },

	m_HorSpeed{ 100.f },
	m_JumpSpeed{ 300.f },
	m_ClimbSpeed{ 100.f },
	m_Velocity{ 0.f,0.f },
	m_Acceleration{ 0.f,-981.f },

	m_MaxHealth{ 100 },
	m_CurrentHealth{ m_MaxHealth },
	m_AttackRange{ 100 },
	m_AttackDamage{ 25 },

	m_IsInvulnerable{ false },
	m_CanFireATG{ false },

	m_pInventory{ new PowerUpInventory{ pTextureManager, 20 } },

	m_JumpCounter{ 0 },
	m_MaxJumps{ 1 },

	m_PlayerState{ PlayerState::idle },
	m_PreviousPlayerState{PlayerState::idle},

	m_CurrentWalkFrame{ 0 },
	m_WalkFrames{ 8 },

	m_CurrentClimbFrame{ 0 },
	m_ClimbFrames{ 2 },

	m_CurrentRollFrame{ 0 },
	m_RollFrames{ 9 },

	m_CurrentDoubleTapFrame{ 0 },
	m_DoubleTapFrames{ 5 },

	m_CurrentFullMetalJacketFrame{0},
	m_FulllMetalJacketFrames{5},

	m_CurrentDeathFrame{ 0 },
	m_DeathFrames{ 5 }
{
	InitializeStopwatches();
	InitializeSounds();
}

Player::~Player()
{
	delete m_pInventory;
	m_pTextureManager->DeleteTexture("player");
	DeleteStopwatches();
	DeleteSounds();
}

void Player::Update(float elapsedSec, Level* pLevel, const Uint8* pInput)
{
	m_PreviousPlayerState = m_PlayerState;

	HandleHealthRegen();

	ChangePlayerState(pLevel, pInput);
		ChangeCharacterShape();

	HandleMovementKeyboardInput(pInput);
	HandleMovement(elapsedSec);
	HandleAttackStates(pLevel);

	if (m_PlayerState != PlayerState::climbing)
	{
		pLevel->HandleCollision(m_Shape, m_Velocity);
	}

	HandleStopwatches(elapsedSec);

	m_pInventory->ActivatePowerUps(this, pLevel);
	m_CanFireATG = false;
}

void Player::Draw() const
{
	DrawPlayer();
}

void Player::DrawInventory(float windowWidth, float windowHeight) const
{
	m_pInventory->Draw(windowWidth, windowHeight);
}

void Player::DrawHealth(float windowWidth, float windowHeight) const
{
	const float healthBarWidth{ windowWidth/6.4f };
	const float healthBarHeight{ windowHeight/72 };
	
	constexpr float offset{ 1.f };

	const float healthBarLeft{windowWidth/2 - healthBarWidth /2};
	const float healthBarBottom{ 40.f };

	float numWidth{};
	float symbolWidth{};

	Point2f bottomLeft{ healthBarLeft - offset, healthBarBottom };

	//Background
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, .5f });
	utils::FillRect(healthBarLeft - offset, healthBarBottom, healthBarWidth + 2 * offset, healthBarHeight);

	//Health
	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	utils::FillRect(healthBarLeft, healthBarBottom, healthBarWidth * m_CurrentHealth/m_MaxHealth, healthBarHeight);

	m_pTextureManager->DrawNumber(digTexture, bottomLeft, m_CurrentHealth, 0.f, numWidth);
	bottomLeft.x += numWidth;
	m_pTextureManager->DrawSymbol(digTexture, bottomLeft, TextureManager::Symbols::Slash, 0.f, symbolWidth);
	bottomLeft.x += symbolWidth;
	m_pTextureManager->DrawNumber(digTexture, bottomLeft, m_MaxHealth, 0.f);

	#undef digTexture
}

void Player::DrawAbilities(float windowWidth, float windowHeight) const
{
	constexpr float scale{ 2.f };
	constexpr float abWidth{ 18.f };
	constexpr float abHeight{ 18.f };

	constexpr size_t amountOfAbilities{ 4 };

	constexpr float spaceBetwAb{ 10.f };

	const Rectf doesntExistSourceRect{ 4.f + (5) * abWidth + 4.f * (5),
										514.f,
										abWidth ,
										abHeight };

	for (size_t index{ 0 }; index < amountOfAbilities; ++index)
	{
		constexpr float spritesheetOffset{ 4.f };

		const Rectf destRect{ spritesheetOffset + index * (scale * abWidth) + spaceBetwAb * index,
							  0.f,
							  scale * abWidth,
							  scale * abHeight };

		const Rectf sourceRect{ spritesheetOffset + index * abWidth + spritesheetOffset * index,
								514.f,
								abWidth ,
								abHeight };

		const Color4f usingOverlay{ 0.f,0.f,0.f,.25f };
		const Color4f cooldownOverLay{ 0.f,0.f,0.f,.5f };

		switch (index)
		{
		case 0:
			m_pTexture->Draw(destRect, sourceRect);

			if (m_pDoubleTapStopwatch->IsRunning())
			{
				utils::SetColor(usingOverlay);
				utils::FillRect(destRect);
			}
			else if (m_pDoubleTapCooldownStopwatch->IsRunning())
			{
				const Rectf cooldownOverlayRect{ destRect.left,
												 destRect.bottom,
												 destRect.width,
												(1.f - m_pDoubleTapCooldownStopwatch->GetCurrSec() / m_pDoubleTapCooldownStopwatch->GetMaxSec()) * destRect.height };
			
				utils::SetColor(cooldownOverLay);
				utils::FillRect(cooldownOverlayRect);
			}
			break;
		case 1:
			m_pTexture->Draw(destRect, sourceRect);

			if (m_FullMetalJacketStopwatch->IsRunning())
			{
				utils::SetColor(usingOverlay);
				utils::FillRect(destRect);
			}
			else if (m_FullMetalJacketCooldownStopwatch->IsRunning())
			{
				const Rectf cooldownOverlayRect{ destRect.left,
												 destRect.bottom,
												 destRect.width,
												(1.f - m_FullMetalJacketCooldownStopwatch->GetCurrSec() / m_FullMetalJacketCooldownStopwatch->GetMaxSec()) * destRect.height };
			
				utils::SetColor(cooldownOverLay);
				utils::FillRect(cooldownOverlayRect);
			}
			break;
		case 2:
			m_pTexture->Draw(destRect, sourceRect);

			if (m_pRollStopwatch->IsRunning())
			{
				utils::SetColor(usingOverlay);
				utils::FillRect(destRect);
			}
			else if (m_pRollCooldownStopwatch->IsRunning())
			{
				const Rectf cooldownOverlayRect{ destRect.left, 
												 destRect.bottom, 
												 destRect.width, 
												(1.f - m_pRollCooldownStopwatch->GetCurrSec()/ m_pRollCooldownStopwatch->GetMaxSec()) * destRect.height };
				
				utils::SetColor(cooldownOverLay);
				utils::FillRect(cooldownOverlayRect);
			}
			break;
		case 3:
			m_pTexture->Draw(destRect, doesntExistSourceRect);
			break;
		}
	}
}

const Rectf& Player::GetShape() const
{
	return m_Shape;
}

bool Player::CanFireATG() const
{
	return m_CanFireATG;
}

void Player::ClimbRope(const Rope* pRope)
{
	if (m_PlayerState != PlayerState::climbing)
	{
		m_PlayerState = PlayerState::climbing;
		m_Shape.left = pRope->GetShape().left + pRope->GetShape().width / 2 - m_Shape.width / 2;
	}
}

void Player::StopClimbing()
{
	if (m_PlayerState == PlayerState::climbing)
	{
		m_PlayerState = PlayerState::idle;
	}
}

void Player::UseLaunchPad(const Vector2f& velocity)
{
	m_Velocity.y = velocity.y;
	m_PlayerState = PlayerState::jumping;
}

void Player::TakeDamage(unsigned int attackDamage)
{
	if (m_PlayerState == PlayerState::dying || m_IsInvulnerable)
	{
		return;
	}

	m_CurrentHealth = std::max(int(m_CurrentHealth - attackDamage), 0);

	m_pNoDamageTakenStopwatch->Reset();
	m_pNoDamageTakenStopwatch->Start();
		
	m_pHealthRegenStopwatch->Reset();
}

void Player::CollectPowerUp(BasePowerUp* pPowerUp)
{
	m_pInventory->AddPowerUp(pPowerUp);
	m_pCollectPowerUp->Play(0);
}

void Player::IncreaseMaxHealth(unsigned int healthBoost)
{
	m_MaxHealth += healthBoost;
	//std::cout << "Max Health: " << m_MaxHealth << "\n";
}

void Player::IncreaseMovementSpeed(float movementSpeedBoost)
{
	m_HorSpeed += movementSpeedBoost;
	//std::cout << "WalkSpeed: " << m_HorSpeed << "\n";
}

void Player::InitializeStopwatches()
{
	m_pWalkStopwatch = m_pPlayerStopwatchManager->CreateStopwatch(m_HorSpeed / 750);
	m_pClimbStopwatch = m_pPlayerStopwatchManager->CreateStopwatch(m_ClimbSpeed / 750);
	m_pRollStopwatch = m_pPlayerStopwatchManager->CreateStopwatch(m_HorSpeed / 750);
	m_pRollCooldownStopwatch = m_pPlayerStopwatchManager->CreateStopwatch( 3.f );

	m_pDoubleTapStopwatch = m_pPlayerStopwatchManager->CreateStopwatch(.2f);
	m_pDoubleTapCooldownStopwatch = m_pPlayerStopwatchManager->CreateStopwatch(.5f );

	m_FullMetalJacketStopwatch = m_pPlayerStopwatchManager->CreateStopwatch(.1f);
	m_FullMetalJacketCooldownStopwatch = m_pPlayerStopwatchManager->CreateStopwatch(4.f);

	m_pDyingStopwatch = m_pPlayerStopwatchManager->CreateStopwatch(.5f );

	m_pHealthRegenStopwatch = m_pPlayerStopwatchManager->CreateStopwatch(1.f );
	m_pNoDamageTakenStopwatch = m_pPlayerStopwatchManager->CreateStopwatch(9.f );

	m_pNoDamageTakenStopwatch->Start();
}

void Player::DeleteStopwatches()
{
	m_pTimeObjectManager->DeleteStopwatchManager(m_pPlayerStopwatchManager);
}

void Player::InitializeSounds()
{
	m_pAttackBulletSound = m_pSoundManager->CreateSoundEffect("playerBullet", "Sounds/SoundEffects/wBullet1.ogg");
	m_pCollectPowerUp = m_pSoundManager->CreateSoundEffect("useSound", "Sounds/SoundEffects/wPickup.ogg");
}

void Player::DeleteSounds()
{
	m_pSoundManager->DeleteSoundEffect("playerBullet");
	m_pSoundManager->DeleteSoundEffect("useSound");
}

void Player::HandleHealthRegen()
{
	if (m_CurrentHealth == m_MaxHealth || m_PlayerState == PlayerState::dying)
	{
		return;
	}
	if (m_pHealthRegenStopwatch->IsTimeReached() )
	{
		constexpr unsigned int healthRegen{1};
		m_CurrentHealth = std::min(m_CurrentHealth + healthRegen, m_MaxHealth);
	}
}

void Player::ChangePlayerState(const Level* pLevel, const Uint8* pInput)
{
	if (m_CurrentHealth == 0 || m_PlayerState == PlayerState::dying)
	{
		m_PlayerState = PlayerState::dying;
		return;
	}

	const bool isOnGround{ pLevel->IsOnGround(m_Shape) };

	const bool isJumpInput{ pInput[SDL_SCANCODE_SPACE] && true };

	const bool isWalkInput{ pInput[SDL_SCANCODE_LEFT] ||
							pInput[SDL_SCANCODE_RIGHT] };

	const bool isClimbInput{ pInput[SDL_SCANCODE_UP] ||
							 pInput[SDL_SCANCODE_DOWN] };

	const bool isRollInput{ pInput[SDL_SCANCODE_C] && !m_pRollCooldownStopwatch->IsRunning() };

	const bool isDoubleTapInput{ pInput[SDL_SCANCODE_W] &&  !m_pDoubleTapCooldownStopwatch->IsRunning()};

	const bool isFullMetalJacketInput{ pInput[SDL_SCANCODE_X] && !m_FullMetalJacketCooldownStopwatch->IsRunning() };

	switch (m_PlayerState)
	{
	case PlayerState::idle:
		if (isOnGround)
		{
			if (isJumpInput)
			{
				m_PlayerState = PlayerState::jumping;
			}
			else if (isWalkInput && m_Velocity.x != 0.f)
			{
				m_PlayerState = PlayerState::walking;
			}
			else if (isRollInput)
			{
				m_PlayerState = PlayerState::rolling;
			}
			else if (isDoubleTapInput)
			{
				m_PlayerState = PlayerState::doubleTap;
			}
			else if (isFullMetalJacketInput)
			{
				m_PlayerState = PlayerState::fullMetalJacket;
			}
			else
			{
				m_PlayerState = PlayerState::idle;
			}
		}
		else
		{
			m_PlayerState = PlayerState::jumping;
		}
		break;
	case PlayerState::walking:
		if (isOnGround)
		{
			if ((!isWalkInput && !isJumpInput) || m_Velocity.x == 0.f)
			{
				m_PlayerState = PlayerState::idle;
			}
			else if (isJumpInput)
			{
				m_PlayerState = PlayerState::jumping;
			}
			else if (isRollInput)
			{
				m_PlayerState = PlayerState::rolling;
			}
			else if (isDoubleTapInput)
			{
				m_PlayerState = PlayerState::doubleTap;
			}
			else if (isFullMetalJacketInput)
			{
				m_PlayerState = PlayerState::fullMetalJacket;
			}
			else
			{
				m_PlayerState = PlayerState::walking;
			}
		}
		else
		{
			m_PlayerState = PlayerState::jumping;
		}
		break;
	case PlayerState::jumping:
		if (isOnGround)
		{
			if (isWalkInput)
			{
				m_PlayerState = PlayerState::walking;
			}
			else if (isRollInput)
			{
				m_PlayerState = PlayerState::rolling;
			}
			else if (isDoubleTapInput)
			{
				m_PlayerState = PlayerState::doubleTap;
			}
			else if (isFullMetalJacketInput)
			{
				m_PlayerState = PlayerState::fullMetalJacket;
			}
			else
			{
				m_PlayerState = PlayerState::idle;
			}
		}
		else
		{
			if (isDoubleTapInput)
			{
				m_PlayerState = PlayerState::doubleTap;
			}
			else
			{
			 m_PlayerState = PlayerState::jumping;
			}
		}
		break;
	case PlayerState::climbing:
		if (isJumpInput && !isClimbInput && !pLevel->IsIntersectingMapOnRope(m_Shape))
		{
			m_PlayerState = PlayerState::jumping;
		}
		break;
	case PlayerState::rolling:
		if (!m_pRollCooldownStopwatch->IsRunning())
		{
			m_PlayerState = PlayerState::rolling;
		}
		else
		{
			if (isOnGround)
			{
				if (isJumpInput)
				{
					m_PlayerState = PlayerState::jumping;
				}
				else if (isWalkInput && m_Velocity.x != 0.f)
				{
					m_PlayerState = PlayerState::walking;
				}
				else if (isDoubleTapInput)
				{
					m_PlayerState = PlayerState::doubleTap;
				}
				else if (isFullMetalJacketInput)
				{
					m_PlayerState = PlayerState::fullMetalJacket;
				}
				else
				{
					m_PlayerState = PlayerState::idle;
				}
			}
			else
			{
				m_PlayerState = PlayerState::jumping;
			}
		}
		break;
	case PlayerState::doubleTap:
		if (!m_pDoubleTapCooldownStopwatch->IsRunning())
		{
			m_PlayerState = PlayerState::doubleTap;
		}
		else
		{
			if (isOnGround)
			{
				if (isJumpInput)
				{
					m_PlayerState = PlayerState::jumping;
				}
				else if (isWalkInput && m_Velocity.x != 0.f)
				{
					m_PlayerState = PlayerState::walking;
				}
				else if (isRollInput)
				{
					m_PlayerState = PlayerState::rolling;
				}
				else
				{
					m_PlayerState = PlayerState::idle;
				}
			}
			else
			{
				m_PlayerState = PlayerState::jumping;
			}
		}
		break;
	case PlayerState::fullMetalJacket:
		if (!m_FullMetalJacketCooldownStopwatch->IsRunning())
		{
			m_PlayerState = PlayerState::fullMetalJacket;
		}
		else
		{
			if (isOnGround)
			{
				if (isJumpInput)
				{
					m_PlayerState = PlayerState::jumping;
				}
				else if (isWalkInput && m_Velocity.x != 0.f)
				{
					m_PlayerState = PlayerState::walking;
				}
				else if (isRollInput)
				{
					m_PlayerState = PlayerState::rolling;
				}
				else
				{
					m_PlayerState = PlayerState::idle;
				}
			}
			else
			{
				m_PlayerState = PlayerState::jumping;
			}
		}
		break;
	}
}

void Player::HandleMovementKeyboardInput(const Uint8* pInput)
{
	const bool isHorizontalMovementAllowed{	   m_PlayerState != PlayerState::climbing 
											&& m_PlayerState != PlayerState::doubleTap 
											&& m_PlayerState != PlayerState::dying };

	m_Velocity.x = 0.f;

	if (m_PlayerState != PlayerState::jumping
		&& m_PlayerState != PlayerState::climbing) 
	{
		m_JumpCounter = 0;
	}

	if (isHorizontalMovementAllowed)
	{
		if (m_PlayerState == PlayerState::rolling)
		{
			if (m_IsFacingLeft)
			{
				m_Velocity.x -= m_HorSpeed;
			}
			else
			{
				m_Velocity.x += m_HorSpeed;
			}
		}
		else
		{
			if (pInput[SDL_SCANCODE_LEFT])
			{
				m_Velocity.x -= m_HorSpeed;
			}
			if (pInput[SDL_SCANCODE_RIGHT])
			{
				m_Velocity.x += m_HorSpeed;
			}
		}
	}

	if (m_PlayerState == PlayerState::jumping && m_JumpCounter < m_MaxJumps && m_Velocity.y >= 0.f)
	{
		if (pInput[SDL_SCANCODE_SPACE])
		{
			m_Velocity.y = m_JumpSpeed;
			++m_JumpCounter;
		}
	}

	if (m_PlayerState == PlayerState::climbing)
	{
		m_Velocity.y = 0.f;
		if (pInput[SDL_SCANCODE_UP])
		{
			m_Velocity.y += m_ClimbSpeed;
		}
		if (pInput[SDL_SCANCODE_DOWN])
		{
			m_Velocity.y -= m_ClimbSpeed;
		}
	}
}
void Player::HandleMovement(const float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;

	if (m_PlayerState != PlayerState::climbing)
	{
		m_Velocity.y += m_Acceleration.y * elapsedSec;
	}
		
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	if (m_Velocity.x < 0.f)
	{
		m_IsFacingLeft = true;
	}
	else if (m_Velocity.x > 0.f)
	{
		m_IsFacingLeft = false;
	}
}

void Player::HandleAttackStates(Level* pLevel)
{
	switch (m_PlayerState)
	{
	case PlayerState::doubleTap:
		if (m_PreviousPlayerState != PlayerState::doubleTap)
		{
			m_CanFireATG = pLevel->HandleBulletRayCast(m_Shape, m_IsFacingLeft, m_AttackRange, m_AttackDamage);
		}
		break;
	case PlayerState::fullMetalJacket: //TODO: knockback
		if (m_PreviousPlayerState != PlayerState::fullMetalJacket)
		{
			m_CanFireATG = pLevel->HandleBulletRayCast(m_Shape, m_IsFacingLeft, m_AttackRange, m_AttackDamage, 5);
		}
		break;
	}
}

void Player::HandleStopwatches(const float elapsedSec)
{
	StartStopwatches();
	ResetAnimationStopwatches();
	ResetCooldownStopwatches(elapsedSec);
}

void Player::StartStopwatches()
{
	switch (m_PlayerState)
	{
	case Player::PlayerState::walking:
		m_pWalkStopwatch->Start();

		if (m_pWalkStopwatch->IsTimeReached())
		{
			++m_CurrentWalkFrame %= m_WalkFrames;
		}
		break;

	case Player::PlayerState::climbing:
		m_pClimbStopwatch->Start();

		if (m_Velocity.y == 0.f)
		{
			m_pClimbStopwatch->Reset();
		}

		if (m_pClimbStopwatch->IsTimeReached())
		{
			++m_CurrentClimbFrame %= m_ClimbFrames;
		}
		break;

	case Player::PlayerState::rolling:
		m_pRollStopwatch->Start();

		if (m_pRollStopwatch->IsTimeReached())
		{
			++m_CurrentRollFrame;

			if (m_CurrentRollFrame == m_RollFrames)
			{
				m_pRollCooldownStopwatch->Start();
				m_pRollStopwatch->Reset();

				m_CurrentRollFrame = 0;
			}
		}

		break;

	case Player::PlayerState::doubleTap:
		m_pDoubleTapStopwatch->Start();

		if (m_pDoubleTapStopwatch->IsTimeReached())
		{
			if (m_CurrentDoubleTapFrame == 0 || m_CurrentDoubleTapFrame == 2)
			{
				m_pAttackBulletSound->Play(0);
			}

			++m_CurrentDoubleTapFrame;

			if (m_CurrentDoubleTapFrame == m_DoubleTapFrames)
			{
				m_pDoubleTapCooldownStopwatch->Start();
				m_pDoubleTapStopwatch->Reset();

				m_CurrentDoubleTapFrame = 0;
			}
		}		
		break;

	case Player::PlayerState::fullMetalJacket:
		m_FullMetalJacketStopwatch->Start();

		if (m_FullMetalJacketStopwatch->IsTimeReached())
		{
			if (m_CurrentFullMetalJacketFrame == 0)
			{
				m_pAttackBulletSound->Play(0);
			}

			++m_CurrentFullMetalJacketFrame;

			if (m_CurrentFullMetalJacketFrame == m_FulllMetalJacketFrames)
			{
				m_FullMetalJacketCooldownStopwatch->Start();
				m_FullMetalJacketStopwatch->Reset();

				m_CurrentFullMetalJacketFrame = 0;
			}
		}
		break;

	case Player::PlayerState::dying:
		if (m_CurrentDeathFrame != m_DeathFrames - 1)
		{
			m_pDyingStopwatch->Start();
		}

		if (m_pDyingStopwatch->IsTimeReached())
		{
			if (m_CurrentDeathFrame == m_DeathFrames - 1)
			{
				m_pDyingStopwatch->Reset();

				std::cout << "dead \n";
			}
			else
			{
				++m_CurrentDeathFrame;
			}
		}
		break;
	}

	if (m_pNoDamageTakenStopwatch->IsTimeReached())
	{
		m_pHealthRegenStopwatch->Start();
	}
}

void Player::ResetCooldownStopwatches(float elapsedSec)
{
	//Cooldown timers for abilities
	if (m_pRollCooldownStopwatch->IsTimeReached())
	{
		m_pRollCooldownStopwatch->Reset();
	}

	if (m_pDoubleTapCooldownStopwatch->IsTimeReached())
	{
		m_pDoubleTapCooldownStopwatch->Reset();
	}

	if (m_FullMetalJacketCooldownStopwatch->IsTimeReached())
	{
		m_FullMetalJacketCooldownStopwatch->Reset();
	}
}

void Player::ResetAnimationStopwatches()
{
	if (m_PlayerState != PlayerState::walking)
	{
		m_pWalkStopwatch->Reset();
		m_CurrentWalkFrame = 0;
	}

	if (m_PlayerState != PlayerState::climbing)
	{
		m_pClimbStopwatch->Reset();
		m_CurrentClimbFrame = 0;
	}
}

void Player::ChangeCharacterShape()
{
	constexpr float scale{ 2.f };

	constexpr float sourceWidth{ 8.f }, sourceHeight{ 12.f };
	constexpr float rollSourceWidth{ 12.f }, rollSourceHeight{ 12.f };
	constexpr float dyingSourceWidth{ 18.f }, dyingSourceHeight{ 9.f };

	m_Shape.width = sourceWidth * scale;
	m_Shape.height = sourceHeight * scale;

	switch (m_PlayerState)
	{
	case PlayerState::rolling:
		m_Shape.width = rollSourceWidth * scale;
		m_Shape.height = rollSourceHeight * scale;
		break;
	case PlayerState::dying:
		m_Shape.width = dyingSourceWidth * scale;
		m_Shape.height = dyingSourceWidth * scale;
		break;
	}

}

const Rectf Player::ChangeCharacterSourceRect() const
{
	Rectf sourceRect{};

	constexpr float sourceWidth{ 8.f }, sourceHeight{ 12.f };

	constexpr float rollSourceWidth{ 12.f }, rollSourceHeight{ 12.f };
	constexpr float dyingSourceWidth{ 18.f }, dyingSourceHeight{ 9.f };

	constexpr float spaceBetweenSprites{ 4.f };

	switch (m_PlayerState)
	{
	case PlayerState::idle:
		sourceRect = Rectf{ spaceBetweenSprites,
							31.f,
							sourceWidth,
							sourceHeight };
		break;

	case PlayerState::walking:
		sourceRect = Rectf{ (m_CurrentWalkFrame + 1) * spaceBetweenSprites + m_CurrentWalkFrame * sourceWidth,
							61.f,
							sourceWidth,
							sourceHeight };
		break;

	case PlayerState::jumping:
		sourceRect = Rectf{ spaceBetweenSprites,
							91.f,
							sourceWidth,
							sourceHeight };
		break;

	case PlayerState::climbing:
		sourceRect = Rectf{ (m_CurrentClimbFrame + 1) * spaceBetweenSprites + m_CurrentClimbFrame * sourceWidth,
							122.f,
							sourceWidth,
							sourceHeight };
		break;
	case PlayerState::rolling:
		sourceRect = Rectf{ (m_CurrentRollFrame + 1) * spaceBetweenSprites + m_CurrentRollFrame * rollSourceWidth,
							214.f,
							rollSourceWidth,
							rollSourceHeight };
		break;
	case PlayerState::doubleTap:
		sourceRect = Rectf{ (m_CurrentDoubleTapFrame + 1) * spaceBetweenSprites + m_CurrentDoubleTapFrame * sourceWidth,
							153.f,
							sourceWidth,
							sourceHeight };
		break;
	case PlayerState::fullMetalJacket:
		sourceRect = Rectf{ (m_CurrentFullMetalJacketFrame + 1) * spaceBetweenSprites + m_CurrentFullMetalJacketFrame * sourceWidth,
							183.f,
							sourceWidth,
							sourceHeight };
		break;
	case PlayerState::dying:
		sourceRect = Rectf{ (m_CurrentDeathFrame + 1) * spaceBetweenSprites + m_CurrentDeathFrame * dyingSourceWidth,
							477.f,
							dyingSourceWidth,
							dyingSourceHeight };
		break;
	}

	return sourceRect;
}

void Player::DrawPlayer() const
{
	const Rectf sourceRect{ ChangeCharacterSourceRect() };

	if (m_IsFacingLeft)
	{
		glPushMatrix();
			glTranslatef(m_Shape.left + m_Shape.width / 2, 0, 0);
			glScalef(-1, 1, 1);
			glTranslatef(-(m_Shape.left + m_Shape.width / 2), 0, 0);
				m_pTexture->Draw(m_Shape, sourceRect);
				DrawBullet(); 
		glPopMatrix();
	}
	else
	{
		m_pTexture->Draw(m_Shape, sourceRect);
		DrawBullet();
	}
}

void Player::DrawBullet() const
{
	constexpr float scale{ 2.f };


	const Rectf destRect{ Rectf{ m_Shape.left + m_Shape.width,
								 m_Shape.bottom,
								 11.f * scale,
								 12.f * scale} };

	const Rectf doulbeTapsourceRect{ Rectf{  64.f + m_CurrentDoubleTapFrame * 11.f + m_CurrentDoubleTapFrame * 4.f,
									153.f,
									11.f,
									12.f} };

	const Rectf metalJacketSourceRect{ Rectf{  64.f + m_CurrentFullMetalJacketFrame * 24.f + m_CurrentFullMetalJacketFrame * 4.f,
										183.f,
										24.f,
										12.f} };
	switch (m_PlayerState)
	{

	case Player::PlayerState::doubleTap:
		if (m_pDoubleTapCooldownStopwatch->IsRunning())
		{
			return;
		}
		m_pTexture->Draw(destRect, doulbeTapsourceRect);
		break;

	case Player::PlayerState::fullMetalJacket:
		if (m_FullMetalJacketCooldownStopwatch->IsRunning())
		{
			return;
		}
		m_pTexture->Draw(destRect, metalJacketSourceRect);
		break;
	}
}