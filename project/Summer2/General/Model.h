#pragma once
#include <DxLib.h>
#include <memory>
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

class Animator;
class Model
{
public:
	Model(int modelHandle, VECTOR pos);
	virtual ~Model();
	int GetModelHandle() { return m_modelHandle; };
	void Update();
	void Draw()const;
	void SetPos(VECTOR pos);
	void SetScale(VECTOR pos);
	void SetDir(Vector2 vec);
	Vector3 GetDir();
	//アニメーション関係
	void SetAnim(const char* animName,bool isLoop);//アニメーション変更
	void SetAnim(const char* animName,bool isLoop,const float& animSpeed);//アニメーション変更(再生速度も)
	void SetAnimSpeed(const float& animSpeed);//アニメーションの速度変更
	bool IsFinishAnim();//アニメーションが終わったかどうか
	float GetTotalAnimFrame();//全体フレーム
	void SetTotalAnimFrame(float totalFrame);//全体フレームセット
	float GetNowAnimFrame();//現在のアニメーションフレーム
	void ReplayAnim();//アニメーションを最初から再生
	void DeleteAnim();//再生中のアニメーションを削除
	void SetFixedLoopFrame(float loopFrame);//指定ループ再生
	bool IsFinishFixedLoop();//指定ループ再生終了

	//バウンディングボックス
	//void DrawBoundingBox()const;
private:
	int m_modelHandle;
	//アニメーション
	std::unique_ptr<Animator> m_animator;
	//向き
	Vector3 m_dir;
	//現在の角度
	float m_nowAngleY;
	//最終的な角度
	float m_endAngleY;
	//回転の向き
	Vector3 m_angleAxis;
	//モデルの回転情報
	Quaternion m_rotation;
	//モデルの正面
	Vector3 m_forward;
	//回転クォータニオン
	Quaternion m_rotaQ;
	//回転フレーム
	int m_rotaFrame;
};

