#pragma once
#include "Math/Vector3.h"
class Rigidbody
{
public:
	Rigidbody();
	Rigidbody(Vector3 pos);
	virtual ~Rigidbody() {};
	Vector3 GetPos() { return m_pos; };
	void SetPos(Position3 pos) { m_pos = pos; };
	Vector3 GetVec() { return m_vec; };
	void SetVec(Vector3 vec) { m_vec = vec; };
	//éüÇÃç¿ïW
	Vector3 GetNextPos() { return m_pos + m_vec; };
private:
	Vector3 m_pos;
	Vector3 m_vec;
};

