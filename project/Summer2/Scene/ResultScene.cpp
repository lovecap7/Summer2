#include <DxLib.h>
#include <memory>
#include "ResultScene.h"
#include  "../General/Input.h"
#include "SceneController.h"
#include "../Game/Score/ScoreManager.h"
#include "../Result/ResultChara.h"
#include "TitleScene.h"

namespace
{
	constexpr int kScorePosX = 500;
	constexpr int kScorePosY = 400;

	constexpr int kAddScoreFirstFrame = 30.0f;
}

ResultScene::ResultScene(SceneController& controller):
	SceneBase(controller),
	m_countFrame(0),
	m_scorePos(kScorePosX,0)
{
	m_chara = std::make_shared<ResultChara>();
	//0から増えていってほしいので一度リセット
	m_controller.GetScoreManager()->ResetDrawScere();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update(Input& input)
{
	if (input.IsTriggered("Pause"))
	{
		//スコアリセット
		m_controller.GetScoreManager()->ResetTotalScere();
		m_controller.GetScoreManager()->ResetDrawScere();
		//次のシーンへ
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
		return;
	}
	if (m_countFrame >= kAddScoreFirstFrame)
	{
		//スコア
		m_controller.GetScoreManager()->Update();
	}

	//キャラクターのアニメーション
	m_chara->Update();

	m_countFrame++;
	m_scorePos.y = ((1.0f - 0.8f) * kScorePosY) + (0.8f * m_scorePos.y);
}

void ResultScene::Draw()
{
	DrawString(0, 0, "Result Scene", 0xffffff);
	m_chara->Draw();
	m_controller.GetScoreManager()->Draw(m_scorePos.x, m_scorePos.y);
}
