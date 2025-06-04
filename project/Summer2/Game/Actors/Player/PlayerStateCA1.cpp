#include "PlayerStateCA1.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "Player.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/AttackManager.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/MeleeAttack.h"
#include "../../Attack/HurtPoint.h"
namespace
{
	//チャージ攻撃の段階別ダメージ
	constexpr int kCA1AnimDamage = 100.0f;
	//チャージ攻撃の持続
	constexpr float kCA1KeepFrame = 30.0f;
	//アニメーション
	const char* kAnim = "Player|CA";//チャージ攻撃
	//チャージ攻撃の段階別アニメーションの速度
	constexpr float kCA1AnimSpeed = 2.0f;
	//左足の根本と足先のインデックス
	constexpr int kRootIndex = 64;
	constexpr int kToeIndex = 68;
	//左足の半径
	constexpr float kLeftLegRadius = 20.0f;
	//移動速度
	constexpr float kAttackMoveSpeed = 5.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateCA1::PlayerStateCA1(std::shared_ptr<Player> player, const std::shared_ptr<AttackManager>& attackManager) :
	PlayerStateBase(player)
{
	m_player->GetCollidable()->SetState(State::None);
	//チャージ攻撃1
	auto model = m_player->GetModel();
	model->SetAnim(kAnim, true, kCA1AnimSpeed);
	model->SetFixedLoopFrame(kCA1KeepFrame);//指定ループ
	//攻撃判定の準備
	CreateAttack();
	attackManager->Entry(m_attackC);
}

PlayerStateCA1::~PlayerStateCA1()
{
	//攻撃判定を消す
	m_attackC->Delete();
}
void PlayerStateCA1::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateCA1::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//攻撃を受けた時
	if (m_player->GetHurtPoint()->IsHit())
	{
		//やられ状態
		ChangeState(std::make_shared<PlayerStateHit>(m_player));
		return;
	}
	auto model = m_player->GetModel();
	//アニメーションが終了したら
	if (model->IsFinishFixedLoop())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//アニメーションが一周するたびに攻撃判定のリセット
	if (model->IsFinishAnim())
	{
		//初期化
		m_attackC->Init();
	}
	//入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//移動
		m_player->GetCollidable()->GetRb()->SetMoveVec(GetForwardVec(input, camera) * kAttackMoveSpeed);
	}
	else
	{
		//少しずつ減速する
		SpeedDown();
	}
	//攻撃の位置更新
	UpdateAttack();
}

void PlayerStateCA1::CreateAttack()
{
	auto model = m_player->GetModel();
	//左足
	//付け根と足先
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//付け根
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//足先
	m_leftLeg = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(toe, kLeftLegRadius),
		std::make_shared<Rigidbody>(root));

	//武器の座標と当たり判定を攻撃に紐図ける
	m_attackC = std::make_shared<MeleeAttack>(m_leftLeg, kCA1AnimDamage, kCA1KeepFrame, m_player);
}

void PlayerStateCA1::UpdateAttack()
{
	//左足の状態を更新したら攻撃も更新される
	auto model = m_player->GetModel();
	//左足
	//付け根と足先
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//付け根
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//足先
	//座標更新
	m_leftLeg->GetRb()->SetPos(root);
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftLeg->GetColl())->SetEndPos(toe);
}

void PlayerStateCA1::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
