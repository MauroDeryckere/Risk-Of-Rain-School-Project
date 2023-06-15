#include "pch.h"
#include "UI.h"

#include "SoundManager.h"
#include "TextureManager.h"

#include "Texture.h"
#include "SoundEffect.h"
#include "utils.h"

UI::UI(float windowWidth, float windowHeight, SoundManager* pSoundManager, TextureManager* pTextureManager):
	m_pTextureManager{ pTextureManager },
	m_pSoundManager{ pSoundManager },

	m_WindowWidth{ windowWidth },
	m_WindowHeight{ windowHeight },

	m_pButtonHitSound{ m_pSoundManager->CreateSoundEffect("useSound", "Sounds/SoundEffects/wPickup.ogg") }
{
	InitializeTextures();
}

UI::~UI()
{
	FreeStartScreenResources();

	m_pTextureManager->DeleteTexture("MouseCursor");
	FreeMonsterLogResources();

	m_pTextureManager->DeleteTexture("returnText");
	m_pTextureManager->DeleteTexture("musicSoundText");
	m_pTextureManager->DeleteTexture("gameVolText");

	m_pTextureManager->DeleteTexture("resumeText");

	m_pSoundManager->DeleteSoundEffect("useSound");
}

void UI::DrawMouseCursor(const Point2f& mousePosition) const
{
	const Rectf dstRect{ mousePosition.x,
						 mousePosition.y - m_pCursor->GetHeight() * 2,
						 m_pCursor->GetWidth() * 2,
						 m_pCursor->GetHeight() * 2 };

	m_pCursor->Draw(dstRect);
}

void UI::DrawStartScreen() const
{
	const Rectf startScreenDst{ 0.f,0.f, m_WindowWidth, m_WindowHeight };
	m_pStartScreen->Draw(startScreenDst);

	for (size_t index{ 0 }; index < m_pStartScreenTxtTextures.size(); ++index)
	{
		DrawStartScreenButton(m_pStartScreenTxtTextures[index]);
	}
}

void UI::DrawMonsterLogScreen() const
{
	const Rectf monsterLogScreenDst{ 0.f,0.f, m_WindowWidth, m_WindowHeight };
	m_pMonsterLogScreen->Draw(monsterLogScreenDst);

	for (size_t index{ 0 }; index < m_pMonsterLogTxtTextures.size(); ++index)
	{
		DrawStartScreenButton(m_pMonsterLogTxtTextures[index]);
	}
}

void UI::DrawPausedScreen() const
{
	for (size_t index{ 0 }; index < m_pPausedTxtTextures.size(); ++index)
	{
		DrawStartScreenButton(m_pPausedTxtTextures[index]);
	}
}

void UI::DrawAudioScreen() const
{
	for (size_t index{ 0 }; index < m_pAudioTxtTextures.size(); ++index)
	{
		DrawStartScreenButton(m_pAudioTxtTextures[index]);
	}
}

void UI::DrawEndScreen() const
{
	utils::SetColor(Color4f{ 0.f,0.f,0.f,.5f });
	utils::FillRect(0.f, 0.f, m_WindowWidth, m_WindowHeight);
}

bool UI::IsButtonPressed(StartScreenButton button, const Point2f& mousePos) const
{
	if (utils::IsPointInRect(mousePos, m_pStartScreenTxtTextures[size_t(button)].second))
	{
		m_pButtonHitSound->Play(0);
		return true;
	}
	return false;
}

bool UI::IsButtonPressed(MonsterLogButton button, const Point2f& mousePos) const
{
	if (utils::IsPointInRect(mousePos, m_pMonsterLogTxtTextures[size_t(button)].second))
	{
		m_pButtonHitSound->Play(0);
		return true;
	}
	return false;
}

bool UI::IsButtonPressed(PausedButton button, const Point2f& mousePos) const
{
	if (utils::IsPointInRect(mousePos, m_pPausedTxtTextures[size_t(button)].second))
	{
		m_pButtonHitSound->Play(0);
		return true;
	}
	return false;
}

bool UI::IsButtonPressed(AudioButton button, const Point2f& mousePos) const
{
	if (utils::IsPointInRect(mousePos, m_pAudioTxtTextures[size_t(button)].second))
	{
		m_pButtonHitSound->Play(0);
		return true;
	}
	return false;
}

