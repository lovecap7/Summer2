#pragma once
class Input;
class SceneController;

/// <summary>
/// �e�V�[���̊��N���X
/// </summary>
class SceneBase
{
protected:
	//�e�V�[���̏�Ԃ�؂�ւ��邽��
	SceneController& m_controller;
public:
	SceneBase(SceneController& controller);
	/// <summary>
	/// �����ϐ��̍X�V
	/// </summary>
	/// <param name="input"></param>
	virtual void Update(Input& input) = 0;
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;
};

