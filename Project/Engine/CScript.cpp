#include "pch.h"
#include "CScript.h"



CScript::CScript(UINT _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_ScriptType(_ScriptType)
{
}

CScript::~CScript()
{

}

void CScript::Instantiate(Ptr<CPrefab> _Pref, int _LayerIdx, Vec3 _WorldPos, const wstring& _Name)
{
	CGameObject* pInst = _Pref->Instantiate();

	pInst->SetName(_Name);
	pInst->Transform()->SetRelativePos(_WorldPos);

	CreateObject(pInst, _LayerIdx);
}