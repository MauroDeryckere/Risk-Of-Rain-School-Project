#pragma once
#include "utils.h"

class Level;
class Texture;

class Avatar final {
	public: 
		Avatar();
		~Avatar();
		void Update(float elapsedSec, const Level& level);
		void Draw() const;
		void PowerUpHit();
		Rectf GetShape() const;

	private: 
		enum class ActionState
		{
			waiting = 0,
			moving = 1,
			transforming = 2
		};

		//Avatar
		Rectf m_Shape;
		const float m_HorSpeed;
		const float m_JumpSpeed;
		Vector2f m_Velocity;
		const Vector2f m_Acceleration;
		ActionState m_ActionState;
		float m_AccuTransformSec;
		const float m_MaxTransformSec;
		int m_Power;

		//Spritesheet
		const Texture* m_pSpritesTexture;
		const float m_ClipHeight;
		const float m_ClipWidth;
		const int m_NrOfFrames;
		const int m_NrOfFramesPerSec;
		float m_AnimTime;
		int m_AnimFrame;

		void DrawAvatar() const;
		void HandleKeyboardInput(const Uint8* pStates);
		void HandleMovement(float elapsedSec);
		void Transform(float elapsedSec);

		void ChangeAnimationFrame(float elapsedSec);

};