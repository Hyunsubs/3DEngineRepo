#include "pch.h"
#include "CTestLevel.h"

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CCollisionMgr.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CMissileScript.h>
#include <Scripts/CCameraMoveScript.h>

#include <Engine/CSetColorCS.h>
#include <Engine/CStructuredBuffer.h>

#include "CLevelSaveLoad.h"

void CTestLevel::CreateTestLevel()
{
	// Material
	Ptr<CMaterial> pStd3DMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3DMtrl");
	Ptr<CMaterial> pStd3D_DeferredMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std3D_DeferredMtrl");
	
	// Level 생성
	CLevel* pLevel = new CLevel;
	
	pLevel->GetLayer(0)->SetName(L"Default");
	pLevel->GetLayer(1)->SetName(L"Background");
	pLevel->GetLayer(2)->SetName(L"Tile");
	pLevel->GetLayer(3)->SetName(L"Player");
	pLevel->GetLayer(4)->SetName(L"Monster");
	pLevel->GetLayer(5)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(6)->SetName(L"MonsterProjectile");

	// 카메라 오브젝트
	CGameObject* CamObj = new CGameObject;
	CamObj->SetName(L"MainCamera");
	CamObj->AddComponent(new CTransform);
	CamObj->AddComponent(new CCamera);
	CamObj->AddComponent(new CCameraMoveScript);

	// 우선순위를 0 : MainCamera 로 설정
	CamObj->Camera()->SetPriority(0);

	// 카메라 레이어 설정 (31번 레이어 제외 모든 레이어를 촬영)
	CamObj->Camera()->SetLayerAll();
	CamObj->Camera()->SetLayer(31, false);
	CamObj->Camera()->SetFar(100000.f);
	CamObj->Camera()->SetProjType(PERSPECTIVE);

	pLevel->AddObject(0, CamObj);

	CGameObject* pObject = nullptr;

	// 3D 광원 추가
	pObject = new CGameObject;
	pObject->SetName(L"Directional Light");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight3D);

	pObject->Transform()->SetRelativePos(-100.f, -300.f, 0.f);
	pObject->Transform()->SetRelativeRotation(XM_PI / 4.f, XM_PI / 4.f, 0.f);

	pObject->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObject->Light3D()->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
	pObject->Light3D()->SetLightAmbient(Vec3(0.f, 0.f, 0.f));
	pObject->Light3D()->SetSpecularCoefficient(0.3f);
	pObject->Light3D()->SetRadius(400.f);

	pLevel->AddObject(0, pObject);


	// SkyBox 추가
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	pSkyBox->Transform()->SetRelativeScale(1000.f, 1000.f, 1000.f);

	//Ptr<CTexture> pSkyBoxTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\skybox\\SkyWater.dds");
	//pSkyBox->SkyBox()->SetSkyBoxTexture(pSkyBoxTex);
	//pSkyBox->SkyBox()->SetSkyBoxType(CUBE);		

	Ptr<CTexture> pSkyBoxTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\skybox\\Sky02.jpg");
	pSkyBox->SkyBox()->SetSkyBoxTexture(pSkyBoxTex);
	pSkyBox->SkyBox()->SetSkyBoxType(SPHERE);		

	pLevel->AddObject(0, pSkyBox);

	// Player 추가
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\LandScapeTexture\\gl1_ground_II_albedo.TGA");
	Ptr<CTexture> pNTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\LandScapeTexture\\gl1_ground_II_normal.TGA");

	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->AddComponent(new CMeshRender);

	pPlayer->Transform()->SetRelativePos(0.f, 1000.f, 0.f);
	pPlayer->Transform()->SetRelativeScale(500.f, 500.f, 500.f);
	pPlayer->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);

	pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
	pPlayer->MeshRender()->SetMaterial(pStd3D_DeferredMtrl);

	pPlayer->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTex);
	pPlayer->MeshRender()->GetMaterial()->SetTexParam(TEX_1, pNTex);
	//pPlayer->MeshRender()->GetMaterial()->SetTexParam(TEXCUBE_0, pSkyBoxTex);	
	pLevel->AddObject(3, pPlayer);

	// Decal Object 추가
	CGameObject* pDecal = new CGameObject;
	pDecal->SetName(L"Decal");
	pDecal->AddComponent(new CTransform);
	pDecal->AddComponent(new CDecal);

	pDecal->Transform()->SetRelativePos(0.f, 0.f, 1000.f);
	pDecal->Transform()->SetRelativeScale(100.f, 100.f, 100.f);

	pDecal->Decal()->SetDecalTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\MagicCircle.png"));
	//pDecal->Decal()->SetEmissiveTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\MagicCircle.png"));

	pLevel->AddObject(3, pDecal);

	// LandScape 추가
	CGameObject* pLandScape = new CGameObject;
	pLandScape->SetName(L"LandScape");
	pLandScape->AddComponent(new CTransform);
	pLandScape->AddComponent(new CLandScape);

	pLandScape->Transform()->SetRelativePos(0, 0.f, 0.f);
	pLandScape->Transform()->SetRelativeScale(1000.f, 3000.f, 1000.f);

	pLandScape->LandScape()->SetFace(16, 16);
	//pLandScape->LandScape()->SetHeightMap(CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\HeightMap_01.jpg"));
	pLandScape->LandScape()->CreateHeightMap(1024, 1024);

	pLevel->AddObject(3, pLandScape);

	ChangeLevel(pLevel, LEVEL_STATE::STOP);

	// 충돌 지정
	CCollisionMgr::GetInst()->CollisionCheck(3, 4); // Player vs Monster
	CCollisionMgr::GetInst()->CollisionCheck(5, 4); // Player Projectile vs Monster	
}

void CTestLevel::CreatePrefab()
{
	/*CGameObject* pProto = new CGameObject;
	pProto->SetName("Missile");
	pProto->AddComponent(new CTransform);
	pProto->AddComponent(new CMeshRender);
	pProto->AddComponent(new CMissileScript);

	pProto->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pProto->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pProto->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->SetProtoObject(pProto);

	CAssetMgr::GetInst()->AddAsset<CPrefab>(L"MissilePref", pPrefab);

	wstring FilePath = CPathMgr::GetInst()->GetContentPath();
	FilePath += L"prefab\\Missile.pref";
	pPrefab->Save(FilePath);*/
}
