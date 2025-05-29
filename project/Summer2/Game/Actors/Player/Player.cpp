#include "Player.h"
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
#include <DxLib.h>
#include <cmath>

namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 20.0f; //カプセルの半径
	//移動速度
	constexpr float kLowMoveSpeed = 2.0f;//地上の小移動速度
	constexpr float kMediumMoveSpeed = 5.0f;//地上の中移動速度
	constexpr float kHighMoveSpeed = 10.0f;//地上の大移動速度
	constexpr float kAirMoveSpeed = 1.5f;//空中移動速度
	constexpr float kMaxAirMoveSpeed = 20.0f;//空中移動最高速度
	constexpr float kLightAttackMoveSpeed = 0.5f;//弱攻撃中の移動速度
	constexpr float kHighAttackMoveSpeed = 13.0f;//強攻撃中の移動速度
	constexpr float kRollingMoveSpeed = 10.0f;//回避速度
	//ジャンプ
	constexpr float kMaxGravity = -10.0f;//落下スピードが大きくなりすぎないように
	const Vector3 kJumpVec = { 0.0f,13.0f,0.0f };//ジャンプ
	constexpr unsigned int kMaxJumpNum = 2;//ジャンプ回数
	constexpr int kNextJumpFrame = 5;//次のジャンプができるまでのフレーム
	constexpr float kChangeStateFall = -2.0f;//落下状態に切り替わる落下ベクトルの大きさ
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	constexpr float kAirMoveDeceRate = 0.9f;
	//攻撃終了前にキャンセル可能フレーム
	constexpr float kAttackCancelFrame = 20.0f;
	//武器
	//右手の薬指のインデックス
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//武器の長さと半径
	constexpr float kSwordHeight = 150.0f;
	constexpr float kRightSwordRadius = 10.0f;
	//左足の根本と足先のインデックス
	constexpr int kRootIndex = 60;
	constexpr int kToeIndex = 64;
	//左足の半径
	constexpr float kLeftLegRadius = 20.0f;

	//通常攻撃1のダメージと持続フレームと発生フレーム
	constexpr int kAttackN1Damege = 100.0f;
	constexpr int kAttackN1KeepFrame = 6;
	constexpr int kAttackN1StartFrame = 15;
	//通常攻撃2のダメージと持続フレーム
	constexpr int kAttackN2Damege = 200.0f;
	constexpr int kAttackN2KeepFrame = 10;
	constexpr int kAttackN2StartFrame = 15;
	//通常攻撃3のダメージと持続フレーム
	constexpr int kAttackN3Damege = 300.0f;
	constexpr int kAttackN3KeepFrame = 13;
	constexpr int kAttackN3StartFrame = 15;
	//弱攻撃の段階別アニメーションの速度
	constexpr float kAN1AnimSpeed = 1.3f;
	constexpr float kAN2AnimSpeed = 1.3f;
	constexpr float kAN3AnimSpeed = 1.3f;
	//強攻撃の段階別攻撃フレーム
	constexpr float kCharge1KeepFrame = 30.0f;
	constexpr float kCharge2KeepFrame = 60.0f;
	constexpr float kCharge3KeepFrame = 100.0f;
	//強攻撃の段階別アニメーションの速度
	constexpr float kCharge1AnimSpeed = 2.0f;
	constexpr float kCharge2AnimSpeed = 2.5f;
	constexpr float kCharge3AnimSpeed = 3.0f;
	//強攻撃の段階別ダメージ
	constexpr int kCharge1AnimDamage = 10.0f;
	constexpr int kCharge2AnimDamage = 12.0f;
	constexpr int kCharge3AnimDamage = 15.0f;
	//回避モーションの速度
	constexpr float kRollingAnimSpeed = 1.2f;

	//アニメーションの名前
	const char* kIdleAnim = "Player|Idle";//待機
	const char* kRunAnim = "Player|Run";//走る
	const char* kRollingAnim = "Player|Rolling";//回避
	const char* kJumpAnim = "Player|Jump";//ジャンプ
	const char* kFallAnim = "Player|Fall";//落ちる
	const char* kAttack_N1Anim = "Player|Attack_N1";//通常攻撃1
	const char* kAttack_N2Anim = "Player|Attack_N2";//通常攻撃2
	const char* kAttack_N3Anim = "Player|Attack_N3";//通常攻撃3
	const char* kAttack_C1Anim = "Player|Attack_C1";//チャージ
	const char* kAttack_C2Anim = "Player|Attack_C2";//チャージ攻撃
}

