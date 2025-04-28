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
	SetCameraPositionAndTarget_UpVecY(VGet(m_pos.x, m_pos.y, m_pos.z), VGet(m_target.x, m_target.y, m_target.z));

	//ディレクショナルライト
	ChangeLightTypeDir(VGet(0.0f, 0.0f, -1.0f));

#if _DEBUG
	m_cameraHAngle = 0.0f;
	m_cameraVAngle = -20.0f;
#endif
}

Camera::~Camera()
{
}

void Camera::Update()
{
	m_target = m_player->GetCollidable()->GetRb()->GetPos();
	m_pos += m_player->GetCollidable()->GetRb()->GetVec();
	//カメラの座標と注視点
	SetCameraPositionAndTarget_UpVecY(VGet(m_pos.x, m_pos.y, m_pos.z), VGet(m_target.x, m_target.y, m_target.z));
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
	
}
#endif