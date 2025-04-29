#include "Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collidable.h"
#include "../Actors/Actor.h"
#include <DxLib.h>
#if _DEBUG
#include "../../General/Input.h"
#endif


Camera::Camera(Position3 firstPos, std::shared_ptr<Actor> player):
	m_pos(firstPos),
	m_target(player->GetCollidable()->GetRb()->GetPos()),
	m_player(player)
{
	//���s50�`3000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(50.0f, 3000.0f);

	//�J�����̍��W�ƒ����_
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_target.ToDxLibVector());

	//�f�B���N�V���i�����C�g
	ChangeLightTypeDir(VGet(0.0f, 0.0f, -1.0f));

#if _DEBUG
	m_cameraHAngle = 0.0f;
	m_cameraVAngle = -20.0f;
#endif
}

Camera::~Camera()
{
}

void Camera::Update()
{
	m_target = m_player->GetCollidable()->GetRb()->GetPos();
	m_pos += m_player->GetCollidable()->GetRb()->GetVec();
	//�J�����̍��W�ƒ����_
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_target.ToDxLibVector());
}

Vector3 Camera::GetDir()
{
	Vector3 dir = m_target - m_pos;
	dir = dir.Normalize();
	return dir;
}

#if _DEBUG
void Camera::RotaCamera(const Input& input)
{
	//�ω�������c�Ɖ��̃A���O�����E�X�e�b�N�̂���擾
	float hAngle = 1.0f * input.GetStickInfo().rightStickX / 1000.0f;
	float vAngle = -1.0f * input.GetStickInfo().rightStickY / 1000.0f;
	//���f
	m_cameraHAngle += hAngle;
	m_cameraVAngle += vAngle;
	//�A���O����������̊p�x�𒴂�����␳����
	if (m_cameraHAngle >= 180.0f)
	{
		m_cameraHAngle -= 360.0f;
	}
	else if (m_cameraHAngle <= -180.0f)
	{
		m_cameraHAngle += 360.0f;
	}
	if (m_cameraVAngle <= -80.0f)
	{
		m_cameraVAngle = -80.0f;
	}
	else if (m_cameraVAngle >= 0.0f)
	{
		m_cameraVAngle = 0.0f;
	}

	//�v���C���[�𒆐S�ɉ�]
	m_pos = RotateYPositionMatrix4x4(m_target, hAngle / 180.0f * DX_PI_F) * m_pos;

	//�J�����̍��W�ƒ����_
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_target.ToDxLibVector());
}
#endif