Player::Player(int modelHandle, Position3 firstPos) :
	Actor(ActorKind::Player),
	m_stickVec(0.0f,0.0f),
	m_update(&Player::UpdateIdle),
	m_lastUpdate(&Player::UpdateFall),
	m_isGround(true),
	m_jumpNum(0),
	m_nextJumpFrame(kNextJumpFrame),
	m_attackCountFrame(0),
	m_chargeFrame(0)
{
	//モデル
	m_model = std::make_unique<Model>(modelHandle, firstPos.ToDxLibVector());
	//衝突判定
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //カプセルの上端
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));
	//やられ判定(衝突判定と同じにする)
	m_hurtPoint = std::make_shared<HurtPoint>(m_collidable, 100, *this);
	//攻撃の判定の準備
	CreateAttack();
}

Player::~Player()
{
}

void Player::Update(const Input& input,const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//スティックの向きを入れる
	m_stickVec.x = -static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = static_cast<float>(input.GetStickInfo().leftStickY);
	//状態に合わせて初期化
	InitState();
	//状態に合わせた更新
	(this->*m_update)(input,camera, attackManager);
	//アニメーションの更新
	m_model->Update();
	//戦闘に関する更新処理
	BattleUpdate();
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
	//剣
	DrawCapsule3D(
		m_rightSword->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->GetRadius(),
		16,
		0xffff00,
		0xffff00,
		false
	);
	//左足
	unsigned int color = 0xff0000;
	//2段回目
	if (m_chargeFrame > kCharge1KeepFrame && m_chargeFrame <= kCharge2KeepFrame)
	{
		color = 0xffff00;
	}
	//3段回目
	else if (m_chargeFrame > kCharge2KeepFrame && m_chargeFrame <= kCharge3KeepFrame)
	{
		color = 0x00ffff;
	}
	DrawCapsule3D(
		m_leftLeg->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_leftLeg->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_leftLeg->GetColl())->GetRadius(),
		16,
		color,
		color,
		false
	);
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

void Player::HitReaction()
{
	//やられリアクション
}

void Player::UpdateIdle(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//落下しているかチェック
	if (m_collidable->GetRb()->GetVec().y <= kChangeStateFall)
	{
		//落下
		m_update = &Player::UpdateFall;
		return;
	}
	//回避ボタンを押したら
	if (input.IsTrigger("RB"))
	{
		//回避
		m_update = &Player::UpdateRolling;
		return;
	}
	//ジャンプボタンを押してるならジャンプ
	if (input.IsTrigger("A") && m_isGround)
	{
		//ジャンプ
		m_update = &Player::UpdateJump;
		return;
	}
	//弱攻撃ボタンを押したら
	if (input.IsTrigger("X"))
	{
		//攻撃を入れる(テスト)
		attackManager->SetAttack(m_attackN1);
		m_attackN1->Init();

		//弱攻撃
		m_update = &Player::UpdateAttackNormal1;
		return;
	}
	//強攻撃ボタンを押したら
	if (input.IsTrigger("Y"))
	{
		//強攻撃
		m_update = &Player::UpdateAttackCharge1;
		return;
	}
	//入力があるなら移動
	if (m_stickVec.Magnitude() != 0)
	{
		//移動
		m_update = &Player::UpdateMove;
		return;
	}
	//少しずつ減速する
	SpeedDown();
}

