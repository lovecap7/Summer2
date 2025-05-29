#include "Dxlib.h"
#include "Input.h"
#include "game.h"
namespace
{
	constexpr float kTriggerPower = 128;

	//�X�e�B�b�N�̓��͐����̑傫��
	constexpr int kLeftStickPowerX = 20;
	constexpr int kLeftStickPowerY = 20;

	//�R�}���h�P�\�t���[��
	//�ʏ�
	constexpr int kDirInputgraceFrame = 15;
	//�^��
	constexpr int kKeepInputgraceFrame = 25;
	//�^���ɕK�v�ȃt���[��
	constexpr int kKeepFrame = 30;
}

void Input::DirInfoSave()
{
	//���݂̃X�e�B�b�N�̕�����ۑ�
//��U���݂̃X�e�B�b�N�̕�����Neutral�ŏ�����
	StickDir nowDir = StickDir::Neutral;
	//�E�����ɓ��͂������Ă���Ƃ�
	if ((m_stickInfo.leftStickX < -kLeftStickPowerX) || (kLeftStickPowerX < m_stickInfo.leftStickX) || IsPress("Left") || IsPress("Right"))
	{
		//�X�e�B�b�N����(-500)����(500)�ɓ����Ă���Ȃ�true��Neutral�Ȃ�false
		bool isY = ((m_stickInfo.leftStickY < -kLeftStickPowerY) || (kLeftStickPowerY < m_stickInfo.leftStickY) || IsPress("Up") || IsPress("Down"));
		//�X�e�B�b�N���E�ɓ����Ă���Ȃ�true�ō��Ȃ�false
		bool isRight = ((m_stickInfo.leftStickX > kLeftStickPowerX) || IsPress("Right"));
		if (isY)
		{
			//���ɓ��͂������Ă���Ȃ�true
			bool isDown = ((m_stickInfo.leftStickY > kLeftStickPowerY) || IsPress("Down"));
			//���ƉE�ɓ��͂������Ă���Ȃ�E��
			if (isDown && isRight)			 nowDir = StickDir::RightDown;
			//���ƍ��ɓ��͂������Ă���Ȃ獶��
			else if (isDown && !isRight)	 nowDir = StickDir::LeftDown;
			//��ƉE�ɓ��͂������Ă���Ȃ�E��
			else if (!isDown && isRight)	 nowDir = StickDir::RightUp;
			//��ƍ��ɓ��͂������Ă���Ȃ獶��
			else							 nowDir = StickDir::LeftUp;
		}
		//�㉺�ǂ�������͂������Ă��Ȃ��Ƃ�
		else
		{
			if (isRight)	nowDir = StickDir::Right;
			else			nowDir = StickDir::Left;
		}
	}
	//�E���ɓ��͂��Ȃ��ォ���ɓ��͂������Ă���Ƃ�
	else if ((m_stickInfo.leftStickY < -kLeftStickPowerY) || (kLeftStickPowerY < m_stickInfo.leftStickY) || IsPress("Up") || IsPress("Down"))
	{
		//���ɓ��͂������Ă���Ƃ�
		if ((m_stickInfo.leftStickY > kLeftStickPowerY) || IsPress("Down"))	nowDir = StickDir::Down;
		//��ɓ��͂������Ă���Ƃ�
		else											nowDir = StickDir::Up;
	}

	//list�̒��g����̎�	(1���)
	if (m_stickDirInfo.empty())
	{
		//list�̐擪�Ɍ��݂̃t���[���̕���������
		StickDirInfo info;
		info.frame = 1;
		info.dir = nowDir;
		m_stickDirInfo.push_front(info);
	}
	//2��ڈȍ~
	else
	{
		//�ЂƂO�̃t���[���̕����ƌ��݂̓��͂̕����������Ȃ�
		if (m_stickDirInfo.begin()->dir == nowDir)
		{
			//�t���[�����������Ă���
			m_stickDirInfo.begin()->frame++;
		}
		//�ЂƂO�̃t���[���̕����ƌ��݂̓��͂̕������Ⴄ�Ȃ�
		else
		{
			//list�̐擪�Ɍ��݂̃t���[���̕���������
			StickDirInfo info;
			info.frame = 1;
			info.dir = nowDir;
			m_stickDirInfo.push_front(info);
		}
	}



#if _DEBUG	
	//DrawFormatString(300, 32, 0xffff00, "dir:%d", m_stickDirInfo.begin()->dir);
	DxLib::DrawFormatString(580, 190, 0xffff00, "dir:%d", nowDir);

	DxLib::DrawFormatString(580, 168, 0x00ff00, "%d", StickDir::LeftDown);	//1
	DxLib::DrawFormatString(600, 168, 0x00ff00, "%d", StickDir::Down);		//2
	DxLib::DrawFormatString(620, 168, 0x00ff00, "%d", StickDir::RightDown);	//3
	DxLib::DrawFormatString(580, 142, 0x00ff00, "%d", StickDir::Left);		//4
	DxLib::DrawFormatString(600, 142, 0x00ff00, "%d", StickDir::Neutral);	//5
	DxLib::DrawFormatString(620, 142, 0x00ff00, "%d", StickDir::Right);		//6
	DxLib::DrawFormatString(580, 116, 0x00ff00, "%d", StickDir::LeftUp);	//7
	DxLib::DrawFormatString(600, 116, 0x00ff00, "%d", StickDir::Up);		//8
	DxLib::DrawFormatString(620, 116, 0x00ff00, "%d", StickDir::RightUp);	//9
#endif
}

