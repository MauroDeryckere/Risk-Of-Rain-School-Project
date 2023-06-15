#pragma once
#include "BaseEnemy.h"
class Level;
class Player;

class TimeObjectManager;
class SoundManager;
class TextureManager;

class Lemurian :
    public BaseEnemy
{
    public:
        Lemurian(const Point2f& position, float movementSpeed, float jumpSpeed, unsigned int health, unsigned int attackDamage,
				TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager);
        ~Lemurian();

		Lemurian(const Lemurian&) = delete;
		Lemurian& operator=(const Lemurian&) = delete;
		Lemurian(Lemurian&&) = delete;
		Lemurian& operator=(Lemurian&&) = delete;

		virtual void Update(float elapsedSec, Level* pLevel, Player* pPlayer) override;
		virtual void Draw() const override;
		virtual void TakeDamage(unsigned int attackDamage) override;

    private:


};

