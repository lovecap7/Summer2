#pragma once
#include "../Actor.h"

//アイテムの種類
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
    //アイテムの種類
    ItemKind m_kind;
};