Input::~Input()
{
}

void Input::Init()
{
	m_inputActionMap["Ok"] = { {InputType::kKeyboard,KEY_INPUT_RETURN},{InputType::kPad,PAD_INPUT_A} };
	m_inputActionMap["Pause"] = { {InputType::kKeyboard,KEY_INPUT_P}, {InputType::kPad,PAD_INPUT_START} };
	m_inputActionMap["Start"] = { {InputType::kKeyboard,KEY_INPUT_P}, {InputType::kPad,PAD_INPUT_8} };
	m_inputActionMap["Select"] = { {InputType::kKeyboard,KEY_INPUT_TAB}, {InputType::kPad,PAD_INPUT_7} };
	m_inputActionMap["Cancel"] = { {InputType::kKeyboard,KEY_INPUT_ESCAPE}, {InputType::kPad,PAD_INPUT_B} };
	m_inputActionMap["Up"] = { {InputType::kKeyboard,KEY_INPUT_W}, {InputType::kPad,PAD_INPUT_UP} };
	m_inputActionMap["Down"] = { {InputType::kKeyboard,KEY_INPUT_S}, {InputType::kPad,PAD_INPUT_DOWN} };
	m_inputActionMap["Left"] = { {InputType::kKeyboard,KEY_INPUT_A}, {InputType::kPad,PAD_INPUT_LEFT} };
	m_inputActionMap["Right"] = { {InputType::kKeyboard,KEY_INPUT_D}, {InputType::kPad,PAD_INPUT_RIGHT} };
	m_inputActionMap["LB"] = { {InputType::kKeyboard,KEY_INPUT_J}, {InputType::kPad,PAD_INPUT_5} };
	m_inputActionMap["RB"] = { {InputType::kKeyboard,KEY_INPUT_L}, {InputType::kPad,PAD_INPUT_6} };
	m_inputActionMap["A"] = { {InputType::kKeyboard,KEY_INPUT_U}, {InputType::kPad,PAD_INPUT_1} };
	m_inputActionMap["B"] = { {InputType::kKeyboard,KEY_INPUT_H}, {InputType::kPad,PAD_INPUT_2} };
	m_inputActionMap["X"] = { {InputType::kKeyboard,KEY_INPUT_G}, {InputType::kPad,PAD_INPUT_3} };
	m_inputActionMap["Y"] = { {InputType::kKeyboard,KEY_INPUT_Y}, {InputType::kPad,PAD_INPUT_4} };
	//�R�}���h���X�g(�Z��łL�����N�^�[�̌������E�������Ă���Ƃ�)
	// 7 8 9
	// 4 5 6
	// 1 2 3
	

	
	// ���͂��V�����ق�����`�F�b�N���Ă����̂Ő����R�}���h�͌�납�珑��
	//�R�}���h
	// �g����
	m_commandList["236"] = { StickDir::Right,StickDir::RightDown ,StickDir::Down };	//�E����
	m_commandList["214"] = { StickDir::Left,StickDir::LeftDown ,StickDir::Down };	//������
	//������
	m_commandList["623"] = { StickDir::RightDown,StickDir::Down, StickDir::Right };	//�E����
	m_commandList["421"] = { StickDir::LeftDown,StickDir::Down, StickDir::Left };//������
	//�����ȈՃR�}���h
	m_commandList["323"] = { StickDir::RightDown,StickDir::Down, StickDir::RightDown };	//�E����
	m_commandList["121"] = { StickDir::LeftDown,StickDir::Down, StickDir::LeftDown };//������
	m_commandList["636"] = { StickDir::Right,StickDir::RightDown, StickDir::Right };	//�E����
	m_commandList["414"] = { StickDir::Left,StickDir::LeftDown, StickDir::Left };//������

	//�^���R�}���h(K = keep)
	//���^���O
	// �\�j�b�N�u�[��
	m_commandList["4K6"] = { StickDir::Right ,StickDir::Left };	//�E����
	m_commandList["6K4"] = { StickDir::Left ,StickDir::Right };	//�������\�j�b�N�u�[��
	//���^����
	m_commandList["2K8"] = { StickDir::Up ,StickDir::Down };	
	

	//�X�N�����[
	m_commandList["RightOneRevolution"] = { StickDir::LeftUp ,StickDir::Left,StickDir::Down, StickDir::Right };	//�E����
	m_commandList["LeftOneRevolution"] = { StickDir::RightUp ,StickDir::Right,StickDir::Down, StickDir::Left };	//������

	//����]
	m_commandList["HalfTurnRightStart"] = { StickDir::Left ,StickDir::Down,StickDir::Right };	//�E����
	m_commandList["HalfTurnLeftStart"] = { StickDir::Right ,StickDir::Down,StickDir::Left };	//������

	//�p�b�h�̔ԍ�
	m_padIndex = 1;
}

