#include "pch.h"
#include "UI.h"

#include "SoundManager.h"
#include "TextureManager.h"

#include "Texture.h"
#include "SoundEffect.h"
#include "utils.h"

UI::UI(float windowWidth, float windowHeight, SoundManager* pSoundManager, TextureManager* pTextureManager) :
	m_pTextureManager{ pTextureManager },
	m_pSoundManager{ pSoundManager },

	m_WindowWidth{ windowWidth },
	m_WindowHeight{ windowHeight },

	m_pVictoryTxt{ nullptr },
	m_pGameOverTxt{ nullptr },

	m_pButtonHitSound{ m_pSoundManager->CreateSoundEffect("useSound", "Sounds/SoundEffects/wPickup.ogg") }
{
	InitializeTextures();
}

UI::~UI()
{
	FreeStartScreenResources();
	FreeMonsterLogResources();
	FreeEndScreenResources();

	m_pTextureManager->DeleteTexture("MouseCursor");

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
		std::pair<const Texture*, Rectf> button { m_pStartScreenTxtTextures[index] };
		DrawScreenButton(button);
	}
}

void UI::DrawMonsterLogScreen() const
{
	const Rectf monsterLogScreenDst{ 0.f,0.f, m_WindowWidth, m_WindowHeight };
	m_pMonsterLogScreen->Draw(monsterLogScreenDst);

	for (size_t index{ 0 }; index < m_pMonsterLogTxtTextures.size(); ++index)
	{
		std::pair<const Texture*, Rectf> button{ m_pMonsterLogTxtTextures[index] };
		DrawScreenButton(button);
	}
}

void UI::DrawPausedScreen() const
{
	for (size_t index{ 0 }; index < m_pPausedTxtTextures.size(); ++index)
	{
		std::pair<const Texture*, Rectf> button{ m_pPausedTxtTextures[index] };
		DrawScreenButton(button);
	}
}

void UI::DrawAudioScreen() const
{
	const float offsetNum{ m_WindowWidth / 256.f };

	std::pair<const Texture*, Rectf> button{ m_pAudioTxtTextures[0] };
	DrawScreenButton(button);

	button = m_pAudioTxtTextures[1];
	DrawScreenButton(button);	

	float musicVolWidth{};
	const size_t musicVol{ m_pSoundManager->GetMusicVolumePercent() };
	Point2f bottomLeft{ button.second.left + button.second.width + offsetNum , button.second.bottom - offsetNum };

	m_pTextureManager->DrawNumber(TextureManager::NumberTextures::_20PxWhiteDigits, bottomLeft, musicVol, 0, musicVolWidth);
	m_pTextureManager->DrawSymbol(TextureManager::NumberTextures::_20PxWhiteDigits, Point2f{ bottomLeft.x + musicVolWidth, bottomLeft.y }, TextureManager::Symbols::Percent, 0);

	button = m_pAudioTxtTextures[2];
	DrawScreenButton(button);

	float gameVolWidth{};
	const size_t gameVol{ m_pSoundManager->GetGameVolumePercent() };
	bottomLeft = Point2f{ button.second.left + button.second.width + offsetNum , button.second.bottom - offsetNum };

	m_pTextureManager->DrawNumber(TextureManager::NumberTextures::_20PxWhiteDigits, bottomLeft, gameVol, 0, gameVolWidth);
	m_pTextureManager->DrawSymbol(TextureManager::NumberTextures::_20PxWhiteDigits, Point2f{ bottomLeft.x + gameVolWidth, bottomLeft.y }, TextureManager::Symbols::Percent, 0);

}

