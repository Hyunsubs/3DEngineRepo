#include "pch.h"
#include "CTimeMgr.h"

#include "CKeyMgr.h"

#include "CEngine.h"
#include "CLevelMgr.h"
#include "CLevel.h"

#include "CFontMgr.h"

CTimeMgr::CTimeMgr()
	: m_llCurCount{}
	, m_llPrevCount{}
	, m_llFrequency{}
	, m_FPS(0)
	, m_DeltaTime(0.f)
	, m_Time(0.f)
	, m_E_DeltaTime(0.f)
	, m_E_Time(0.f)
	, m_TimeInfo{}
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Init()
{
	// 초당 1000 을 카운팅하는 GetTickCount 함수는 미세한 시간을 측정하기에는 정확도가 떨어진다.

	// 1초에 셀수있는 카운트 기준을 얻는다.
	QueryPerformanceFrequency(&m_llFrequency);

	QueryPerformanceCounter(&m_llCurCount);
	m_llPrevCount = m_llCurCount;	
}

void CTimeMgr::Tick()
{
	// 현재 카운트 계산
	QueryPerformanceCounter(&m_llCurCount);

	// 이전 카운트와 현재 카운트의 차이값을 통해서 1프레임 간의 시간값을 계산
	m_E_DeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	// DT 보정
	if (1.f / 60.f < m_E_DeltaTime)
		m_E_DeltaTime = 1.f / 60.f;

	// 누적시간을 통해서 프로그램이 실행된 이후로 지나간 시간값을 기록
	m_E_Time += m_E_DeltaTime;

	// 현재 카운트 값을 이전 카운트로 복사해둠
	m_llPrevCount = m_llCurCount;

	// 초당 실행 횟수(FPS) 계산
	++m_FPS;

	// 1초에 한번씩 TextOut 출력
	static float AccTime = 0.f;
	AccTime += m_E_DeltaTime;
	
	if (1.f < AccTime)
	{		
		swprintf_s(m_TimeInfo, L"DeltaTime : %f, FPS : %d ", m_E_DeltaTime, m_FPS);
		//TextOut(CEngine::GetInst()->GetMainDC(), 10, 10, szBuff, wcslen(szBuff));		
		AccTime = 0.f;
		m_FPS = 0;
	}

	// Level 용 DT
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel || pCurLevel->GetState() != LEVEL_STATE::PLAY)
	{
		m_DeltaTime = 0.f;	
	}
	else
	{
		m_DeltaTime = m_E_DeltaTime;
	}

	m_Time += m_DeltaTime;


	// GlobalData
	g_GlobalData.g_DT = m_DeltaTime;
	g_GlobalData.g_EngineDT = m_E_DeltaTime;
	g_GlobalData.g_Time = m_Time;
	g_GlobalData.g_EngineTime = m_E_Time;

	if (KEY_PRESSED(KEY::RBTN))
	{
		g_GlobalData.g_EngineDT = 0.f;
	}
}

void CTimeMgr::Render()
{
	CFontMgr::GetInst()->DrawFont(m_TimeInfo, 10, 20, 16, FONT_RGBA(255, 20, 20, 255));
}
