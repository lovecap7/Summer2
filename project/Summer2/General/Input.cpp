#include "Input.h"
#include <DxLib.h>
#include "Math/Vector3.h"

namespace
{
	//少しだけ倒していると判定する範囲
	constexpr float kLowPowerStickMin = 50.0f;
	constexpr float kLowPowerStickMax = 400.0f;
	//そこそこ倒していると判定する範囲
	constexpr float kMediumPowerStickMin = kLowPowerStickMax;
	constexpr float kMediumPowerStickMax = 800.0f;
	//最大まで倒していると判定する範囲
	constexpr float kHighPowerStickMin = kMediumPowerStickMax;
	constexpr float kHighPowerStickMax = 1000.0f;
}

Input::Input(int padID):
	m_padID(std::move(padID))
{
	//ゲームパッドの連想配列
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
	//最後の入力を取得(IsTriggeredで使う)
	m_lastInput = m_currentInput;
	//全ての入力を取得する
	int padState = {};
	//チェック
	padState = GetJoypadInputState(m_padID);
	//それぞれのボタン名に割り当たっている全ての入力をチェック
	for (const auto& keyInfo : m_gamePadActionMap)
	{
		bool isPressed = false;
		for (const auto& inputInfo : keyInfo.second)
		{
			//入力があるならtrue
			isPressed = (padState & inputInfo);
			//ヒットしたらループを抜ける
			if (isPressed)
			{
				break;
			}
		}
		//現在のボタンの状態を入れていく
		m_currentInput[keyInfo.first] = isPressed;
	}

	//スティックの入力を初期化
	m_stickInfo.leftStickX = 0;
	m_stickInfo.leftStickY  = 0;
	m_stickInfo.rightStickX  = 0;
	m_stickInfo.rightStickY = 0;
	
	//スティックの入力をチェックする
	//左スティック
	DxLib::GetJoypadAnalogInput(&m_stickInfo.leftStickX, &m_stickInfo.leftStickY, m_padID);
	//右スティック
	DxLib::GetJoypadAnalogInputRight(&m_stickInfo.rightStickX, &m_stickInfo.rightStickY, m_padID);
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

bool Input::IsLowPowerLeftStick()
{
	//少しだけ倒しているならtrue
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
	//そこそこ倒しているならtrue
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
	//倒し切ってるならtrue
	if ((m_stickInfo.leftStickX > kHighPowerStickMin && m_stickInfo.leftStickX <= kHighPowerStickMax) ||
		(m_stickInfo.leftStickX < -kHighPowerStickMin && m_stickInfo.leftStickX >= -kHighPowerStickMax) ||
		(m_stickInfo.leftStickY > kHighPowerStickMin && m_stickInfo.leftStickY <= kHighPowerStickMax) ||
		(m_stickInfo.leftStickY < -kHighPowerStickMin && m_stickInfo.leftStickY >= -kHighPowerStickMax))
	{
		return true;
	}
	return false;
}
