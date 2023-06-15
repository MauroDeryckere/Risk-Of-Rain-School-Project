#pragma once
class Texture;

class PowerUp final
{
public:
	enum class Type 
	{  
		green = 1, 
		brown = 2	
	};

	explicit PowerUp(const Point2f& center, PowerUp::Type type );
	~PowerUp();
	void Update( float elapsedSec );
	void Draw( ) const;
	bool IsOverlapping(const Rectf& rect ) const;

private:
	const Type m_Type;
	Circlef m_Shape;
	const Texture* m_pTexture;
	Rectf m_TextClip;
	float m_Angle;
	const float m_RotSpeed;

	float GetDistance(float xPointA, float yPointA, float xPointB, float yPointB) const;
	float GetDistance(const Point2f& pointA, const Point2f& pointB) const;
	bool IsPointInCircle(float xPoint, float yPoint, float xCenterCircle, float yCenterCircle, float radiusCircle) const;
	bool IsPointInCircle(const Point2f& point, const Circlef& circle) const;

	bool IsRectInCircle(const Rectf& rect, const Circlef& circle) const;


};


