#pragma once
//�X�}�[�g�|�C���^���g��
#include<memory>
class SceneBase;
class Input;
class ScoreManager;
/// <summary>
/// �e�V�[�����Ǘ�����N���X
/// �Ǘ��͂��邪�A�@�ۂ��Ǘ��͂��Ȃ�(�@�ۂ̓N���X�ǂ������)
/// �C���^�[�t�F�C�X��񋟂���N���X
/// </summary>
class SceneController
{
public:
	SceneController();
	/// <summary>
	/// Application����Ă΂��֐��B���݂̃V�[����Update�����̂܂܎��s�B
	/// </summary>
	/// <param name="input"></param>
	void Update(Input& input);
	/// <summary>
	/// Application����Ă΂��֐��B���݂̃V�[����Draw�����̂܂܎��s�B
	/// </summary>
	/// <param name="input"></param>
	void Draw();
	/// <summary>
	/// ���̏�Ԃ��Z�b�g����(�Z�b�g����̂͊e��Ԃ̖���)
	/// �e�V�[�����R���g���[���[�����Đ؂�ւ�������
	/// </summary>
	/// <param name="">���̏�Ԃ̃|�C���^</param>
	void ChangeScene(std::shared_ptr<SceneBase>);

	/// <summary>
	/// �X�R�A�}�l�[�W���[�̎Q��
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<ScoreManager>& GetScoreManager() { return m_scoreManager; }
private:
	//���ݎ��s���̃V�[��(���������͒m��Ȃ�)
	std::shared_ptr<SceneBase> m_scene;

private:
	//�X�R�A(�e�V�[���Ŏg���̂�)
	std::shared_ptr<ScoreManager> m_scoreManager;
};

