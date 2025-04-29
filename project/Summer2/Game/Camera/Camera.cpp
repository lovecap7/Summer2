#include "Camera.h"
#include "../../General/Rigidbody.h"
#include "../../General/Collidable.h"
#include "../Actors/Actor.h"
#include <DxLib.h>
#if _DEBUG
#include "../../General/Input.h"
#endif


Camera::Camera(Position3 firstPos, std::shared_ptr<Actor> player):
	m_pos(firstPos),
	m_target(player->GetCollidable()->GetRb()->GetPos()),
	m_player(player)
{
	//奥行50～3000までをカメラの描画範囲とする
	SetCameraNearFar(50.0f, 3000.0f);
	
	//カメラの座標と注視点
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_target.ToDxLibVector());

	//ディレクショナルライト
	ChangeLightTypeDir(VGet(0.0f, 0.0f, -1.0f));
}

Camera::~Camera()
{
}

void Camera::Update()
{
	m_target = m_player->GetCollidable()->GetRb()->GetPos();
	m_pos += m_player->GetCollidable()->GetRb()->GetVec();
	//カメラの座標と注視点
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_target.ToDxLibVector());
}

Vector3 Camera::GetDir()
{
	Vector3 dir = m_target - m_pos;
	dir = dir.Normalize();
	return dir;
}

#if _DEBUG
void Camera::RotaCamera(const Input& input)
{
	//右ステックのから取得
	float hAngle = 1.0f * input.GetStickInfo().rightStickX / 1000.0f;
	//反映
	//ターゲットをを中心に回転
	m_pos = RotateYPositionMatrix4x4(m_target, hAngle / 180.0f * DX_PI_F) * m_pos;
	//カメラの座標と注視点
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLibVector(), m_target.ToDxLibVector());
}
#endif