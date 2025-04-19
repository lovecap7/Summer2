#include "Input.h"
#include <DxLib.h>

Input::Input()
{
	//連想配列
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
	//最後の入力を取得(IsTriggeredで使う)
	m_lastInput = m_currentInput;
	//全ての入力を取得する
	char keyState[256] = {};
	int padState = {};
	int mouseState = {};
	//チェック
	GetHitKeyStateAll(keyState);
	padState = GetJoypadInputState(DX_INPUT_PAD1);
	mouseState = GetMouseInput();
	//それぞれのボタン名に割り当たっている全ての入力をチェック
	for (const auto& mapInfo : m_inputActionMap)
	{
		bool isPressed = false;
		for (const auto& inputInfo : mapInfo.second)
		{
			//入力があるならtrue
			isPressed = (inputInfo.type == InputType::Keyboard && keyState[inputInfo.buttonID]) ||
				(inputInfo.type == InputType::Gamepad && padState & inputInfo.buttonID) ||
				(inputInfo.type == InputType::Mouse && mouseState & inputInfo.buttonID);
			//ヒットしたらループを抜ける
			if (isPressed)
			{
				break;
			}
		}
		//現在のボタンの状態を入れていく
		m_currentInput[mapInfo.first] = isPressed;
	}
}

//押しているか
bool Input::IsPressed(const string& action) const
{
	//ボタン名と同じものが定義されているかチェック
	auto keyInfo = m_currentInput.find(action);
	if (keyInfo == m_currentInput.end())//未定義のボタン名が来たら無条件でfalseを返す
	{
		return false;
	}
	else
	{
		//あったらそのボタンの状態を返す
		return keyInfo->second;
	}
}

//押した瞬間
bool Input::IsTriggered(const string& action) const
{
	//押してるかをチェック
	if (IsPressed(action))
	{
		//ボタン名と同じものが定義されているかチェック
		auto lastKeyInfo = m_lastInput.find(action);
		if (lastKeyInfo == m_lastInput.end())//未定義のボタン名が来たら無条件でfalseを返す
		{
			return false;
		}
		else
		{
			//前のフレームでfalseならtrueを返す
			return !lastKeyInfo->second;
		}
	}
	else
	{
		return false;
	}
}
