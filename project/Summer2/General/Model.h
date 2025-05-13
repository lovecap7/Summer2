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
	float GetTatalAnimFrame();//�S�̃t���[��
	float GetNowAnimFrame();//���݂̃A�j���[�V�����t���[��
	void ReplayAnim();//�A�j���[�V�������ŏ�����Đ�
	void DeleteAnim();//�Đ����̃A�j���[�V�������폜
private:
	int m_modelHandle;
	//�A�j���[�V����
	std::unique_ptr<Animator> m_animator;
	//����
	Vector3 m_dir;
};

