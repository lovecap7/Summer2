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
	//初期位置
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
	//スティックの向きを入れる
	m_stickVec.x = -static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = static_cast<float>(input.GetStickInfo().leftStickY);
	//移動
	m_collidable->GetRb()->SetMoveVec(GetForwardVec(camera) * InputValueSpeed(input));
}

void TestSphere::Gravity(const Vector3& gravity)
{
	//重力がかかりすぎたら止めたいので上限を設ける
	if (m_collidable->GetRb()->GetVec().y >= -10.0f)
	{
		//重力
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
	//位置更新
	m_collidable->GetRb()->SetNextPos();
}

void TestSphere::HitReaction()
{
}

Vector3 TestSphere::GetForwardVec(const std::unique_ptr<Camera>& camera)
{
	//入力がないなら0を返す
	if (m_stickVec.Magnitude() == 0)
	{
		return Vector3{ 0.0f,0.0f,0.0f };
	}
	//カメラの向きにあわせる
	//カメラの向き
	Vector2 cameraDir{ camera->GetDir().x,camera->GetDir().z };
	cameraDir = cameraDir.Normalize();
	//ワールド座標のZ方向を基準にカメラがどのくらい向いているのかを計算
	Vector2 z = Vector2{ 0.0f, -1.0f };
	//カメラの向き(角度)
	float cameraTheata = Theata(z, cameraDir);
	//基準に対してスティックがどのくらい向いているのかを計算
	float stickTheata = Theata(z, m_stickVec.Normalize());
	//プレイヤーを中心に次の座標を回転
	Matrix4x4 rotaMat = Matrix4x4::RotateYMat4x4(cameraTheata + stickTheata);
	//ベクトルにかける(回転)
	Vector3 moveVec = Vector3{ 0.0f, 0.0f, -1.0f };
	moveVec = rotaMat * moveVec;
	moveVec.y = 0.0f; //Y軸は無視
	return moveVec.Normalize();
}

float TestSphere::InputValueSpeed(const Input& input)
{
	float moveSpeed = 10.0f;
	return moveSpeed;
}
