#include "pch.h"
#include "CCamera.h"
#include "CDevice.h"
#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CTransform.h"
#include "CAssetMgr.h"
#include "CMRT.h"

#include "CFrustum.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_Priority(-1)
	, m_LayerCheck(0)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_Frustum(nullptr)
	, m_Width(0)
	, m_Height(0)
	, m_Far(100000.f)
	, m_FOV( XM_PI / 2.f)
	, m_ProjectionScale(1.f)
{
	Vec2 vResolution = CDevice::GetInst()->GetResolution();
	m_Width = vResolution.x;
	m_Height = vResolution.y;
	m_AspectRatio = m_Width / m_Height;

	// Frustum 생성
	m_Frustum = new CFrustum(this);
}

CCamera::CCamera(const CCamera& _Other)
	: CComponent(_Other)
	, m_Priority(-1)
	, m_LayerCheck(_Other.m_LayerCheck)
	, m_ProjType(_Other.m_ProjType)
	, m_Frustum(nullptr)
	, m_Width(_Other.m_Width)
	, m_Height(_Other.m_Height)
	, m_Far(_Other.m_Far)
	, m_FOV(_Other.m_FOV)
	, m_ProjectionScale(_Other.m_ProjectionScale)
{
	m_Frustum = _Other.m_Frustum->Clone();
}

CCamera::~CCamera()
{
	if (nullptr != m_Frustum)
		delete m_Frustum;
}

void CCamera::Begin()
{
	// 카메라를 등록
	if (-1 != m_Priority)
	{
		CRenderMgr::GetInst()->RegisterCamera(this, m_Priority);
	}
}

void CCamera::FinalTick()
{
	// View Space 란 카메라가 좌표계의 기준이 되는 좌표계
	// 1. 카메라가 원점에 존재
	// 2. 카메라가 바라보는 방향이 Z 축

	// 1. 카메라가 있는 곳이 원점이었을 경우를 기준으로한 물체들의 좌표를 알아내야 한다.
	// 2. 카메라가 월드에서 바라보던 방향을 Z 축으로 돌려두어야 한다.
	//    물체들도 같이 회전을 한다.

	// View 행렬을 계산한다.
	// View 행렬은 World Space -> View Space 로 변경할때 사용하는 행렬		
	Matrix matTrans = XMMatrixTranslation(-Transform()->GetRelativePos().x, -Transform()->GetRelativePos().y, -Transform()->GetRelativePos().z);

	Matrix matRot;
	Vec3 vR = Transform()->GetWorldDir(DIR::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR::FRONT);

	matRot._11 = vR.x; matRot._12 = vU.x; matRot._13 = vF.x;
	matRot._21 = vR.y; matRot._22 = vU.y; matRot._23 = vF.y;
	matRot._31 = vR.z; matRot._32 = vU.z; matRot._33 = vF.z;

	m_matView = matTrans * matRot;


	// Projection Space 투영 좌표계 (NDC)
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 1. 직교투영 (Orthographic)
		// 투영을 일직선으로
		// 시야 범위를 NDC 로 압축
		m_matProj = XMMatrixOrthographicLH(m_Width * m_ProjectionScale, m_Height * m_ProjectionScale, 1.f, m_Far);
	}

	else
	{
		// 2. 원근투영 (Perspective)		
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

	// 역행렬 계산
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);

	// 마우스방향 Ray 계산
	CalcRay();

	// Frustum Update
	m_Frustum->FinalTick();
}

void CCamera::CalcRay()
{
	// ViewPort 정보
	CMRT* pSwapChainMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	if (nullptr == pSwapChainMRT)
		return;
	const D3D11_VIEWPORT& VP = pSwapChainMRT->GetViewPort();

	// 현재 마우스 좌표
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	// 마우스를 향하는 직선은 카메라 위치를 지난다.
	m_Ray.vStart = Transform()->GetWorldPos();

	// View 공간 상에서 카메라에서 마우스 방향을 향하는 방향벡터를 구한다.
	//  - 마우스가 있는 좌표를 -1 ~ 1 사이의 정규화된 좌표로 바꾼다.
	//  - 투영행렬의 _11, _22 에 있는 값은 Near 평면상에서 Near 값을 가로 세로 길이로 나눈값
	//  - 실제 ViewSpace 상에서의 Near 평면상에서 마우스가 있는 지점을 향하는 위치를 구하기 위해서 비율을 나누어서 
	//  - 실제 Near 평면상에서 마우스가 향하는 위치를 좌표를 구함
	m_Ray.vDir.x = (((vMousePos.x - VP.TopLeftX) * 2.f / VP.Width) - 1.f) / m_matProj._11;
	m_Ray.vDir.y = -(((vMousePos.y - VP.TopLeftY) * 2.f / VP.Height) - 1.f) / m_matProj._22;
	m_Ray.vDir.z = 1.f;

	// 방향 벡터에 ViewMatInv 를 적용, 월드상에서의 방향을 알아낸다.
	m_Ray.vDir = XMVector3TransformNormal(m_Ray.vDir, m_matViewInv);
	m_Ray.vDir.Normalize();
}