void Player::UpdateMove(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//落下しているかチェック
	if (m_collidable->GetRb()->GetVec().y <= kChangeStateFall)
	{
		//落下
		m_update = &Player::UpdateFall;
		return;
	}
	//回避ボタンを押したら
	if (input.IsTrigger("RB"))
	{
		//回避
		m_update = &Player::UpdateRolling;
		return;
	}
	//ジャンプ
	if (input.IsTrigger("A") && m_isGround)
	{
		//ジャンプ
		m_update = &Player::UpdateJump;
		return;
	}
	//弱攻撃ボタンを押したら
	if (input.IsTrigger("X"))
	{
		//弱攻撃
		m_update = &Player::UpdateAttackNormal1;
		return;
	}
	//強攻撃ボタンを押したら
	if (input.IsTrigger("Y"))
	{
		//強攻撃
		m_update = &Player::UpdateAttackCharge1;
		return;
	}
	//入力がないなら待機状態へ
	if (m_stickVec.Magnitude() == 0)
	{
		//待機
		m_update = &Player::UpdateIdle;
		return;
	}
	else
	{
		//移動
		m_collidable->GetRb()->SetMoveVec(GetForwardVec(camera) * InputValueSpeed(input));
		//向きの更新
		m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
	}
}

void Player::UpdateJump(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//落下しているなら
	if (m_collidable->GetRb()->GetVec().y < 0.0f)
	{
		//落下
		m_update = &Player::UpdateFall;
		return;
	}
	//次のジャンプのクールタイム
	--m_nextJumpFrame;
	if (m_nextJumpFrame <= 0 && input.IsTrigger("A"))//上昇中にジャンプを押した場合
	{
		m_nextJumpFrame = 0;
		//落下状態に遷移して遷移先でジャンプ
		m_update = &Player::UpdateFall;
		return;
	}
	//空中移動
	m_collidable->GetRb()->AddVec(GetForwardVec(camera) * kAirMoveSpeed);
	//横移動速度に上限をつける
	float speed = m_collidable->GetRb()->GetMoveVec().Magnitude();
	if (speed > 0.0f)
	{
		speed = ClampFloat(speed, 0.0f, kMaxAirMoveSpeed);
		m_collidable->GetRb()->SetMoveVec(m_collidable->GetRb()->GetMoveVec().Normalize() * speed);
	}

	//少しずつ減速する
	SpeedDown();
	//向きの更新
	m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
}

void Player::UpdateFall(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//ジャンプできるなら
	if (input.IsTrigger("A") && (m_jumpNum < kMaxJumpNum))
	{
		//ジャンプ
		m_update = &Player::UpdateJump;
		return;
	}
	//地面に付いているなら
	if (m_isGround)
	{
		//待機
		m_update = &Player::UpdateIdle;
		return;
	}
	//空中移動
	m_collidable->GetRb()->AddVec(GetForwardVec(camera) * kAirMoveSpeed);
	//横移動速度に上限をつける
	float speed = m_collidable->GetRb()->GetMoveVec().Magnitude();
	if (speed > 0.0f)
	{
		speed = ClampFloat(speed, 0.0f, kMaxAirMoveSpeed);
		m_collidable->GetRb()->SetMoveVec(m_collidable->GetRb()->GetMoveVec().Normalize() * speed);
	}
	//少しずつ減速する
	SpeedDown();
	//向きの更新
	m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
}

void Player::UpdateAttackNormal1(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackN1StartFrame)
	{
		//攻撃を入れる
		AppearAttack(m_attackN1, attackManager);
	}
	//モデルのアニメーションが終わったら
	if (m_model->IsFinishAnim())
	{
		//攻撃判定を消す
		m_attackN1->Delete();
		//待機
		m_update = &Player::UpdateIdle;
		return;
	}
	//アニメーションのラスト数フレーム以内で入力があるなら2段回目の攻撃
	if (m_model->GetTotalAnimFrame() - kAttackCancelFrame <= m_model->GetNowAnimFrame())
	{
		//回避ボタンを押したら
		if (input.IsTrigger("RB"))
		{
			//攻撃判定を消す
			m_attackN1->Delete();
			//回避
			m_update = &Player::UpdateRolling;
			return;
		}
		//2段目
		if (input.IsTrigger("X"))
		{
			//攻撃判定を消す
			m_attackN1->Delete();
			//2段目
			m_update = &Player::UpdateAttackNormal2;
			return;
		}
		//タメ攻撃
		if (input.IsPress("Y"))
		{
			//攻撃判定を消す
			m_attackN1->Delete();
			//強攻撃
			m_update = &Player::UpdateAttackCharge1;
			return;
		}
	}

	//少しずつ減速する
	SpeedDown();
}

