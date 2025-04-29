#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class Actor;
class Rigidbody;
#if _DEBUG
class Input;
#endif
class Camera
{
public:
	Camera(Position3 firstPos, std::shared_ptr<Actor> player);
	~Camera();
	void Update();
	//�J�����̍��W
	Position3 GetPos() { return m_pos; }
	//�J�����̌���
	Vector3 GetDir();

#if _DEBUG
	//�J������]
	void RotaCamera(const Input& input);
#endif
private:
	//�����̍��W
	Position3 m_pos;
	//�J�����̌���
	Position3 m_target;
	//�v���C���[
	std::shared_ptr<Actor> m_player;
};

