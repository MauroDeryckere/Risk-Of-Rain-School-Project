#include "pch.h"
#include "Game.h"
#include "utils.h"

#include <iostream>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw( ) const
{
	ClearBackground( );

	// The surface
	Point2f surfacePoint1{ 350,50 },
			surfacePoint2{ 150,250 };
	utils::SetColor(Color4f{ 1.f,.5f,.2f,1.f });
	utils::DrawLine(surfacePoint1, surfacePoint2);

	//The ray (Yellow)
	Point2f origin1{ 500,400 };

	utils::SetColor(Color4f{ 1.f,1.f,0.f,1.f });
	utils::DrawLine(origin1, m_MousePosition);

	//Do the raycast
	utils::HitInfo hitInfo{}; //Raycast needs empty hitinfo to return multiple variables
	std::vector<Point2f> surface{ surfacePoint1 , surfacePoint2 }; 	// All vertexes object = surface

	if (utils::Raycast(surface, origin1, m_MousePosition, hitInfo))
	{
		//Intersection point
		utils::SetColor(Color4f{ 1.f,1.f,0.f,1.f }); 
		utils::DrawEllipse(hitInfo.intersectPoint,5.f,5.f);

		// The normal
		utils::SetColor(Color4f{ 0.f,1.f,0.f,1.f }); //Green
		utils::DrawLine(hitInfo.intersectPoint, hitInfo.intersectPoint + hitInfo.normal*50.f); // multiply normal to make it visible

		//The reflection
		utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });
		Vector2f rayVector{ origin1,m_MousePosition },
				 reflectedRay{ rayVector.Reflect(hitInfo.normal) };

		utils::DrawLine(hitInfo.intersectPoint, hitInfo.intersectPoint + reflectedRay * (1 - hitInfo.lambda)); // lambda = which part of line is intersects at (E.g 70% = 0.7 of normalized line)
		//lambda allows to draw reflection as long as part of line passing through

		std::cout << hitInfo.normal << "\n";
	}	
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
	m_MousePosition.x = float(e.x);
	m_MousePosition.y = float(e.y);
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