void UI::InitializeStartScreenTextures() //TODO: fix bottom pos to not be magic Number
{
	const std::string fontPath{ "Font/8-BIT WONDER.TTF" };
	const Color4f white{ 1.f,1.f,1.f,1.f };
	constexpr int fontSize{ 12 };

	const float scalingfactor{ m_WindowHeight / 1440.f };

	m_pStartScreen = m_pTextureManager->CreateTexture("Hud/StartScreen.png", "StartScreen");

	const Texture* pStartSinglePlayerText = m_pTextureManager->CreateTexture("Start Single Player", fontPath, fontSize, white, "startSinglePlayerTxt");
	m_pStartScreenTxtTextures.emplace_back(std::make_pair(pStartSinglePlayerText,
		InitializeDestRect(pStartSinglePlayerText, scalingfactor * (1440.f - 308.f - 32.f))));


	const Texture* pComingSoonText = m_pTextureManager->CreateTexture("Coming Soon", fontPath, fontSize, white, "comingSoonTxt");
	m_pStartScreenTxtTextures.emplace_back(std::make_pair(pComingSoonText,
		InitializeDestRect(pComingSoonText, scalingfactor * (1440.f - 356.f - 32.f))));


	const Texture* pComingSoonText2 = m_pTextureManager->CreateTexture("Coming Soon", fontPath, fontSize, white, "comingSoonTxt2");
	m_pStartScreenTxtTextures.emplace_back(std::make_pair(pComingSoonText2,
		InitializeDestRect(pComingSoonText2, scalingfactor * (1440.f - 404.f - 32.f))));

	const Texture* pItemLogText = m_pTextureManager->CreateTexture("Item Log", fontPath, fontSize, white, "itemLogTxt");
	m_pStartScreenTxtTextures.emplace_back(std::make_pair(pItemLogText,
		InitializeDestRect(pItemLogText, scalingfactor * (1440.f - 452.f - 32.f))));

	const Texture* pMonsterLogText = m_pTextureManager->CreateTexture("Monster Log", fontPath, fontSize, white, "monsterLogTxt");
	m_pStartScreenTxtTextures.emplace_back(std::make_pair(pMonsterLogText,
		InitializeDestRect(pMonsterLogText, scalingfactor * (1440.f - 500.f - 32.f))));

	const Texture* pScoresAndUnlockablesText = m_pTextureManager->CreateTexture("Scores and Unlockables", fontPath, fontSize, white, "scoresTxt");
	m_pStartScreenTxtTextures.emplace_back(std::make_pair(pScoresAndUnlockablesText,
		InitializeDestRect(pScoresAndUnlockablesText, scalingfactor * (1440.f - 548.f - 32.f))));

	const Texture* pOptionsText = m_pTextureManager->CreateTexture("Options", fontPath, fontSize, white, "opionsTxt");
	m_pStartScreenTxtTextures.emplace_back(std::make_pair(pOptionsText,
		InitializeDestRect(pOptionsText, scalingfactor * (1440.f - 596.f - 32.f))));

	const Texture* pQuitText = m_pTextureManager->CreateTexture("Quit", fontPath, fontSize, white, "quitTxt");
	m_pStartScreenTxtTextures.emplace_back(std::make_pair(pQuitText,
		InitializeDestRect(pQuitText, scalingfactor * (1440.f - 644.f - 32.f))));
}

void UI::InitializeMonsterLogScreenTextures()
{
	m_pMonsterLogScreen = m_pTextureManager->CreateTexture("Hud/MonsterLogScreen.png", "monsterLogScreen");

	const std::string fontPath{ "Font/8-BIT WONDER.TTF" };
	const Color4f white{ 1.f,1.f,1.f,1.f };
	constexpr int fontSize{ 12 };

	const Texture* pBackTxt = m_pTextureManager->CreateTexture("Back to main menu", fontPath, fontSize, white, "backToMainMenuText");
	m_pMonsterLogTxtTextures.emplace_back(std::make_pair(pBackTxt,
		InitializeDestRect(pBackTxt, m_WindowHeight / 2)));
}

