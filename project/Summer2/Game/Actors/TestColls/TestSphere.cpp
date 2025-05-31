#include "TestSphere.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../Game/Camera/Camera.h"
#include <DxLib.h>

TestSphere::TestSphere(Position3 pos, float radius, bool isStatic) :
	Actor(ActorKind::Obstacle),
	m_stickVec{ 0.0f,0.0f}
{
	//�����ʒu
	m_collidable = std::make_shared<Collidable>(std::make_shared<SphereCollider>(radius), std::make_shared<Rigidbody>(pos));
	m_collidable->SetIsStatic(isStatic);
}

TestSphere::~TestSphere()
{
}

void TestSphere::Init()
{
}

void TestSphere::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	m_collidable->GetRb()->ResetVec();
	//�X�e�B�b�N�̌���������
	m_stickVec.x = -static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = static_cast<float>(input.GetStickInfo().leftStickY);
	//�ړ�
	m_collidable->GetRb()->SetMoveVec(GetForwardVec(camera) * InputValueSpeed(input));
}

void TestSphere::Gravity(const Vector3& gravity)
{
	//�d�͂������肷������~�߂����̂ŏ����݂���
	if (m_collidable->GetRb()->GetVec().y >= -10.0f)
	{
		//�d��
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void TestSphere::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	if (other->GetColl()->GetShape() == Shape::Polygon)
	{
		if (std::dynamic_pointer_cast<PolygonCollider>(other->GetColl())->IsFloor())
		{
			m_isGround = true;
		}
	}
}

void TestSphere::Draw() const
{
	DrawSphere3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0x00ff00,
		0x00ff00,
		m_collidable->IsStatic()
	);
}

void TestSphere::Complete()
{
	//�ʒu�X�V
	m_collidable->GetRb()->SetNextPos();
}

void TestSphere::HitReaction()
{
}

Vector3 TestSphere::GetForwardVec(const std::unique_ptr<Camera>& camera)
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
	Matrix4x4 rotaMat = Matrix4x4::RotateYMat4x4(cameraTheata + stickTheata);
	//�x�N�g���ɂ�����(��])
	Vector3 moveVec = Vector3{ 0.0f, 0.0f, -1.0f };
	moveVec = rotaMat * moveVec;
	moveVec.y = 0.0f; //Y���͖���
	return moveVec.Normalize();
}

float TestSphere::InputValueSpeed(const Input& input)
{
	float moveSpeed = 10.0f;
	return moveSpeed;
}
