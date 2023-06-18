#pragma once
class SoundManager;
class TextureManager;

class Texture;
class SoundEffect;

#include <vector>

class UI final
{
	public:
		enum class StartScreenButton
		{
			StartSinglePlayer = 0,
			ComingSoon = 1,
			ComingSoon2 = 2,
			ItemLog = 3,
			MonsterLog = 4,
			ScoresAndUnlockAbles = 5,
			Options = 6,
			Quit = 7
		};
		enum class MonsterLogButton
		{
			GoBackToMain = 0
		};
		enum class PausedButton
		{
			Resume = 0,
			Audio = 1,
			ReturnToMainMenu = 2
		};
		enum class AudioButton
		{
			Return = 0,
			MusicVolume = 1,
			GameVolume = 2
		};
		enum class EndScreenButton
		{
			BackToMainMenu = 0,
			Quit = 1
		};

		UI(float windowWidth, float windowHeight, SoundManager* pSoundManager, TextureManager* pTextureManager);
		~UI();

		UI(const UI&) = delete;
		UI& operator=(const UI&) = delete;
		UI(UI&&) = delete;
		UI& operator=(UI&&) = delete;

		void DrawMouseCursor(const Point2f& mousePosition) const;

		void DrawStartScreen() const;
		void DrawMonsterLogScreen() const;
		void DrawPausedScreen() const;
		void DrawAudioScreen() const;
		void DrawEndScreen(bool wonGame) const;

		bool IsButtonPressed(StartScreenButton button, const Point2f& mousePos) const;
		bool IsButtonPressed(MonsterLogButton button, const Point2f& mousePos) const;
		bool IsButtonPressed(PausedButton button, const Point2f& mousePos) const;
		bool IsButtonPressed(AudioButton button, const Point2f& mousePos) const;
		bool IsButtonPressed(EndScreenButton button, const Point2f& mousePos) const;

		void InitializeStartScreenTextures();
		void InitializeMonsterLogScreenTextures();
		void InitializeEndScreenTextures();

		bool FreeStartScreenResources();
		bool FreeMonsterLogResources();
		bool FreeEndScreenResources();

	private:
		//Not owned by UI
		TextureManager* m_pTextureManager;
		SoundManager* m_pSoundManager;
		//--------

		float m_WindowWidth;
		float m_WindowHeight;

		const Texture* m_pCursor;

		const Texture* m_pStartScreen;
		std::vector<std::pair<const Texture*, Rectf>> m_pStartScreenTxtTextures;

		const Texture* m_pMonsterLogScreen;
		std::vector<std::pair<const Texture*, Rectf>> m_pMonsterLogTxtTextures;

		std::vector<std::pair<const Texture*, Rectf>> m_pPausedTxtTextures;

		std::vector<std::pair<const Texture*, Rectf>> m_pAudioTxtTextures;

		std::vector<std::pair<const Texture*, Rectf>> m_pEndScreenTxtTextures;
		const Texture* m_pVictoryTxt;
		const Texture* m_pGameOverTxt;

		SoundEffect* m_pButtonHitSound;

		void InitializeTextures();
		const Rectf InitializeDestRect(const Texture* pTexture, float bottomPos);

		void InitializePausedTextures();
		void InitialializeAudioTextures();

		void DrawScreenButton(std::pair<const Texture*, Rectf>& buttonPair, const Color4f& bgColor = Color4f{0.117f, 0.109f, 0.160f, 1.f}, const Rectf& boxSize = Rectf{}, float boxPadding = 5.f) const;
};

