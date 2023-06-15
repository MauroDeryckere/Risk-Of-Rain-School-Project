#pragma once

class TextureManager;
class Texture;
class Stopwatch;

class LevelHitBullet
{
	public:
		LevelHitBullet(const Point2f& intersectPoint, bool isFacingLeft, TextureManager* pTextureManager);
		~LevelHitBullet();

		LevelHitBullet(const LevelHitBullet&) = delete;
		LevelHitBullet& operator=(const LevelHitBullet&) = delete;
		LevelHitBullet(LevelHitBullet&&) = delete;
		LevelHitBullet& operator=(LevelHitBullet&&) = delete;

		void Draw() const;

	private:
		const Rectf m_Shape;
		const bool m_IsFacingLeft;
		const Texture* m_pTexture;

		unsigned int m_CurrentFrame;
		unsigned int m_Frames;
		const Stopwatch* m_pStopwatch;
};