void Player::UpdateAttackNormal2(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackN2StartFrame)
	{
		//攻撃を入れる
		AppearAttack(m_attackN2, attackManager);
	}
	//モデルのアニメーションが終わったら
	if (m_model->IsFinishAnim())
	{
		//攻撃判定を消す
		m_attackN2->Delete();
		//待機
		m_update = &Player::UpdateIdle;
		return;
	}
	//アニメーションのラスト数フレーム以内で入力があるなら2段回目の攻撃
	if (m_model->GetTotalAnimFrame() - kAttackCancelFrame <= m_model->GetNowAnimFrame())
	{
		//回避ボタンを押したら
		if (input.IsTrigger("RB"))
		{
			//攻撃判定を消す
			m_attackN2->Delete();
			//回避
			m_update = &Player::UpdateRolling;
			return;
		}
		//3段目
		if (input.IsTrigger("X"))
		{
			//攻撃判定を消す
			m_attackN2->Delete();
			//3段目
			m_update = &Player::UpdateAttackNormal3;
			return;
		}
		//タメ攻撃
		if (input.IsPress("Y"))
		{
			//攻撃判定を消す
			m_attackN2->Delete();
			//強攻撃
			m_update = &Player::UpdateAttackCharge1;
			return;
		}
	}

	//少しずつ減速する
	SpeedDown();
}
void Player::UpdateAttackNormal3(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackN3StartFrame)
	{
		AppearAttack(m_attackN3,attackManager);
	}
	//モデルのアニメーションが終わったら
	if (m_model->IsFinishAnim())
	{
		//攻撃判定を消す
		m_attackN3->Delete();
		//待機
		m_update = &Player::UpdateIdle;
		return;
	}
	//アニメーションのラスト数フレーム以内で強攻撃の入力があるなら
	if (m_model->GetTotalAnimFrame() - kAttackCancelFrame <= m_model->GetNowAnimFrame())
	{
		if (input.IsTrigger("RB"))
		{
			//攻撃判定を消す
			m_attackN3->Delete();
			//回避
			m_update = &Player::UpdateRolling;
			return;
		}
		if (input.IsPress("Y"))
		{
			//攻撃判定を消す
			m_attackN3->Delete();
			//強攻撃
			m_update = &Player::UpdateAttackCharge1;
			return;
		}
	}
	//少しずつ減速する
	SpeedDown();
}
//タメ
void Player::UpdateAttackCharge1(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//回避ボタンを押したら
	if (input.IsTrigger("RB"))
	{
		//回避
		m_update = &Player::UpdateRolling;
		return;
	}
	//少しずつ減速する
	SpeedDown();
	//向きの更新
	m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
	//溜めてる時
	if (input.IsPress("Y"))
	{
		//タメ攻撃チャージ
		++m_chargeFrame;
		//最大
		if (m_chargeFrame >= kCharge3KeepFrame)
		{
			m_chargeFrame = kCharge3KeepFrame;
		}
	}
	//ボタンを離す
	else
	{
		//攻撃発生
		AppearAttack(m_attackC, attackManager);
		//タメ攻撃
		m_update = &Player::UpdateAttackCharge2;
		return;
	}
}

void Player::UpdateAttackCharge2(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//アニメーションが終了したら
	if (m_model->IsFinishFixedLoop())
	{
		//攻撃判定を消す
		m_attackC->Delete();
		//待機
		m_update = &Player::UpdateIdle;
		return;
	}
	//アニメーションが一周するたびに攻撃判定のリセット
	if (m_model->IsFinishAnim())
	{
		//初期化
		m_attackC->Init();
	}
	 //向いてる方向に移動
	 m_collidable->GetRb()->SetMoveVec(m_model->GetDir() * kHighAttackMoveSpeed);
}
void Player::UpdateRolling(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//モデルのアニメーションが終わったら
	if (m_model->IsFinishAnim())
	{
		//待機
		m_update = &Player::UpdateIdle;
		return;
	}
	//向いてる方向に移動
	m_collidable->GetRb()->SetMoveVec(m_model->GetDir() * kRollingMoveSpeed);
}