bool UI::FreeStartScreenResources()
{
	if (m_pStartScreen == nullptr)
	{
		return false;
	}

	m_pTextureManager->DeleteTexture("StartScreen");
	m_pStartScreen = nullptr;

	m_pTextureManager->DeleteTexture("startSinglePlayerTxt");
	m_pTextureManager->DeleteTexture("comingSoonTxt");
	m_pTextureManager->DeleteTexture("comingSoonTxt2");
	m_pTextureManager->DeleteTexture("itemLogTxt");
	m_pTextureManager->DeleteTexture("monsterLogTxt");
	m_pTextureManager->DeleteTexture("scoresTxt");
	m_pTextureManager->DeleteTexture("opionsTxt");
	m_pTextureManager->DeleteTexture("quitTxt");

	m_pStartScreenTxtTextures.clear();

	return true;
}

bool UI::FreeMonsterLogResources()
{
	if (m_pMonsterLogScreen == nullptr)
	{
		return false;
	}

	m_pTextureManager->DeleteTexture("monsterLogScreen");
	m_pMonsterLogScreen = nullptr;

	return true;
}

void UI::InitializeTextures()
{
	m_pCursor = m_pTextureManager->CreateTexture("Hud/Cursor.png", "MouseCursor");

	InitializeStartScreenTextures();
	InitializeMonsterLogScreenTextures();
	InitializePausedTextures();
	InitialializeAudioTextures();
}

const Rectf UI::InitializeDestRect(const Texture* pTexture, float bottomPos)
{
	return Rectf{ m_WindowWidth / 2 - pTexture->GetWidth() / 2,
			bottomPos,
			pTexture->GetWidth(),
			pTexture->GetHeight() };
}

void UI::InitializePausedTextures()
{
	const std::string fontPath{ "Font/8-BIT WONDER.TTF" };
	const Color4f white{ 1.f,1.f,1.f,1.f };
	constexpr int fontSize{ 12 };

	const Texture* pResumeText = m_pTextureManager->CreateTexture("Resume", fontPath, fontSize, white, "resumeText");
	m_pPausedTxtTextures.emplace_back(std::make_pair(pResumeText,
		InitializeDestRect(pResumeText, m_WindowHeight / 2)));

	const Texture* pAudioText = m_pTextureManager->CreateTexture("Audio", fontPath, fontSize, white, "audioText");
	m_pPausedTxtTextures.emplace_back(std::make_pair(pAudioText,
		InitializeDestRect(pAudioText, m_WindowHeight / 2 - 40.f)));

	const Texture* pToMainText = m_pTextureManager->CreateTexture("Go to main menu", fontPath, fontSize, white, "mainMenuTxt");
	m_pPausedTxtTextures.emplace_back(std::make_pair(pToMainText,
		InitializeDestRect(pToMainText, m_WindowHeight / 2 - 80.f)));
}

void UI::InitialializeAudioTextures()
{
	const std::string fontPath{ "Font/8-BIT WONDER.TTF" };
	const Color4f white{ 1.f,1.f,1.f,1.f };
	constexpr int fontSize{ 12 };

	const Texture* pReturnText = m_pTextureManager->CreateTexture("Return", fontPath, fontSize, white, "returnText");
	m_pAudioTxtTextures.emplace_back(std::make_pair(pReturnText,
		InitializeDestRect(pReturnText, m_WindowHeight / 2)));

	const Texture* pMusicVolText = m_pTextureManager->CreateTexture("Music Volume", fontPath, fontSize, white, "musicSoundText");
	m_pAudioTxtTextures.emplace_back(std::make_pair(pMusicVolText,
		InitializeDestRect(pMusicVolText, m_WindowHeight / 2 - 40.f)));

	const Texture* pGameVolText = m_pTextureManager->CreateTexture("Game Volume", fontPath, fontSize, white, "gameVolText");
	m_pAudioTxtTextures.emplace_back(std::make_pair(pGameVolText,
		InitializeDestRect(pGameVolText, m_WindowHeight / 2 - 80.f)));
}

void UI::DrawStartScreenButton(const Texture* pTexture, const Rectf& dstRect) const
{
	constexpr float boxPadding{ 5.f };

	const Color4f boxColor{ 30.f / 255, 28.f / 255, 41.f / 255, 1.f };


	const Rectf buttonBox{ dstRect.left - boxPadding,
							dstRect.bottom - boxPadding,
							dstRect.width + 2 * boxPadding,
							dstRect.height + 2 * boxPadding };

	utils::SetColor(boxColor);
	utils::FillRect(buttonBox);
	pTexture->Draw(dstRect);
}

void UI::DrawStartScreenButton(const std::pair<const Texture*, Rectf>& buttonPair) const
{
	DrawStartScreenButton(buttonPair.first, buttonPair.second);
}
