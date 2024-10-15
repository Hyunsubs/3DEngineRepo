#pragma once


class CTaskMgr
	: public CSingleton<CTaskMgr>
{
	SINGLE(CTaskMgr)
private:
	vector<tTask>			m_vecTask;
	vector<CGameObject*>	m_GC; // Garbage Collector;

public:
	void Tick();
	void AddTask(const tTask& _Task);

private:
	void ClearGC();
	void ExecuteTask();	
};

