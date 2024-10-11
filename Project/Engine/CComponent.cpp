#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _Type)
	: m_Type(_Type)
	, m_Owner(nullptr)
{
}

CComponent::CComponent(const CComponent& _Origin)
	: m_Type(_Origin.m_Type)
	, m_Owner(nullptr)
{
}

CComponent::~CComponent()
{
}

void CComponent::Begin()
{
}

void CComponent::Tick()
{
}
