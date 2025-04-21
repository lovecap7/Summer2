#include "Camera.h"
#include "../../General/Rigidbody.h"
#include <DxLib.h>

Camera::Camera(Position3 firstPos, Position3 target):
	m_pos(firstPos)
{
	//奥行50～3000までをカメラの描画範囲とする
	SetCameraNearFar(50.0f, 3000.0f);

	//カメラの座標と注視点
	SetCameraPositionAndTarget_UpVecY(VGet(m_pos.x, m_pos.y, m_pos.z), VGet(target.x, target.y, target.z));

	// Ｘ軸のマイナス方向のディレクショナルライトに変更
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
}

Camera::~Camera()
{
}

void Camera::Update(Position3 target)
{
	//カメラの座標と注視点
	SetCameraPositionAndTarget_UpVecY(VGet(target.x, m_pos.y, m_pos.z), VGet(target.x, target.y, target.z));
}
