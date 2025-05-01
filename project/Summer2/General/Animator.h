#pragma once

class Animator
{
private:
	int m_attachAnim;//�Đ����̃A�j���[�V������
	int m_attachAnimIndex;//�A�j���[�V�����̃A�^�b�`���ꂽ�ԍ�
	float m_animTimer;//�A�j���[�V�����̃^�C�}�[
	float m_animStopTime;//�A�j���[�V�����̏I���^�C�}�[
	bool m_isLoopAnim;//���[�v����A�j���[�V�������ǂ���
	bool m_isFinishAnim;//�A�j���[�V�������I�������true
public:
	//�R���X�g���N�^
	Animator();
	//�f�X�g���N�^
	~Animator();

	/// <summary>
	/// �A�j���[�V�����̃A�^�b�`
	/// </summary>
	/// <param name="modelHandle">���f���̃n���h��</param>
	/// <param name="anim">animation enum</param>
	/// <param name="isLoop">���[�v���邩</param>
	void SetAnim(const int& modelHandle, const int& anim, const bool& isLoop);

	/// <summary>
	/// �w�肳�ꂽ�A�j���[�V�������Đ����Ȃ�폜
	/// </summary>
	/// <param name="modelHandle">���f���̃n���h��</param>
	/// <param name="anim">animation enum</param>
	void RemoveAnim(const int& modelHandle);

	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	/// <param name="modelHandle"></param>
	void PlayAnim(const int& modelHandle);

	/// <summary>
	/// �A�j���[�V�������I��������ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsFinishAnim() { return m_isFinishAnim; }
};
