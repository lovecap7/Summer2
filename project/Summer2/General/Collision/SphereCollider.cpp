#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius):
	ColliderBase(ColliderKind::Sphere),//���̌`
	m_radius(radius)
{
}
