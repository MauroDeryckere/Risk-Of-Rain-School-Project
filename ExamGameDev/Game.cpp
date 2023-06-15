#include "pch.h"
#include "Game.h"

#include "Level.h"
#include "Player.h"
#include "Camera.h"
#include "Hud.h"
#include "UI.h"

#include "TextureManager.h"
#include "SoundManager.h"
#include "LevelObjectManager.h"
#include "EnemyManager.h"

#include "TimeObjectManager.h"

#include "LevelManager.h"
#include "Timer.h"
#include "SoundStream.h"

#include <iostream>

Game::Game(const Window& window)
	:BaseGame{ window },
	m_GameState{ GameState::startScreen },
	m_PreviousGameState{ m_GameState },

	m_MousePos{ },

	m_pTextureManager{ new TextureManager{ } },
	m_pSoundManager{ new SoundManager{ } },
	m_pTimeObjectManager{ new TimeObjectManager{ } },

	m_pLevelManager{ new LevelManager{ } },

	m_pGameTimer{ m_pTimeObjectManager->CreateTimer() },
	m_pTitleScreenMusic{ m_pSoundManager->CreateSoundStream("titleScreen", "Sounds/SoundStreams/musicTitle.ogg") },

	m_pCurrentLevel{ nullptr },
	m_pPlayer{ nullptr },
	m_pCamera{ new Camera{ GetViewPort().width ,GetViewPort().height} },
	m_pHud{ new Hud{ GetViewPort().width ,GetViewPort().height, m_pGameTimer, m_pSoundManager, m_pTextureManager } },
	m_pUI{ new UI{ GetViewPort().width ,GetViewPort().height, m_pSoundManager, m_pTextureManager } }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	SDL_ShowCursor(false);
	m_pTitleScreenMusic->Play(true);
}

void Game::Cleanup( )
{
	delete m_pPlayer;
	delete m_pCamera;
	delete m_pHud;
	delete m_pUI;

	m_pSoundManager->DeleteSoundStream("titleScreen");
	m_pTimeObjectManager->DeleteTimer(m_pGameTimer);

	delete m_pLevelManager;
	delete m_pTextureManager;
	delete m_pSoundManager;
	delete m_pTimeObjectManager;
}

void Game::Update(float elapsedSec )
{
	const Uint8* pInput = SDL_GetKeyboardState(nullptr);

	if (pInput[SDL_SCANCODE_ESCAPE])
	{
		if (m_GameState == GameState::playing)
		{
			m_PreviousGameState = m_GameState;
			m_GameState = GameState::paused;
			m_pSoundManager->PauseAll();
		}
	}

	switch (m_GameState)
	{
	case Game::GameState::playing:
		m_pTimeObjectManager->Update(elapsedSec);
		m_pLevelManager->Update(elapsedSec, m_pPlayer, pInput);
		m_pPlayer->Update(elapsedSec, m_pCurrentLevel, pInput);
		break;
	}
}

