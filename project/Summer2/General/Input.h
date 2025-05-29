#pragma once
#include <map>
#include <string>
#include <vector>
#include <list>

class Input
{
	/*/// <summary>
	/// �V���O���g��
	/// </summary>
private:
	Input() = default;
	~Input();
	Input(const Input&) = delete;
	Input& operator = (const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator = (Input&&) = delete;*/

	//�L�[�̕����L�^
	void DirInfoSave();

public:
	/*static Input& GetInstance()
	{
		static Input instance;
		return instance;
	}*/

	/// <summary>
	/// �X�e�B�b�N�̌X����ێ�����\����
	/// </summary>
	struct StickInfo
	{
		StickInfo()
		{
			leftStickX = 0;
			leftStickY = 0;
			rightStickX = 0;
			rightStickY = 0;
		}
		int leftStickX;
		int leftStickY;
		int rightStickX;
		int rightStickY;
	};

	struct TriggerInfo
	{
		TriggerInfo()
		{
			left = 0;
			right = 0;
		}
		int left;
		int right;
	};
	~Input();
	/// <summary>
	/// �L�[�̏����X�V����
	/// </summary>
	void Init();

	/// <summary>
	/// �����p�b�h�̃i���o�[
	/// </summary>
	/// <param name="padIndex"></param>
	void PadInit(int padIndex);

	/// <summary>
	/// ���͏����X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// ���͏����󂯕t���Ȃ��悤�ɂ���
	/// </summary>
	void StopUpdate();

	/// <summary>
	/// �L�[���������ǂ���
	/// </summary>
	/// <param name="keyName">���肵�����L�[</param>
	// <returns>���������ǂ���</returns>
	bool IsPress(const std::string& action)const;


	/// <summary>
	/// �L�[�������ꂽ�u�Ԃ��擾����
	/// </summary>
	/// <param name="keyName">���肵�����L�[</param>
	/// <returns>�����ꂽ�u�Ԃ̂�true</returns>
	bool IsTrigger(const std::string& action)const;

	/// <summary>
	/// �L�[�𗣂����u�Ԃ��擾����
	/// </summary>
	/// <param name="keyName">���肵�����L�[</param>
	/// <returns>�����ꂽ�u�Ԃ̂�true</returns>
	bool IsRelease(const std::string& action)const;

	/// <summary>
	/// �X�e�B�b�N�̌X�����擾����
	/// </summary>
	/// <returns>�X�e�B�b�N���ǂꂾ���|��Ă��邩</returns>
	StickInfo GetStickInfo() { return m_stickInfo; }

	/// <summary>
	/// �g���K�[�̉������݋���擾����
	/// </summary>
	/// <returns>�g���K�[���ǂ̂��炢�������܂�Ă��邩</returns>
	TriggerInfo GetTriggerInfo() { return m_triggerInfo; }

	/// <summary>
	/// �g���K�[�{�^���������Ă��邩�ǂ�����Ԃ�(�������݂̋�����)
	/// </summary>
	/// <param name="right">�E���̃{�^�����擾�������Ȃ�true</param>
	/// <param name="power">�ǂ̂��炢�������܂�Ă�����true�Ƃ��邩(�ő�255,�ŏ�0)</param>
	/// <returns></returns>
	bool IsPushTrigger(bool right, int power);

	/// <summary>
	/// �g���K�[�{�^���������Ă��邩�ǂ�����Ԃ�(�������݂̋����Œ�)
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	bool IsPushTrigger(bool right);

	

	/// <summary>
	/// �R�}���h���`�F�b�N����
	/// </summary>
	bool CheckDirCommand(std::string command);

	//�^���R�}���h
	bool CheckKeepCommand(std::string command);
	
private:
	/// <summary>
	/// ���͂̎��
	/// </summary>
	enum class InputType
	{
		kKeyboard,
		kPad,
		kMouse
	};
	struct InputMapInfo
	{
		InputType type;
		int buttonID;
	};

	std::map<std::string, std::vector<InputMapInfo>> m_inputActionMap;

	//���݂̃t���[���̓���
	std::map<std::string, bool>m_currentInput;
	//1�t���[���O�̓���
	std::map<std::string, bool>m_lastInput;

	StickInfo m_stickInfo = StickInfo();

	TriggerInfo m_triggerInfo = TriggerInfo();
	//�X�e�B�b�N�̕���
	enum class StickDir
	{
		Command,	//�e���L�[�\������������0��K���ɗp�ӂ��Ă݂�
		LeftDown,	//1
		Down,		//2
		RightDown,	//3
		Left,		//4
		Neutral,	//5
		Right,		//6
		LeftUp,		//7
		Up,			//8
		RightUp,	//9
	};
	//�X�e�B�b�N�̕����̏������\����
	struct StickDirInfo
	{
		StickDir dir;	//�X�e�B�b�N�̕���
		int frame;		//��������ɃX�e�B�b�N��������Ă���Ԃ̃t���[��
	};
	//StickDirInfo�\���̂�list�Ŏ���
	//�����̏�������ɓ���Ă���
	std::list<StickDirInfo> m_stickDirInfo;

	//�R�}���h���X�g
	std::map < std::string, std::vector< StickDir>> m_commandList;

	//PAD��Index
	int m_padIndex;

};


