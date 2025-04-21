#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class Rigidbody;
class Camera
{
public:
	Camera(Position3 firstPos, Position3 target);
	~Camera();
	void Update(Position3 target);
private:
	Position3 m_pos;
};

