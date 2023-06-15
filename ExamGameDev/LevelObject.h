#pragma once
class Texture;
class Player;
class TextureManager;

class LevelObject
{
	public:
		explicit LevelObject(const Rectf& shape, TextureManager* pTextureManager);
		virtual ~LevelObject();

		LevelObject(const LevelObject&) = delete;
		LevelObject& operator=(const LevelObject&) = delete;
		LevelObject(LevelObject&&) = delete;
		LevelObject& operator=(LevelObject&&) = delete;

		virtual void Update(float elapsedSec, Player* pPlayer, const Uint8* pInput) = 0;
		virtual void Draw() const = 0;

		const Rectf& GetShape() const;

	protected:
		Rectf m_Shape;
		const Texture* m_pTexture;

	private:
		TextureManager* m_pTextureManager;
};

