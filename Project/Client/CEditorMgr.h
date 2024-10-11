#pragma once

#include <Engine/singleton.h>

class CGameObject;
class EditorUI;

class CEditorMgr :
    public CSingleton<CEditorMgr>
{
    SINGLE(CEditorMgr);
private:
    vector<CGameObject*>        m_vecEditorObject;
    map<string, EditorUI*>      m_mapUI;

    HANDLE                      m_hNotifyHandle;
    

public:
    EditorUI* FindEditorUI(const string& Name);


public:
    void Init();
    void Tick();

private:
    void ShortCut();

    void CreateEditorObject();
    void EditorObjectProgress();

    void InitImGui();
    void ObserveContent();
    void ImGuiProgress();
    void ImGuiTick();
    void CreateEditorUI();
};

