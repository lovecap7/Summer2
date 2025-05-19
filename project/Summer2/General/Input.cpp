#include "Input.h"
#include <DxLib.h>
#include "Math/Vector3.h"

namespace
{
	//���������|���Ă���Ɣ��肷��͈�
	constexpr float kLowPowerStickMin = 50.0f;
	constexpr float kLowPowerStickMax = 400.0f;
	//���������|���Ă���Ɣ��肷��͈�
	constexpr float kMediumPowerStickMin = kLowPowerStickMax;
	constexpr float kMediumPowerStickMax = 800.0f;
	//�ő�܂œ|���Ă���Ɣ��肷��͈�
	constexpr float kHighPowerStickMin = kMediumPowerStickMax;
	constexpr float kHighPowerStickMax = 1000.0f;
}

Input::Input(int padID):
	m_padID(std::move(padID))
{
	//�Q�[���p�b�h�̘A�z�z��
	m_gamePadActionMap["Start"] = { PAD_INPUT_R };
	m_gamePadActionMap["Pause"] = { PAD_INPUT_L };
	m_gamePadActionMap["Cancel"] = { PAD_INPUT_Z };
	m_gamePadActionMap["Up"] = { PAD_INPUT_UP };
	m_gamePadActionMap["Down"] = { PAD_INPUT_DOWN };
	m_gamePadActionMap["Left"] = { PAD_INPUT_LEFT };
	m_gamePadActionMap["Right"] = { PAD_INPUT_RIGHT };
	m_gamePadActionMap["A"] = { PAD_INPUT_1 };
	m_gamePadActionMap["B"] = { PAD_INPUT_2 };
	m_gamePadActionMap["X"] = { PAD_INPUT_3 };
	m_gamePadActionMap["Y"] = { PAD_INPUT_4 };
	m_gamePadActionMap["LB"] = { PAD_INPUT_5 };
	m_gamePadActionMap["RB"] = { PAD_INPUT_6 };

}

Input::~Input()
{
}

void Input::Update()
{
	//�Ō�̓��͂��擾(IsTriggered�Ŏg��)
	m_lastInput = m_currentInput;
	//�S�Ă̓��͂��擾����
	int padState = {};
	//�`�F�b�N
	padState = GetJoypadInputState(m_padID);
	//���ꂼ��̃{�^�����Ɋ��蓖�����Ă���S�Ă̓��͂��`�F�b�N
	for (const auto& keyInfo : m_gamePadActionMap)
	{
		bool isPressed = false;
		for (const auto& inputInfo : keyInfo.second)
		{
			//���͂�����Ȃ�true
			isPressed = (padState & inputInfo);
			//�q�b�g�����烋�[�v�𔲂���
			if (isPressed)
			{
				break;
			}
		}
		//���݂̃{�^���̏�Ԃ����Ă���
		m_currentInput[keyInfo.first] = isPressed;
	}

	//�X�e�B�b�N�̓��͂�������
	m_stickInfo.leftStickX = 0;
	m_stickInfo.leftStickY  = 0;
	m_stickInfo.rightStickX  = 0;
	m_stickInfo.rightStickY = 0;
	
	//�X�e�B�b�N�̓��͂��`�F�b�N����
	//���X�e�B�b�N
	DxLib::GetJoypadAnalogInput(&m_stickInfo.leftStickX, &m_stickInfo.leftStickY, m_padID);
	//�E�X�e�B�b�N
	DxLib::GetJoypadAnalogInputRight(&m_stickInfo.rightStickX, &m_stickInfo.rightStickY, m_padID);
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

bool Input::IsLowPowerLeftStick()
{
	//���������|���Ă���Ȃ�true
	if ((m_stickInfo.leftStickX > kLowPowerStickMin && m_stickInfo.leftStickX <= kLowPowerStickMax) ||
		(m_stickInfo.leftStickX < -kLowPowerStickMin && m_stickInfo.leftStickX >= -kLowPowerStickMax) || 
		(m_stickInfo.leftStickY > kLowPowerStickMin && m_stickInfo.leftStickY <= kLowPowerStickMax) ||
		(m_stickInfo.leftStickY < -kLowPowerStickMin && m_stickInfo.leftStickY >= -kLowPowerStickMax))
	{
		return true;
	}
	return false;
}

bool Input::IsMediumPowerLeftStick()
{
	//���������|���Ă���Ȃ�true
	if ((m_stickInfo.leftStickX > kMediumPowerStickMin && m_stickInfo.leftStickX <= kMediumPowerStickMax) ||
		(m_stickInfo.leftStickX < -kMediumPowerStickMin && m_stickInfo.leftStickX >= -kMediumPowerStickMax) ||
		(m_stickInfo.leftStickY > kMediumPowerStickMin && m_stickInfo.leftStickY <= kMediumPowerStickMax) ||
		(m_stickInfo.leftStickY < -kMediumPowerStickMin && m_stickInfo.leftStickY >= -kMediumPowerStickMax))
	{
		return true;
	}
	return false;
}

bool Input::IsHighPowerLeftStick()
{
	//�|���؂��Ă�Ȃ�true
	if ((m_stickInfo.leftStickX > kHighPowerStickMin && m_stickInfo.leftStickX <= kHighPowerStickMax) ||
		(m_stickInfo.leftStickX < -kHighPowerStickMin && m_stickInfo.leftStickX >= -kHighPowerStickMax) ||
		(m_stickInfo.leftStickY > kHighPowerStickMin && m_stickInfo.leftStickY <= kHighPowerStickMax) ||
		(m_stickInfo.leftStickY < -kHighPowerStickMin && m_stickInfo.leftStickY >= -kHighPowerStickMax))
	{
		return true;
	}
	return false;
}
