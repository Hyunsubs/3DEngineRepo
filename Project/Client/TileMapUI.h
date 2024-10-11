#pragma once
#include "ComponentUI.h"
class TileMapUI :
    public ComponentUI
{
    virtual void Update() override;

public:
    TileMapUI();
    ~TileMapUI();
};

