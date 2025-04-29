#include "Rigidbody.h"

Rigidbody::Rigidbody():
	m_pos(),
	m_vec()
{
}

Rigidbody::Rigidbody(Vector3 pos):
	m_pos(pos),
	m_vec()
{
}

void Rigidbody::SetMoveVec(Vector3 vec)
{
	m_vec.x = vec.x;
	m_vec.z = vec.z;
}
