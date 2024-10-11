#pragma once
#include "CEntity.h"
class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
private:
    CLayer*         m_Layer[MAX_LAYER];
    LEVEL_STATE     m_State;


public:
    void AddObject(int LayerIdx, CGameObject* _Object, bool _bMoveChild = false);
    void RegisterAsParent(int LayerIdx, CGameObject* _Object);

    CLayer* GetLayer(int _LayerIdx) { return m_Layer[_LayerIdx]; }
    LEVEL_STATE GetState() { return m_State; }
    CGameObject* FindObjectByName(const wstring& _Name);

private:
    void ChangeState(LEVEL_STATE _NextState);

public:
    void Begin();   // 레벨이 시작되는 순간 or 레벨에 추가(합류) 하는 순간
    void Tick();
    void FinalTick();   
    void ClearObject();


public:
    virtual CLevel* Clone() { return new CLevel(*this); }
    CLevel();
    CLevel(const CLevel& _Origin);
    ~CLevel();

    friend class CTaskMgr;
};

