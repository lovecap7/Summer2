#pragma once
#include <DxLib.h>
#include <memory>
#include "Math/Vector3.h"

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
	void SetDir(VECTOR vec);
	Vector3 GetDir();
	//アニメーション関係
	void SetAnim(const char* animName,bool isLoop);//アニメーション変更
	void SetAnim(const char* animName,bool isLoop,const float animSpeed);//アニメーション変更(再生速度も)
	bool IsFinishAnim();//アニメーションが終わったかどうか
	float GetTatalAnimFrame();//全体フレーム
	float GetNowAnimFrame();//現在のアニメーションフレーム
	void ReplayAnim();//アニメーションを最初から再生
	void DeleteAnim();//再生中のアニメーションを削除
private:
	int m_modelHandle;
	//アニメーション
	std::unique_ptr<Animator> m_animator;
	//向き
	Vector3 m_dir;
};

