#pragma once
#include "AttackBase.h"
class Actor;
class Slash :
    public AttackBase
{
public:
	Slash(std::shared_ptr<Collidable> coll, float damege, int keepFrame);
	~Slash();
	//������
	void Init() override;
	//�X�V����
	void Update() override;
	//�`��
	void Draw() override;
	//����������
	void OnHit(std::shared_ptr<Actor> actor) override;
private:
	float m_initDamege;//�������p
	float m_initKeepFrame;//�������p
};

