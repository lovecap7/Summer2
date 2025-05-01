#include "Animator.h"
#include <DxLib.h>

Animator::Animator():
	m_attachAnim(-1),
	m_attachAnimIndex(-1),
	m_animStopTime(0.0f),
	m_animTimer(0.0f),
	m_isLoopAnim(false),
	m_isFinishAnim(false)
{
}

Animator::~Animator()
{
}

void Animator::SetAnim(const int& modelHandle, const int& anim, const bool& isLoop)
{
	//現在再生中のアニメーションを指定されても再設定しない
	if (anim == m_attachAnim)return;

	//今のモーションを消す
	RemoveAnim(modelHandle);

	//新しいモーションに更新
	m_attachAnimIndex = MV1AttachAnim(modelHandle, anim, -1, false);

	m_attachAnim = anim;//今のアニメーションの番号
	m_animStopTime = MV1GetAttachAnimTotalTime(modelHandle, m_attachAnimIndex);//アニメーションの終了時間
	m_animTimer = 0.0f;//タイマー初期化
	m_isLoopAnim = isLoop;//ループするか

}

void Animator::RemoveAnim(const int& modelHandle)
{
	//そもそも何もアタッチされていないなら早期リターン
	if (m_attachAnimIndex == -1)return;
	
	//今のモーションを消す
	MV1DetachAnim(modelHandle, m_attachAnimIndex);
	m_attachAnim = -1;
	m_attachAnimIndex = -1;
	m_animStopTime = 0.0f;
	m_animTimer = 0.0f;
	m_isLoopAnim = false;
}

void Animator::PlayAnim(const int& modelHandle)
{
	//何もアタッチされてないなら再生しない
	if (m_attachAnimIndex == -1)return;

	//アニメーションが終わっていないかもしれないのでfalse
	m_isFinishAnim = false;
	//アニメーションの終わりまで再生
	if (m_animStopTime <= m_animTimer)
	{
		if (m_isLoopAnim)
		{
			//ループする
			m_animTimer = 0.0f;
		}
		else
		{
			//終わったらtrue
			m_isFinishAnim = true;
		}
	}
	//アニメーションを進める
	MV1SetAttachAnimTime(modelHandle, m_attachAnimIndex, m_animTimer);
	
	m_animTimer += 0.5f;
}
