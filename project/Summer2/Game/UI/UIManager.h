#pragma once
#include  <vector>
#include  <memory>
class UIBase;
class UIManager
{
public:
	UIManager();
	~UIManager();
	//登録処理
	void Entry(std::shared_ptr<UIBase>);
	//登録解除
	void Exit(std::shared_ptr<UIBase> ui);
	//初期化処理
	void Init();
	//更新処理
	void Update();
	//描画
	void Draw()const;
private:
	//UI
	std::vector<std::shared_ptr<UIBase>> m_uis;
};

