#include "Model.h"
#include "Math/MyMath.h"
#include "Animator.h"

Model::Model(int modelHandle, VECTOR pos):
	m_modelHandle(modelHandle)
{
	//座標
	MV1SetPosition(m_modelHandle, pos);
	//アニメーション
	m_animator = std::make_unique<Animator>();
}

Model::~Model()
{
}

void Model::Update()
{
	//アニメーションの更新
	m_animator->PlayAnim(m_modelHandle);
}

void Model::Draw() const
{
	//描画
	MV1DrawModel(m_modelHandle);
}

void Model::SetPos(VECTOR pos)
{
	//座標
	MV1SetPosition(m_modelHandle, pos);
}

void Model::SetScale(VECTOR pos)
{
	//大きさ
	MV1SetScale(m_modelHandle, pos);
}

void Model::SetDir(VECTOR vec)
{
	//向きが決められないのでリターン
	if (vec.x == 0.0f && vec.z == 0.0f)return;
	//向きを計算
	Vector2 z = { 0.0f,-1.0f };
	Vector2 dir = { vec.x,vec.z };
	dir = dir.Normalize();
	float angle = Theata(z, dir);
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, angle, 0.0f));
}

void Model::SetAnim(const char* animName, bool isLoop)
{
	//アニメーションを変更
	m_animator->SetAnim(m_modelHandle, MV1GetAnimIndex(m_modelHandle, animName), isLoop);
}

bool Model::IsFinishAnim()
{
	//アニメーションが終わったかどうか
	return m_animator->IsFinishAnim();
}

void Model::RemoveAnim()
{
	//削除
	m_animator->RemoveAnim(m_modelHandle);
}
