#pragma once

class Texture;
class Player;
class TextureManager;
class Timer;

class Hud final
{
	public:
		Hud(float windowWidth, float windowHeight, const Timer* pGameTimer, const TextureManager* pTextureManager);
		~Hud() = default;

		Hud(const Hud&) = delete;
		Hud& operator=(const Hud&) = delete;
		Hud(Hud&&) = delete;
		Hud& operator=(Hud&&) = delete;

		void Draw(const Player* pPlayer) const;
		
	private:
		//Not owned by HUD
		const TextureManager* m_pTextureManager;
		const Timer* m_pGameTimer;
		//--------------------------

		const float m_WindowWidth;
		const float m_WindowHeight;

		void DrawBalance(const Player* player) const;
		void DrawTime() const;
		void DrawInventory(const Player* pPlayer) const;
};