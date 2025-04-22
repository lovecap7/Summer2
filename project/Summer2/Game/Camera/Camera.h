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
	//カメラの座標
	Position3 GetPos() { return m_pos; }
	//カメラの向き
	Vector3 GetDir();
private:
	//自分の座標
	Position3 m_pos;
	//カメラの向き
	Position3 m_target;
};

