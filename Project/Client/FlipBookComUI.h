#pragma once
#include "ComponentUI.h"
class FlipBookComUI :
    public ComponentUI
{
private:
    virtual void Update() override;

public:
    FlipBookComUI();
    ~FlipBookComUI();

};

