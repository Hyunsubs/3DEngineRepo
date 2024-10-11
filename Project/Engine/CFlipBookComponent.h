#pragma once
#include "CComponent.h"

#include "CFlipBook.h"

class CFlipBookComponent :
    public CComponent
{
private:
    vector<Ptr<CFlipBook>>  m_vecFlipBook;  // FliBook 컴포넌트가 보유한 모든 FlipBook 목록
    Ptr<CFlipBook>          m_CurFlipBook;  // 현재 재생중인 FlipBook
    Ptr<CSprite>            m_CurFrmSprite; // 현재 재생중인 FlipBook 에서 현재 프레임 인덱스에 해당하는 스프라이트
    int                     m_CurFrmIdx;    // 현재 재생중인 FlipBook 에서 몇번째 Sprite 가 재생중인지 인덱스 기록
    float                   m_FPS;          // 현재 재생중인 FlipBook 의 초당 프레임 진행 수
    float                   m_AccTime;      // 누적 시간값 체크
    bool                    m_Repeat;
    bool                    m_Finish;       // FlipBook 재생이 끝에 도달했는지 여부
    
public:
    virtual void FinalTick() override;

public:
    void AddFlipBook(int _Idx, Ptr<CFlipBook> _Flipbook);
    Ptr<CFlipBook> FindFlipBook(const wstring& _Key);
    void Play(int _FliBookIdx, float _FPS, bool _Repeat);
    Ptr<CSprite> GetCurSprite() { return m_CurFrmSprite; }
    void Reset();
    void Binding();
    static void Clear();
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CFlipBookComponent);
    CFlipBookComponent();
    CFlipBookComponent(CFlipBookComponent& _Origin);
    ~CFlipBookComponent();
};

