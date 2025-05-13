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
#include "../../Attack/Slash.h"
#include "../../Attack/AttackManager.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 20.0f; //カプセルの半径
	//移動速度
	constexpr float kMoveSpeed = 10.0f;//地上の移動速度
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
	//キャンセル可能フレーム
	constexpr float kCancelAttackFrame = 15.0f;

	//強攻撃の段階別アニメーションの速度
	constexpr float kAN1AnimSpeed = 1.1f;
	constexpr float kAN2AnimSpeed = 1.1f;
	constexpr float kAN3AnimSpeed = 1.2f;
	//強攻撃の段階別攻撃フレーム
	constexpr float kCharge1Frame = 30.0f;
	constexpr float kCharge2Fraem = 60.0f;
	constexpr float kCharge3Frame = 100.0f;
	//強攻撃の段階別アニメーションの速度
	constexpr float kCharge1AnimSpeed = 1.0f;
	constexpr float kCharge2AnimSpeed = 1.5f;
	constexpr float kCharge3AnimSpeed = 2.0f;

	//武器
	//右手の薬指のフレーム
	constexpr int kRightHandPinky1 = 55;
	constexpr int kRightHandPinky2 = 43;
	//武器の長さと半径
	constexpr float kSwordHeight = 100.0f;
	constexpr float kSwordRadius = 20.0f;

	//通常攻撃1のダメージと持続フレームと発生フレーム
	constexpr float kAttackN1Damege = 10.0f;
	constexpr int kAttackN1KeepFrame = 20;
	constexpr int kAttackN1StartFrame = 10;
	//通常攻撃2のダメージと持続フレーム
	constexpr float kAttackN2Damege = 20.0f;
	constexpr int kAttackN2KeepFrame = 20;
	constexpr int kAttackN2StartFrame = 10;
	//通常攻撃3のダメージと持続フレーム
	constexpr float kAttackN3Damege = 30.0f;
	constexpr int kAttackN3KeepFrame = 40;
	constexpr int kAttackN3StartFrame = 10;

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
	m_update(&Player::IdleUpdate),
	m_lastUpdate(&Player::FallUpdate),
	m_isGround(true),
	m_jumpNum(0),
	m_nextJumpFrame(kNextJumpFrame),
	m_chargeHighAttackFrame(0),
	m_attackCountFrame(0)
{
	//衝突判定
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //カプセルの上端
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));

	//やられ判定(衝突判定と同じにする)
	m_hurtPoint = std::make_shared<HurtPoint>(m_collidable, 100, *this);

	//モデル
	m_model = std::make_unique<Model>(modelHandle, firstPos.ToDxLibVector());

	//武器
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR rightHand = MV1GetFramePosition(m_model->GetModelHandle(), kRightHandPinky1);
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(MV1GetFramePosition(m_model->GetModelHandle(), kRightHandPinky2),rightHand));
	swordDir = VScale(swordDir, kSwordHeight);//武器の長さ
	swordDir = VAdd(rightHand, swordDir);//持ち手の座標に加算して剣先の座標を出す
	m_rightSword = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(swordDir.x, swordDir.y, swordDir.z), kSwordRadius), std::make_shared<Rigidbody>(Vector3(rightHand.x, rightHand.y, rightHand.z)));


	//攻撃の準備
	m_attackN1 = std::make_shared<Slash>(m_rightSword, kAttackN1Damege, kAttackN1KeepFrame);
	m_attackN2 = std::make_shared<Slash>(m_rightSword, kAttackN2Damege, kAttackN2KeepFrame);
	m_attackN3 = std::make_shared<Slash>(m_rightSword, kAttackN3Damege, kAttackN3KeepFrame);

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
	StateInit();
	//更新
	(this->*m_update)(input,camera, attackManager);
	//アニメーションの更新
	m_model->Update();
	//衝突判定をもとにやられ判定の位置更新

	//武器の位置更新
	WeaponUpdate();
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
	DrawCapsule3D(
		m_rightSword->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->GetRadius(),
		16,
		0xffff00,
		0xffff00,
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

void Player::IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//落下しているかチェック
	if (m_collidable->GetRb()->GetVec().y <= kChangeStateFall)
	{
		//落下
		m_update = &Player::FallUpdate;
		return;
	}
	//回避ボタンを押したら
	if (input.IsTriggered("RB"))
	{
		//回避
		m_update = &Player::RollingUpdate;
		return;
	}
	//ジャンプボタンを押してるならジャンプ
	if (input.IsTriggered("A") && m_isGround)
	{
		//ジャンプ
		m_update = &Player::JumpUpdate;
		return;
	}
	//弱攻撃ボタンを押したら
	if (input.IsTriggered("X"))
	{
		//攻撃を入れる(テスト)
		attackManager->SetAttack(m_attackN1);
		m_attackN1->Init();

		//弱攻撃
		m_update = &Player::AttackNormal1Update;
		return;
	}
	//強攻撃ボタンを押したら
	if (input.IsTriggered("Y"))
	{
		//強攻撃
		m_update = &Player::AttackCharge1Update;
		return;
	}
	//入力があるなら移動
	if (m_stickVec.Magnitude() != 0)
	{
		//移動
		m_update = &Player::MoveUpdate;
		return;
	}
	//少しずつ減速する
	SpeedDown();
}

