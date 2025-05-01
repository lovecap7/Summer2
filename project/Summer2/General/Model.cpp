#include "Model.h"
#include "Math/MyMath.h"
#include "Animator.h"

Model::Model(int modelHandle, VECTOR pos):
	m_modelHandle(modelHandle)
{
	//���W
	MV1SetPosition(m_modelHandle, pos);
	//�A�j���[�V����
	m_animator = std::make_unique<Animator>();
}

Model::~Model()
{
}

void Model::Update()
{
	//�A�j���[�V�����̍X�V
	m_animator->PlayAnim(m_modelHandle);
}

void Model::Draw() const
{
	//�`��
	MV1DrawModel(m_modelHandle);
}

void Model::SetPos(VECTOR pos)
{
	//���W
	MV1SetPosition(m_modelHandle, pos);
}

void Model::SetScale(VECTOR pos)
{
	//�傫��
	MV1SetScale(m_modelHandle, pos);
}

void Model::SetDir(VECTOR vec)
{
	//���������߂��Ȃ��̂Ń��^�[��
	if (vec.x == 0.0f && vec.z == 0.0f)return;
	//�������v�Z
	Vector2 z = { 0.0f,-1.0f };
	Vector2 dir = { vec.x,vec.z };
	dir = dir.Normalize();
	float angle = Theata(z, dir);
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, angle, 0.0f));
}

void Model::SetAnim(const char* animName, bool isLoop)
{
	//�A�j���[�V������ύX
	m_animator->SetAnim(m_modelHandle, MV1GetAnimIndex(m_modelHandle, animName), isLoop);
}

bool Model::IsFinishAnim()
{
	//�A�j���[�V�������I��������ǂ���
	return m_animator->IsFinishAnim();
}

void Model::RemoveAnim()
{
	//�폜
	m_animator->RemoveAnim(m_modelHandle);
}
