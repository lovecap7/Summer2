#include "Collidable.h"

Collidable::Collidable(std::shared_ptr<ColliderBase> coll, std::shared_ptr<Rigidbody> rid):
	m_coll(coll),
	m_rb(rid),
	m_isCollide(true),
	m_isStatic(false),
	m_state(State::None)
{
}
