#pragma once
#include "Math/Vector3.h"
class Rigidbody
{
public:
	Rigidbody();
	Rigidbody(Vector3 pos);
	virtual ~Rigidbody() {};
	//位置
	Vector3 GetPos() { return m_pos; };
	void SetPos(Position3 pos) { m_pos = pos; };
	//ベクトル
	Vector3 GetVec() { return m_vec; };
	void SetVec(Vector3 vec) { m_vec = vec; };
	void AddVec(Vector3 vec) { m_vec += vec; };
	void ResetVec() { m_vec = m_vec.Zero(); };
	//次の座標
	Vector3 GetNextPos() { return m_pos + m_vec; };
	void SetNextPos() { m_pos += m_vec; };
private:
	//座標とベクトル
	Vector3 m_pos;
	Vector3 m_vec;
};