void UI::DrawEndScreen(bool wonGame) const
{
	utils::SetColor(Color4f{ 0.f,0.f,0.f,.5f });
	utils::FillRect(0.f, 0.f, m_WindowWidth, m_WindowHeight);

	if (wonGame)
	{
		m_pVictoryTxt->Draw(Point2f{ m_WindowWidth / 2 - m_pVictoryTxt->GetWidth() / 2, m_WindowHeight - m_WindowHeight / 10 - m_pVictoryTxt->GetHeight() });
	}
	else
	{
		m_pGameOverTxt->Draw(Point2f{ m_WindowWidth / 2 - m_pGameOverTxt->GetWidth() / 2, m_WindowHeight - m_WindowHeight / 10 - m_pGameOverTxt->GetHeight() });
	}

	for (size_t index{ 0 }; index < m_pEndScreenTxtTextures.size(); ++index)
	{
		std::pair<const Texture*, Rectf> button{ m_pEndScreenTxtTextures[index] };
		DrawScreenButton(button);
	}
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

bool UI::IsButtonPressed(EndScreenButton button, const Point2f& mousePos) const
{
	if (utils::IsPointInRect(mousePos, m_pEndScreenTxtTextures[size_t(button)].second))
	{
		m_pButtonHitSound->Play(0);
		return true;
	}
	return false;
}

void UI::InitializeStartScreenTextures()
{
	const std::string fontPath{ "Font/8-BIT WONDER.TTF" };
	const Color4f white{ 1.f,1.f,1.f,1.f };
	constexpr int fontSize{ 12 };

	const float scalingfactor{ m_WindowHeight / 1440.f };

	m_pStartScreen = m_pTextureManager->CreateTexture("Hud/StartScreen.png", "StartScreen");

	const Texture* pStartSinglePlayerText = m_pTextureManager->CreateTexture("Start Single Player", fontPath, fontSize, white, "startSinglePlayerTxt");
	m_pStartScreenTxtTextures.emplace_back
	(std::make_pair(pStartSinglePlayerText, InitializeDestRect(pStartSinglePlayerText, scalingfactor * 1100.f)));

	const Texture* pComingSoonText = m_pTextureManager->CreateTexture("Coming Soon", fontPath, fontSize, white, "comingSoonTxt");
	m_pStartScreenTxtTextures.emplace_back
	(std::make_pair(pComingSoonText, InitializeDestRect(pComingSoonText, scalingfactor * 1052.f)));

	const Texture* pComingSoonText2 = m_pTextureManager->CreateTexture("Coming Soon", fontPath, fontSize, white, "comingSoonTxt2");
	m_pStartScreenTxtTextures.emplace_back
	(std::make_pair(pComingSoonText2, InitializeDestRect(pComingSoonText2, scalingfactor * 1004.f)));

	const Texture* pItemLogText = m_pTextureManager->CreateTexture("Item Log", fontPath, fontSize, white, "itemLogTxt");
	m_pStartScreenTxtTextures.emplace_back
	(std::make_pair(pItemLogText, InitializeDestRect(pItemLogText, scalingfactor * 956.f)));

	const Texture* pMonsterLogText = m_pTextureManager->CreateTexture("Monster Log", fontPath, fontSize, white, "monsterLogTxt");
	m_pStartScreenTxtTextures.emplace_back
	(std::make_pair(pMonsterLogText, InitializeDestRect(pMonsterLogText, scalingfactor * 908.f)));

	const Texture* pScoresAndUnlockablesText = m_pTextureManager->CreateTexture("Scores and Unlockables", fontPath, fontSize, white, "scoresTxt");
	m_pStartScreenTxtTextures.emplace_back
	(std::make_pair(pScoresAndUnlockablesText, InitializeDestRect(pScoresAndUnlockablesText, scalingfactor * 860.f)));

	const Texture* pOptionsText = m_pTextureManager->CreateTexture("Options", fontPath, fontSize, white, "opionsTxt");
	m_pStartScreenTxtTextures.emplace_back
	(std::make_pair(pOptionsText, InitializeDestRect(pOptionsText, scalingfactor * 812.f)));

	const Texture* pQuitText = m_pTextureManager->CreateTexture("Quit", fontPath, fontSize, white, "quitTxt");
	m_pStartScreenTxtTextures.emplace_back
	(std::make_pair(pQuitText, InitializeDestRect(pQuitText, scalingfactor * 764.f)));
}

void UI::InitializeMonsterLogScreenTextures()
{
	m_pMonsterLogScreen = m_pTextureManager->CreateTexture("Hud/MonsterLogScreen.png", "monsterLogScreen");

	const std::string fontPath{ "Font/8-BIT WONDER.TTF" };
	const Color4f white{ 1.f,1.f,1.f,1.f };
	constexpr int fontSize{ 12 };

	const Texture* pBackTxt = m_pTextureManager->CreateTexture("Back to main menu", fontPath, fontSize, white, "backToMainMenuText");
	m_pMonsterLogTxtTextures.emplace_back
	(std::make_pair(pBackTxt, InitializeDestRect(pBackTxt, m_WindowHeight / 2)));
}

void UI::InitializeEndScreenTextures()
{
	const std::string fontPath{ "Font/8-BIT WONDER.TTF" };
	const Color4f white{ 1.f,1.f,1.f,1.f };
	constexpr int fontSize{ 12 };

	m_pGameOverTxt = m_pTextureManager->CreateTexture("Game Over", fontPath, 52, white, "gameOverText");
	m_pVictoryTxt = m_pTextureManager->CreateTexture("Victory", fontPath, 52, white, "victoryText");

	const Texture* pBackTxt = m_pTextureManager->CreateTexture("Back to main menu", fontPath, fontSize, white, "backToMainMenuText");
	m_pEndScreenTxtTextures.emplace_back
	(std::make_pair(pBackTxt, InitializeDestRect(pBackTxt, m_WindowHeight / 2)));	

	const Texture* pQuitText = m_pTextureManager->CreateTexture("Quit", fontPath, fontSize, white, "quitTxt");
	m_pEndScreenTxtTextures.emplace_back
	(std::make_pair(pQuitText, InitializeDestRect(pQuitText, m_WindowHeight / 2 - (m_WindowWidth/ 25.6f))));
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

bool UI::FreeEndScreenResources()
{
	if (!m_pGameOverTxt)
	{
		return false;
	}

	m_pTextureManager->DeleteTexture("gameOverText");
	m_pGameOverTxt = nullptr;
	m_pTextureManager->DeleteTexture("victoryText");
	m_pVictoryTxt = nullptr;

	m_pTextureManager->DeleteTexture("backToMainMenuText");
	m_pTextureManager->DeleteTexture("quitTxt");

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
	m_pPausedTxtTextures.emplace_back
	(std::make_pair(pResumeText, InitializeDestRect(pResumeText, m_WindowHeight / 2)));

	const Texture* pAudioText = m_pTextureManager->CreateTexture("Audio", fontPath, fontSize, white, "audioText");
	m_pPausedTxtTextures.emplace_back
	(std::make_pair(pAudioText, InitializeDestRect(pAudioText, m_WindowHeight / 2 - (m_WindowWidth/ 32.f))));

	const Texture* pToMainText = m_pTextureManager->CreateTexture("Go to main menu", fontPath, fontSize, white, "mainMenuTxt");
	m_pPausedTxtTextures.emplace_back
	(std::make_pair(pToMainText, InitializeDestRect(pToMainText, m_WindowHeight / 2 - (m_WindowWidth/ 16.f))));
}

void UI::InitialializeAudioTextures()
{
	const std::string fontPath{ "Font/8-BIT WONDER.TTF" };
	const Color4f white{ 1.f,1.f,1.f,1.f };
	constexpr int fontSize{ 12 };

	const Texture* pReturnText = m_pTextureManager->CreateTexture("Return", fontPath, fontSize, white, "returnText");
	m_pAudioTxtTextures.emplace_back
	(std::make_pair(pReturnText, InitializeDestRect(pReturnText, m_WindowHeight / 2)));

	const Texture* pMusicVolText = m_pTextureManager->CreateTexture("Music Volume", fontPath, fontSize, white, "musicSoundText");
	m_pAudioTxtTextures.emplace_back
	(std::make_pair(pMusicVolText, InitializeDestRect(pMusicVolText, m_WindowHeight / 2 - (m_WindowWidth / 32.f))));

	const Texture* pGameVolText = m_pTextureManager->CreateTexture("Game Volume", fontPath, fontSize, white, "gameVolText");
	m_pAudioTxtTextures.emplace_back
	(std::make_pair(pGameVolText, InitializeDestRect(pGameVolText, m_WindowHeight / 2 - (m_WindowWidth / 16.f))));
}

void UI::DrawScreenButton(std::pair<const Texture*, Rectf>& buttonPair, const Color4f& bgColor, const Rectf& dstRect, float boxPadding) const
{
	if (dstRect.width != 0.f && dstRect.height != 0.f)
	{
		buttonPair.second = dstRect;
	}

	const Rectf btnBox{ dstRect.left - boxPadding,
						dstRect.bottom - boxPadding,
						dstRect.width + 2 * boxPadding,
						dstRect.height + 2 * boxPadding };


	utils::SetColor(bgColor);
	utils::FillRect(btnBox);

	buttonPair.first->Draw(buttonPair.second);
}
