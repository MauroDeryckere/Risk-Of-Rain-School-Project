#include "pch.h"
#include "Avatar.h"

#include "Level.h"
#include "Texture.h"

#include <iostream>


Avatar::Avatar() :
	//Avatar
	m_Shape{ Rectf{50,280,72,97} },
	m_HorSpeed{ 200.f },
	m_JumpSpeed{ 600.f },
	m_Velocity{ Vector2f{0.f,0.f} },
	m_Acceleration{ Vector2f{0.f,-981.f} },
	m_ActionState{ ActionState::moving },
	m_AccuTransformSec{ 0.f },
	m_MaxTransformSec{ 1.f },
	m_Power{ 0 },

	//Spritesheet
	m_pSpritesTexture{ new Texture{"Images/AvatarSheet.png"}},
	m_ClipHeight{97.f},
	m_ClipWidth{72.f},
	m_NrOfFrames{10},
	m_NrOfFramesPerSec{10},
	m_AnimTime{},
	m_AnimFrame{}
{
}
Avatar::~Avatar()
{
	delete m_pSpritesTexture;
	m_pSpritesTexture = nullptr;
}

void Avatar::Update(float elapsedSec, const Level& level)
{
	//Minigame Part 2 - Animation 
	ChangeAnimationFrame(elapsedSec);

	//Character State
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	const bool isKeyboardInput{ pStates[SDL_SCANCODE_LEFT] ||
								pStates[SDL_SCANCODE_RIGHT] ||
								pStates[SDL_SCANCODE_UP]};

	const bool isOnGround{ level.IsOnGround(m_Shape, m_Velocity) };

	switch (m_ActionState)
	{
	case Avatar::ActionState::waiting:
		if (isOnGround && isKeyboardInput)
		{
			m_ActionState = ActionState::moving;
		}
		break;

	case Avatar::ActionState::moving:
		if (isOnGround)
		{
			m_Velocity.x = 0.f; //Reset every time when on ground to make sure no velocity remains saved
			if (!isKeyboardInput)
			{
				m_ActionState = ActionState::waiting;
				break;
			}
			HandleKeyboardInput(pStates);
		}

		HandleMovement(elapsedSec);
		level.HandleCollision(m_Shape, m_Velocity);
		break;

	case Avatar::ActionState::transforming:
		Transform(elapsedSec);
		break;
	}
}

void Avatar::Draw() const
{
	if (m_Velocity.x < 0.f)
	{
		//flip Avatar
		glPushMatrix();
			glTranslatef(m_Shape.left + m_ClipWidth / 2, 0, 0);
			glScalef(-1, 1, 1);
			glTranslatef(-(m_Shape.left + m_ClipWidth / 2), 0, 0);
				DrawAvatar();
		glPopMatrix();
	}
	else
	{
		DrawAvatar();
	}
}

void Avatar::PowerUpHit()
{
	m_Power++;

	//When removing this, jump continues after picking powerup up
	m_Velocity.x = 0.f;
	m_Velocity.y = 0.f;

	m_ActionState = ActionState::transforming;
}

Rectf Avatar::GetShape() const
{
	return m_Shape;
}


void Avatar::DrawAvatar() const
{
	Rectf sourceRect{ m_AnimFrame * m_ClipWidth,
					  m_ClipHeight + (int(m_ActionState) * m_ClipHeight) + (m_Power * 3 * m_ClipHeight),
					  m_ClipWidth,
					  m_ClipHeight };

	if (m_ActionState == ActionState::transforming)
		sourceRect.bottom -= 3 * m_ClipHeight;

	m_pSpritesTexture->Draw(m_Shape, sourceRect);
}

void Avatar::HandleKeyboardInput(const Uint8* pStates)
{
	if (pStates[SDL_SCANCODE_LEFT])
	{
		m_Velocity.x = -m_HorSpeed;
	}
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_Velocity.x = m_HorSpeed;
	}

	if (pStates[SDL_SCANCODE_UP])
	{
		m_Velocity.y = m_JumpSpeed;
	}
}

void Avatar::HandleMovement(float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;

	m_Velocity.y += m_Acceleration.y * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
}

void Avatar::Transform(float elapsedSec)
{
	m_AccuTransformSec += elapsedSec;

	if (m_AccuTransformSec >= m_MaxTransformSec)
	{
		m_AccuTransformSec = 0.f;
		m_ActionState = ActionState::moving;
	}
}


void Avatar::ChangeAnimationFrame(float elapsedSec)
{
	m_AnimTime += elapsedSec;

	if (m_AnimTime >= 1.f / m_NrOfFramesPerSec)
	{
		m_AnimTime = 0.f;

		if (m_AnimFrame - 1 == m_NrOfFrames)
		{
			m_AnimFrame = 0;
			return;
		}
		m_AnimFrame++;
	}
}


