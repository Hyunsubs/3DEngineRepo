#include "pch.h"
#include "FlipBookUI.h"

void FlipBookUI::Update()
{
	Title();

}

FlipBookUI::FlipBookUI()
	: AssetUI(ASSET_TYPE::FLIPBOOK)
{
}

FlipBookUI::~FlipBookUI()
{
}
