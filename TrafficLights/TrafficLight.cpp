#include "pch.h"
#include "TrafficLight.h"
#include "utils.h"

TrafficLight::TrafficLight(const Point2f& pos):
	m_State{State::off},
	m_Position{pos},
	m_AccumulatedSec{ 0.f }
{

}

TrafficLight::~TrafficLight()
{
}

void TrafficLight::DoHitTest(const Point2f& point)
{
	if (true && m_State == State::off)
	{

	}
	else if(true && m_State != State::off)
	{
		m_State = State::off;

	}
}

void TrafficLight::Update(float elapsedSec)
{
	m_AccumulatedSec += elapsedSec;

	switch (m_State)
	{
	case TrafficLight::State::green:
		if (m_AccumulatedSec >= 4)
		{
			m_AccumulatedSec = 0.f;
			m_State = State::orange;
			return;
		}
		break;

	case TrafficLight::State::orange:
		if (m_AccumulatedSec >= 1)
		{
			m_AccumulatedSec = 0.f;
			m_State = State::red;
			return;
		}
		break;

	case TrafficLight::State::red:
		if (m_AccumulatedSec >= 4)
		{
			m_AccumulatedSec = 0.f;
			m_State = State::green;
			return;
		}
		break;

	case TrafficLight::State::off:
		m_AccumulatedSec = 0.f;
		break;

	default:
		break;
	}

}

void TrafficLight::Draw() const
{
	utils::SetColor(Color4f{1.f,1.f,1.f,1.f});
	utils::FillRect(m_Position,30.f,90.f);
}

float TrafficLight::GetWidth() const
{
	return 0.0f;
}

float TrafficLight::GetHeight() const
{
	return 0.0f;
}
