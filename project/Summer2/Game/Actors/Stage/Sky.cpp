#include "Sky.h"
#include <DxLib.h>
#include "../../../General/Model.h"
#include "../../Camera/Camera.h"

namespace
{
	constexpr float kScale = 2.0f;
	constexpr float kRotaAngle = 0.01f;
}

Sky::Sky(int handle):
	Actor(ActorKind::None)//Noneにしたら当たり判定をしないので描画用のクラスになる
{
	//モデル
	m_model = std::make_shared<Model>(handle, Vector3::Zero().ToDxLibVector());
	m_model->SetScale(VGet(kScale, kScale, kScale));
}

Sky::~Sky()
{
}

void Sky::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<AttackManager>& attackManager)
{
	m_model->SetPos(camera->GetPos().ToDxLibVector());
	m_model->SetRot(VGet(0.0f, kRotaAngle, 0.0f));
}

void Sky::Draw() const
{
	m_model->Draw();
}
