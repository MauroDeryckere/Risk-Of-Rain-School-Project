#include "pch.h"
#include "PowerUpManager.h"

PowerUpManager::PowerUpManager() :
	m_pItems{}
{

}

PowerUpManager::~PowerUpManager()
{
	for (size_t index = 0; index < Size(); index++)
	{
		delete m_pItems[index];
		m_pItems[index] = nullptr;
	}
}

PowerUp* PowerUpManager::AddItem(const Point2f& center, PowerUp::Type type)
{
	m_pItems.push_back(new PowerUp{center, type});
	return m_pItems.back();
}

void PowerUpManager::Update(float elapsedSec)
{
	for (size_t index = 0; index < Size(); index++)
	{
		m_pItems[index]->Update(elapsedSec);
	}
}

void PowerUpManager::Draw() const
{
	for (size_t index = 0; index < Size(); index++)
	{
		m_pItems[index]->Draw();
	}
}

size_t PowerUpManager::Size() const
{
	return m_pItems.size();
}

bool PowerUpManager::HitItem(const Rectf& rect)
{
	for (size_t index = 0; index < Size(); index++)
	{
		if (m_pItems[index]->IsOverlapping(rect))
		{
			//copy last item on this items pos
			delete m_pItems[index];
			m_pItems[index] = m_pItems.back();
			m_pItems.pop_back();
		 	return true;
		}
	}
	return false;
}