void Player::MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//落下しているかチェック
	if (m_collidable->GetRb()->GetVec().y <= kChangeStateFall)
	{
		//落下
		m_update = &Player::FallUpdate;
		return;
	}
	//回避ボタンを押したら
	if (input.IsTriggered("RB"))
	{
		//回避
		m_update = &Player::RollingUpdate;
		return;
	}
	//ジャンプ
	if (input.IsTriggered("A") && m_isGround)
	{
		//ジャンプ
		m_update = &Player::JumpUpdate;
		return;
	}
	//弱攻撃ボタンを押したら
	if (input.IsTriggered("X"))
	{
		//弱攻撃
		m_update = &Player::AttackNormal1Update;
		return;
	}
	//強攻撃ボタンを押したら
	if (input.IsTriggered("Y"))
	{
		//強攻撃
		m_update = &Player::AttackCharge1Update;
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
		//向きの更新
		m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
	}
}

void Player::JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//落下しているなら
	if (m_collidable->GetRb()->GetVec().y < 0.0f)
	{
		//落下
		m_update = &Player::FallUpdate;
		return;
	}
	//次のジャンプのクールタイム
	--m_nextJumpFrame;
	if (m_nextJumpFrame <= 0 && input.IsTriggered("A"))//上昇中にジャンプを押した場合
	{
		m_nextJumpFrame = 0;
		//落下状態に遷移して遷移先でジャンプ
		m_update = &Player::FallUpdate;
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

void Player::FallUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//ジャンプできるなら
	if (input.IsTriggered("A") && (m_jumpNum < kMaxJumpNum))
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

void Player::AttackNormal1Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackN1StartFrame)
	{
		//攻撃を入れる
		attackManager->SetAttack(m_attackN1);
		m_attackN1->Init();
	}
	//モデルのアニメーションが終わったら
	if (m_model->IsFinishAnim())
	{
		//待機
		m_update = &Player::IdleUpdate;
		return;
	}
	//アニメーションのラスト数フレーム以内で入力があるなら2段回目の攻撃
	if (m_model->GetTatalAnimFrame() - kCancelAttackFrame <= m_model->GetNowAnimFrame())
	{
		if (input.IsTriggered("X"))
		{
			//2段目
			m_update = &Player::AttackNormal2Update;
			return;
		}
	}

	//少しずつ減速する
	SpeedDown();
}

void Player::AttackNormal2Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackN2StartFrame)
	{
		//攻撃を入れる
		attackManager->SetAttack(m_attackN2);
		m_attackN2->Init();
	}
	//モデルのアニメーションが終わったら
	if (m_model->IsFinishAnim())
	{
		//待機
		m_update = &Player::IdleUpdate;
		return;
	}
	//アニメーションのラスト数フレーム以内で入力があるなら2段回目の攻撃
	if (m_model->GetTatalAnimFrame() - kCancelAttackFrame <= m_model->GetNowAnimFrame())
	{
		if (input.IsTriggered("X"))
		{
			//3段目
			m_update = &Player::AttackNormal3Update;
			return;
		}
	}

	//少しずつ減速する
	SpeedDown();
}
void Player::AttackNormal3Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackN3StartFrame)
	{
		//攻撃を入れる
		attackManager->SetAttack(m_attackN3);
		m_attackN3->Init();
	}
	//モデルのアニメーションが終わったら
	if (m_model->IsFinishAnim())
	{
		//待機
		m_update = &Player::IdleUpdate;
		return;
	}

	//少しずつ減速する
	SpeedDown();
}
//タメ
void Player::AttackCharge1Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//少しずつ減速する
	SpeedDown();
	//向きの更新
	m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
	//溜めてる時
	if (input.IsPressed("Y"))
	{
		//タメ攻撃チャージ
		++m_chargeHighAttackFrame;
		//最大
		if (m_chargeHighAttackFrame >= kCharge3Frame)
		{
			m_chargeHighAttackFrame = kCharge3Frame;
		}
	}
	//ボタンを離す
	else
	{
		//タメ攻撃
		m_update = &Player::AttackCharge2Update;
		return;
	}
}

