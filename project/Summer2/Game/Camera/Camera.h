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
	//�J�����̍��W
	Position3 GetPos() { return m_pos; }
	//�J�����̌���
	Vector3 GetDir();
private:
	//�����̍��W
	Position3 m_pos;
	//�J�����̌���
	Position3 m_target;
};

