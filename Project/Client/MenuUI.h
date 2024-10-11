#pragma once
#include "EditorUI.h"


class MenuUI :
    public EditorUI
{
private:



public:
    virtual void Tick() override;
    virtual void Update() override;

private:
    void File();
    void Level();
    void GameObject();
    void Assets();


private:
    void AddScript();
    
    wstring GetAssetKey(ASSET_TYPE _Type, const wstring& _Format);


public:
    MenuUI();
    ~MenuUI();
};

