#pragma once
/// <summary>
/// UI�̊��N���X
/// </summary>
class UIBase abstract
{
public:
	UIBase();
	~UIBase();
	virtual void Update() abstract;
	virtual void Draw()const abstract;
	//����
	bool IsDelete() { return m_isDelete; };
protected:
	bool m_isDelete;
};

