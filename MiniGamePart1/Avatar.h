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
			waiting,
			moving,
			transforming
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

		void DrawAvatar() const;
		void DrawPowerSquares() const;
		void HandleKeyboardInput(const Uint8* pStates);
		void HandleMovement(const Level& level, float elapsedSec);
};