#include "PlayerStateBase.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "../../../General/Input.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/AttackManager.h"
#include "../../Attack/AttackBase.h"
PlayerStateBase::PlayerStateBase(std::shared_ptr<Player>  player):
	m_player(player)
{
}

PlayerStateBase::~PlayerStateBase()
{
}

void PlayerStateBase::ChangeState(std::shared_ptr<PlayerStateBase> nextState)
{
	//状態変化
	m_nextState = move(nextState);
}

Vector3 PlayerStateBase::GetForwardVec(const Input& input, const std::unique_ptr<Camera>& camera)
{
	Vector3 rV = { 0.0f,0.0f,0.0f, };
	//入力
	Vector2 stickVec = m_player->GetStickVec();
	if (stickVec.Magnitude() < 0.0f)
	{
		return rV;
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
	float stickTheata = Theata(z, stickVec.Normalize());
	//プレイヤーを中心に次の座標を回転
	Quaternion rotaQ = Quaternion::AngleAxis(cameraTheata + stickTheata, Vector3::Up());
	//ベクトルにかける(回転)
	Vector3 moveVec = Vector3{ 0.0f, 0.0f, -1.0f };
	moveVec = rotaQ * moveVec;
	moveVec.y = 0.0f; //Y軸は無視
	rV = moveVec.Normalize();
	return rV;
}
void PlayerStateBase::AppearAttack(std::shared_ptr<AttackBase> attack, const std::unique_ptr<AttackManager>& attackManager)
{
	//攻撃を入れる
	attack->Init();//状態をリセット
	attackManager->Entry(attack);//登録
}
