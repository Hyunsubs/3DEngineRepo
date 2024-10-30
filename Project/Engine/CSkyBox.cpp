#include "pch.h"
#include "CSkyBox.h"

#include "CTransform.h"

#include "CAssetMgr.h"


CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(SPHERE)
{
	SetSkyBoxType(m_Type);
	SetFrustumCheck(false);
}

CSkyBox::CSkyBox(const CSkyBox& _Origin)
	: CRenderComponent(_Origin)
	, m_Type(_Origin.m_Type)
	, m_SkyBoxTex(_Origin.m_SkyBoxTex)
{
	SetSkyBoxType(m_Type);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::SetSkyBoxType(SKYBOX_TYPE _Type)
{
	m_Type = _Type;

	if (SKYBOX_TYPE::SPHERE == m_Type)
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
	else if(SKYBOX_TYPE::CUBE == m_Type)
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));

	// Mesh �� ����Ǹ� ������ �ٽ� �����ؾ� �Ѵ�.
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyBoxMtrl"));
}

void CSkyBox::FinalTick()
{
	// 1. ���� ī�޶� �� �ִ� �þ߰Ÿ���, Transform �� Scale �� �����Ѵ�.
	//  - Play -> Main Cam
	//  - Pause, Stop -> Editor Cam

	// 2. SkyBox ��ġ�� ���� ī�޶� ��ġ�� �ű��.

}

void CSkyBox::Render()
{
	Transform()->Binding();

	GetMaterial()->SetScalarParam(INT_0, (int)m_Type);

	if (m_Type == SPHERE)
	{
		if(!m_SkyBoxTex->IsCubeMap())
			GetMaterial()->SetTexParam(TEX_0, m_SkyBoxTex);
		else
			GetMaterial()->SetTexParam(TEX_0, nullptr);
	}
	
	else if (m_Type == CUBE)
	{
		if (m_SkyBoxTex->IsCubeMap())
			GetMaterial()->SetTexParam(TEXCUBE_0, m_SkyBoxTex);
		else
			GetMaterial()->SetTexParam(TEXCUBE_0, nullptr);
	}
	
	GetMaterial()->Binding();

	GetMesh()->Render();
}

void CSkyBox::SaveToFile(FILE* _File)
{

}

void CSkyBox::LoadFromFile(FILE* _File)
{

}