void Game::Draw( ) const
{
	ClearBackground();

	switch (m_GameState)
	{
	case Game::GameState::startScreen:
		m_pUI->DrawStartScreen();
		m_pUI->DrawMouseCursor(m_MousePos);
		break;

	case Game::GameState::monsterLog:
		m_pUI->DrawMonsterLogScreen();
		m_pUI->DrawMouseCursor(m_MousePos);
		break;

	case Game::GameState::playing:
		glPushMatrix();
			m_pCamera->Transform(m_pPlayer->GetShape());
				m_pLevelManager->Draw(m_pCamera->GetPosition());
				m_pPlayer->Draw();
		glPopMatrix();
		m_pHud->Draw(m_pPlayer);
		break;

	case Game::GameState::paused:
		if (m_PreviousGameState != GameState::startScreen)
		{
			glPushMatrix();
				m_pCamera->Transform(m_pPlayer->GetShape());
					m_pLevelManager->Draw(m_pCamera->GetPosition());
					m_pPlayer->Draw();
			glPopMatrix();
			m_pHud->Draw(m_pPlayer);
		}
		m_pUI->DrawPausedScreen();
		m_pUI->DrawMouseCursor(m_MousePos);
		break;

	case Game::GameState::audio:
		if (m_PreviousGameState != GameState::startScreen)
		{
			glPushMatrix();
				m_pCamera->Transform(m_pPlayer->GetShape());
					m_pLevelManager->Draw(m_pCamera->GetPosition());
					m_pPlayer->Draw();
			glPopMatrix();
			m_pHud->Draw(m_pPlayer);
		}
		m_pUI->DrawAudioScreen();
		m_pUI->DrawMouseCursor(m_MousePos);
		break;

	case Game::GameState::endScreen:
		m_pUI->DrawEndScreen();
		m_pUI->DrawMouseCursor(m_MousePos);
		break;
	}

}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_i:
		PrintInputKeys();
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_MousePos.x = float(e.x);
	m_MousePos.y = float(e.y);
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	const Point2f mousePos{ float(e.x), float(e.y) };
	switch (m_GameState)
	{
	case Game::GameState::startScreen:
		HandleStartScreenState(mousePos);
		break;

	case Game::GameState::monsterLog:
		HandleMonsterLogState(mousePos);
		break;

	case Game::GameState::paused:
		HandlePausedState(mousePos);
		break;

	case Game::GameState::audio:
		HandleAudioState(mousePos);
		break;

	case Game::GameState::endScreen:
		HandleEndScreenState(mousePos);
		break;

	default:
		break;
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::PrintInputKeys() const
{
	std::cout << "Input: \n";
	std::cout << "Walk Left: Left Arrow Key \n";
	std::cout << "Walk Right: Right Arrow Key \n";

	std::cout << "Climb Up: Up Arrow Key \n";
	std::cout << "Climb Down: Down Arrow Key \n";

	std::cout << "Jump: Space Key \n";

	std::cout << "Basic Attack (Double Tap): W Key \n";
	std::cout << "Piercing Attack (Full Metal Jacket) X Key \n";
	std::cout << "Roll Ability: C Key \n";

	std::cout << "Open Chest: E Key \n";
	std::cout << "Collect PowerUp: R Key \n";

	std::cout << "Pause Game: Escape \n";

	std::cout << std::endl;
}

void Game::HandleStartScreenState(const Point2f& mousePos)
{
	if (m_pUI->IsButtonPressed(UI::StartScreenButton::StartSinglePlayer, mousePos))
	{
		m_PreviousGameState = m_GameState;
		m_GameState = GameState::playing;

		m_pTitleScreenMusic->Stop();

		m_pLevelManager->InitializeStage1(m_pTextureManager, m_pSoundManager, m_pTimeObjectManager);
		m_pCurrentLevel = m_pLevelManager->GetCurrentLevel();

		m_pPlayer = new Player{ Point2f{500.f,500.f}, m_pTextureManager, m_pSoundManager, m_pTimeObjectManager };

		m_pGameTimer->Start();

		m_pCamera->SetLevelBoundaries(m_pCurrentLevel->GetBoundaries());

		m_pUI->FreeStartScreenResources();
		m_pUI->FreeMonsterLogResources();
	}

	else if (m_pUI->IsButtonPressed(UI::StartScreenButton::ComingSoon, mousePos) ||
			 m_pUI->IsButtonPressed(UI::StartScreenButton::ComingSoon2, mousePos))
	{
		std::cout << "Coming soon! \n";
	}

	else if (m_pUI->IsButtonPressed(UI::StartScreenButton::ItemLog, mousePos))
	{
		std::cout << "no item log yet \n";
	}
	else if (m_pUI->IsButtonPressed(UI::StartScreenButton::MonsterLog, mousePos))
	{
		m_PreviousGameState = m_GameState;
		m_GameState = GameState::monsterLog;
	}
	else if (m_pUI->IsButtonPressed(UI::StartScreenButton::ScoresAndUnlockAbles, mousePos))
	{
		std::cout << "no scores yet \n";
	}
	else if (m_pUI->IsButtonPressed(UI::StartScreenButton::Options, mousePos))
	{
		m_GameState = GameState::paused;
	}
	else if (m_pUI->IsButtonPressed(UI::StartScreenButton::Quit, mousePos))
	{
		SDL_Quit();
	}
}

void Game::HandleMonsterLogState(const Point2f& mousePos)
{
	if (m_pUI->IsButtonPressed(UI::MonsterLogButton::GoBackToMain , mousePos))
	{
		m_PreviousGameState = m_GameState;
		m_GameState = GameState::startScreen;
	}
}

void Game::HandlePausedState(const Point2f& mousePos)
{
	if (m_pUI->IsButtonPressed(UI::PausedButton::Resume, mousePos))
	{
		m_GameState = m_PreviousGameState;
		m_pSoundManager->ResumeAll();
	}
	else if (m_pUI->IsButtonPressed(UI::PausedButton::Audio, mousePos))
	{
		m_GameState = GameState::audio;
	}
	else if (m_pUI->IsButtonPressed(UI::PausedButton::ReturnToMainMenu, mousePos))
	{
		m_pUI->InitializeStartScreenTextures();
		m_pUI->InitializeMonsterLogScreenTextures();

		m_pLevelManager->DeleteCurrentLevel();
		m_pCurrentLevel = nullptr;

		delete m_pPlayer;
		m_pPlayer = nullptr;

		m_pGameTimer->Reset();

		m_PreviousGameState = GameState::startScreen;
		m_GameState = GameState::startScreen;
	}
}

void Game::HandleAudioState(const Point2f& mousePos)
{
	if (m_pUI->IsButtonPressed(UI::AudioButton::Return, mousePos))
	{
		m_GameState = GameState::paused;
	}
	else if (m_pUI->IsButtonPressed(UI::AudioButton::MusicVolume, mousePos))
	{
		m_pSoundManager->AdjustMusicSound();
	}
	else if (m_pUI->IsButtonPressed(UI::AudioButton::GameVolume, mousePos))
	{
		m_pSoundManager->AdjustGameSound();
	}
}

void Game::HandleEndScreenState(const Point2f& mousePos)
{
}