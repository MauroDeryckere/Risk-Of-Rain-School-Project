#pragma once
class TextureManager;
class Texture;
class BaseEnemy;
class Level;
class Player;

class ATGMissile final
{
	public:
		ATGMissile(const Point2f& spawnPosition, TextureManager* pTextureManager, BaseEnemy* pTarget, size_t damage);
		~ATGMissile();

		ATGMissile(const ATGMissile&) = delete;
		ATGMissile& operator=(const ATGMissile&) = delete;
		ATGMissile(ATGMissile&&) = delete;
		ATGMissile& operator=(ATGMissile&&) = delete;

		void Update(Player* pPlayer, Level* pLevel, float elapsedSec);
		void Draw() const;

	private:
		TextureManager* m_pTextureManager;
		const Texture* m_pTexture;

		Rectf m_Shape;
		size_t m_Damage;

		BaseEnemy* m_pTarget;

		enum class MissileState {
			upwards,
			downwards,
			despawn
		};

		MissileState m_MissileState;
		
		float m_t;
		float m_RotationAngle;

		CubicBezier m_upBez;
		CubicBezier m_DownBez;

		void InitializeBezierPoints();

		void ChangeMissileState(const Rectf& targetShape);
		void HandleBezierCalculations(const Rectf& targetShape);
};

