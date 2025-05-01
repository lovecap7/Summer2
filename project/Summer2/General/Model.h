#pragma once
#include <DxLib.h>
#include <memory>

class Animator;
class Model
{
public:
	Model(int modelHandle, VECTOR pos);
	virtual ~Model();
	void Update();
	void Draw()const;
	void SetPos(VECTOR pos);
	void SetScale(VECTOR pos);
	void SetDir(VECTOR vec);
	//アニメーション関係
	void SetAnim(const char* animName,bool isLoop);//アニメーション変更
	bool IsFinishAnim();//アニメーションが終わったかどうか
	void RemoveAnim();//アニメーション削除
private:
	int m_modelHandle;
	//アニメーション
	std::unique_ptr<Animator> m_animator;
};

