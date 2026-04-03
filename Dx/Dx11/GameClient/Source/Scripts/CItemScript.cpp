#include "pch.h"
#include "CItemScript.h"

#include "CCollider2D.h"

#include "../../TaskMgr.h"
#include "../../TimeMgr.h"
#include "../../LevelMgr.h"

#include "CInventoryScript.h"

CItemScript::CItemScript()
	: CScript(SCRIPT_TYPE::ITEMSCRIPT)
    , m_pMaster()
    , m_ItemInfo{}
    , m_fGetSpeed()
    , m_fVelocity()
    , m_fFloatingTime()
    , m_fBaseY()
    , m_fFloatTime()
    , m_fRotZ()
    , m_bGet()
    , m_bGround()
    , m_prevPos()
    , m_vOriginPos()
    , m_vVelocity()
    , m_fAlpha(1.f)
{
}

CItemScript::~CItemScript()
{
}

void CItemScript::Begin()
{
    Ptr<CCollider2D> col = GetOwner()->Collider2D();

    col->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CItemScript::OnBeginOverlap);
    col->AddDynamicOverlap(this, (COLLISION_EVENT)&CItemScript::OnOverlap);
    col->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CItemScript::OnEndOverlap);
}

void CItemScript::Tick()
{
    Vec3 vPos = Transform()->GetRelativePos();
    Vec3 vRot = Transform()->GetRelativeRot();

    // ==========================================
    // 1. 플레이어가 아이템을 줍고 있는 상태
    // ==========================================
    if (m_bGet && nullptr != m_pMaster)
    {
        // 이미 아이템 데이터를 넘겨줬다면(중복 방지) 로직 중단
        if (m_ItemInfo == nullptr) return;

        Vec3 vTargetPos = m_pMaster->Transform()->GetRelativePos();
        vTargetPos.y += 20.f;

        Vec3 vDir = vTargetPos - vPos;
        float fDist = vDir.Length();
        vDir.Normalize();

        float fAccel = 3500.f;
        m_vGetVelocity += vDir * fAccel * DT;
        m_vGetVelocity *= (1.f - 5.0f * DT);

        vPos += m_vGetVelocity * DT;

        m_fAlpha -= 0.5f * DT;
        if (m_fAlpha < 0.1f) m_fAlpha = 0.1f;

        Ptr<AMaterial> pMtrl = GetOwner()->GetRenderComponent()->GetMaterial();
        if (nullptr != pMtrl) pMtrl->SetScalar(SCALAR_PARAM::FLOAT_0, m_fAlpha);

        vRot.z = 0.f;
        Transform()->SetRelativePos(vPos);
        Transform()->SetRelativeRot(vRot);

        // [습득 완료 판정]
        if (fDist < 25.f || m_fAlpha <= 0.15f)
        {
            GameObject* pInvenObj = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Charactor_Inventory").Get();
            if (nullptr != pInvenObj)
            {
                CInventoryScript* pInven = pInvenObj->GetScript<CInventoryScript>().Get();
                if (nullptr != pInven)
                {
                    // 데이터 추가 시도
                    bool bSuccess = pInven->AddItem(m_ItemInfo);

                    if (bSuccess)
                    {
                        // [핵심] 성공 즉시 포인터를 날려버림 (다음 프레임 중복 방지)
                        m_ItemInfo = nullptr;

                        TaskInfo info{};
                        info.Type = TASK_TYPE::DESTROY_OBJECT;
                        info.Param_0 = (DWORD_PTR)GetOwner();
                        TaskMgr::GetInst()->AddTask(info);

                        return; // 즉시 함수 종료
                    }
                    else
                    {
                        // 인벤토리 풀 상태 복구
                        m_bGet = false;
                        m_fAlpha = 1.0f;
                        if (nullptr != pMtrl) pMtrl->SetScalar(SCALAR_PARAM::FLOAT_0, 1.f);
                        m_vVelocity = Vec3(0.f, 200.f, 0.f);
                        m_bGround = false;
                    }
                }
            }
        }
        return;
    }

    // ==========================================
    // 2. 일반 상태 (낙하 및 부유)
    // ==========================================
    if (!m_bGround)
    {
        m_prevPos = vPos;
        m_vVelocity.y -= 800.f * DT;
        vPos.x += m_vVelocity.x * DT;
        vPos.y += m_vVelocity.y * DT;
        m_fRotZ += XM_2PI * DT * 1.5f;
        vRot.z = m_fRotZ;
    }
    else
    {
        vRot.z = 0.f;
        m_fFloatTime += DT;
        float fOffset = sinf(m_fFloatTime * 3.5f) * 5.f;
        vPos.y = m_fBaseY + fOffset;
    }

    Transform()->SetRelativePos(vPos);
    Transform()->SetRelativeRot(vRot);
}

void CItemScript::GetItem(GameObject* _pPlayer)
{
    if (m_bGet || m_ItemInfo == nullptr) return;

    m_bGet = true;
    m_pMaster = _pPlayer;

    if (GetOwner()->GetRenderComponent())
    {
        Ptr<AMaterial> pMtrl = GetOwner()->GetRenderComponent()->CreateDynamicMaterial();
        if (nullptr != pMtrl) pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
    }

    Vec3 vPos = Transform()->GetRelativePos();
    Vec3 vTargetPos = m_pMaster->Transform()->GetRelativePos();
    Vec3 vDir = vTargetPos - vPos;
    vDir.Normalize();
    m_vGetVelocity = Vec3(vDir.x * 200.f, 500.f, 0.f);
}

void CItemScript::OnBeginOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol)
{
    int a = 1;
}

void CItemScript::OnOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol)
{

    // 핵심: Z를 눌러서 플레이어에게 빨려 들어가는 중이라면 지형 충돌을 무시합니다.
    if (m_bGet)
        return;

    GameObject* Other = _OtherCol->GetOwner();

    if (Other->GetLayerIdx() == (int)LAYER_TYPE::Layer_Tile && Other->GetName() == L"Ground" ||
        Other->GetLayerIdx() == (int)LAYER_TYPE::Layer_Background && Other->GetName() == L"FootBord")
    {
        // 이미 착지했다면 중복 계산 방지
        if (m_bGround) return;

        Vec3 vPos = Transform()->GetRelativePos();
        Vec3 myScale = Transform()->GetRelativeScale();
        Vec3 otherPos = Other->Transform()->GetRelativePos();
        Vec3 otherScale = Other->Transform()->GetRelativeScale();

        float prevBottom = m_prevPos.y - myScale.y * 0.5f;
        float boardTop = otherPos.y + otherScale.y * 0.5f;

        // 떨어지는 중이고 바닥 높이에 걸쳤을 때
        if (m_vVelocity.y <= 0.f && prevBottom >= boardTop - 5.f)
        {
            // 메이플처럼 바닥에서 10픽셀 띄운 곳을 기준점으로 잡음
            m_fBaseY = boardTop + (myScale.y * 0.5f) + 10.f;

            m_bGround = true;
            m_vVelocity = Vec3(0.f, 0.f, 0.f); // 속도 제로

            vPos.y = m_fBaseY;
            Transform()->SetRelativePos(vPos);
        }
    }
}

void CItemScript::OnEndOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol)
{
}