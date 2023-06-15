#include "pch.h"
#include "Rope.h"

#include "Player.h"

Rope::Rope(const Rectf& shape, TextureManager* pTextureManager):
	LevelObject{ shape, pTextureManager },
	m_IsOverLapping{ false }
{

}

void Rope::Update(float elapsedSec, Player* pPlayer, const Uint8* pInput)
{
	const bool wasOverlapping{ m_IsOverLapping };

	m_IsOverLapping = utils::IsOverlapping(m_Shape, pPlayer->GetShape());

	const bool isClimbInput{ pInput[SDL_SCANCODE_UP] ||
							 pInput[SDL_SCANCODE_DOWN] };	

	if (m_IsOverLapping && isClimbInput)
	{
		Interact(pPlayer);
	}

	else if (wasOverlapping && isClimbInput) //Player was overlapping the previous frame but is no longer on this frame
	{
		pPlayer->StopClimbing();
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
