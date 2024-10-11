#include "pch.h"
#include "SpriteEditor.h"


#include "CEditorMgr.h"
#include "SE_AtlasView.h"
#include "SE_Detail.h"

SpriteEditor::SpriteEditor()
	: m_AtlasView(nullptr)
	, m_Detail(nullptr)
{
	UseMenuBar(true);
}

SpriteEditor::~SpriteEditor()
{
}

void SpriteEditor::Init()
{
	m_AtlasView = (SE_AtlasView*)CEditorMgr::GetInst()->FindEditorUI("SE_AtlasView");
	m_Detail = (SE_Detail*)CEditorMgr::GetInst()->FindEditorUI("SE_Detail");

	m_AtlasView->SetMove(false);
	m_Detail->SetMove(false);

	m_AtlasView->m_Owner = this;
	m_Detail->m_Owner = this;

	m_Detail->SetAtlasTex(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\link.png", L"texture\\link.png"));
}

void SpriteEditor::Update()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			bool AtlasView = m_AtlasView->IsActive();
			bool Detail = m_Detail->IsActive();

			if (ImGui::MenuItem("AtlasView", nullptr, &AtlasView))
			{
				m_AtlasView->SetActive(AtlasView);
			}

			if (ImGui::MenuItem("Detail", nullptr, &Detail))
			{
				m_Detail->SetActive(Detail);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void SpriteEditor::Activate()
{
	m_AtlasView->SetActive(true);
	m_Detail->SetActive(true);
}

void SpriteEditor::Deactivate()
{
	m_AtlasView->SetActive(false);
	m_Detail->SetActive(false);
}
