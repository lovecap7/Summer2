#include "Bomb.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"
#include "../Player/Player.h"
#include "../../Attack/HurtPoint.h"
#include "../../Attack/BlastAttack.h"

namespace
{
	//ジャンプ力
	constexpr float kJumpPower = 10.0f;
	//当たり判定の半径
	constexpr float kCollRadius = 50.0f;
	//爆発の当たり判定の半径
	constexpr float kBlastRadius = 90.0f;
	//爆発までのフレーム数
	constexpr int kBlastFrame = 180;
}

Bomb::Bomb(int modelHandle, Vector3 pos) :
	ItemBase(ItemKind::Heart),
	m_blastCountFrame(kBlastFrame)
{
	auto firstPos = pos;
	firstPos.y += kCollRadius;
	//モデル
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//衝突判定
	m_collidable = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kCollRadius), std::make_shared<Rigidbody>(firstPos));
	//力を与える
	m_collidable->GetRb()->SetVecY(kJumpPower);
	//コライダブルの初期化
	m_collidable->Init(State::None, Priority::Low, GameTag::Item);
}

Bomb::~Bomb()
{
}

void Bomb::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャーに登録
	actorManager->Entry(shared_from_this());
}

void Bomb::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャー解除
	actorManager->Exit(shared_from_this());
}

void Bomb::Init()
{
	//コライダーに自分のポインタを持たせる
	m_collidable->SetOwner(shared_from_this());
}

void Bomb::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//移動量を初期化
	m_collidable->GetRb()->SetMoveVec(Vector3::Zero());
	//爆発のカウントを減らす
	m_blastCountFrame--;
	//爆発までのフレームが0になったら爆発
	if (m_blastCountFrame <= 0 && !m_isDelete)
	{
		//爆発の当たり判定を作成
		CreateAttack();
		//削除フラグを立てる
		m_isDelete = true;
		return;
	}
}

void Bomb::Gravity(const Vector3& gravity)
{
	//重力がかかりすぎたら止めたいので上限を設ける
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
	{
		//重力
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Bomb::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	
}

void Bomb::Draw() const
{
#if _DEBUG
	//衝突判定
	DrawSphere3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
#endif
	m_model->Draw();
}

void Bomb::Complete()
{
	//次の座標へ
	m_collidable->GetRb()->SetNextPos();
	//モデルの座標更新
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void Bomb::CreateAttack()
{
	std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->SetRadius(kBlastRadius);
	//爆発の当たり判定を作成
	m_blastAttack = std::make_shared<BlastAttack>(m_collidable, 100.0f, 5,20.0f, shared_from_this());
}