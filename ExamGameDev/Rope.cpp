#include "pch.h"
#include "Rope.h"

#include "Player.h"
#include <iostream>

Rope::Rope(const Rectf& shape, TextureManager* pTextureManager):
	LevelObject{ shape, pTextureManager },
	m_IsOverLapping{ false }
{

}

void Rope::Update(float elapsedSec, Player* pPlayer, const Uint8* pInput)
{
	const bool wasOverlapping{ m_IsOverLapping };

	const Rectf& playerShape{ pPlayer->GetShape() };

	m_IsOverLapping = utils::IsOverlapping(m_Shape, playerShape);

	const bool isClimbInput{ pInput[SDL_SCANCODE_UP] ||
							 pInput[SDL_SCANCODE_DOWN] };	

	if (!pPlayer->IsClimbing())
	{
		if (m_IsOverLapping && isClimbInput)
		{
			const Rectf bottomHalfPlayerShape{ playerShape.left, playerShape.bottom, playerShape.width, playerShape.height / 2 };
			const Rectf topHalfRopeShape{ m_Shape.left, m_Shape.bottom + m_Shape.height / 2, m_Shape.width, m_Shape.height / 2 };

			const Rectf topHalfPlayerShape{ playerShape.left, playerShape.bottom + playerShape.height / 2, playerShape.width, playerShape.height / 2 };
			const Rectf bottomHalfRopeShape{ m_Shape.left, m_Shape.bottom, m_Shape.width, m_Shape.height / 2 };

			if (pPlayer->IsOnGround())
			{
				if (utils::IsOverlapping(bottomHalfPlayerShape, topHalfRopeShape) && pInput[SDL_SCANCODE_DOWN])
				{
					Interact(pPlayer);
				}
				else if (utils::IsOverlapping(topHalfPlayerShape, bottomHalfRopeShape) && pInput[SDL_SCANCODE_UP])
				{
					Interact(pPlayer);
				}
			}
			else
			{
				Interact(pPlayer);
			}
		}
	}
	else
	{
		if (m_IsOverLapping)
		{
			//Stop climbing once player is on the ground
			if (utils::IsOverlapping(playerShape, Rectf{m_Shape.left, m_Shape.bottom, m_Shape.width, playerShape.height}) && pInput[SDL_SCANCODE_DOWN])
			{
				if (pPlayer->IsOnGround())
				{
					pPlayer->StopClimbing();
					return;
				}
			}
			if (isClimbInput)
			{
				Interact(pPlayer);
			}
		}
		else if (wasOverlapping && isClimbInput) //Player was overlapping the previous frame but is no longer on this frame
		{
			pPlayer->StopClimbing();
		}
	}
}

void Rope::Draw() const
{
	//utils::SetColor(Color4f{ 1.f,0.f,0.f,1.f });
	//utils::FillRect(m_Shape);
}

void Rope::Interact(Player* pPlayer) const
{
	pPlayer->ClimbRope(this);
}
