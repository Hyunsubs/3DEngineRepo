#pragma once

#include "CRenderComponent.h"

class CMeshRender :
    public CRenderComponent
{

public:
    virtual void FinalTick() override;
    virtual void Render() override;   

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CMeshRender);
    CMeshRender();
    ~CMeshRender();
};

