#include "pch.h"
#include "CRenderMgr.h"

#include "CAssetMgr.h"
#include "CGameObject.h"
#include "components.h"
#include "assets.h"

#include "CMRT.h"


void CRenderMgr::Init()
{
	// AssetMgr 가 초기화될때 만들어둔 후처리용 텍스쳐를 참조한다.
	m_PostProcessTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");

	// 디버그 렌더링용 게임 오브젝트
	m_DebugObject = new CGameObject;
	m_DebugObject->AddComponent(new CTransform);
	m_DebugObject->AddComponent(new CMeshRender);
	m_DebugObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));

	// MRT 생성
	CreateMRT();
}

void CRenderMgr::CreateMRT()
{
	CMRT* pMRT = nullptr;

	// =============
	// SwapChain MRT
	// =============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex"), };
		Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		pMRT = new CMRT;
		pMRT->SetName(L"SwapChain");
		pMRT->Create(1, arrRT, pDSTex);
		pMRT->SetClearColor(arrClearColor, false);

		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] = pMRT;
	}

	// =============
	// Effect MRT
	// =============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectTargetTex"), };
		Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectDepthStencilTex");
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		pMRT = new CMRT;
		pMRT->SetName(L"Effect");
		pMRT->Create(1, arrRT, pDSTex);
		pMRT->SetClearColor(arrClearColor, false);

		m_arrMRT[(UINT)MRT_TYPE::EFFECT] = pMRT;
	}

	// ===============
	// EffectBlur MRT
	// ===============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectBlurTargetTex"), };
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		pMRT = new CMRT;
		pMRT->SetName(L"EffectBlur");
		pMRT->Create(1, arrRT, nullptr);
		pMRT->SetClearColor(arrClearColor, false);

		m_arrMRT[(UINT)MRT_TYPE::EFFECT_BLUR] = pMRT;
	}
}

