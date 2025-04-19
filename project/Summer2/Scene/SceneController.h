#pragma once
//スマートポインタを使う
#include<memory>
class SceneBase;
class Input;
class ScoreManager;
/// <summary>
/// 各シーンを管理するクラス
/// 管理はするが、繊維を管理はしない(繊維はクラスどもがやる)
/// インターフェイスを提供するクラス
/// </summary>
class SceneController
{
public:
	SceneController();
	/// <summary>
	/// Applicationから呼ばれる関数。現在のシーンのUpdateをそのまま実行。
	/// </summary>
	/// <param name="input"></param>
	void Update(Input& input);
	/// <summary>
	/// Applicationから呼ばれる関数。現在のシーンのDrawをそのまま実行。
	/// </summary>
	/// <param name="input"></param>
	void Draw();
	/// <summary>
	/// 次の状態をセットする(セットするのは各状態の役割)
	/// 各シーンがコントローラーを見て切り替えさせる
	/// </summary>
	/// <param name="">次の状態のポインタ</param>
	void ChangeScene(std::shared_ptr<SceneBase>);

	/// <summary>
	/// スコアマネージャーの参照
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<ScoreManager>& GetScoreManager() { return m_scoreManager; }
private:
	//現在実行中のシーン(中が何かは知らない)
	std::shared_ptr<SceneBase> m_scene;

private:
	//スコア(各シーンで使うので)
	std::shared_ptr<ScoreManager> m_scoreManager;
};

