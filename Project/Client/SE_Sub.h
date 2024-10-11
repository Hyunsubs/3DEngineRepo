#pragma once
#include "EditorUI.h"

#include "SpriteEditor.h"
#include <Engine/CAssetMgr.h>

class SE_Sub :
    public EditorUI
{
private:
    SpriteEditor* m_Owner;

public:
    SpriteEditor* GetOwner() { return m_Owner; }
    class SE_Detail* GetDetail() { return m_Owner->GetDetail(); }
    class SE_AtlasView* GetAtlasView() { return m_Owner->GetAtlasView(); }


public:
    SE_Sub();
    ~SE_Sub();

    friend class SpriteEditor;
};

