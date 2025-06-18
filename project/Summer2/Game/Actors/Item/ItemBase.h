#pragma once
#include "../Actor.h"

//�A�C�e���̎��
enum class ItemKind
{
    Heart,
};

class ItemBase abstract:
    public Actor
{
public:
    ItemBase(ItemKind kind);
    ~ItemBase();
    ItemKind GetItemKind() { return m_kind; }
protected:
    //�A�C�e���̎��
    ItemKind m_kind;
};

