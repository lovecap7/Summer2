#include "Player.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../Game/Camera/Camera.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	const Vector3 kCapsuleHeight = { 0.0f,20.0f,0.0f };//�J�v�Z���̏�[
	constexpr float kCapsuleRadius = 10.0f; //�J�v�Z���̔��a
	constexpr float kMoveSpeed = 4.0f;
	constexpr float kAirMoveSpeed = 1.0f;
	constexpr float kMaxGravity = -10.0f;
	const Vector3 kJumpVec = { 0.0f,10.0f,0.0f };//�W�����v
	constexpr unsigned int kMaxJumpNum = 2;
	constexpr float kChangeFall = -2.0f;

}

Player::Player(int modelHandle, Position3 firstPos) :
	Actor(ActorKind::Player),
	m_modelHandle(modelHandle),
	m_stickVec(0.0f,0.0f),
	m_update(&Player::IdleUpdate),
	m_lastUpdate(&Player::IdleUpdate),
	m_isGround(true),
	m_jumpNum(0)
{
	//�����ʒu
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //�J�v�Z���̏�[
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));
}

Player::~Player()
{
}

void Player::Update(const Input& input,const std::unique_ptr<Camera>& camera)
{
	//�X�e�B�b�N�̌���������
	m_stickVec.x = -static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = static_cast<float>(input.GetStickInfo().leftStickY);
	//��Ԃɍ��킹�ď�����
	StateInit();
	//�X�V
	(this->*m_update)(input,camera);
}

void Player::Gravity(const Vector3& gravity)
{
	//�d�͂������肷������~�߂����̂ŏ����݂���
	if (m_collidable->GetRb()->GetVec().y >= kMaxGravity)
	{
		//�d��
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Player::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	if (other->GetColl()->GetShape() == Shape::Polygon)
	{
		m_isGround = std::dynamic_pointer_cast<PolygonCollider>(other->GetColl())->IsFloor();
	}
}

void Player::Draw() const
{
#if _DEBUG
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		m_isGround
	);
	printf("m_jumpNum = %d\n", m_jumpNum);
#endif
}

void Player::Complete()
{
	m_collidable->GetRb()->SetNextPos();
	std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->SetNextEndPos(m_collidable->GetRb()->GetVec());

	//DxLib::MV1SetPosition(m_modelHandle, m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void Player::IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//�������Ă��邩�`�F�b�N
	if (m_collidable->GetRb()->GetVec().y <= kChangeFall)
	{
		//����
		m_update = &Player::FallUpdate;
		return;
	}
	//�W�����v�{�^���������Ă�Ȃ�W�����v
	if (input.IsTriggered("A") && m_isGround)
	{
		//�W�����v
		m_update = &Player::JumpUpdate;
		return;
	}
	//���͂�����Ȃ�ړ�
	if (m_stickVec.Magnitude() != 0)
	{
		//�ړ�
		m_update = &Player::MoveUpdate;
		return;
	}
	Vector3 vec = m_collidable->GetRb()->GetVec();
	vec.x *= 0.8f;
	vec.z *= 0.8f;
	m_collidable->GetRb()->SetVec(vec);
}

void Player::MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//�������Ă��邩�`�F�b�N
	if (m_collidable->GetRb()->GetVec().y <= kChangeFall)
	{
		//����
		m_update = &Player::FallUpdate;
		return;
	}
	//�W�����v
	if (input.IsTriggered("A") && m_isGround)
	{
		//�W�����v
		m_update = &Player::JumpUpdate;
		return;
	}
	//���͂��Ȃ��Ȃ�ҋ@��Ԃ�
	if (m_stickVec.Magnitude() == 0)
	{
		//�ҋ@
		m_update = &Player::IdleUpdate;
		return;
	}
	else
	{
		//�ړ�
		m_collidable->GetRb()->SetMoveVec(GetForwardVec(camera) * kMoveSpeed);
	}
}

void Player::JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//�������Ă���Ȃ�
	if (m_collidable->GetRb()->GetVec().y < 0.0f)
	{
		//����
		m_update = &Player::FallUpdate;
		return;
	}
	//�󒆈ړ�
	m_collidable->GetRb()->AddVec(GetForwardVec(camera) * kAirMoveSpeed);
	Vector3 vec = m_collidable->GetRb()->GetVec();
	vec.x *= 0.8f;
	vec.z *= 0.8f;
	m_collidable->GetRb()->SetVec(vec);
}

void Player::FallUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//�W�����v�ł���Ȃ�
	if (input.IsTriggered("A") && (m_jumpNum <= kMaxJumpNum))
	{
		//�W�����v
		m_update = &Player::JumpUpdate;
		return;
	}
	//�n�ʂɕt���Ă���Ȃ�
	if (m_isGround)
	{
		//�ҋ@
		m_update = &Player::IdleUpdate;
		return;
	}
	//�󒆈ړ�
	m_collidable->GetRb()->AddVec(GetForwardVec(camera) * kAirMoveSpeed);
	Vector3 vec = m_collidable->GetRb()->GetVec();
	vec.x *= 0.8f;
	vec.z *= 0.8f;
	m_collidable->GetRb()->SetVec(vec);
}

//��Ԃɍ��킹�ď���������
void Player::StateInit()
{
	//��Ԃ��ς���Ă��Ȃ��Ȃ瑁�����^�[��
	if (m_lastUpdate == m_update)return;
	m_lastUpdate = m_update;
	if (m_update == &Player::IdleUpdate)
	{
		//�W�����v�J�E���g���Z�b�g
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::MoveUpdate)
	{
		//�W�����v�J�E���g���Z�b�g
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::JumpUpdate)
	{
		//���ł�̂�
		m_isGround = false;
		m_collidable->SetState(State::Jump);
		//�͂�^����
		m_collidable->GetRb()->ResetVec();
		m_collidable->GetRb()->SetVec(kJumpVec);
	}
	else if (m_update == &Player::FallUpdate)
	{
		//�������Ă�̂�
		m_isGround = false;
		//�W�����v�J�E���g�͗�����ԂɂȂ��Ă���J�E���g��i�߂�
		++m_jumpNum;
		m_collidable->SetState(State::Fall);
	}
}

Vector3 Player::GetForwardVec(const std::unique_ptr<Camera>& camera)
{
	//���͂��Ȃ��Ȃ�0��Ԃ�
	if (m_stickVec.Magnitude() == 0)
	{
		return Vector3{ 0.0f,0.0f,0.0f };
	}
	//�J�����̌����ɂ��킹��
	//�J�����̌���
	Vector2 cameraDir{ camera->GetDir().x,camera->GetDir().z };
	cameraDir = cameraDir.Normalize();
	//���[���h���W��Z��������ɃJ�������ǂ̂��炢�����Ă���̂����v�Z
	Vector2 z = Vector2{ 0.0f, -1.0f };
	//�J�����̌���(�p�x)
	float cameraTheata = Theata(z, cameraDir);
	//��ɑ΂��ăX�e�B�b�N���ǂ̂��炢�����Ă���̂����v�Z
	float stickTheata = Theata(z, m_stickVec.Normalize());
	//�v���C���[�𒆐S�Ɏ��̍��W����]
	Matrix4x4 rotaMat = RotateYMat4x4(cameraTheata + stickTheata);
	//�x�N�g���ɂ�����(��])
	Vector3 moveVec = Vector3{ 0.0f, 0.0f, -1.0f };
	moveVec = rotaMat * moveVec;
	moveVec.y = 0.0f; //Y���͖���
	return moveVec.Normalize();
}
