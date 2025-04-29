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
	const Vector3 kCapsuleHeight = { 0.0f,20.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 10.0f; //カプセルの半径
	constexpr float kMoveSpeed = 4.0f;
	constexpr float kAirMoveSpeed = 1.0f;
	constexpr float kMaxGravity = -10.0f;
	const Vector3 kJumpVec = { 0.0f,10.0f,0.0f };//ジャンプ
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
	//状態に合わせて初期化
	StateInit();
	//更新
	(this->*m_update)(input,camera);
}

void Player::Gravity(const Vector3& gravity)
{
	//重力がかかりすぎたら止めたいので上限を設ける
	if (m_collidable->GetRb()->GetVec().y >= kMaxGravity)
	{
		//重力
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
	//落下しているかチェック
	if (m_collidable->GetRb()->GetVec().y <= kChangeFall)
	{
		//落下
		m_update = &Player::FallUpdate;
		return;
	}
	//ジャンプボタンを押してるならジャンプ
	if (input.IsTriggered("A") && m_isGround)
	{
		//ジャンプ
		m_update = &Player::JumpUpdate;
		return;
	}
	//入力があるなら移動
	if (m_stickVec.Magnitude() != 0)
	{
		//移動
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
	//落下しているかチェック
	if (m_collidable->GetRb()->GetVec().y <= kChangeFall)
	{
		//落下
		m_update = &Player::FallUpdate;
		return;
	}
	//ジャンプ
	if (input.IsTriggered("A") && m_isGround)
	{
		//ジャンプ
		m_update = &Player::JumpUpdate;
		return;
	}
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
		m_collidable->GetRb()->SetMoveVec(GetForwardVec(camera) * kMoveSpeed);
	}
}

void Player::JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//落下しているなら
	if (m_collidable->GetRb()->GetVec().y < 0.0f)
	{
		//落下
		m_update = &Player::FallUpdate;
		return;
	}
	//空中移動
	m_collidable->GetRb()->AddVec(GetForwardVec(camera) * kAirMoveSpeed);
	Vector3 vec = m_collidable->GetRb()->GetVec();
	vec.x *= 0.8f;
	vec.z *= 0.8f;
	m_collidable->GetRb()->SetVec(vec);
}

void Player::FallUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//ジャンプできるなら
	if (input.IsTriggered("A") && (m_jumpNum <= kMaxJumpNum))
	{
		//ジャンプ
		m_update = &Player::JumpUpdate;
		return;
	}
	//地面に付いているなら
	if (m_isGround)
	{
		//待機
		m_update = &Player::IdleUpdate;
		return;
	}
	//空中移動
	m_collidable->GetRb()->AddVec(GetForwardVec(camera) * kAirMoveSpeed);
	Vector3 vec = m_collidable->GetRb()->GetVec();
	vec.x *= 0.8f;
	vec.z *= 0.8f;
	m_collidable->GetRb()->SetVec(vec);
}

//状態に合わせて初期化処理
void Player::StateInit()
{
	//状態が変わっていないなら早期リターン
	if (m_lastUpdate == m_update)return;
	m_lastUpdate = m_update;
	if (m_update == &Player::IdleUpdate)
	{
		//ジャンプカウントリセット
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::MoveUpdate)
	{
		//ジャンプカウントリセット
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::JumpUpdate)
	{
		//飛んでるので
		m_isGround = false;
		m_collidable->SetState(State::Jump);
		//力を与える
		m_collidable->GetRb()->ResetVec();
		m_collidable->GetRb()->SetVec(kJumpVec);
	}
	else if (m_update == &Player::FallUpdate)
	{
		//落下してるので
		m_isGround = false;
		//ジャンプカウントは落下状態になってからカウントを進める
		++m_jumpNum;
		m_collidable->SetState(State::Fall);
	}
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
	float stickTheata = Theata(z, m_stickVec.Normalize());
	//プレイヤーを中心に次の座標を回転
	Matrix4x4 rotaMat = RotateYMat4x4(cameraTheata + stickTheata);
	//ベクトルにかける(回転)
	Vector3 moveVec = Vector3{ 0.0f, 0.0f, -1.0f };
	moveVec = rotaMat * moveVec;
	moveVec.y = 0.0f; //Y軸は無視
	return moveVec.Normalize();
}
