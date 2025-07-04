#include "UIManager.h"
#include "UIBase.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::Entry(std::shared_ptr<UIBase> ui)
{
	//すでに登録されているなら登録しない
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())
	{
		return;
	}
	//UIをセット
	m_uis.emplace_back(ui);
}

void UIManager::Exit(std::shared_ptr<UIBase> ui)
{
	//すでに登録されているなら古いUIを消す
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())
	{
		//削除
		m_uis.erase(it);
	}
}

void UIManager::Update()
{
	if (m_uis.empty())return;//空なら何もしない

	//UIの更新処理
	for (auto& ui : m_uis)
	{
		ui->Update();
	}
}

void UIManager::Draw() const
{
	if (m_uis.empty())return;//空なら何もしない

	//UIの描画処理
	for (auto& ui : m_uis)
	{
		ui->Draw();
	}
}