void Input::PadInit(int padIndex)
{
	m_padIndex = padIndex;
}

void Input::Update()
{
	//�O�̃t���[���̓��͏���ۑ�����
	m_lastInput = m_currentInput;

	//���ׂĂ̓��͂��擾����
	char keyState[256] = {};
	int padState = {};
	int mouseState = {};
	GetHitKeyStateAll(keyState);
	if (m_padIndex == static_cast<int>(PlayerIndex::Player2))
	{
		padState = GetJoypadInputState(DX_INPUT_PAD2);
	}
	else
	{
		padState = GetJoypadInputState(DX_INPUT_PAD1);
	}
	mouseState = GetMouseInput();

	//�A�N�V�������Ɋ��蓖�Ă��Ă��邷�ׂẴL�[�̓��͂��`�F�b�N����
	for (const auto& keyInfo : m_inputActionMap)
	{
		bool isPress = false;
		for (const auto& inputInfo : keyInfo.second)
		{
			//�L�[�{�[�h�̃`�F�b�N
			if (inputInfo.type == InputType::kKeyboard && keyState[inputInfo.buttonID])
			{
				isPress = true;
			}
			//�p�b�h�̃`�F�b�N
			if (inputInfo.type == InputType::kPad && padState & inputInfo.buttonID)
			{
				isPress = true;
			}
			//�}�E�X�̃`�F�b�N
			if (inputInfo.type == InputType::kMouse && mouseState & inputInfo.buttonID)
			{
				isPress = true;
			}

			if (isPress)
			{
				break;
			}
		}
		//���݂̃t���[���ŉ�����Ă������ǂ�����Ԃ�
		m_currentInput[keyInfo.first] = isPress;
	}

	//�X�e�B�b�N�̓��͂�����������
	m_stickInfo.leftStickX = 0;
	m_stickInfo.leftStickY = 0;
	m_stickInfo.rightStickX = 0;
	m_stickInfo.rightStickY = 0;

	//�X�e�B�b�N�̓��͂��擾����
	if (m_padIndex == static_cast<int>(PlayerIndex::Player2))
	{
		GetJoypadAnalogInput(&m_stickInfo.leftStickX, &m_stickInfo.leftStickY, DX_INPUT_PAD2);
		GetJoypadAnalogInputRight(&m_stickInfo.rightStickX, &m_stickInfo.rightStickY, DX_INPUT_PAD2);
		XINPUT_STATE* xInputState = new XINPUT_STATE;
		GetJoypadXInputState(DX_INPUT_PAD2, xInputState);
		m_triggerInfo.left = xInputState->LeftTrigger;
		m_triggerInfo.right = xInputState->RightTrigger;
	}
	else
	{
		GetJoypadAnalogInput(&m_stickInfo.leftStickX, &m_stickInfo.leftStickY, DX_INPUT_PAD1);
		GetJoypadAnalogInputRight(&m_stickInfo.rightStickX, &m_stickInfo.rightStickY, DX_INPUT_PAD1);
		XINPUT_STATE* xInputState = new XINPUT_STATE;
		GetJoypadXInputState(DX_INPUT_PAD1, xInputState);
		m_triggerInfo.left = xInputState->LeftTrigger;
		m_triggerInfo.right = xInputState->RightTrigger;
	}

	//�����L�[��ۑ�
	DirInfoSave();
	
}

