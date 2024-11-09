#include "pch.h"
#include "MenuUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/CScript.h>
#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>
#include <Engine/CRenderMgr.h>


#include <Scripts/CScriptMgr.h>


#include "CEditorMgr.h"
#include "Inspector.h"
#include "CLevelSaveLoad.h"


MenuUI::MenuUI()
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::Tick()
{
    if (!IsActive())
        return;

    if (ImGui::BeginMainMenuBar())
    {

        Update();

        ImGui::EndMainMenuBar();
    }
}


void MenuUI::Update()
{
	File();

	Level();

	GameObject();

	Assets();	

	RenderTarget();
}

void MenuUI::File()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Level Save"))
		{

		}

		if (ImGui::MenuItem("Level Load"))
		{

		}

		ImGui::EndMenu();
	}
}

void MenuUI::Level()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	ImGui::BeginDisabled(!pCurLevel);

	if (ImGui::BeginMenu("Level"))
	{
		LEVEL_STATE State = LEVEL_STATE::STOP;
		if (nullptr != pCurLevel)
			State = pCurLevel->GetState();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY == State);
		if (ImGui::MenuItem("Play"))
		{			
			if (LEVEL_STATE::STOP == State)
			{
				wstring strLevelPath = CPathMgr::GetInst()->GetContentPath();
				strLevelPath += L"level\\Temp.lv";
				CLevelSaveLoad::SaveLevel(strLevelPath, pCurLevel);
			}
			
			ChangeLevelState(LEVEL_STATE::PLAY);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY != State);
		if (ImGui::MenuItem("Pause"))
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::STOP == State);
		if (ImGui::MenuItem("Stop"))
		{
			wstring StrLevelLoadPath = CPathMgr::GetInst()->GetContentPath();
			StrLevelLoadPath += L"level\\Temp.lv";
			CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(StrLevelLoadPath);
			ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

			// Inspector Clear �ϱ� (���� ������Ʈ ������ �����ְ� ���� ���� �ֱ� ������)				
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			pInspector->SetTargetObject(nullptr);
			pInspector->SetTargetAsset(nullptr);
		}
		ImGui::EndDisabled();

		ImGui::EndMenu();
	}

	ImGui::EndDisabled();
}

void MenuUI::GameObject()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create Empty Object"))
		{

		}

		if (ImGui::BeginMenu("Add Component"))
		{
			ImGui::MenuItem("MeshRender");
			ImGui::MenuItem("Collider2D");
			ImGui::MenuItem("Camera");

			ImGui::EndMenu();
		}

		AddScript();


		ImGui::EndMenu();
	}
}

void MenuUI::AddScript()
{
	if (ImGui::BeginMenu("Add Script"))
	{
		vector<wstring> vecScriptsName;
		CScriptMgr::GetScriptInfo(vecScriptsName);

		for (size_t i = 0; i < vecScriptsName.size(); ++i)
		{			
			if (ImGui::MenuItem(string(vecScriptsName[i].begin(), vecScriptsName[i].end()).c_str()))
			{
				//CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);

				// �ν�����
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");

				// Ÿ�� ������Ʈ �˾Ƴ�
				CGameObject* pObject = pInspector->GetTargetObject();
				if (nullptr != pObject)
				{
					// ������Ʈ��, ������ ��ũ��Ʈ�� �߰�����
					CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);
					pObject->AddComponent(pScript);
				}
			}
		}

		ImGui::EndMenu();
	}

}


void MenuUI::Assets()
{
	if (ImGui::BeginMenu("Assets"))
	{
		if (ImGui::MenuItem("Create Empty Material"))
		{
			Ptr<CMaterial> pMtrl = new CMaterial;						
			wstring Key = GetAssetKey(ASSET_TYPE::MATERIAL, L"Default Material");
			CAssetMgr::GetInst()->AddAsset<CMaterial>(Key, pMtrl);
			pMtrl->Save(Key);
		}



		EditorUI* pSpriteEditor = CEditorMgr::GetInst()->FindEditorUI("SpriteEditor");				
		bool IsActive = pSpriteEditor->IsActive();

		if (ImGui::MenuItem("Sprite Editor", nullptr, &IsActive))
		{			
			CEditorMgr::GetInst()->FindEditorUI("SpriteEditor")->SetActive(IsActive);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::RenderTarget()
{
	if (ImGui::BeginMenu("RenderTarget"))
	{
		Ptr<CTexture> pTarget = CRenderMgr::GetInst()->GetSpecifiedTarget();

		bool IsAlbedo = pTarget == CAssetMgr::GetInst()->FindAsset<CTexture>(L"AlbedoTargetTex");
		bool IsNoraml = pTarget == CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex");
		bool IsPosition = pTarget == CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex");
		bool IsDiffuse = pTarget == CAssetMgr::GetInst()->FindAsset<CTexture>(L"DiffuseTargetTex");
		bool IsSpecular = pTarget == CAssetMgr::GetInst()->FindAsset<CTexture>(L"SpecularTargetTex");

		if (ImGui::MenuItem("Albedo Target", nullptr, &IsAlbedo))
		{
			if (!IsAlbedo)
				CRenderMgr::GetInst()->SetSpecifiedTarget(nullptr);
			else
				CRenderMgr::GetInst()->SetSpecifiedTarget(CAssetMgr::GetInst()->FindAsset<CTexture>(L"AlbedoTargetTex"));
		}
	}

}



wstring MenuUI::GetAssetKey(ASSET_TYPE _Type, const wstring& _KeyFormat)
{
	wstring Key;

	switch (_Type)
	{	
	case ASSET_TYPE::MATERIAL:
	{
		Key = wstring(L"material\\") + _KeyFormat + L" %d.mtrl";
	}	
		break;
	case ASSET_TYPE::PREFAB:
	{
		Key = wstring(L"prefab\\") + _KeyFormat + L" %d.pref";
	}
		break;
	case ASSET_TYPE::SPRITE:
	{
		Key = wstring(L"sprite\\") + _KeyFormat + L" %d.sprite";
	}
		break;
	case ASSET_TYPE::FLIPBOOK:
	{
		Key = wstring(L"flipbook\\") + _KeyFormat + L" %d.flip";
	}
		break;	
	default:
		assert(nullptr);
		break;
	}

	wchar_t szKey[255] = {};
	wstring FilePath = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < 0xffffffff; ++i)
	{		
		swprintf_s(szKey, 255, Key.c_str(), i);
		
		if (false == std::filesystem::exists(FilePath + szKey))
		{
			break;
		}
	}

	return szKey;
}
