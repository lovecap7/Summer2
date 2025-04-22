#include "Camera.h"
#include "../../General/Rigidbody.h"
#include <DxLib.h>

Camera::Camera(Position3 firstPos, Position3 target):
	m_pos(firstPos),
	m_target(target)
{
	//���s50�`3000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(50.0f, 3000.0f);

	//�J�����̍��W�ƒ����_
	SetCameraPositionAndTarget_UpVecY(VGet(m_pos.x, m_pos.y, m_pos.z), VGet(m_target.x, m_target.y, m_target.z));

	//�f�B���N�V���i�����C�g
	ChangeLightTypeDir(VGet(0.0f, 0.0f, -1.0f));
}

Camera::~Camera()
{
}

void Camera::Update(Position3 target)
{
	m_target = target;
	m_pos = m_target;
	m_pos.y += 200.0f;
	m_pos.z += 400.0f;
	//�J�����̍��W�ƒ����_
	SetCameraPositionAndTarget_UpVecY(VGet(m_pos.x, m_pos.y, m_pos.z), VGet(m_target.x, m_target.y, m_target.z));
}

Vector3 Camera::GetDir()
{
	Vector3 dir = m_target - m_pos;
	dir = dir.Normalize();
	return dir;
}
