#include "Camera.h"
#include "../../General/Rigidbody.h"
#include <DxLib.h>

Camera::Camera(Position3 firstPos, Position3 target):
	m_pos(firstPos)
{
	//���s50�`3000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(50.0f, 3000.0f);

	//�J�����̍��W�ƒ����_
	SetCameraPositionAndTarget_UpVecY(VGet(m_pos.x, m_pos.y, m_pos.z), VGet(target.x, target.y, target.z));

	// �w���̃}�C�i�X�����̃f�B���N�V���i�����C�g�ɕύX
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
}

Camera::~Camera()
{
}

void Camera::Update(Position3 target)
{
	//�J�����̍��W�ƒ����_
	SetCameraPositionAndTarget_UpVecY(VGet(target.x, m_pos.y, m_pos.z), VGet(target.x, target.y, target.z));
}
