#pragma once
#include "CRenderComponent.h"

#include "CParticleTickCS.h"

class CStructuredBuffer;

struct tSpawnCount
{
    UINT    SpawnCount;
    UINT    padding[3];
};

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleTickCS>    m_TickCS;
    CStructuredBuffer*      m_ParticleBuffer;       // 모든 파티클 정보
    CStructuredBuffer*      m_SpawnCountBuffer;     // 파티클 활성화 숫자 전달용 버퍼
    CStructuredBuffer*      m_ModuleBuffer;         // Module Data Buffer

    Ptr<CTexture>           m_ParticleTex;
    float                   m_Time;                 // 누적시간    
    float                   m_BurstTime;            // SpawnBurst 체크용 변수

    int                     m_MaxParticeCount;      // 파티클 최대 개수

    tParticleModule         m_Module;               // 파티클의 기능 정의





public:
    void SetParticleTexture(Ptr<CTexture> _Texture) { m_ParticleTex = _Texture; }

public:
    virtual void FinalTick() override;
    virtual void Render() override;

    void CaculateSpawnCount();

    virtual void SaveToFile(FILE* _File) override;   // 파일에 컴포넌트의 정보를 저장
    virtual void LoadFromFile(FILE* _File) override; // 파일에 컴포넌트의 정보를 로드

public:
    CLONE(CParticleSystem);
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _Other);
    ~CParticleSystem();
};

