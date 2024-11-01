#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"


CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_RelativeDir{}
	, m_WorldDir{}
	, m_IndependentScale(false)
{
}

CTransform::~CTransform()
{
}



void CTransform::FinalTick()
{
	// 오브젝트의 월드행렬 계산
	// 크기행렬
	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);

	// 이동 행렬
	Matrix matTranslation = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);

	// 회전 행렬
	Matrix matRot = XMMatrixRotationX(m_RelativeRotation.x) 
		          * XMMatrixRotationY(m_RelativeRotation.y) 
		          * XMMatrixRotationZ(m_RelativeRotation.z);

	m_matWorld = matScale * matRot * matTranslation;


	// 방향벡터 계산
	static Vec3 vDefaultAxis[3] =
	{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};
		
	for (int i = 0; i < 3; ++i)
	{		
		m_RelativeDir[i] = XMVector3TransformNormal(vDefaultAxis[i], matRot);
		m_RelativeDir[i].Normalize();
	}


	// 부모 오브젝트가 있는지 확인
	if (GetOwner()->GetParent())
	{
		// 부모의 월드행렬을 곱해서 최종 월드행렬을 계산함
		const Matrix& matParentWorldMat = GetOwner()->GetParent()->Transform()->GetWorldMat();

		if (m_IndependentScale)
		{
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetWorldScale();
			Matrix matParentScale = XMMatrixScaling(vParentScale.x, vParentScale.y, vParentScale.z);
			Matrix matParentScaceInv = XMMatrixInverse(nullptr, matParentScale);

			m_matWorld = m_matWorld * matParentScaceInv * matParentWorldMat;
		}
		else
		{
			m_matWorld *= matParentWorldMat;
		}
			
		// 최종 월드기준 오브젝트의 방향벡터를 구함
		for (int i = 0; i < 3; ++i)
		{	
			m_WorldDir[i] = XMVector3TransformNormal(vDefaultAxis[i], m_matWorld);
		}
	}

	// 부모가 없으면, RelativeDir 이 곧 WorldDir
	else
	{
		for (int i = 0; i < 3; ++i)
		{			
			m_WorldDir[i] = m_RelativeDir[i];			
		}
	}

	// 월드 역행렬 계산
	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::Binding()
{
	g_Trans.matWorld = m_matWorld;	
	g_Trans.matWorldInv = m_matWorldInv;

	g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
	g_Trans.matWVP  = g_Trans.matWV * g_Trans.matProj;	

	CConstBuffer* pTransformCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pTransformCB->SetData(&g_Trans);
	pTransformCB->Binding();
}


Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = Vec3(1.f, 1.f, 1.f);

	CGameObject* pObject = GetOwner();

	while (pObject)
	{
		vWorldScale *= pObject->Transform()->GetRelativeScale();

		if (pObject->Transform()->m_IndependentScale)
			break;

		pObject = pObject->GetParent();
	}

	return vWorldScale;
}

CTransform& CTransform::operator=(const CTransform& _Other)
{
	m_RelativePos = _Other.m_RelativePos;
	m_RelativeScale = _Other.m_RelativeScale;
	m_RelativeRotation = _Other.m_RelativeRotation;
	m_matWorldInv = _Other.m_matWorldInv;
	m_IndependentScale = _Other.m_IndependentScale;

	for (int i = 0; i < 3; i++)
	{
		m_RelativeDir[i] = _Other.m_RelativeDir[i];
		m_WorldDir[i] = _Other.m_WorldDir[i];
	}

	return *(this);
}

void CTransform::SaveToFile(FILE* _File)
{
	fwrite(&m_RelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeRotation, sizeof(Vec3), 1, _File);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _File);
}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_RelativePos, sizeof(Vec3), 1, _File);
	fread(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_RelativeRotation, sizeof(Vec3), 1, _File);
	fread(&m_IndependentScale, sizeof(bool), 1, _File);
}