void Player::AttackCharge2Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//持続フレームが終了したら
	if (m_chargeHighAttackFrame <= 0.0f)
	{
		//待機
		m_update = &Player::IdleUpdate;
		return;
	}
	 //向いてる方向に移動
	 m_collidable->GetRb()->SetMoveVec(m_model->GetDir() * kHighAttackMoveSpeed);
	 --m_chargeHighAttackFrame;
}
void Player::RollingUpdate(const Input& input, const std::unique_ptr<Camera>& camera, const std::unique_ptr<AttackManager>& attackManager)
{
	//モデルのアニメーションが終わったら
	if (m_model->IsFinishAnim())
	{
		//待機
		m_update = &Player::IdleUpdate;
		return;
	}
	//向いてる方向に移動
	m_collidable->GetRb()->SetMoveVec(m_model->GetDir() * kRollingMoveSpeed);
}
//状態に合わせて初期化処理
void Player::StateInit()
{
	//状態が変わっていないなら早期リターン
	if (m_lastUpdate == m_update)return;
	if (m_update == &Player::IdleUpdate)
	{
		//待機状態
		m_model->SetAnim(kIdleAnim, true);
		//ジャンプカウントリセット
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::MoveUpdate)
	{
		//走る
		m_model->SetAnim(kRunAnim, true);
		//ジャンプカウントリセット
		m_jumpNum = 0;
		m_collidable->SetState(State::None);
	}
	else if (m_update == &Player::JumpUpdate)
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
	else if (m_update == &Player::FallUpdate)
	{
		//落下
		m_model->SetAnim(kFallAnim, true);
		//落下してるので
		m_isGround = false;
		//ジャンプカウントは落下状態になってからカウントを進める
		++m_jumpNum;
		m_collidable->SetState(State::Fall);
	}
	else if (m_update == &Player::AttackNormal1Update)
	{
		m_collidable->SetState(State::None);
		//攻撃1
		m_model->SetAnim(kAttack_N1Anim, false, kAN1AnimSpeed);
		//向きの更新
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//攻撃フレームリセット
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::AttackNormal2Update)
	{
		m_collidable->SetState(State::None);
		//攻撃2
		m_model->SetAnim(kAttack_N2Anim, false, kAN2AnimSpeed);
		//向きの更新
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//攻撃フレームリセット
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::AttackNormal3Update)
	{
		m_collidable->SetState(State::None);
		//攻撃3
		m_model->SetAnim(kAttack_N3Anim, false, kAN3AnimSpeed);
		//向きの更新
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//攻撃フレームリセット
		m_attackCountFrame = 0;
	}
	else if (m_update == &Player::AttackCharge1Update)
	{
		m_collidable->SetState(State::None);
		//チャージ攻撃1
		m_model->SetAnim(kAttack_C1Anim, true);
	}
	else if (m_update == &Player::AttackCharge2Update)
	{
		m_collidable->SetState(State::None);
		//チャージ時間に合わせて持続させる
		//1段回目
		if (m_chargeHighAttackFrame <= kCharge1Frame)
		{
			//チャージ攻撃2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge1AnimSpeed);
			m_chargeHighAttackFrame = kCharge1Frame;
		}
		//2段回目
		else if (m_chargeHighAttackFrame <= kCharge2Fraem)
		{
			//チャージ攻撃2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge2AnimSpeed);
			m_chargeHighAttackFrame = kCharge2Fraem;
		}
		//3段回目
		else if (m_chargeHighAttackFrame <= kCharge3Frame)
		{
			//チャージ攻撃2
			m_model->SetAnim(kAttack_C2Anim, true, kCharge3AnimSpeed);
			m_chargeHighAttackFrame = kCharge3Frame;
		}
	}
	else if (m_update == &Player::RollingUpdate)
	{
		m_collidable->SetState(State::None);
		//回避
		m_model->SetAnim(kRollingAnim, false);
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
	Matrix4x4 rotaMat = RotateYMat4x4(cameraTheata + stickTheata);
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

void Player::WeaponUpdate()
{
	//武器
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR rightHand = MV1GetFramePosition(m_model->GetModelHandle(), kRightHandPinky1);
	//武器の剣先
	VECTOR swordDir = VNorm(VSub(MV1GetFramePosition(m_model->GetModelHandle(), kRightHandPinky2),rightHand));
	swordDir = VScale(swordDir, kSwordHeight);//武器の長さ
	swordDir = VAdd(rightHand, swordDir);//持ち手の座標に加算して剣先の座標を出す
	//武器の持ち手をセット
	m_rightSword->GetRb()->SetPos(Position3(rightHand.x, rightHand.y, rightHand.z));
	//武器の剣先をセット
	std::dynamic_pointer_cast<CapsuleCollider>(m_rightSword->GetColl())->SetEndPos(Position3(swordDir.x, swordDir.y, swordDir.z));
}

void Player::HurtPointUpdate()
{
	//移動量を取得
	m_hurtPoint->GetCollidable()->GetRb()->SetVec(m_collidable->GetRb()->GetVec());
	//座標更新
	m_hurtPoint->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->SetEndPos(std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos());
}
