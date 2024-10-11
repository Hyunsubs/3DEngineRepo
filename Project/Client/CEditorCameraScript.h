#pragma once
#include <Engine/CScript.h>

class CEditorCameraScript :
    public CScript
{
private:
    float   m_Speed;

public:
    virtual void Tick() override;
      
private:
    void OrthoGraphicMove();
    void PerspectiveMove();

public:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File)override {}

public:
    CLONE(CEditorCameraScript);
    CEditorCameraScript();
    ~CEditorCameraScript();
};

