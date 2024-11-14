#include "pch.h"
#include "CLandScape.h"

#include "CAssetMgr.h"
#include "CKeyMgr.h"

#include "CTransform.h"

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_FaceX(1)
	, m_FaceZ(1)
	, m_TessLevel(8.f)
	, m_BrushIdx(-1)
	, m_BrushScale(Vec2(0.2f, 0.2f))
	, m_IsHeightMapCreated(false)
{
	SetFace(m_FaceX, m_FaceZ);
	Init();
}

CLandScape::~CLandScape()
{
}

void CLandScape::FinalTick()
{
	// �귯�� �ٲٱ�
	if (KEY_TAP(KEY::NUM7))
	{
		++m_BrushIdx;
		if (m_vecBrush.size() <= m_BrushIdx)
			m_BrushIdx = 0;
	}

	if (m_IsHeightMapCreated && KEY_PRESSED(KEY::LBTN))
	{
		// ���̸� ����
		m_HeightMapCS->SetBrushPos(Vec2(0.5f, 0.5f));
		m_HeightMapCS->SetBrushScale(m_BrushScale);
		m_HeightMapCS->SetHeightMap(m_HeightMap);
		m_HeightMapCS->SetBrushTex(m_vecBrush[m_BrushIdx]);
		m_HeightMapCS->Execute();
	}
}

void CLandScape::Render()
{
	Transform()->Binding();

	// GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);
	

	// ������ �� ����
	GetMaterial()->SetScalarParam(INT_0, m_FaceX);
	GetMaterial()->SetScalarParam(INT_1, m_FaceZ);
	GetMaterial()->SetScalarParam(FLOAT_0, m_TessLevel);

	// ������ ���� ��ų ���̸�
	GetMaterial()->SetTexParam(TEX_0, m_HeightMap);
	
	// ���� ���ε�
	GetMaterial()->Binding();

	// ������
	GetMesh()->Render();
}

void CLandScape::SetFace(int _X, int _Z)
{
	m_FaceX = _X;
	m_FaceZ = _Z;

	CreateMesh();
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl"));
}


void CLandScape::SaveToFile(FILE* _File)
{
}

void CLandScape::LoadFromFile(FILE* _File)
{
}


