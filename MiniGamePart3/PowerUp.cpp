#include "pch.h"
#include "PowerUp.h"
#include "Texture.h"
#include "utils.h"
#include <iostream>

PowerUp::PowerUp(const Point2f& center, PowerUp::Type type) :
	m_Type{ type },
	m_Angle{},
	m_pTexture{ new Texture{ "Images/PowerUp.png" } },
	m_RotSpeed{360}
{
	m_Shape = Circlef{ center, m_pTexture->GetWidth() / 2 };
	m_TextClip = Rectf{ 0.f, m_pTexture->GetHeight() / 2 * int(type) ,m_pTexture->GetWidth(),m_pTexture->GetHeight() / 2};
}

PowerUp::~PowerUp()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void PowerUp::Update(float elapsedSec)
{
	m_Angle += m_RotSpeed * elapsedSec;
}

void PowerUp::Draw() const
{
	glPushMatrix();
		glTranslatef(m_Shape.center.x, m_Shape.center.y, 0);
		glRotatef(m_Angle, 0, 0, 1);
		glTranslatef(-m_Shape.radius, -m_Shape.radius, 0);

		m_pTexture->Draw(Rectf{0.f ,0.f , m_pTexture->GetWidth(), m_pTexture->GetHeight() / 2 }, m_TextClip);	
	glPopMatrix();
}

bool PowerUp::IsOverlapping(const Rectf& rect) const
{

	return utils::IsOverlapping(rect, m_Shape);
}