//状態に合わせて初期化処理
void Player::InitState()
{
	//状態が変わっていないなら早期リターン
	if (m_lastUpdate == m_update)return;
	if (m_update == &Player::UpdateIdle)
	{
		//待機状態
		m_model->SetAnim(kIdleAnim, true);
		//ジャンプカウントリセット
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::UpdateMove)
	{
		//走る
		m_model->SetAnim(kRunAnim, true);
		//ジャンプカウントリセット
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::UpdateJump)
	{
		//ジャンプ
		m_model->SetAnim(kJumpAnim, false);
		//飛んでるので
		m_isGround = false;
		m_collidable->SetState(State::Jump);
		//力を与える
		m_collidable->GetRb()->ResetVec();
		m_collidable->GetRb()->SetVec(kJumpVec);
		//クールタイム
		m_nextJumpFrame = kNextJumpFrame;
	}
	else if (m_update == &Player::UpdateFall)
	{
		//落下
		m_model->SetAnim(kFallAnim, true);
		//落下してるので
		m_isGround = false;
		//ジャンプカウントは落下状態になってからカウントを進める
		++m_jumpNum;
		m_collidable->SetState(State::Fall);
	}
	else if (m_update == &Player::UpdateAttackNormal1)
	{
		m_collidable->SetState(State::None);
		//攻撃1
		m_model->SetAnim(kAttack_N1Anim, false, kAN1AnimSpeed);
		//向きの更新
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//攻撃フレームリセット
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::UpdateAttackNormal2)
	{
		m_collidable->SetState(State::None);
		//攻撃2
		m_model->SetAnim(kAttack_N2Anim, false, kAN2AnimSpeed);
		//向きの更新
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//攻撃フレームリセット
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::UpdateAttackNormal3)
	{
		m_collidable->SetState(State::None);
		//攻撃3
		m_model->SetAnim(kAttack_N3Anim, false, kAN3AnimSpeed);
		//向きの更新
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//攻撃フレームリセット
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::UpdateAttackCharge1)
	{
		m_collidable->SetState(State::None);
		//チャージ攻撃1
		m_model->SetAnim(kAttack_C1Anim, true);
	}
	else if (m_update == &Player::UpdateAttackCharge2)
	{
		m_collidable->SetState(State::None);
		//チャージ時間に合わせて持続させる
		//1段回目
		if (m_chargeFrame <= kCharge1KeepFrame)
		{
			//チャージ攻撃2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge1AnimSpeed);
			m_model->SetFixedLoopFrame(kCharge1KeepFrame);//指定ループ
			m_attackC->SetDamage(kCharge1AnimDamage);//ダメージ
		}
		//2段回目
		else if (m_chargeFrame <= kCharge2KeepFrame)
		{
			//チャージ攻撃2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge2AnimSpeed);
			m_model->SetFixedLoopFrame(kCharge2KeepFrame);//指定ループ
			m_attackC->SetDamage(kCharge2AnimDamage);//ダメージ
		}
		//3段回目
		else if (m_chargeFrame <= kCharge3KeepFrame)
		{
			//チャージ攻撃2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge3AnimSpeed);
			m_model->SetFixedLoopFrame(kCharge3KeepFrame);//指定ループ
			m_attackC->SetDamage(kCharge3AnimDamage);//ダメージ
		}
		m_chargeFrame = 0;//初期化
	}
	else if (m_update == &Player::UpdateRolling)
	{
		m_collidable->SetState(State::None);
		//回避
		m_model->SetAnim(kRollingAnim, false, kRollingAnimSpeed);
		//向きの更新
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
	}
	m_lastUpdate = m_update;
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
	Matrix4x4 rotaMat = Matrix4x4::RotateYMat4x4(cameraTheata + stickTheata);
	//ベクトルにかける(回転)
	Vector3 moveVec = Vector3{ 0.0f, 0.0f, -1.0f };
	moveVec = rotaMat * moveVec;
	moveVec.y = 0.0f; //Y軸は無視
	return moveVec.Normalize();
}

void Player::SpeedDown()
{
	//減速
	Vector3 vec = m_collidable->GetRb()->GetVec();
	if (m_isGround)
	{
		vec.x *= kMoveDeceRate;
		vec.z *= kMoveDeceRate;
	}
	else
	{
		vec.x *= kAirMoveDeceRate;
		vec.z *= kAirMoveDeceRate;
	}
	m_collidable->GetRb()->SetVec(vec);
}

