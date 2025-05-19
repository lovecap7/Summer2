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
	/// 入力情報を更新する(毎フレーム呼び出してください)
	/// </summary>
	void Update();
	/// <summary>
	/// 入力チェック(押下状態)
	/// </summary>
	/// <param name="action">調べたいアクションの名前</param>
	/// <returns>true 押されている / false 押されてない</returns>
	bool IsPressed(const string& action)const;
	/// <summary>
	/// 入力チェック(トリガー状態[押した瞬間])
	/// </summary>
	/// <param name="action">調べたいアクションの名前</param>
	/// <returns>true 今押された / false 押されていないか押しっぱなし</returns>
	bool IsTriggered(const string& action)const;

	/// <summary>
	/// スティックの傾きをチェックするのに使う
	/// X方向（-1000～1000）
	/// Y方向（-1000～1000）
	/// </summary>
	struct StickInfo
	{
		/// <summary>
		/// 左(Max -1000)
		/// 右(Max  1000)
		/// </summary>
		int leftStickX;
		/// <summary>
		/// 上(Max -1000)
		/// 下(Max  1000)
		/// </summary>
		int leftStickY;
		/// <summary>
		/// 左(Max -1000)
		/// 右(Max  1000)
		/// </summary>
		int rightStickX;
		/// <summary>
		/// 上(Max -1000)
		/// 下(Max  1000)
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
	/// スティックの傾きを取得
	/// </summary>
	/// <returns>スティックがどれだけ倒れているか</returns>
	StickInfo GetStickInfo() const { return m_stickInfo; }

	/// <summary>
	/// 少しだけ倒しているならtrue
	/// </summary>
	/// <returns></returns>
	bool IsLowPowerLeftStick();
	/// <summary>
	/// 真ん中ぐらいで倒したらtrue
	/// </summary>
	/// <returns></returns>
	bool IsMediumPowerLeftStick();
	/// <summary>
	/// 最大まで倒したら
	/// </summary>
	/// <returns></returns>
	bool IsHighPowerLeftStick();

private:
	/// <summary>
	/// 入力対応マップに使用する
	/// </summary>
	unsigned int buttonID;
	//自分のパッドのID
	int m_padID;


	//ゲームパッド限定のテーブル
	using GamePadActionMap_t = map<string, vector<unsigned int>>;
	GamePadActionMap_t m_gamePadActionMap;
	map<string, bool> m_currentInput;//現在の押下状態
	map<string, bool> m_lastInput;//直前の押下状態

	//スティック
	StickInfo m_stickInfo;
};

