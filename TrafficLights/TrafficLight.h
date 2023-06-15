#pragma once
class TrafficLight
{
	public:
		enum class State
		{
			green,
			orange,
			red,
			off
		};

		explicit TrafficLight(const Point2f& pos);
		~TrafficLight();
		void DoHitTest(const Point2f& point);
		void Update(float elapsedSec);
		void Draw() const;
		float GetWidth() const;
		float GetHeight() const;

	private:
		State m_State;
		Point2f m_Position;
		float m_AccumulatedSec;
};	