bool Input::CheckDirCommand(std::string command)
{
	//�t���[���𐔂��邽�߂ɗp��
	int frame = 0;
	//�R�}���h�����̕����L�[�̍ŏ���0�Ƃ���
	int index = 0;
	//����܂ł̓��͓��e�����Ă���
	for (auto& data : m_stickDirInfo)
	{
		//���͓��e�ɃR�}���h�̏����ɓ��Ă͂܂�L�[����������
		if (data.dir == m_commandList.at(command).at(index))
		{
			//auto check = m_commandList.at(command).at(index);
			//�t���[�������Z�b�g
			//frame = 0;
			//���̕����L�[���`�F�b�N���邽�߂�index��1���₷
			index++;
			//�R�}���h�̕����L�[�̏��������ׂĖ���������true��Ԃ�
			if (index == m_commandList.at(command).size())
			{
				return true;
			}
		}
		//�t���[���𑫂��Ă���
		frame += data.frame;
		//�R�}���h�̗P�\�t���[���𒴂�����false
		if (frame > kDirInputgraceFrame)return false;
	}

	return false;
}

bool Input::CheckKeepCommand(std::string command)
{
	//�t���[���𐔂��邽�߂ɗp��
	int frame = 0;
	//�^���̃t���[���𐔂��邽�߂ɗp��
	int keepFrame = 0;
	//�R�}���h�����̕����L�[�̍ŏ���0�Ƃ���
	int index = 0;

	//����܂ł̓��͓��e�����Ă���
	for (auto& data : m_stickDirInfo)
	{
		//�^���̕��������΂߂ł��������Ă��邱�Ƃɂ�����
		if (index == (m_commandList.at(command).size() - 1))
		{
			//���̕������o���Ă���
			StickDir originDir = data.dir;
			if ((data.dir == StickDir::RightUp) || (data.dir == StickDir::RightDown))
			{
				//�E�Ƃ��Ĉ���
				data.dir =  StickDir::Right ;
				//���s���Ă�Ȃ���Ƃɖ߂�
				if (!(data.dir == m_commandList.at(command).at(index)))
				{
					data.dir = originDir;
				}
			}
			if ((data.dir == StickDir::LeftUp) || (data.dir == StickDir::LeftDown))
			{
				//���Ƃ��Ĉ���
				data.dir = StickDir::Left;
				//���s���Ă�Ȃ���Ƃɖ߂�
				if (!(data.dir == m_commandList.at(command).at(index)))
				{
					data.dir = originDir;
				}
			}
			if ((data.dir == StickDir::RightDown) || (data.dir == StickDir::LeftDown))
			{
				//���Ƃ��Ĉ���
				data.dir = StickDir::Down;
				//���s���Ă�Ȃ���Ƃɖ߂�
				if (!(data.dir == m_commandList.at(command).at(index)))
				{
					data.dir = originDir;
				}
			}
		}
		else//�^���̕����ȊO�̎�
		{
			//���^����̃R�}���h�̍�[��]�̓��͂��΂ߏ�ł̐�������悤�ɂ������̂�
			//[�΂ߏ�]��[��]�Ƃ��Ĉ���
			if (data.dir == StickDir::RightUp || data.dir == StickDir::LeftUp)
			{
				data.dir = StickDir::Up;
			}
		}
		//���͓��e�ɃR�}���h�̏����ɓ��Ă͂܂�L�[����������
		if (data.dir == m_commandList.at(command).at(index))
		{
			//���̕����L�[���`�F�b�N���邽�߂�index��1���₷
			index++;
			//�R�}���h�̕����L�[�̃^���̕������`�F�b�N
			if (index == m_commandList.at(command).size())
			{
				//�������Ă���Ȃ�^���̃t���[���𑫂��Ă���
				keepFrame += data.frame;
				//�t���[����40�t���[���𒴂��Ă邩
				if (keepFrame > kKeepFrame)
				{
					//����
					return true;
				}
				else
				{
					//��������߂�
					index = 1;
					continue;
				}
			}
		}
		//�t���[���𑫂��Ă���
		frame += data.frame;
		//�R�}���h�̗P�\�t���[���𒴂�����false
		if (frame > kKeepInputgraceFrame)return false;
	}
	return false;
}