float Player::InputValueSpeed(const Input& input)
{
	float moveSpeed = 0.0f;
	//速度をスティック入力の深度に合わせる
	if (input.IsLowPowerLeftStick())moveSpeed = kLowMoveSpeed;
	if (input.IsMediumPowerLeftStick())moveSpeed = kMediumMoveSpeed;
	if (input.IsHighPowerLeftStick())moveSpeed = kHighMoveSpeed;
	return moveSpeed;
}

void Player::BattleUpdate()
{
	//武器の位置更新
	UpdateRightSword();
	//左足の位置更新
	UpdateLeftLeg();
	//やられ判定の位置更新
	UpdateHurtPoint();
}

void Player::CreateRightSword()
{
	//武器
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR ringFinger = MV1GetFramePosition(m_model->GetModelHandle(), kRightRingFingerIndex);//薬指
	VECTOR indexFinger = MV1GetFramePosition(m_model->GetModelHandle(), kRightIndexFingerIndex);//人差し指
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//武器の長さ
	swordDir = VAdd(ringFinger, swordDir);//持ち手の座標に加算して剣先の座標を出す
	m_rightSword = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(swordDir.x, swordDir.y, swordDir.z), kRightSwordRadius), std::make_shared<Rigidbody>(Vector3(ringFinger.x, ringFinger.y, ringFinger.z)));
}

void Player::UpdateRightSword()
{
	//武器
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR ringFinger = MV1GetFramePosition(m_model->GetModelHandle(), kRightRingFingerIndex);//薬指
	VECTOR indexFinger = MV1GetFramePosition(m_model->GetModelHandle(), kRightIndexFingerIndex);//人差し指
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//武器の長さ
	swordDir = VAdd(ringFinger, swordDir);//持ち手の座標に加算して剣先の座標を出す
	//武器の持ち手をセット
	m_rightSword->GetRb()->SetPos(Position3(ringFinger.x, ringFinger.y, ringFinger.z));
	//武器の剣先をセット
	std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->SetEndPos(Position3(swordDir.x, swordDir.y, swordDir.z));
}

void Player::CreateLeftLeg()
{
	//左足
	//付け根と足先
	VECTOR root = MV1GetFramePosition(m_model->GetModelHandle(), kRootIndex);//付け根
	VECTOR toe = MV1GetFramePosition(m_model->GetModelHandle(), kToeIndex);//足先
	m_leftLeg = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(toe.x, toe.y, toe.z), kLeftLegRadius), 
		std::make_shared<Rigidbody>(Vector3(root.x, root.y, root.z)));
}

void Player::UpdateLeftLeg()
{
	//左足
	//付け根と足先
	VECTOR root = MV1GetFramePosition(m_model->GetModelHandle(), kRootIndex);//付け根
	VECTOR toe = MV1GetFramePosition(m_model->GetModelHandle(), kToeIndex);//足先
	//付け根をセット
	m_leftLeg->GetRb()->SetPos(Position3(root.x, root.y, root.z));
	//足先をセット
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftLeg->GetColl())->SetEndPos(Position3(toe.x, toe.y, toe.z));
}

void Player::CreateAttack()
{
	//攻撃に必要な判定の作成
	//剣
	CreateRightSword();
	//左足
	CreateLeftLeg();
	//攻撃の準備
	m_attackN1 = std::make_shared<MeleeAttack>(m_rightSword, kAttackN1Damege, kAttackN1KeepFrame, *this);
	m_attackN2 = std::make_shared<MeleeAttack>(m_rightSword, kAttackN2Damege, kAttackN2KeepFrame, *this);
	m_attackN3 = std::make_shared<MeleeAttack>(m_rightSword, kAttackN3Damege, kAttackN3KeepFrame, *this);
	m_attackC = std::make_shared<MeleeAttack>(m_leftLeg, kCharge1AnimDamage, kCharge1KeepFrame, *this);
}

void Player::AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager>& attackManager)
{
	//攻撃を入れる
	attack->Init();
	attackManager->SetAttack(attack);
}

void Player::UpdateHurtPoint()
{
	//移動量を取得
	m_hurtPoint->GetCollidable()->GetRb()->SetVec(m_collidable->GetRb()->GetVec());
	//座標更新
	m_hurtPoint->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->SetEndPos(std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos());
}