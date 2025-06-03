#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "../../../General/game.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/HurtPoint.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/MeleeAttack.h"
#include "../../Attack/AttackManager.h"
#include "../ActorManager.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 20.0f; //カプセルの半径
}

Player::Player(int modelHandle, Position3 firstPos) :
	Actor(ActorKind::Player),
	m_stickVec(0.0f,0.0f),
	m_isGround(false),
	m_isHit(false)
{
	//モデル
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//衝突判定
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //カプセルの上端
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));
}

Player::~Player()
{
}

void Player::Entry(std::shared_ptr<ActorManager> actorManager)
{
}

void Player::Exit(std::shared_ptr<ActorManager> actorManager)
{
}

void Player::Init()
{
	//コンストラクタで全部初期化したかったけど
	//shared_from_this()を使う場合コンストラクタ時点では
	//メモリが確定していないので使うことができない
	//対策としてInitを使う

	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = shared_from_this();
	m_state = std::make_shared<PlayerStateIdle>(thisPointer);
	//次の状態を待機状態に
	m_state->ChangeState(m_state);
	//やられ判定(衝突判定と同じにする)
	m_hurtPoint = std::make_shared<HurtPoint>(m_collidable, 100, thisPointer);
}

void Player::Update(const Input& input,const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//スティックの向きを入れる
	m_stickVec.x = -static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = static_cast<float>(input.GetStickInfo().leftStickY);

	//状態に合わせた更新
	m_state->Update(input, camera, attackManager);
	//状態が変わったかをチェック
	if (m_state != m_state->GetNextState())
	{
		//状態を変化する
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//アニメーションの更新
	m_model->Update();
	//やられ判定の位置更新
	UpdateHurtPoint();
}

void Player::Gravity(const Vector3& gravity)
{
	//重力がかかりすぎたら止めたいので上限を設ける
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
	{
		//重力
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Player::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//地面に当たった時の処理
	if (other->GetColl()->GetShape() == Shape::Polygon)
	{
		if (std::dynamic_pointer_cast<PolygonCollider>(other->GetColl())->IsFloor())
		{
			m_isGround = true;
		}
	}
}

void Player::Draw() const
{
#if _DEBUG
	//衝突判定
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		m_isGround//地面にいると塗りつぶされる
	);
	//やられ判定
	DrawCapsule3D(
		m_hurtPoint->GetCollidable()->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->GetRadius(),
		32,
		0x0000ff,
		0x0000ff,
		m_hurtPoint->IsNoDamege()//無敵の時は塗りつぶされる
	);
	printf("POS = %2f, %2f, %2f\n", m_collidable->GetRb()->GetPos().x, m_collidable->GetRb()->GetPos().y, m_collidable->GetRb()->GetPos().z);
	printf("VEC = %2f, %2f, %2f\n", m_collidable->GetRb()->GetVec().x, m_collidable->GetRb()->GetVec().y, m_collidable->GetRb()->GetVec().z);
#endif
	m_model->Draw();
}

void Player::Complete()
{
	m_collidable->GetRb()->SetNextPos();//次の座標へ
	Vector3 endPos = m_collidable->GetRb()->GetPos();
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}

//攻撃されたときに呼び出す
void Player::HitReaction()
{
	//ヒット状態にする
	m_isHit = true;
}

void Player::UpdateHurtPoint()
{
	//移動量を取得
	m_hurtPoint->GetCollidable()->GetRb()->SetVec(m_collidable->GetRb()->GetVec());
	//座標更新
	m_hurtPoint->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->SetEndPos(std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos());
}