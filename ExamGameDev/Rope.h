#pragma once
#include "LevelObject.h"

class Rope final:
	public LevelObject
{
	public:
		explicit Rope(const Rectf& shape, TextureManager* pTextureManager);
		virtual ~Rope() override = default;

		virtual void Update(float elapsedSec, Player* pPlayer, const Uint8* pInput ) override;
		virtual void Draw() const override;

	private:
		void Interact(Player* pPlayer) const;

		bool m_IsOverLapping;
};

