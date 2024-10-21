#include "pch.h"
#include "CRenderMgr.h"

#include "CAssetMgr.h"
#include "CGameObject.h"
#include "components.h"
#include "assets.h"

#include "CDevice.h"
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

		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetName(L"SwapChain");
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(1, arrRT, pDSTex);
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetClearColor(arrClearColor, false);
	}

	// =============
	// Effect MRT
	// =============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectTargetTex"), };
		Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectDepthStencilTex");
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::EFFECT] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::EFFECT]->SetName(L"Effect");
		m_arrMRT[(UINT)MRT_TYPE::EFFECT]->Create(1, arrRT, pDSTex);
		m_arrMRT[(UINT)MRT_TYPE::EFFECT]->SetClearColor(arrClearColor, false);

	}

	// ===============
	// EffectBlur MRT
	// ===============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectBlurTargetTex"), };
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::EFFECT_BLUR] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::EFFECT_BLUR]->SetName(L"EffectBlur");
		m_arrMRT[(UINT)MRT_TYPE::EFFECT_BLUR]->Create(1, arrRT, nullptr);
		m_arrMRT[(UINT)MRT_TYPE::EFFECT_BLUR]->SetClearColor(arrClearColor, false);
	}
	// ========
	// Deferred
	// ========
	{
		Vec2 vResolution = CDevice::GetInst()->GetResolution();

		Ptr<CTexture> arrRT[8] =
		{
			CAssetMgr::GetInst()->CreateTexture(L"AlbedoTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R8G8B8A8_UNORM
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"NormalTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"PositionTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"EmissiveTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"DataTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
		};
		Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->SetName(L"Deferred");
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->Create(5, arrRT, pDSTex);
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->SetClearColor(arrClearColor, false);
	}

	// =====
	// LIGHT
	// =====
	{
		Vec2 vResolution = CDevice::GetInst()->GetResolution();

		Ptr<CTexture> arrRT[8] =
		{
			CAssetMgr::GetInst()->CreateTexture(L"DiffuseTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
			CAssetMgr::GetInst()->CreateTexture(L"SpecularTargetTex"
											, (UINT)vResolution.x, (UINT)vResolution.y
											, DXGI_FORMAT_R32G32B32A32_FLOAT
											, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE),
		};
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		m_arrMRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->SetName(L"Light");
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->Create(2, arrRT, pDSTex);
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->SetClearColor(arrClearColor, false);
	}
}

void CRenderMgr::ClearMRT()
{
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Clear();
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->ClearRT();
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->ClearRT();
}
