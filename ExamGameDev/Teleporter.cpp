#include "pch.h"
#include "Teleporter.h"
#include "Texture.h"
#include "Player.h"
#include "LevelManager.h"

#include "utils.h"
#include <iostream>

Teleporter::Teleporter(const Rectf& shape, LevelManager* pLevelManager, TextureManager* pTextureManager):
	LevelObject{shape, pTextureManager},
	m_pLevelManager{pLevelManager}
{
}

void Teleporter::Update(float elapsedSec, Player* pPlayer, const Uint8* pInput)
{
	const bool isInteractInput{ pInput[SDL_SCANCODE_E] && true};
	if (utils::IsOverlapping(m_Shape, pPlayer->GetShape()) && isInteractInput )
	{
		Interact();
	}
}

void Teleporter::Draw() const
{
	const Rectf sourceRect = Rectf{ 4.f,
									243.f ,
									m_Shape.width,
									m_Shape.height };

	m_pTexture->Draw(m_Shape, sourceRect);
}

void Teleporter::Interact() const
{
	m_pLevelManager->StartNextStage();
}
