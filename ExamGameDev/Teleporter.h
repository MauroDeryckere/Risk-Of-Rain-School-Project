#pragma once
#include "LevelObject.h"

class LevelManager;

class Teleporter final:
	public LevelObject
{
	public:
		Teleporter(const Rectf& shape, LevelManager* pLevelManager, TextureManager* pTextureManager);
		virtual ~Teleporter() override = default;

		virtual void Update(float elapsedSec, Player* pPlayer, const Uint8* pInput) override;
		virtual void Draw() const override;

	private:
		//Not owned by teleporter
		LevelManager* m_pLevelManager;
		//-----

		void Interact() const;
};