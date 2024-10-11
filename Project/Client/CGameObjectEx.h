#pragma once
#include <Engine/CGameObject.h>

class CGameObjectEx :
    public CGameObject
{
public:
    virtual void FinalTick() override;

public:
    CLONE(CGameObjectEx);
};

