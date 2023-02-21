#include "pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\Component.h"


USING(Engine)

CComponent::CComponent()
{
	m_tag = "";
	m_bEnable = true;
}

CComponent::~CComponent()
{
}

// Basic Update Function
void CComponent::Update(const _float& dt)
{
}

// Basic Render Function
void CComponent::Render()
{
}

// Set this component enabled
void CComponent::Enable()
{
	m_bEnable = true;
}

// Set this component disabled
void CComponent::Disable()
{
	m_bEnable = false;
}

// Call instead of destructor to manage class internal data
void CComponent::Destroy()
{
}

// Set component tag
void CComponent::SetTag(std::string componentTag)
{
	m_tag = componentTag;
}
