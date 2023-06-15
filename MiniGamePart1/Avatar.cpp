#include "pch.h"
#include "Avatar.h"
#include "Level.h"

#include <iostream>


Avatar::Avatar() :
	m_Shape{ Rectf{50,280,36,97} },
	m_HorSpeed{ 200.f },
	m_JumpSpeed{ 600.f },
	m_Velocity{ Vector2f{0.f,0.f} },
	m_Acceleration{ Vector2f{0.f,-981.f} },
	m_ActionState{ ActionState::moving },
	m_AccuTransformSec{ 0.f },
	m_MaxTransformSec{ 1.f },
	m_Power{ 0 }
{
}
Avatar::~Avatar()
{
	
}

void Avatar::Update(float elapsedSec, const Level& level)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	switch (m_ActionState)
	{
	case Avatar::ActionState::waiting:
		if (level.IsOnGround(m_Shape))
		{
			if (pStates[SDL_SCANCODE_LEFT] || 
				pStates[SDL_SCANCODE_RIGHT] || 
				pStates[SDL_SCANCODE_UP]) 
			{
				m_ActionState = ActionState::moving;
			}
		}
		break;

	case Avatar::ActionState::moving:

		if (level.IsOnGround(m_Shape))
		{
			m_Velocity.x = 0.f; //Reset every time when on ground to make sure no velocity remains saved (bhopping)

			if (!pStates[SDL_SCANCODE_LEFT] &&
				!pStates[SDL_SCANCODE_RIGHT] &&
				!pStates[SDL_SCANCODE_UP])
			{
				m_ActionState = ActionState::waiting;
				break;
			}
			else
			{
				HandleKeyboardInput(pStates);
			}
		}

		HandleMovement(level, elapsedSec);
		break;

	case Avatar::ActionState::transforming:
		m_AccuTransformSec += elapsedSec;

		if (m_AccuTransformSec>=m_MaxTransformSec)
		{
			m_AccuTransformSec = 0.f;
			m_ActionState = ActionState::moving;
		}
		break;

	default:
		break;
	}
}

void Avatar::Draw() const
{
	DrawAvatar();
	DrawPowerSquares();
}

void Avatar::PowerUpHit()
{
	m_Power++;

	//When removing this jump continues after picking powerup up
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
	switch (m_ActionState)
	{
	case Avatar::ActionState::waiting:
		utils::SetColor(Color4f{ 1.f,1.f,0.f,1.f });
		break;
	case Avatar::ActionState::moving:
		utils::SetColor(Color4f{ 1.f,0.f,0.f,1.f });
		break;
	case Avatar::ActionState::transforming:
		utils::SetColor(Color4f{ 0.f,1.f,1.f,1.f });
		break;
	default:
		break;
	}
	utils::FillRect(m_Shape);

}

void Avatar::DrawPowerSquares() const
{
	utils::SetColor(Color4f{ 0.f,0.f,0.f,1.f });
	float width{ 10.f }, height{ 10.f };

	for (int index = 0; index < m_Power; ++index)
	{
		utils::FillRect(m_Shape.left + 5.f, index*2.f + (m_Shape.bottom + 5.f) + index * height, width, height);
	}
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

void Avatar::HandleMovement(const Level& level, float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;

	m_Velocity.y += m_Acceleration.y * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	level.HandleCollision(m_Shape, m_Velocity);
}


