#include "Animator.h"
#include <DxLib.h>

Animator::Animator():
	m_attachAnim(-1),
	m_attachAnimIndex(-1),
	m_animStopTime(0.0f),
	m_animTimer(0.0f),
	m_isLoopAnim(false),
	m_isFinishAnim(false)
{
}

Animator::~Animator()
{
}

void Animator::SetAnim(const int& modelHandle, const int& anim, const bool& isLoop)
{
	//���ݍĐ����̃A�j���[�V�������w�肳��Ă��Đݒ肵�Ȃ�
	if (anim == m_attachAnim)return;

	//���̃��[�V����������
	RemoveAnim(modelHandle);

	//�V�������[�V�����ɍX�V
	m_attachAnimIndex = MV1AttachAnim(modelHandle, anim, -1, false);

	m_attachAnim = anim;//���̃A�j���[�V�����̔ԍ�
	m_animStopTime = MV1GetAttachAnimTotalTime(modelHandle, m_attachAnimIndex);//�A�j���[�V�����̏I������
	m_animTimer = 0.0f;//�^�C�}�[������
	m_isLoopAnim = isLoop;//���[�v���邩

}

void Animator::RemoveAnim(const int& modelHandle)
{
	//�������������A�^�b�`����Ă��Ȃ��Ȃ瑁�����^�[��
	if (m_attachAnimIndex == -1)return;
	
	//���̃��[�V����������
	MV1DetachAnim(modelHandle, m_attachAnimIndex);
	m_attachAnim = -1;
	m_attachAnimIndex = -1;
	m_animStopTime = 0.0f;
	m_animTimer = 0.0f;
	m_isLoopAnim = false;
}

void Animator::PlayAnim(const int& modelHandle)
{
	//�����A�^�b�`����ĂȂ��Ȃ�Đ����Ȃ�
	if (m_attachAnimIndex == -1)return;

	//�A�j���[�V�������I����Ă��Ȃ���������Ȃ��̂�false
	m_isFinishAnim = false;
	//�A�j���[�V�����̏I���܂ōĐ�
	if (m_animStopTime <= m_animTimer)
	{
		if (m_isLoopAnim)
		{
			//���[�v����
			m_animTimer = 0.0f;
		}
		else
		{
			//�I�������true
			m_isFinishAnim = true;
		}
	}
	//�A�j���[�V������i�߂�
	MV1SetAttachAnimTime(modelHandle, m_attachAnimIndex, m_animTimer);
	
	m_animTimer += 0.5f;
}
