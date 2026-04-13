#include "pch.h"
#include "CFlipbookRender.h"

#include "AssetMgr.h"
#include "TimeMgr.h"

CFlipbookRender::CFlipbookRender()
	: CRenderComponent(COMPONENT_TYPE::FLIPBOOK_RENDER)
	, m_FPS(0.f)
	, m_AccTime(0.f)
	, m_CurSprite(0)
	, m_CurFlipbook(0)
	, m_Finish(false)
	, m_RepeatCount(0)
{
}

CFlipbookRender::~CFlipbookRender()
{
}

void CFlipbookRender::FinalTick()
{
	if (CheckFinish())
		return;

	// 안전 체크: 인덱스 범위 확인 및 nullptr 확인
	if (m_CurFlipbook < 0 || m_CurFlipbook >= (int)m_vecFlipbook.size())
		return;

	Ptr<AFlipbook> curFlipbook = m_vecFlipbook[m_CurFlipbook];
	if (nullptr == curFlipbook)
		return;

	// 기존 로직
	if (curFlipbook->GetSpriteCount() == 1)
	{
		m_CurSprite = 0; // 무조건 0
		m_Finish = true; // 한 장이면 바로 끝 처리
	}
	else
	{
		float fLimit = 1.f / m_FPS;
		m_AccTime += DT;

		if (fLimit < m_AccTime)
		{
			m_AccTime -= fLimit;
			++m_CurSprite;

			if (curFlipbook->GetSpriteCount() <= m_CurSprite)
			{
				m_Finish = true;
				--m_CurSprite;
			}
		}
	}
}

void CFlipbookRender::Render()
{
	// 안전 체크: 인덱스 범위와 nullptr 확인
	if (m_CurFlipbook < 0 || m_CurFlipbook >= (int)m_vecFlipbook.size())
		return;

	Ptr<AFlipbook> pCurFlipbook = m_vecFlipbook[m_CurFlipbook];
	if (nullptr == pCurFlipbook)
		return;

	// 현재 스프라이트 체크
	if (m_CurSprite < 0 || m_CurSprite >= pCurFlipbook->GetSpriteCount())
		return;

	Ptr<ASprite> pCurSprite = pCurFlipbook->GetSprite(m_CurSprite);
	if (nullptr == pCurSprite)
		return;

	if (m_CurFlipbook >= 0 && m_CurFlipbook < (int)m_vecRenderData.size())
	{
		const FlipbookRenderData& data = m_vecRenderData[m_CurFlipbook];
		SetRenderOffset(data.Offset);
		SetRenderScale(data.Scale);
	}

	// Material과 Mesh 바인딩
	if (nullptr != GetMaterial())
	{
		GetMaterial()->SetTexture(TEX_0, pCurSprite->GetAtlas());
		GetMaterial()->SetScalar(VEC2_0, pCurSprite->GetLeftTopUV());
		GetMaterial()->SetScalar(VEC2_1, pCurSprite->GetSliceUV());
		GetMaterial()->SetScalar(VEC2_2, pCurSprite->GetBackgroundUV());
		GetMaterial()->SetScalar(VEC2_3, pCurSprite->GetOffsetUV());

		
		// Render 전용 Offset 및 Scale 값 세팅
		GetMaterial()->SetScalar(VEC4_3, Vec4(
			GetRenderOffset().x, GetRenderOffset().y,
			GetRenderScale().x, GetRenderScale().y));

		GetMaterial()->Binding();
	}

	if (nullptr != GetMesh())
		GetMesh()->Render();
}

void CFlipbookRender::CreateMaterial()
{
	wstring MeshName = L"RectMesh";
	wstring MtrlName = L"FlipbookMtrl";
	wstring ShaderName = L"FlipbookShader";
	wstring FilePath = L"Shader\\flipbook.fx";
	string VS = "VS_Flipbook";
	string PS = "PS_Flipbook";

	// RectMesh 설정
	SetMesh(AssetMgr::GetInst()->Find<AMesh>(MeshName));

	// 재질 생성
	Ptr<AMaterial> pMtrl = AssetMgr::GetInst()->Find<AMaterial>(MtrlName);

	// 찾는 재질이 없으면 생성한다.
	if (nullptr == pMtrl)
	{
		pMtrl = new AMaterial;
		pMtrl->SetName(MtrlName);

		// 쉐이더를 찾아서 재질에 세팅해준다.
		Ptr<AGraphicShader> pShader = AssetMgr::GetInst()->Find<AGraphicShader>(ShaderName);

		// 찾은 or 생성한 쉐이더를 재질에 설정해주고, 재질도 에셋매니저에 등록한다.
		pMtrl->SetShader(pShader);
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
		AssetMgr::GetInst()->AddAsset(pMtrl->GetName(), pMtrl.Get());
	}

	SetMaterial(pMtrl);
}


bool CFlipbookRender::CheckFinish()
{
	if (m_Finish)
	{
		if (0 < m_RepeatCount)
		{
			m_CurSprite = 0;
			m_Finish = false;
			--m_RepeatCount;
			return false;
		}
		else if (-1 == m_RepeatCount)
		{
			m_CurSprite = 0;
			m_Finish = false;
			return false;
		}
		else
		{
			return true;
		}
	}

	return m_Finish;
}


void CFlipbookRender::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);

	size_t FlipbookCount = m_vecFlipbook.size();
	fwrite(&FlipbookCount, sizeof(size_t), 1, _File);

	for (const auto& Flipbook : m_vecFlipbook)
	{
		SaveAssetRef(_File, Flipbook.Get());
	}

	fwrite(&m_CurFlipbook, sizeof(int), 1, _File);
	fwrite(&m_CurSprite, sizeof(int), 1, _File);
	fwrite(&m_FPS, sizeof(int), 1, _File);
	fwrite(&m_RepeatCount, sizeof(int), 1, _File);

	// Save
	size_t count = m_vecRenderData.size();
	fwrite(&count, sizeof(size_t), 1, _File);
	fwrite(m_vecRenderData.data(), sizeof(FlipbookRenderData), count, _File);
}

void CFlipbookRender::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	size_t FlipbookCount = 0;
	fread(&FlipbookCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < FlipbookCount; ++i)
	{
		m_vecFlipbook.push_back(LoadAssetRef<AFlipbook>(_File));
	}

	fread(&m_CurFlipbook, sizeof(int), 1, _File);
	fread(&m_CurSprite, sizeof(int), 1, _File);
	fread(&m_FPS, sizeof(int), 1, _File);
	fread(&m_RepeatCount, sizeof(int), 1, _File);

	// Load
	size_t count = 0;
	fread(&count, sizeof(size_t), 1, _File);
	m_vecRenderData.resize(count);
	fread(m_vecRenderData.data(), sizeof(FlipbookRenderData), count, _File);
}