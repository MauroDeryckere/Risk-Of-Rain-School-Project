#pragma once
#include "BaseGame.h"

class Timer;
class SoundStream;

//Game classes
class Level;
class Player;
class Camera;
class Hud;
class UI;

//Managers
class TextureManager;
class TimeObjectManager;
class SoundManager;

class PowerUpManager;
class LevelManager;

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game() override;

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	void EndGame(bool wonGame);

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	enum class GameState {
		startScreen,
		monsterLog,
		playing,
		paused,
		audio,
		endScreen
	};

	GameState m_GameState;
	GameState m_PreviousGameState;

	bool m_WonGame;
	
	Point2f m_MousePos;

	//Managers
	TextureManager* m_pTextureManager;
	SoundManager* m_pSoundManager;
	TimeObjectManager* m_pTimeObjectManager;

	PowerUpManager* m_pPowerUpManager;
	LevelManager* m_pLevelManager;

	//Member variables
	Timer* m_pGameTimer;
	SoundStream* m_pTitleScreenMusic;

	Player* m_pPlayer;
	Camera* m_pCamera;
	Hud* m_pHud;
	UI* m_pUI;


	// FUNCTIONS
	void Initialize();
	void Cleanup();
	void ClearBackground() const;

	void PrintInputKeys() const;

	void DrawGameplay() const;

	//For Game State Machine
	void HandleStartScreenState(const Point2f& mousePos);
	void HandleMonsterLogState(const Point2f& mousePos);
	void HandlePausedState(const Point2f& mousePos);
	void HandleAudioState(const Point2f& mousePos);
	void HandleEndScreenState(const Point2f& mousePos);

	void InitializeAtStage1();
	void ResetGameToMainMenu();
};