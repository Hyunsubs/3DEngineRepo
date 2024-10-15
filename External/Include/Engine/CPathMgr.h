#pragma once

class CPathMgr
	: public CSingleton<CPathMgr>
{
	SINGLE(CPathMgr);
private:
	wstring		m_Content;
	wstring		m_Solution;


public:
	void Init();
	void render();

private:
	void GetParentPath(_Inout_ wchar_t* _Buffer);


public:
	const wstring& GetContentPath() { return m_Content; }
	const wstring& GetSolutionPath() { return m_Solution; }	
	wstring GetRelativePath(const wstring& _FilePath);
};

