#include "Player.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../Game/Camera/Camera.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	const Vector3 kCapsuleHeight = { 0.0f,20.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 10.0f; //カプセルの半径
}

Player::Player(int modelHandle, Position3 firstPos) :
	Actor(ActorKind::Player),
	m_modelHandle(modelHandle),
	m_stickVec(0.0f,0.0f),
	m_update(&Player::IdleUpdate)
{
	//初期位置
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //カプセルの上端
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));
}

Player::~Player()
{
}

void Player::Update(const Input& input,const std::unique_ptr<Camera>& camera)
{
	//スティックの向きを入れる
	m_stickVec.x = -static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = static_cast<float>(input.GetStickInfo().leftStickY);
	//更新
	(this->*m_update)(input,camera);
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
		false
	);
	printf("Pos = X:%f Z:%f\n", m_collidable->GetRb()->GetPos().x, m_collidable->GetRb()->GetPos().z);
	printf("Vec = X:%f Z:%f\n", m_collidable->GetRb()->GetVec().x, m_collidable->GetRb()->GetVec().z);
#endif
}

void Player::Complete()
{
	m_collidable->GetRb()->SetNextPos();
	std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->SetNextEndPos(m_collidable->GetRb()->GetVec());
	// ３Dモデルのポジション設定
	//DxLib::MV1SetPosition(m_modelHandle, m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void Player::IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//入力がないなら待機状態へ
	if (m_stickVec.Magnitude() != 0)
	{
		//移動
		m_update = &Player::MoveUpdate;
		return;
	}
	Vector3 vec = m_collidable->GetRb()->GetVec();
	vec.x *= 0.8f;
	vec.y *= 0.8f;
	vec.z *= 0.8f;
	m_collidable->GetRb()->SetVec(vec);
}

void Player::MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//入力がないなら待機状態へ
	if (m_stickVec.Magnitude() == 0)
	{
		//待機
		m_update = &Player::IdleUpdate;
		return;
	}
	else
	{
		//移動
		m_collidable->GetRb()->ResetVec();
		m_collidable->GetRb()->SetVec(GetForwardVec(camera) * 2.0f);
	}
}

void Player::JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
}

Vector3 Player::GetForwardVec(const std::unique_ptr<Camera>& camera)
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
	float stickTheata = Theata(z, m_stickVec);
	//プレイヤーを中心に次の座標を回転
	Matrix4x4 rotaMat = RotateYMat4x4(cameraTheata + stickTheata);
	//ベクトルにかける(回転)
	Vector3 moveVec = Vector3{ 0.0f, 0.0f, -1.0f };
	moveVec = rotaMat * moveVec;
	moveVec.y = 0.0f; //Y軸は無視
	return moveVec.Normalize();
}
