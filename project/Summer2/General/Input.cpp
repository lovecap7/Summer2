#include "Input.h"
#include <DxLib.h>

Input::Input()
{
	//�A�z�z��
	m_inputActionMap["OK"] = { {InputType::Keyboard,KEY_INPUT_RETURN},{InputType::Gamepad,PAD_INPUT_R} };
	m_inputActionMap["Pause"] = { {InputType::Keyboard,KEY_INPUT_P},{InputType::Gamepad,PAD_INPUT_L} };
	m_inputActionMap["Cancel"] = { {InputType::Keyboard,KEY_INPUT_ESCAPE},{InputType::Gamepad,PAD_INPUT_Z} };
	m_inputActionMap["Up"] = { {InputType::Keyboard,KEY_INPUT_W},{InputType::Gamepad,PAD_INPUT_UP} };
	m_inputActionMap["Down"] = { {InputType::Keyboard,KEY_INPUT_S},{InputType::Gamepad,PAD_INPUT_DOWN} };
	m_inputActionMap["Left"] = { {InputType::Keyboard,KEY_INPUT_A},{InputType::Gamepad,PAD_INPUT_LEFT} };
	m_inputActionMap["Right"] = { {InputType::Keyboard,KEY_INPUT_D},{InputType::Gamepad,PAD_INPUT_RIGHT} };
	m_inputActionMap["A"] = { {InputType::Keyboard,KEY_INPUT_U},{InputType::Gamepad,PAD_INPUT_1} };
	m_inputActionMap["B"] = { {InputType::Keyboard,KEY_INPUT_H},{InputType::Gamepad,PAD_INPUT_2} };
	m_inputActionMap["X"] = { {InputType::Keyboard,KEY_INPUT_G},{InputType::Gamepad,PAD_INPUT_3} };
	m_inputActionMap["Y"] = { {InputType::Keyboard,KEY_INPUT_Y},{InputType::Gamepad,PAD_INPUT_4} };
	m_inputActionMap["LB"] = { {InputType::Keyboard,KEY_INPUT_J},{InputType::Gamepad,PAD_INPUT_5} };
	m_inputActionMap["RB"] = { {InputType::Keyboard,KEY_INPUT_L},{InputType::Gamepad,PAD_INPUT_6} };
	m_inputActionMap["MouseLeft"] = { {InputType::Mouse,MOUSE_INPUT_LEFT} };
	m_inputActionMap["MouseRight"] = { {InputType::Mouse,MOUSE_INPUT_RIGHT} };
}

Input::~Input()
{
}

void Input::Update()
{
	//�Ō�̓��͂��擾(IsTriggered�Ŏg��)
	m_lastInput = m_currentInput;
	//�S�Ă̓��͂��擾����
	char keyState[256] = {};
	int padState = {};
	int mouseState = {};
	//�`�F�b�N
	GetHitKeyStateAll(keyState);
	padState = GetJoypadInputState(DX_INPUT_PAD1);
	mouseState = GetMouseInput();
	//���ꂼ��̃{�^�����Ɋ��蓖�����Ă���S�Ă̓��͂��`�F�b�N
	for (const auto& mapInfo : m_inputActionMap)
	{
		bool isPressed = false;
		for (const auto& inputInfo : mapInfo.second)
		{
			//���͂�����Ȃ�true
			isPressed = (inputInfo.type == InputType::Keyboard && keyState[inputInfo.buttonID]) ||
				(inputInfo.type == InputType::Gamepad && padState & inputInfo.buttonID) ||
				(inputInfo.type == InputType::Mouse && mouseState & inputInfo.buttonID);
			//�q�b�g�����烋�[�v�𔲂���
			if (isPressed)
			{
				break;
			}
		}
		//���݂̃{�^���̏�Ԃ����Ă���
		m_currentInput[mapInfo.first] = isPressed;
	}
}

//�����Ă��邩
bool Input::IsPressed(const string& action) const
{
	//�{�^�����Ɠ������̂���`����Ă��邩�`�F�b�N
	auto keyInfo = m_currentInput.find(action);
	if (keyInfo == m_currentInput.end())//����`�̃{�^�����������疳������false��Ԃ�
	{
		return false;
	}
	else
	{
		//�������炻�̃{�^���̏�Ԃ�Ԃ�
		return keyInfo->second;
	}
}

//�������u��
bool Input::IsTriggered(const string& action) const
{
	//�����Ă邩���`�F�b�N
	if (IsPressed(action))
	{
		//�{�^�����Ɠ������̂���`����Ă��邩�`�F�b�N
		auto lastKeyInfo = m_lastInput.find(action);
		if (lastKeyInfo == m_lastInput.end())//����`�̃{�^�����������疳������false��Ԃ�
		{
			return false;
		}
		else
		{
			//�O�̃t���[����false�Ȃ�true��Ԃ�
			return !lastKeyInfo->second;
		}
	}
	else
	{
		return false;
	}
}
