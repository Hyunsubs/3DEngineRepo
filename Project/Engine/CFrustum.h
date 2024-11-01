#pragma once
#include "CEntity.h"

enum class FACE_TYPE
{
    FT_NEAR,
    FT_FAR,
    FT_TOP,
    FT_BOTTOM,
    FT_LEFT,
    FT_RIGHT,

    END,
};

class CCamera;



class CFrustum :
    public CEntity
{
private:
    CCamera* m_Owner;

    // NDC ��ǥ�迡�� �þ��� �ִ� ��ġ ��ǥ
    Vec3    m_arrProj[8];

    // ����ü�� �����ϴ� 6���� ���(���� �����̽� ����)
    Vec4    m_arrFace[(UINT)FACE_TYPE::END];

public:
    void FinalTick();
    bool FrustumCheck(Vec3 _WorldPos, float _Radius = 0.f);


public:
    CLONE(CFrustum)
    CFrustum(CCamera* _Owner);
    ~CFrustum();
};
