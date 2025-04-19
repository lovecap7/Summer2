#include "Collider2D.h"

Collider2D::Collider2D()
{
}

Collider2D::Collider2D(Rect2D& rect):
	m_rect(rect),
	m_capsule(),
	m_circle()
{
	collType = CollType::Rect;
}

Collider2D::Collider2D(Circle2D& circle):
	m_circle(circle),
	m_rect(),
	m_capsule()
{
	collType = CollType::Circle;
}

Collider2D::Collider2D(Capsule2D& capsule):
	m_capsule(capsule),
	m_rect(),
	m_circle()
{
	collType = CollType::Capsule;
}

Collider2D::~Collider2D()
{
}

Rect2D& Collider2D::GetRect2D()
{
	return m_rect;
}

Circle2D& Collider2D::GetCircle2D()
{
	return m_circle;
}

Capsule2D& Collider2D::GetCapsule2D()
{
	return m_capsule;
}

void Collider2D::OnCollision()
{
	m_isHit = true;
}

void Collider2D::Reset()
{
	m_isHit = false;
}

