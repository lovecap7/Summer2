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
	//‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚é‚È‚ç“o˜^‚µ‚È‚¢
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())
	{
		return;
	}
	//UI‚ğƒZƒbƒg
	m_uis.emplace_back(ui);
}

void UIManager::Exit(std::shared_ptr<UIBase> ui)
{
	//‚·‚Å‚É“o˜^‚³‚ê‚Ä‚¢‚é‚È‚çŒÃ‚¢UI‚ğÁ‚·
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())
	{
		//íœ
		m_uis.erase(it);
	}
}

void UIManager::Update()
{
}

void UIManager::Draw() const
{
}
