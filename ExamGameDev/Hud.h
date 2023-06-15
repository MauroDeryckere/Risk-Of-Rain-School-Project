#pragma once

class Texture;
class Player;
class SoundManager;
class TextureManager;
class Timer;
class SoundEffect;

#include <vector>

class Hud final
{
	public:
		Hud(float windowWidth, float windowHeight, const Timer* pGameTimer, SoundManager* pSoundManager, TextureManager* pTextureManager);
		~Hud();

		Hud(const Hud&) = delete;
		Hud& operator=(const Hud&) = delete;
		Hud(Hud&&) = delete;
		Hud& operator=(Hud&&) = delete;

		void Draw(const Player* pPlayer) const;
		
	private:
		//Not owned by HUD
		TextureManager* m_pTextureManager;
		SoundManager* m_pSoundManager;
		const Timer* m_pGameTimer;
		//--------------------------

		const float m_WindowWidth;
		const float m_WindowHeight;

		void InitializeTextures();

		void DrawBalance() const; //TODO
		void DrawTime() const;
		void DrawDifficulty() const; //TODO

		void DrawInventory(const Player* pPlayer) const;
		void DrawAbilities(const Player* pPlayer) const;

		void DrawHealth(const Player* pPlayer) const;
		void DrawLevel() const; //TODO
};