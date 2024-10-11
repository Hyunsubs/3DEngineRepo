#include "pch.h"
#include "CAsset.h"

CAsset::CAsset(ASSET_TYPE _Type)
	: m_Type(_Type)
	, m_RefCount(0)
	, m_Engine(false)
{
}

CAsset::CAsset(const CAsset& _Other)
	: CEntity(_Other)
	, m_Key(_Other.m_Key)
	, m_RelativePath(_Other.m_RelativePath)
	, m_Type(_Other.m_Type)
	, m_RefCount(0)
{
}

CAsset::~CAsset()
{
}