void CCamera::SortGameObject()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (false == (m_LayerCheck & (1 << i)))
		{
			continue;
		}

		CLayer* pLayer = pLevel->GetLayer(i);

		const vector<CGameObject*>& vecObjects = pLayer->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if ( nullptr == vecObjects[j]->GetRenderComponent()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMesh()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader())
			{
				continue;
			}

			// 절두체 검사를 진행 함, 실패 함
			/*if ( vecObjects[j]->GetRenderComponent()->IsFrustumCheck()
				&& false == m_Frustum->FrustumCheck(vecObjects[j]->Transform()->GetWorldPos()
												  , vecObjects[j]->Transform()->GetWorldScale().x / 2.f))
			{
				continue;
			}*/

			Ptr<CGraphicShader> pShader = vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader();
			SHADER_DOMAIN Domain = pShader->GetDomain();

			switch (Domain)
			{
			case DOMAIN_DEFERRED:				
				m_vecDeferred.push_back(vecObjects[j]);
				break;
			case DOMAIN_DECAL:
				m_vecDecal.push_back(vecObjects[j]);
				break;
			case DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObjects[j]);
				break;
			case DOMAIN_MASKED:
				m_vecMasked.push_back(vecObjects[j]);
				break;
			case DOMAIN_TRANSPARENT:
				m_vecTransparent.push_back(vecObjects[j]);
				break;
			case DOMAIN_PARTICLE:
				m_vecParticles.push_back(vecObjects[j]);
				break;
			case DOMAIN_EFFECT:
				m_vecEffect.push_back(vecObjects[j]);
				break;
			case DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j]);
				break;
			case DOMAIN_UI:
				m_vecUI.push_back(vecObjects[j]);
				break;
			}
		}
	}
}

void CCamera::render_deferred()
{
	// Deferred
	for (size_t i = 0; i < m_vecDeferred.size(); ++i)
	{
		m_vecDeferred[i]->Render();
	}
}

void CCamera::render_decal()
{
	// Decal
	for (size_t i = 0; i < m_vecDecal.size(); ++i)
	{
		m_vecDecal[i]->Render();
	}
}

void CCamera::render_opaque()
{
	// Opaque
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->Render();
	}
}

void CCamera::render_masked()
{
	// Masked
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		m_vecMasked[i]->Render();
	}
}

void CCamera::render_transparent()
{
	// Transparent
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->Render();
	}
}


void CCamera::render_effect()
{
	// EffectMRT 로 변경
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->Clear();	
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->OMSet();

	// Effect
	for (size_t i = 0; i < m_vecEffect.size(); ++i)
	{
		m_vecEffect[i]->Render();
	}

	// EffectBlurMRT 로 변경
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT_BLUR)->ClearRT();
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT_BLUR)->OMSet();
		
	Ptr<CMaterial> pBlurMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurMtrl");
	Ptr<CMesh> pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");

	pBlurMtrl->SetTexParam(TEX_0, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->GetRT(0));
	pBlurMtrl->Binding();
	pRectMesh->Render_Particle(2);

	// 원래 렌더타겟(SwapChainMRT) 로 변경	
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
	Ptr<CMaterial> pEffectMergeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EffectMergeMtrl");

	pEffectMergeMtrl->SetTexParam(TEX_0, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->GetRT(0));
	pEffectMergeMtrl->SetTexParam(TEX_1, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT_BLUR)->GetRT(0));
	pEffectMergeMtrl->Binding();
	pRectMesh->Render();
}


void CCamera::render_particle()
{
	// Particles
	for (size_t i = 0; i < m_vecParticles.size(); ++i)
	{
		m_vecParticles[i]->Render();
	}
}

void CCamera::render_postprocess()
{
	// PostProcess 
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		CRenderMgr::GetInst()->PostProcessCopy();
		m_vecPostProcess[i]->Render();
	}
}

void CCamera::render_ui()
{
	// UI
	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->Render();
	}
}

void CCamera::clear()
{
	m_vecDeferred.clear();
	m_vecDecal.clear();
	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTransparent.clear();
	m_vecEffect.clear();
	m_vecParticles.clear();
	m_vecPostProcess.clear();
	m_vecUI.clear();
}

void CCamera::SortGameObject_ShadowMap()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (false == (m_LayerCheck & (1 << i)))
		{
			continue;
		}

		CLayer* pLayer = pLevel->GetLayer(i);

		const vector<CGameObject*>& vecObjects = pLayer->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (nullptr == vecObjects[j]->GetRenderComponent()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMesh()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader())
			{
				continue;
			}

			// 절두체 검사를 진행 함, 실패 함
			/*if ( vecObjects[j]->GetRenderComponent()->IsFrustumCheck()
				&& false == m_Frustum->FrustumCheck(vecObjects[j]->Transform()->GetWorldPos()
												  , vecObjects[j]->Transform()->GetWorldScale().x / 2.f))
			{
				continue;
			}*/

			m_vecShadowMap.push_back(vecObjects[j]);
		}
	}
}

void CCamera::render_shadowmap()
{
	for (size_t i = 0; i < m_vecShadowMap.size(); ++i)
	{
		m_vecShadowMap[i]->GetRenderComponent()->render_shadowmap();
	}

	m_vecShadowMap.clear();
}



void CCamera::SaveToFile(FILE* _File)
{
	fwrite(&m_Priority, sizeof(int), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_Height, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_ProjectionScale, sizeof(float), 1, _File);
}

void CCamera::LoadFromFile(FILE* _File)
{
	fread(&m_Priority, sizeof(int), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_Height, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_ProjectionScale, sizeof(float), 1, _File);
}