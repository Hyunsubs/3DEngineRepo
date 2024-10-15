#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CTaskMgr.h"
#include "components.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _Name)
{
	if (m_CurLevel)
	{
		return m_CurLevel->FindObjectByName(_Name);
	}

	return nullptr;
}

void CLevelMgr::LevelChanged()
{
	CTaskMgr::GetInst()->AddTask(tTask{TASK_TYPE::LEVEL_CHANGED});
}


void CLevelMgr::Init()
{	
}

void CLevelMgr::Progress()
{
	m_LevelChanged = false;
	if (nullptr == m_CurLevel)
		return;
	CGameObject* pSpot = nullptr;
	if (pSpot = FindObjectByName(L"Spot Light 1"))
	{
		DrawDebugRect(pSpot->Transform()->GetWorldPos(), Vec3(10.f, 10.f, 0.f), pSpot->Transform()->GetRelativeRotation(), Vec4(1.f, 0.f, 0.f, 1.f), 0.f, false);
	}

	if (m_CurLevel->GetState() == LEVEL_STATE::PLAY)
	{
		m_CurLevel->Tick();
	}

	m_CurLevel->ClearObject();
	m_CurLevel->FinalTick();
}

bool CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (m_CurLevel == _NextLevel)
		return false;

	if (nullptr != m_CurLevel)
		delete m_CurLevel;

	m_CurLevel = _NextLevel;

	// 레벨이 변경된 이벤트 ON
	LevelChanged();

	return true;
}