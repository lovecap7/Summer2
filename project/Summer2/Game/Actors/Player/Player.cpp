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
	//先行入力フレーム
	constexpr float kAdvanceInput = 15.0f;
	//強攻撃の段階別攻撃フレーム
	constexpr float kOneChargeHighAttackFrame = 30.0f;
	constexpr float kTwoChargeHighAttackFrame = 70.0f;
	constexpr float kThreeChargeHighAttackFrame = 100.0f;
	//強攻撃の段階別アニメーションの速度
	constexpr float kOneChargeHighAttackAnimSpeed = 0.3f;
	constexpr float kTwoChargeHighAttackAnimSpeed = 0.5f;
	constexpr float kThreeChargeHighAttackAnimSpeed = 1.0f;

	//アニメーションの名前
	const char* kIdleAnim = "Player|Idle";//待機
	const char* kWalkAnim = "Player|Walk";//歩く
	const char* kRunAnim = "Player|Run";//走る
	const char* kRollingAnim = "Player|Rolling";//回避
	const char* kJump1Anim = "Player|Jump1";//ジャンプ1
	const char* kJump2Anim = "Player|Jump2";//ジャンプ2
	const char* kAttack_L1Anim = "Player|Attack_L1";//弱攻撃1
	const char* kAttack_L2Anim = "Player|Attack_L2";//弱攻撃2
	const char* kAttack_L3Anim = "Player|Attack_L3";//弱攻撃3
	const char* kAttack_H1Anim = "Player|Attack_H1";//強攻撃1
	const char* kAttack_H2Anim = "Player|Attack_H2";//強攻撃2
}

Player::Player(int modelHandle, Position3 firstPos) :
	Actor(ActorKind::Player),
	m_stickVec(0.0f,0.0f),
	m_update(&Player::IdleUpdate),
	m_lastUpdate(&Player::FallUpdate),
	m_isGround(true),
	m_jumpNum(0),
	m_nextJumpFrame(kNextJumpFrame),
	m_isNextAttackInput(false),
	m_chargeHighAttackFrame(0)
{
	//初期位置
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //カプセルの上端
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));
	//モデル
	m_model = std::make_unique<Model>(modelHandle, firstPos.ToDxLibVector());
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
	//アニメーションの更新
	m_model->Update();
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
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		m_isGround
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

void Player::IdleUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
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
		//弱攻撃
		m_update = &Player::AttackLight1Update;
		return;
	}
	//強攻撃ボタンを押したら
	if (input.IsTriggered("Y"))
	{
		//強攻撃
		m_update = &Player::AttackHigh1Update;
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

void Player::MoveUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
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
		m_update = &Player::AttackLight1Update;
		return;
	}
	//強攻撃ボタンを押したら
	if (input.IsTriggered("Y"))
	{
		//強攻撃
		m_update = &Player::AttackHigh1Update;
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

void Player::JumpUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
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
	if (m_nextJumpFrame <= 0)
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
		speed = ClampFloat(speed, 0.0f, kAirMoveSpeed);
		m_collidable->GetRb()->SetMoveVec(m_collidable->GetRb()->GetMoveVec().Normalize() * speed);
	}

	//少しずつ減速する
	SpeedDown();
	//向きの更新
	m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
}

void Player::FallUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
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
	//少しずつ減速する
	SpeedDown();
	//向きの更新
	m_model->SetDir(m_collidable->GetRb()->GetVec().ToDxLibVector());
}

void Player::AttackLight1Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//モデルのアニメーションが終わったら
	if (m_model->IsFinishAnim())
	{
		//攻撃の入力があるなら
		if (m_isNextAttackInput)
		{
			//2段目
			m_update = &Player::AttackLight2Update;
			return;
		}
		else
		{
			//待機
			m_update = &Player::IdleUpdate;
			return;
		}
	}
	//アニメーションのラスト数フレーム以内で入力があるなら2段回目の攻撃
	if (m_model->GetTatalAnimFrame() - kAdvanceInput <= m_model->GetNowAnimFrame())
	{
		if (input.IsTriggered("X"))
		{
			m_isNextAttackInput = true;
		}
	}

	//少しずつ減速する
	SpeedDown();
}

