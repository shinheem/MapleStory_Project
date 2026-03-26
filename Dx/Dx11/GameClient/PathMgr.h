#pragma once


class PathMgr
	: public singleton<PathMgr>
{
	SINGLE(PathMgr)
private:
	wchar_t		m_ContentPath[255];

public:
	void Init();
	void GetFiles(const wstring& folder, const wstring& ext, vector<wstring>& out);
	const wchar_t* GetContentPath() { return m_ContentPath; }
};

#define CONTENT_PATH wstring(PathMgr::GetInst()->GetContentPath())