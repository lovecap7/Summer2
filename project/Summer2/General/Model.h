#pragma once
#include <DxLib.h>
#include <memory>
#include "Math/Vector3.h"

class Animator;
class Model
{
public:
	Model(int modelHandle, VECTOR pos);
	virtual ~Model();
	int GetModelHandle() { return m_modelHandle; };
	void Update();
	void Draw()const;
	void SetPos(VECTOR pos);
	void SetScale(VECTOR pos);
	void SetDir(VECTOR vec);
	Vector3 GetDir();
	//�A�j���[�V�����֌W
	void SetAnim(const char* animName,bool isLoop);//�A�j���[�V�����ύX
	void SetAnim(const char* animName,bool isLoop,const float animSpeed);//�A�j���[�V�����ύX(�Đ����x��)
	bool IsFinishAnim();//�A�j���[�V�������I��������ǂ���
	float GetTotalAnimFrame();//�S�̃t���[��
	void SetTotalAnimFrame(float totalFrame);//�S�̃t���[���Z�b�g
	float GetNowAnimFrame();//���݂̃A�j���[�V�����t���[��
	void ReplayAnim();//�A�j���[�V�������ŏ�����Đ�
	void DeleteAnim();//�Đ����̃A�j���[�V�������폜
	void SetFixedLoopFrame(float loopFrame);//�w�胋�[�v�Đ�
	bool IsFinishFixedLoop();//�w�胋�[�v�Đ��I��
private:
	int m_modelHandle;
	//�A�j���[�V����
	std::unique_ptr<Animator> m_animator;
	//����
	Vector3 m_dir;
};

