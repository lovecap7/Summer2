#include "PlayerStateAttackN3.h"
#include "PlayerStateIdle.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
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
	//通常攻撃3のダメージと持続フレーム
	constexpr int kAN3Damege = 300.0f;
	constexpr int kAN3KeepFrame = 13;
	constexpr int kAN3StartFrame = 15;
	//アニメーション
	const char* kAnim = "Player|NA3";//通常攻撃3
	//弱攻撃の段階別アニメーションの速度
	constexpr float kAN3AnimSpeed = 1.3f;
	//攻撃終了前にキャンセル可能フレーム
	constexpr float kAttackCancelFrame = 30.0f;
	//武器の座標と当たり判定の情報
	//右手の薬指のインデックス
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//武器の長さと半径
	constexpr float kSwordHeight = 150.0f;
	constexpr float kRightSwordRadius = 10.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateAttackN3::PlayerStateAttackN3(std::shared_ptr<Player> player) :
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
	//通常攻撃2
	m_player->GetCollidable()->SetState(State::None);
	//攻撃2
	m_player->GetModel()->SetAnim(kAnim, false, kAN3AnimSpeed);
	//攻撃判定の準備
	CreateAttack();
}

PlayerStateAttackN3::~PlayerStateAttackN3()
{
	//攻撃判定を消す
	m_attackN3->Delete();
}
void PlayerStateAttackN3::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateAttackN3::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	//攻撃を受けた時
	if (m_player->GetHurtPoint()->IsHit())
	{
		//やられ状態
		ChangeState(std::make_shared<PlayerStateHit>(m_player));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAN3StartFrame)
	{
		//攻撃を入れる
		AppearAttack(m_attackN3, attackManager);
	}
	auto model = m_player->GetModel();
	//モデルのアニメーションが終わったら
	if (model->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//アニメーションのラスト数フレーム以内で入力があるなら3段回目の攻撃
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//回避ボタンを押したら
		if (input.IsTrigger("RB"))
		{
			//回避
			ChangeState(std::make_shared<PlayerStateRolling>(m_player));
			return;
		}
		//チャージボタンを押したら
		if (input.IsTrigger("Y"))
		{
			//チャージ
			ChangeState(std::make_shared<PlayerStateCharge>(m_player));
			return;
		}
	}
	//攻撃の位置更新
	UpdateAttack();
	//少しずつ減速する
	SpeedDown();
}

void PlayerStateAttackN3::CreateAttack()
{
	auto model = m_player->GetModel();
	//武器の位置を出す
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//薬指
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//人差し指
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//武器の長さ
	swordDir = VAdd(ringFinger, swordDir);//持ち手の座標に加算して剣先の座標を出す
	m_rightSword = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(swordDir, kRightSwordRadius),
		std::make_shared<Rigidbody>(ringFinger));

	//武器の座標と当たり判定を攻撃に紐図ける
	m_attackN3 = std::make_shared<MeleeAttack>(m_rightSword, kAN3Damege, kAN3KeepFrame, m_player);
}

void PlayerStateAttackN3::UpdateAttack()
{
	//武器の状態を更新したら攻撃も更新される

	auto model = m_player->GetModel();
	//武器
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//薬指
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//人差し指
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//武器の長さ
	swordDir = VAdd(ringFinger, swordDir);//持ち手の座標に加算して剣先の座標を出す
	//武器の持ち手をセット
	m_rightSword->GetRb()->SetPos(ringFinger);
	//武器の剣先をセット
	std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->SetEndPos(swordDir);
}

void PlayerStateAttackN3::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
