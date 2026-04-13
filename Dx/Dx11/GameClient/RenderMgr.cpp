#include "pch.h"
#include "RenderMgr.h"

#include "Device.h"
#include "LevelMgr.h"
#include "AssetMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

RenderMgr::RenderMgr()
	: m_bDebugRender(false)
{

}

RenderMgr::~RenderMgr()
{

}

void RenderMgr::Init()
{
	m_DbgObj = new GameObject;
	m_DbgObj->AddComponent(new CTransform);
	m_DbgObj->AddComponent(new CMeshRender);
	m_DbgObj->MeshRender()->SetMaterial(FIND(AMaterial, L"DbgMtrl"));

	m_Light2DBuffer = new StructuredBuffer;
}

void RenderMgr::Progress()
{
	if (KEY_TAP(KEY::F9))
		m_bDebugRender ? m_bDebugRender = false : m_bDebugRender = true;
	
	// 렌더링 시작전에 할 일
	Render_Start();
	CCamera* activeCam = nullptr;

	if (m_UICam)
	{
		m_UICam->SortObject();
		m_UICam->Render();
	}

	// 🔹 현재 사용할 월드 카메라 선택
	if (LEVEL_STATE::PLAY == LevelMgr::GetInst()->GetLevelState())
		activeCam = m_MainCam.Get();
	else
		activeCam = m_EditorCam.Get();

	if (activeCam)
	{
		activeCam->SortObject();
		activeCam->Render();
	}

	Render_Debug();

	Render_End();
}

void RenderMgr::Render_Start()
{
	// 타겟 설정
	Device::GetInst()->OMSetTarget();

	// 렌더타겟 클리어
	Device::GetInst()->ClearTarget();

	// 등록받은 Light2D 의 광원 정보를 구조화버퍼에 담는다.
	// 구조화버퍼를 특정 t 레지스터에 바인딩 한다.
	vector<Light2DInfo> vecInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecInfo.push_back(m_vecLight2D[i]->GetInfo());
	}

	// 등록된 광원이 최소 1개 이상인 경우에만 데이터를 구조화버퍼로 보낸다.
	if (!vecInfo.empty())
	{
		// 구조화버퍼 공간이 모자라면 재확장 및 데이터 전달
		if (vecInfo.size() > m_Light2DBuffer->GetElementCount())
			m_Light2DBuffer->Create(sizeof(Light2DInfo), vecInfo.size(), SB_TYPE::SRV_ONLY, true, vecInfo.data());

		// 공간이 여유가 있으면 바로 광원데이터 전달
		else
			m_Light2DBuffer->SetData(vecInfo.data(), sizeof(Light2DInfo) * vecInfo.size());

		// t12 레지스터로 바인딩
		m_Light2DBuffer->Binding(12);
	}

	g_Global.Light2DCount = m_vecLight2D.size();

	// Globa 데이터를 상수버퍼를 통해서 b2 레지스터에 바인딩
	Device::GetInst()->GetCB(CB_TYPE::GLOBAL)->SetData(&g_Global);
	Device::GetInst()->GetCB(CB_TYPE::GLOBAL)->Binding();
}

void RenderMgr::Render_End()
{
	// 구조화버퍼 클리어
	// 등록받았던 광원들 해제
	m_Light2DBuffer->Clear();
	m_vecLight2D.clear();
}

void RenderMgr::Render_Debug()
{
	list<DbgInfo>::iterator iter = m_DbgInfoList.begin();
	for (; iter != m_DbgInfoList.end(); )
	{
		// 1. 디버그 렌더링 옵션이 켜져 있을 때만 실제 그리기 작업을 수행합니다.
		if (m_bDebugRender)
		{
			// Mesh 설정
			switch ((*iter).Shape)
			{
			case DBG_SHAPE::RECT:
				m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"RectMesh_LineStrip"));
				break;
			case DBG_SHAPE::CIRCLE:
				m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"CircleMesh_LineStrip"));
				break;
			case DBG_SHAPE::CUBE:
				m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"CubeMesh"));
				break;
			case DBG_SHAPE::SPHERE:
				m_DbgObj->MeshRender()->SetMesh(FIND(AMesh, L"SphereMesh"));
				break;
			}

			// Transform 설정
			if ((*iter).matWorld == XMMatrixIdentity())
			{
				m_DbgObj->Transform()->SetRelativePos((*iter).Pos);
				m_DbgObj->Transform()->SetRelativeScale((*iter).Scale);
				m_DbgObj->Transform()->SetRelativeRot((*iter).Rotation);
				m_DbgObj->Transform()->FinalTick();
			}
			else
			{
				m_DbgObj->Transform()->SetWorldMat((*iter).matWorld);
			}

			// Material 설정
			m_DbgObj->MeshRender()->GetMaterial()->SetScalar(VEC4_0, (*iter).Color);

			if ((*iter).DepthTest)
				m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS);
			else
				m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

			// 실제 Render 요청
			m_DbgObj->Render();
		}

		// 2. ★ 핵심: 수명 관리는 렌더링 여부와 상관없이 항상 진행합니다. ★
		// 그래야 m_bDebugRender가 false일 때 데이터가 무한히 쌓이지(Leak) 않습니다.
		(*iter).Age += E_DT;

		if ((*iter).Life < (*iter).Age)
			iter = m_DbgInfoList.erase(iter);
		else
			++iter;
	}
}