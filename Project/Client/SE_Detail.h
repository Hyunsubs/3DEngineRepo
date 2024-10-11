#pragma once
#include "SE_Sub.h"


class SE_Detail :
    public SE_Sub
{
private:
    Ptr<CTexture>   m_AtlasTex;

private:
    void Atlas();
    void AtlasInfo();

public:
    void SetAtlasTex(Ptr<CTexture> _Tex);

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void SelectTexture(DWORD_PTR _ListUI);

public:
    SE_Detail();
    ~SE_Detail();
};

