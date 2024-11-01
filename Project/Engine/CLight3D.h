#pragma once
#include "CComponent.h"

class CLight3D :
    public CComponent
{
private:
    tLightInfo      m_Info;
    int             m_LightIdx;

    Ptr<CMesh>      m_VolumeMesh;
    Ptr<CMaterial>  m_LightMtrl;

    // ShadowMap MRT
    class CMRT*     m_ShadowMapMRT;

    // ShadowMap Material
    Ptr<CMaterial>  m_ShadowMapMtrl;

    // ���� �������� ���� �ֺ� �繰�� ����
    Ptr<CTexture>   m_ShadowMapTex;

    CGameObject*    m_Cam;

public:
    void SetLightType(LIGHT_TYPE _Type);
    void SetRadius(float _Radius);
    void SetAngle(float _Angle) { m_Info.Angle = _Angle; }
    void SetLightColor(Vec3 _Color) { m_Info.light.Color = _Color; }
    void SetLightAmbient(Vec3 _Amb) { m_Info.light.Ambient = _Amb; }
    void SetSpecularCoefficient(float _Coef) { m_Info.light.SpecCoef = _Coef; }

    LIGHT_TYPE GetLightType() { return m_Info.Type; }
    float GetRadius() { return m_Info.Radius; }
    float GetAngle() { return m_Info.Angle; }
    Vec3 GetLightColor() { return m_Info.light.Color; }
    Vec3 GetLightAmbient() { return m_Info.light.Ambient; }


    const tLightInfo& GetLightInfo() { return m_Info; }

public:
    void CreateShadowMap();

    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


    void Render();


public:
    CLONE(CLight3D);
    CLight3D();
    CLight3D(const CLight3D& _Origin);
    ~CLight3D();
};

