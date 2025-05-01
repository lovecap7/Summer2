#pragma once

class Animator
{
private:
	int m_attachAnim;//再生中のアニメーション名
	int m_attachAnimIndex;//アニメーションのアタッチされた番号
	float m_animTimer;//アニメーションのタイマー
	float m_animStopTime;//アニメーションの終了タイマー
	bool m_isLoopAnim;//ループするアニメーションかどうか
	bool m_isFinishAnim;//アニメーションが終わったらtrue
public:
	//コンストラクタ
	Animator();
	//デストラクタ
	~Animator();

	/// <summary>
	/// アニメーションのアタッチ
	/// </summary>
	/// <param name="modelHandle">モデルのハンドル</param>
	/// <param name="anim">animation enum</param>
	/// <param name="isLoop">ループするか</param>
	void SetAnim(const int& modelHandle, const int& anim, const bool& isLoop);

	/// <summary>
	/// 指定されたアニメーションが再生中なら削除
	/// </summary>
	/// <param name="modelHandle">モデルのハンドル</param>
	/// <param name="anim">animation enum</param>
	void RemoveAnim(const int& modelHandle);

	/// <summary>
	/// アニメーションの再生
	/// </summary>
	/// <param name="modelHandle"></param>
	void PlayAnim(const int& modelHandle);

	/// <summary>
	/// アニメーションが終わったかどうか
	/// </summary>
	/// <returns></returns>
	bool IsFinishAnim() { return m_isFinishAnim; }
};
