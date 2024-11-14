#pragma once
#include "CRenderComponent.h"

#include "CHeightMapCS.h"

class CLandScape :
    public CRenderComponent
{
private:
    int             m_FaceX;
    int             m_FaceZ;

    // Brush
    Vec2                    m_BrushScale;
    vector<Ptr<CTexture>>   m_vecBrush;
    UINT                    m_BrushIdx;
    
    // HeightMap
    Ptr<CTexture>           m_HeightMap;
    bool                    m_IsHeightMapCreated;
    Ptr<CHeightMapCS>       m_HeightMapCS;

    float           m_TessLevel;

public:
    void SetFace(int _X, int _Z);
    void AddBrushTexture(Ptr<CTexture> _BrushTex) { m_vecBrush.push_back(_BrushTex); }
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMap = _HeightMap;  m_IsHeightMapCreated = false; }
    void CreateHeightMap(UINT _Width, UINT _Height);
public:
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void CreateMesh();
    void CreateComputeShader();
    void CreateTextureAndStructuredBuffer();


public:
    CLONE(CLandScape);
    CLandScape();
    ~CLandScape();
};

