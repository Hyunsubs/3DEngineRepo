#include "pch.h"
#include "Inspector.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include "TransformUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "CameraUI.h"
#include "MeshRenderUI.h"
#include "FlipBookComUI.h"
#include "TileMapUI.h"
#include "ParticleSystemUI.h"

#include "MeshUI.h"
#include "MeshDataUI.h"
#include "TextureUI.h"
#include "PrefabUI.h"
#include "TextureUI.h"
#include "MaterialUI.h"
#include "SpriteUI.h"
#include "FlipBookUI.h"
#include "SoundUI.h"
#include "GraphicShaderUI.h"
#include "ComputeShaderUI.h"

#include "ScriptUI.h"

void Inspector::Init()
{
	CreateComponentUI();

	CreateAssetUI();


	if (nullptr == m_TargetObject)
	{
		SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(L"Player"));
		//SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(L"MainCamera"));
		//SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(L"PointLight 1"));
		return;
	}
}

void Inspector::CreateComponentUI()
{
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetName("TransformUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetChildSize(ImVec2(0.f, 130.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetName("Collider2DUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetChildSize(ImVec2(0.f, 100.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetName("Light2DUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetChildSize(ImVec2(0.f, 200.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetName("CameraUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetChildSize(ImVec2(0.f, 200.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)COMPONENT_TYPE::FLIPBOOKCOMPONENT] = new FlipBookComUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::FLIPBOOKCOMPONENT]->SetName("FlipBookComUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::FLIPBOOKCOMPONENT]->SetChildSize(ImVec2(0.f, 100.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::FLIPBOOKCOMPONENT]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetName("MeshRenderUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetChildSize(ImVec2(0.f, 100.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetName("TileMapUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetChildSize(ImVec2(0.f, 100.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]);

	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new ParticleSystemUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetName("ParticleSystemUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetChildSize(ImVec2(0.f, 100.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]);
}

void Inspector::CreateScriptUI(UINT _Count)
{
	for (UINT i = 0; i < _Count; ++i)
	{
		ScriptUI* pScriptUI = new ScriptUI;
		
		char szScriptUIName[255] = {};
		sprintf_s(szScriptUIName, 255, "ScriptUI##%d", (int)m_vecScriptUI.size());
		pScriptUI->SetName(szScriptUIName);

		AddChild(pScriptUI);
		m_vecScriptUI.push_back(pScriptUI);
	}
}

void Inspector::CreateAssetUI()
{
	AssetUI* UI = nullptr;

	UI = new MeshUI;
	UI->SetName("MeshUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH] = UI;

	UI = new MeshDataUI;
	UI->SetName("MeshDataUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH_DATA] = UI;

	UI = new MaterialUI;
	UI->SetName("MaterialUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::MATERIAL] = UI;

	UI = new PrefabUI;
	UI->SetName("PrefabUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::PREFAB] = UI;

	UI = new TextureUI;
	UI->SetName("TextureUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::TEXTURE] = UI;

	UI = new SpriteUI;
	UI->SetName("SpriteUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::SPRITE] = UI;

	UI = new FlipBookUI;
	UI->SetName("FlipBookUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::FLIPBOOK] = UI;

	UI = new SoundUI;
	UI->SetName("SoundUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::SOUND] = UI;

	UI = new GraphicShaderUI;
	UI->SetName("GraphicShaderUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::GRAPHIC_SHADER] = UI;

	UI = new ComputeShaderUI;
	UI->SetName("ComputeShaderUI");
	AddChild(UI);
	m_arrAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = UI;
}