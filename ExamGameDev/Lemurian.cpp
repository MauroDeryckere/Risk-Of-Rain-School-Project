#include "pch.h"
#include "Lemurian.h"
#include "Level.h"
#include "Player.h"

Lemurian::Lemurian(const Point2f& position, float movementSpeed, float jumpSpeed, unsigned int health, unsigned int attackDamage, 
	TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager):
	BaseEnemy{ 
		Rectf{position.x, position.y, 48.f , 67.f} , movementSpeed, jumpSpeed, health , attackDamage, 
		pTimeObjectManager, pSoundManager, pTextureManager
	}
{

}

Lemurian::~Lemurian()
{
}

void Lemurian::Update(float elapsedSec, Level* pLevel, Player* pPlayer)
{

}

void Lemurian::Draw() const
{

}

void Lemurian::TakeDamage(unsigned int attackDamage)
{
	BaseEnemy::TakeDamage(attackDamage);
	//m_pHitSoundEffect->Play(0);
}
