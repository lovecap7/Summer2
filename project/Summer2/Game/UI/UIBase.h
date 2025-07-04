#pragma once
/// <summary>
/// UIの基底クラス
/// </summary>
class UIBase abstract
{
public:
	UIBase();
	~UIBase();
	virtual void Update() abstract;
	virtual void Draw()const abstract;
};