void Input::StopUpdate()
{
	for (auto& item : m_currentInput)
	{
		item.second = false;
	}
	for (auto& item : m_lastInput)
	{
		item.second = false;
	}

	m_stickInfo.leftStickX = 0;
	m_stickInfo.leftStickY = 0;
	m_stickInfo.rightStickX = 0;
	m_stickInfo.rightStickY = 0;

	m_triggerInfo.left = 0;
	m_triggerInfo.right = 0;
}

bool Input::IsPress(const std::string& action)const
{
	auto keyInfo = m_currentInput.find(action);

	//�{�^��������`����Ă��Ȃ�������false��Ԃ�
	if (keyInfo == m_currentInput.end())
	{
		return false;
	}
	else
	{
		//����������bool�̒l��Ԃ�
		return keyInfo->second;
	}
}

bool Input::IsTrigger(const std::string& action)const
{
	if (IsPress(action))
	{
		//�O�̃t���[�����Q��
		auto last = m_lastInput.find(action);
		//����`�̃{�^������������false��Ԃ�
		if (last == m_lastInput.end())
		{
			return false;
		}
		else
		{
			//�O�̃t���[���ł�������Ă�����
			return !last->second;
		}
	}
	else
	{
		return false;
	}
}

bool Input::IsRelease(const std::string& action)const
{
	//�܂�������Ă��邩�𔻒�
	if (IsPress(action))
	{
		//������Ă�����false��Ԃ�
		return false;
	}
	else
	{
		//�O�̃t���[�����Q��
		auto last = m_lastInput.find(action);
		//������Ă�����false��Ԃ�
		if (last == m_lastInput.end())
		{
			return false;
		}
		//������Ă��Ȃ����
		else
		{
			return last->second;
		}
	}

	return false;
}

bool Input::IsPushTrigger(bool right, int power)
{
	//�g���K�[�̏�Ԏ擾
	auto trigger = GetTriggerInfo();

	//�E���𔻒肷��ꍇ
	if (right)
	{
		//�������������������܂ꂽ��
		if (trigger.right >= power)
		{
			return true;
		}
	}
	//�����𔻒肷��ꍇ
	else
	{
		//�������������������܂ꂽ��
		if (trigger.left >= power)
		{
			return true;
		}
	}
	return false;
}

bool Input::IsPushTrigger(bool right)
{
	return IsPushTrigger(right, kTriggerPower);
}

//�i�Q�[�p
