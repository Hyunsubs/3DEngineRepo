#pragma once
#include "singleton.h"

class CCamera;
class CGameObject;
class CLight2D;
class CLight3D;
class CStructuredBuffer;
class CMRT;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr)
private:
    vector<CCamera*>        m_vecCam;
    CCamera*                m_EditorCamera;

    CGameObject*            m_DebugObject;
    list<tDebugShapeInfo>   m_DebugShapeList;

    // Light
    vector<CLight2D*>       m_vecLight2D;
    CStructuredBuffer*      m_Light2DBuffer;

    vector<CLight3D*>       m_vecLight3D;
    CStructuredBuffer*      m_Light3DBuffer;

    // PostProcess
    Ptr<CTexture>           m_PostProcessTex;

    // DownScale
    Ptr<CTexture>           m_DownScaleTex;

    CMRT*                   m_arrMRT[(UINT)MRT_TYPE::END];

public:
    void RegisterCamera(CCamera* _Cam, int _CamPriority);
    void RegisterEditorCamera(CCamera* _Cam) { m_EditorCamera = _Cam; }
    void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_DebugShapeList.push_back(_Info); }
    void RegisterLight2D(CLight2D* _Light) { m_vecLight2D.push_back(_Light); }    
    void RegisterLight3D(CLight3D* _Light) { m_vecLight3D.push_back(_Light); }
    void PostProcessCopy();

    CMRT* GetMRT(MRT_TYPE _Type) { return m_arrMRT[(UINT)_Type]; }


    CCamera* GetPOVCam();

private:
    void RenderStart();
    void Render(CCamera* _Cam);
    void Render_Sub(CCamera* _Cam);


    void Clear();
    void RenderDebugShape();
    void CreateMRT();
    void ClearMRT();

public:
    void Init();
    void Tick();
};

