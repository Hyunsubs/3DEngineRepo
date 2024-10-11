#pragma once
#include "CAsset.h"

class CSprite :
    public CAsset
{
private:
    Ptr<CTexture>   m_Atlas;
    Vec2            m_LeftTopUV;      // UV ÁÂÇ¥°è
    Vec2            m_SliceUV;        // UV ÁÂÇ¥°è
    Vec2            m_BackgroundUV; 
    Vec2            m_OffsetUV;


public:
    Ptr<CTexture> GetAtlasTexture() { return m_Atlas; }
    
    void SetLeftTop(Vec2 _LeftTop);
    void SetSlice(Vec2 _Slice);
    void SetBackground(Vec2 _Background);
    void SetOffset(Vec2 _Offset);

    Vec2 GetLeftTopUV() { return m_LeftTopUV; }
    Vec2 GetSliceUV() { return m_SliceUV; }
    Vec2 GetBackgroundUV() { return m_BackgroundUV; }
    Vec2 GetOffsetUV() { return m_OffsetUV; }


public:
    void Create(Ptr<CTexture> _Atlas, Vec2 _LeftTopPixel, Vec2 _SlicePixel);

    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;


public:    
    CSprite();
    ~CSprite();
};

