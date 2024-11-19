#include "pch.h"
#include "CRenderComponent.h"

#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"

#include "CTransform.h"


CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
	, m_Mesh(nullptr)
	, m_Mtrl(nullptr)
	, m_FrustumCheck(true)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Origin)
	: CComponent(_Origin)
	, m_Mesh(_Origin.m_Mesh)
	, m_Mtrl(_Origin.m_Mtrl)
	, m_SharedMtrl(_Origin.m_Mtrl)
	, m_FrustumCheck(_Origin.m_FrustumCheck)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr != pCurLevel)
	{
		assert(!(pCurLevel->GetState() != LEVEL_STATE::PLAY 
			     && nullptr != _Origin.m_DynamicMtrl));
	}

	if (nullptr != _Origin.m_DynamicMtrl)
	{
		GetDynamicMaterial();
	}
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::render_shadowmap()
{
	// ������ ShadowMapMtrl �� �̹� Binding �Ǿ��ִ°� ����� ��
	// �ڽ��� ������ Mesh �� ������ ��û�� �ϸ� �ȴ�.
	Transform()->Binding();
	GetMesh()->Render();
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
	m_Mtrl = m_SharedMtrl = _Mtrl;	
}

Ptr<CMaterial> CRenderComponent::GetSharedMtrl()
{
	m_Mtrl = m_SharedMtrl;
	return m_Mtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// �������� ���� �� ��ȯ�� ����(����) �� Play ����� ��쿡�� ȣ���� �Ǿ�� �Ѵ�.
	assert(CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY);

	if (nullptr != m_DynamicMtrl)
		return m_Mtrl = m_DynamicMtrl;

	m_Mtrl = m_DynamicMtrl = m_SharedMtrl->Clone();

	return m_Mtrl;
}



void CRenderComponent::SaveDataToFile(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	SaveAssetRef(m_Mtrl, _File);
	SaveAssetRef(m_SharedMtrl, _File);
}

void CRenderComponent::LoadDataFromFile(FILE* _File)
{
	LoadAssetRef(m_Mesh, _File);
	LoadAssetRef(m_Mtrl, _File);
	LoadAssetRef(m_SharedMtrl, _File);
}