void Player::AttackLight2Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
	//モデルのアニメーションが終わったら
	if (m_model->IsFinishAnim())
	{
		//攻撃の入力があるなら
		if (m_isNextAttackInput)
		{
			//3段目
			m_update = &Player::AttackLight3Update;
			return;
		}
		else
		{
			//待機
			m_update = &Player::IdleUpdate;
			return;
		}
	}
	//アニメーションのラスト数フレーム以内で入力があるなら2段回目の攻撃
	if (m_model->GetTatalAnimFrame() - kAdvanceInput <= m_model->GetNowAnimFrame())
	{
		if (input.IsTriggered("X"))
		{
			m_isNextAttackInput = true;
		}
	}

	//少しずつ減速する
	SpeedDown();
}
void Player::AttackLight3Update(const Input& input, const std::unique_ptr<Camera>& camera)
{
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
void Player::AttackHigh1Update(const Input& input, const std::unique_ptr<Camera>& camera)
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
		if (m_chargeHighAttackFrame >= kThreeChargeHighAttackFrame)
		{
			m_chargeHighAttackFrame = kThreeChargeHighAttackFrame;
		}
	}
	//ボタンを離す
	else
	{
		//タメ攻撃
		m_update = &Player::AttackHigh2Update;
		return;
	}
}

void Player::AttackHigh2Update(const Input& input, const std::unique_ptr<Camera>& camera)
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
void Player::RollingUpdate(const Input& input, const std::unique_ptr<Camera>& camera)
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
		if (m_lastUpdate == &Player::FallUpdate)
		{
			//2回目ジャンプ
			m_model->SetAnim(kJump2Anim, false);
		}
		else
		{
			//1回目ジャンプ
			m_model->SetAnim(kJump1Anim, true);
		}
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
		//ジャンプ以外でこの状態になったなら
		if (m_lastUpdate != &Player::JumpUpdate)
		{
			//落下
			m_model->SetAnim(kJump1Anim, true);
		}
		//落下してるので
		m_isGround = false;
		//ジャンプカウントは落下状態になってからカウントを進める
		++m_jumpNum;
		m_collidable->SetState(State::Fall);
	}
	else if (m_update == &Player::AttackLight1Update)
	{
		m_collidable->SetState(State::None);
		//弱攻撃1
		m_model->SetAnim(kAttack_L1Anim, false);
		//向きの更新
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//先行入力の準備
		m_isNextAttackInput = false;
	}
	else if (m_update == &Player::AttackLight2Update)
	{
		m_collidable->SetState(State::None);
		//弱攻撃2
		m_model->SetAnim(kAttack_L2Anim, false);
		//向きの更新
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//先行入力の準備
		m_isNextAttackInput = false;
	}
	else if (m_update == &Player::AttackLight3Update)
	{
		m_collidable->SetState(State::None);
		//弱攻撃3
		m_model->SetAnim(kAttack_L3Anim, false);
		//向きの更新
		m_model->SetDir(VGet(m_stickVec.x, 0.0f, m_stickVec.y));
		//先行入力の準備
		m_isNextAttackInput = false;
	}
	else if (m_update == &Player::AttackHigh1Update)
	{
		m_collidable->SetState(State::None);
		//強攻撃1
		m_model->SetAnim(kAttack_H1Anim, true);
	}
	else if (m_update == &Player::AttackHigh2Update)
	{
		m_collidable->SetState(State::None);
		//チャージ時間に合わせて持続させる
		//1段回目
		if (m_chargeHighAttackFrame <= kOneChargeHighAttackFrame)
		{
			//強攻撃2
			m_model->SetAnim(kAttack_H2Anim, true, kOneChargeHighAttackAnimSpeed);
			m_chargeHighAttackFrame = kOneChargeHighAttackFrame;
		}
		//2段回目
		else if (m_chargeHighAttackFrame <= kTwoChargeHighAttackFrame)
		{
			//強攻撃2
			m_model->SetAnim(kAttack_H2Anim, true, kTwoChargeHighAttackAnimSpeed);
			m_chargeHighAttackFrame = kTwoChargeHighAttackFrame;
		}
		//3段回目
		else if (m_chargeHighAttackFrame <= kThreeChargeHighAttackFrame)
		{
			//強攻撃2
			m_model->SetAnim(kAttack_H2Anim, true, kThreeChargeHighAttackAnimSpeed);
			m_chargeHighAttackFrame = kThreeChargeHighAttackFrame;
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
