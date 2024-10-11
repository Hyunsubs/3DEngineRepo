#include "pch.h"
#include "PrefabUI.h"

void PrefabUI::Update()
{
	Title();

}

PrefabUI::PrefabUI()
	: AssetUI(ASSET_TYPE::PREFAB)
{
}

PrefabUI::~PrefabUI()
{
}
