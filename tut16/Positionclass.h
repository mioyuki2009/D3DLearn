#pragma once
#include <cmath>


////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////
class PositionClass
{
public:
	PositionClass();
	~PositionClass();

	void SetFrameTime(__int64);
	void GetRotation(float&);

	void TurnLeft(bool);
	void TurnRight(bool);

private:
	__int64 m_frameTime;
	float m_rotationY;
	float m_leftTurnSpeed, m_rightTurnSpeed;
};