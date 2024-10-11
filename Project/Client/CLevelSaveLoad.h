#pragma once


class CLevelSaveLoad
{
public:
	static void SaveLevel(const wstring& _FilePath, CLevel* _Level);
	static void SaveGameObject(FILE* _File, CGameObject* _Object);

	static class CLevel* LoadLevel(const wstring& _FilePath);
	static class CGameObject* LoadGameObject(FILE* _File);
	static class CComponent* GetComponent(COMPONENT_TYPE _Type);
};

