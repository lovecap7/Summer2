#include "Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collidable.h"
#include "../../General/game.h"
#include "../Actors/Actor.h"
#include <DxLib.h>
#if _DEBUG
#include "../../General/Input.h"
#endif

namespace
{
	//��ʒ������炠���苗���v���C���[�����ꂽ�ꍇ�Ǐ]����͈�
	constexpr float kViewPlayerLimitRadius = 70.0f;
	//lerp�̊���
	constexpr float kLerpRate = 0.05f;
}


Camera::Camera(Position3 firstPos, std::shared_ptr<Actor> player):
	m_pos(firstPos),
	m_player(player)
{
	//���s50�`3000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(50.0f, 3000.0f);
	//�J�����̈ʒu�Ɗp�x�̐ݒ�
	m_pos.x = m_player->GetCollidable()->GetRb()->GetPos().x;//�v���C���[�Ɖ������ɂ����킹��
	//�J�����̊p�x
	m_dir = Matrix4x4::RotateXMat4x4(-40.0f / 180.0f * DX_PI_F) *
		Matrix4x4::RotateYMat4x4(180.0f / 180.0f * DX_PI_F) *
		Vector3::Forward();
	if (m_dir.Magnitude() > 0.0f)
	{
		m_dir = m_dir.Normalize();
	}
	//���Ă�ʒu
	m_viewPos = m_pos + m_dir;
	//�J�����̍��W�ƒ����_
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
	//����p
	SetupCamera_Perspective(50.0f / 180.0f * DX_PI_F);
	//�f�B���N�V���i�����C�g
	ChangeLightTypeDir(VGet(0.0f, 0.5f, -1.0f));
}

Camera::~Camera()
{
}

void Camera::Update()
{
	//�v���C���[���J�����̓���͈̔͊O�ɏo�悤�Ƃ����ۂɈړ�
	auto playerPos = m_player->GetCollidable()->GetRb()->GetPos();
	//�ʒu�̍X�V
	Vector3 nextPos = m_pos;
	//�͈͊O�Ȃ�
	if (playerPos.x > m_pos.x + kViewPlayerLimitRadius)//�E
	{
		nextPos.x = playerPos.x;
		nextPos.x -= kViewPlayerLimitRadius;
	}
	else if (playerPos.x < m_pos.x - kViewPlayerLimitRadius)//��
	{
		nextPos.x = playerPos.x;
		nextPos.x += kViewPlayerLimitRadius;
	}
	//���̍��W
	m_pos = Vector3::Lerp(m_pos, nextPos, kLerpRate);
	//�����̍X�V
	m_dir = Matrix4x4::RotateXMat4x4(-40.0f / 180.0f * DX_PI_F) *
		Matrix4x4::RotateYMat4x4(180.0f / 180.0f * DX_PI_F) *
		Vector3::Forward();
	if (m_dir.Magnitude() > 0.0f)
	{
		m_dir = m_dir.Normalize();
	}
	//���Ă�ʒu
	m_viewPos = m_pos + m_dir;
	//�ݒ�
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
}

Vector3 Camera::GetDir()
{
	Vector3 dir = m_dir;
	if (dir.Magnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	return dir;
}