#include "Model.h"
#include "Math/MyMath.h"
#include "Animator.h"
#include <array>

namespace
{
	constexpr float kAnimSpeed = 0.5f;//�Đ����x
	//��]�t���[��
	constexpr int kRotaFrame = 10;
}

Model::Model(int modelHandle, VECTOR pos) :
	m_modelHandle(modelHandle),
	m_forward{0.0f,0.0f,1.0f},
	m_nextForward{0.0f,0.0f,1.0f},
	m_rotation(Quaternion::IdentityQ()),
	m_rotaFrame(0)
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

	//�����̍X�V
	if (m_rotaFrame > 0)
	{
		m_rotaFrame--;
		m_rotation = m_rotaQ * m_rotation;
		m_forward = m_rotaQ * m_forward;
		if (m_rotaQ.w < 1)
		{
			MV1SetRotationMatrix(m_modelHandle, m_rotation.GetMatrix().ToDxLibMATRIX());
		}
	}

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

void Model::SetDir(Vector2 vec)
{
	//���������߂��Ȃ��̂Ń��^�[��
	if (vec.Magnitude() <= 0.0f)	return;
	//�������v�Z
	Vector2 dir = vec;
	dir = dir.Normalize();
	if (m_nextForward.XZ() == dir)return;//�������ς��Ȃ��Ȃ�
	float angle = Vector2::Theata(m_forward.XZ(), dir);
	Vector3 axis = m_forward.Cross(dir.XZ());
	//��]�N�H�[�^�j�I���쐬
	m_rotaQ = Quaternion::AngleAxis(angle/ kRotaFrame, axis);
	m_rotaFrame = kRotaFrame;
	//���̐��ʃx�N�g�����L�^
	m_nextForward = dir.XZ();
}

Vector3 Model::GetDir()
{
	Vector3 dir = m_forward;
	if (dir.Magnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	return dir;
}
//
//void Model::DrawBoundingBox()const
//{
//	//�o�E���f�B���O�{�b�N�X
//	std::array<VECTOR, 8>bbVertices;
//	auto vmax = MV1GetFrameMaxVertexLocalPosition(m_modelHandle,0);
//	auto vmin = MV1GetFrameMinVertexLocalPosition(m_modelHandle, 0);
//
//	//�傫������
//	/*VECTOR scale = MV1GetScale(m_modelHandle);
//	vmax = VScale(vmax, VSize(scale));
//	vmin = VScale(vmin, VSize(scale));*/
//
//	bbVertices[0] = vmin;
//	bbVertices[1] = { vmax.x,vmin.y,vmin.z };
//	bbVertices[2] = { vmin.x,vmax.y,vmin.z };
//	bbVertices[3] = { vmax.x,vmax.y,vmin.z };
//	bbVertices[4] = { vmin.x,vmin.y,vmax.z };
//	bbVertices[5] = { vmax.x,vmin.y,vmax.z };
//	bbVertices[6] = { vmin.x,vmax.y,vmax.z };
//	bbVertices[7] = { vmax.x,vmax.y,vmax.z };
//	for (auto& v : bbVertices)
//	{
//		DrawSphere3D(v, 2.0f, 16, 0xffffff, 0, false);
//	}
//}

void Model::SetAnim(const char* animName, bool isLoop)
{
	//�A�j���[�V������ύX
	m_animator->SetAnim(m_modelHandle, MV1GetAnimIndex(m_modelHandle, animName), isLoop);
}

void Model::SetAnim(const char* animName, bool isLoop, const float& animSpeed)
{
	//�A�j���[�V������ύX
	m_animator->SetAnim(m_modelHandle, MV1GetAnimIndex(m_modelHandle, animName), isLoop, animSpeed);
}

void Model::SetAnimSpeed(const float& animSpeed)
{
	//�A�j���[�V�����̑��x
	m_animator->SetAnimSpeed(animSpeed);
}

bool Model::IsFinishAnim()
{
	//�A�j���[�V�������I��������ǂ���
	return m_animator->IsFinishAnim();
}

float Model::GetTotalAnimFrame()
{
	//�S�̃t���[��
	return m_animator->GetTotalAnimFrame();
}

void Model::SetTotalAnimFrame(float totalFrame)
{
	//�S�̃t���[���Z�b�g
	m_animator->SetTotalAnimFrame(totalFrame);
}

float Model::GetNowAnimFrame()
{
	//���݂̃t���[��
	return m_animator->GetAnimFrame();
}

void Model::ReplayAnim()
{
	//�A�j���[�V�������Đ�
	m_animator->Replay();
}

void Model::DeleteAnim()
{
	//�폜
	m_animator->AllRemoveAnim(m_modelHandle);
}

void Model::SetFixedLoopFrame(float loopFrame)
{
	//�w�胋�[�v
	m_animator->SetFixedLoop(loopFrame);
}

bool Model::IsFinishFixedLoop()
{
	//�w�胋�[�v�I��
	return m_animator->IsFinishFixedLoop();
}
