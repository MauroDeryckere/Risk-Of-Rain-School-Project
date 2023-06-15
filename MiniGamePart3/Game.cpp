#include "pch.h"
#include <iostream>
#include "Game.h"
#include "utils.h"

Game::Game(const Window& window)
	:BaseGame{ window },

	m_EndReached{ false }

{	
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	ShowTestMessage( );
	AddPowerUps( );
	m_pCamera = new Camera{GetViewPort().width ,GetViewPort().height };

	m_pCamera->SetLevelBoundaries(m_Level.GetBoundaries());

	m_pHud = new Hud{ Point2f{ 0.f,GetViewPort().height }, m_PowerUpManager.Size() };
}

void Game::Cleanup( )
{
	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pHud;
	m_pHud = nullptr;
}

void Game::Update( float elapsedSec )
{
	// Update game objects
	if (!m_EndReached)
	{
		m_PowerUpManager.Update(elapsedSec);
		m_Avatar.Update(elapsedSec, m_Level);
	}

	m_EndReached = m_Level.HasReachedEnd(m_Avatar.GetShape());

	// Do collision
	DoCollisionTests( );
}

void Game::Draw( ) const
{
	ClearBackground( );

	//Push current modelview
	//Camera transform model view
	//Draw game objects
	//Pop model view matrix

	glPushMatrix();
		m_pCamera->Transform(m_Avatar.GetShape());
			m_Level.DrawBackground();
			m_PowerUpManager.Draw();
			m_Avatar.Draw();
			m_Level.DrawForeground();
			if (m_EndReached) //TO DO: FIX BLACK OVERLAY
			{
				utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
				utils::DrawRect(0.f, 0.f, GetViewPort().width, GetViewPort().height);
			}
	glPopMatrix();

	m_pHud->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}


void Game::ShowTestMessage( ) const
{
	std::cout << "--> Avatar test <--\n";
	std::cout << "Verify that the avatar behaves as follows.\n";
	std::cout << "- Moves along the level when the left/right arrow is pressed.\n";
	std::cout << "- Doesn't move when it is on the ground and no key is pressed.\n";
	std::cout << "- Jumps only when it is on the ground and the up arrow key is pressed.\n";
	std::cout << "- Doesn't move during 1 second when hitting a power up.\n";
	std::cout << "- Starts moving again ( e.g. falling ) after this second.\n";
	std::cout << "- The number of small rectangles in the bottom left corner is equal to the number of hit power ups.\n";
	std::cout << "- Has a red color when it is moving.\n";
	std::cout << "- Has a yellow color when it is waiting.\n";
	std::cout << "- Has a blue color when it is transforming.\n";

}

void Game::AddPowerUps( )
{
	m_PowerUpManager.AddItem( Point2f{ 185.0f, 500 - 183.0f }, PowerUp::Type::brown );
	m_PowerUpManager.AddItem( Point2f{ 435.0f, 500 - 133.0f }, PowerUp::Type::green );
	m_PowerUpManager.AddItem( Point2f{ 685.0f, 500 - 183.0f }, PowerUp::Type::brown );
}

void Game::DoCollisionTests( )
{
	if ( m_PowerUpManager.HitItem( m_Avatar.GetShape( ) ) )
	{
		m_Avatar.PowerUpHit( );
		m_pHud->PowerUpHit();
	}
}

