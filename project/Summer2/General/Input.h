#pragma once
#include<string>
#include <memory>
#include <vector>
#include <map>


using namespace std;

class Input
{
public:
	Input(int padID);
	~Input();
	/// <summary>
	/// ���͏����X�V����(���t���[���Ăяo���Ă�������)
	/// </summary>
	void Update();
	/// <summary>
	/// ���̓`�F�b�N(�������)
	/// </summary>
	/// <param name="action">���ׂ����A�N�V�����̖��O</param>
	/// <returns>true ������Ă��� / false ������ĂȂ�</returns>
	bool IsPressed(const string& action)const;
	/// <summary>
	/// ���̓`�F�b�N(�g���K�[���[�������u��])
	/// </summary>
	/// <param name="action">���ׂ����A�N�V�����̖��O</param>
	/// <returns>true �������ꂽ / false ������Ă��Ȃ����������ςȂ�</returns>
	bool IsTriggered(const string& action)const;

	/// <summary>
	/// �X�e�B�b�N�̌X�����`�F�b�N����̂Ɏg��
	/// X�����i-1000�`1000�j
	/// Y�����i-1000�`1000�j
	/// </summary>
	struct StickInfo
	{
		/// <summary>
		/// ��(Max -1000)
		/// �E(Max  1000)
		/// </summary>
		int leftStickX;
		/// <summary>
		/// ��(Max -1000)
		/// ��(Max  1000)
		/// </summary>
		int leftStickY;
		/// <summary>
		/// ��(Max -1000)
		/// �E(Max  1000)
		/// </summary>
		int rightStickX;
		/// <summary>
		/// ��(Max -1000)
		/// ��(Max  1000)
		/// </summary>
		int rightStickY;
		StickInfo()
		{
			leftStickX = 0;
			leftStickY = 0;
			rightStickX = 0;
			rightStickY = 0;
		}
	};
	/// <summary>
	/// �X�e�B�b�N�̌X�����擾
	/// </summary>
	/// <returns>�X�e�B�b�N���ǂꂾ���|��Ă��邩</returns>
	StickInfo GetStickInfo() const { return m_stickInfo; }

	/// <summary>
	/// ���������|���Ă���Ȃ�true
	/// </summary>
	/// <returns></returns>
	bool IsLowPowerLeftStick();
	/// <summary>
	/// �^�񒆂��炢�œ|������true
	/// </summary>
	/// <returns></returns>
	bool IsMediumPowerLeftStick();
	/// <summary>
	/// �ő�܂œ|������
	/// </summary>
	/// <returns></returns>
	bool IsHighPowerLeftStick();

private:
	/// <summary>
	/// ���͑Ή��}�b�v�Ɏg�p����
	/// </summary>
	unsigned int buttonID;
	//�����̃p�b�h��ID
	int m_padID;


	//�Q�[���p�b�h����̃e�[�u��
	using GamePadActionMap_t = map<string, vector<unsigned int>>;
	GamePadActionMap_t m_gamePadActionMap;
	map<string, bool> m_currentInput;//���݂̉������
	map<string, bool> m_lastInput;//���O�̉������

	//�X�e�B�b�N
	StickInfo m_stickInfo;
};

