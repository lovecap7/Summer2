#pragma once
#include <DxLib.h>
#include <memory>

class Animator;
class Model
{
public:
	Model(int modelHandle, VECTOR pos);
	virtual ~Model();
	void Update();
	void Draw()const;
	void SetPos(VECTOR pos);
	void SetScale(VECTOR pos);
	void SetDir(VECTOR vec);
	//�A�j���[�V�����֌W
	void SetAnim(const char* animName,bool isLoop);//�A�j���[�V�����ύX
	bool IsFinishAnim();//�A�j���[�V�������I��������ǂ���
	float GetTatalAnimFrame();//�S�̃t���[��
	float GetNowAnimFrame();//���݂̃A�j���[�V�����t���[��
private:
	int m_modelHandle;
	//�A�j���[�V����
	std::unique_ptr<Animator> m_animator;
};

