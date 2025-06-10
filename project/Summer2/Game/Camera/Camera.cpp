#include "Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collidable.h"
#include "../../General/game.h"
#include "../Actors/Actor.h"
#include <DxLib.h>
#if _DEBUG
#include "../../General/Input.h"
#endif

namespace
{
	//画面中央からある一定距離プレイヤーが離れた場合追従する範囲
	constexpr float kViewPlayerLimitRadius = 70.0f;
	//lerpの割合
	constexpr float kLerpRate = 0.05f;
}


Camera::Camera(Position3 firstPos, std::shared_ptr<Actor> player):
	m_pos(firstPos),
	m_player(player)
{
	//奥行50～3000までをカメラの描画範囲とする
	SetCameraNearFar(50.0f, 3000.0f);
	//カメラの位置と角度の設定
	m_pos.x = m_player->GetCollidable()->GetRb()->GetPos().x;//プレイヤーと横方向にを合わせる
	//カメラの角度
	m_dir = Matrix4x4::RotateXMat4x4(-40.0f / 180.0f * DX_PI_F) *
		Matrix4x4::RotateYMat4x4(180.0f / 180.0f * DX_PI_F) *
		Vector3::Forward();
	if (m_dir.Magnitude() > 0.0f)
	{
		m_dir = m_dir.Normalize();
	}
	//見てる位置
	m_viewPos = m_pos + m_dir;
	//カメラの座標と注視点
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
	//視野角
	SetupCamera_Perspective(50.0f / 180.0f * DX_PI_F);
	//ディレクショナルライト
	ChangeLightTypeDir(VGet(0.0f, 0.5f, -1.0f));
}

Camera::~Camera()
{
}

void Camera::Update()
{
	//プレイヤーがカメラの特定の範囲外に出ようとした際に移動
	auto playerPos = m_player->GetCollidable()->GetRb()->GetPos();
	//位置の更新
	Vector3 nextPos = m_pos;
	//範囲外なら
	if (playerPos.x > m_pos.x + kViewPlayerLimitRadius)//右
	{
		nextPos.x = playerPos.x;
		nextPos.x -= kViewPlayerLimitRadius;
	}
	else if (playerPos.x < m_pos.x - kViewPlayerLimitRadius)//左
	{
		nextPos.x = playerPos.x;
		nextPos.x += kViewPlayerLimitRadius;
	}
	//次の座標
	m_pos = Vector3::Lerp(m_pos, nextPos, kLerpRate);
	//向きの更新
	m_dir = Matrix4x4::RotateXMat4x4(-40.0f / 180.0f * DX_PI_F) *
		Matrix4x4::RotateYMat4x4(180.0f / 180.0f * DX_PI_F) *
		Vector3::Forward();
	if (m_dir.Magnitude() > 0.0f)
	{
		m_dir = m_dir.Normalize();
	}
	//見てる位置
	m_viewPos = m_pos + m_dir;
	//設定
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_viewPos.ToDxLibVector());
}

Vector3 Camera::GetDir()
{
	Vector3 dir = m_dir;
	if (dir.Magnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	return dir;
}