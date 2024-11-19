#pragma once
#include "CEntity.h"

#include "Ptr.h"
#include "CTexture.h"

class CMRT :
    public CEntity
{
private:
    Ptr<CTexture>   m_arrRT[8]; // �ִ� ���� ������ RenderTarget ������ 8��
    Vec4            m_ClearColor[8];
    int             m_RTCount;    

    Ptr<CTexture>   m_DSTex;    // ���� �ؽ���, ��� ����

    D3D11_VIEWPORT  m_ViewPort; // MRT ����Ʈ ���� ��


public:
    Ptr<CTexture> GetRT(int _Index) { return m_arrRT[_Index]; }
    const D3D11_VIEWPORT& GetViewPort() { return m_ViewPort; }

    void Create(int _RTCount, Ptr<CTexture>* _arrRT, Ptr<CTexture> _DSTex);
    void SetClearColor(Vec4* _Color, bool _Independent);
    void OMSet();

    void Clear() { ClearRT(); ClearDS(); }
    void ClearRT();
    void ClearDS();


    CLONE_DISABLE(CMRT);
public:
    CMRT();
    ~CMRT();
};

