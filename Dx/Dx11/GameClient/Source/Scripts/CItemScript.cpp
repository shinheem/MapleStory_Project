#include "pch.h"
#include "CItemScript.h"

#include "CCollider2D.h"

#include "../../TaskMgr.h"
#include "../../TimeMgr.h"

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

    if (m_bGet && nullptr != m_pMaster)
    {
        // 1. 위치 및 거리 계산
        Vec3 vTargetPos = m_pMaster->Transform()->GetRelativePos();
        // 플레이어의 가슴팍(약간 위)으로 목적지를 잡으면 발밑으로 파고드는 현상을 방지합니다.
        vTargetPos.y += 20.f;

        Vec3 vDir = vTargetPos - vPos;
        float fDist = vDir.Length();
        vDir.Normalize();

        // --- 핵심 수정: 공전 방지 로직 ---
        // 2. 가속도 적용
        float fAccel = 3000.f; // 인력을 더 강하게 주어 빠르게 꽂히게 합니다.
        m_vGetVelocity += vDir * fAccel * DT;

        // 3. 속도 감속 (공전 방지의 핵심: 매 프레임 속도의 일부를 깎음)
        // 공기 저항처럼 현재 속도를 조금씩 줄여줘야 궤도를 이탈하지 않고 플레이어에게 수렴합니다.
        m_vGetVelocity *= (1.f - 4.0f * DT);

        // 4. 위치 적용
        vPos += m_vGetVelocity * DT;
        Transform()->SetRelativePos(vPos);

        // --- 5. 투명도 연출 (기존 유지) ---
        m_fAlpha -= 0.2f * DT;
        if (m_fAlpha < 0.f) m_fAlpha = 0.f;

        Ptr<AMaterial> pMtrl = GetOwner()->GetRenderComponent()->GetMaterial();
        if (nullptr != pMtrl)
        {
            pMtrl->SetScalar(SCALAR_PARAM::FLOAT_0, m_fAlpha);
        }

        // 6. 회전 및 습득 판정
        Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));

        // --- 4. 습득 판정 (fDist 사용) ---
        if (fDist < 30.f || m_fAlpha <= 0.1f)
        {
            Ptr<CInventoryScript> pInven = m_pMaster->GetScript<CInventoryScript>();
            if (pInven) pInven->AddItem(m_ItemInfo);

            TaskInfo info{};
            info.Type = TASK_TYPE::DESTROY_OBJECT;
            info.Param_0 = (DWORD_PTR)GetOwner();
            TaskMgr::GetInst()->AddTask(info);
        }
        return;
    }

    // --- 기존의 낙하 및 붕붕 뜨는 로직 ---
    if (!m_bGround)
    {
        m_prevPos = vPos;
        m_vVelocity.y -= 800.f * DT;
        vPos.x += m_vVelocity.x * DT;
        vPos.y += m_vVelocity.y * DT;

        m_fRotZ += XM_2PI * DT * 2.f;
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
    if (m_bGet) return;

    m_bGet = true;
    m_pMaster = _pPlayer;
    m_fAlpha = 1.0f; // 알파값 초기화

    // 렌더 컴포넌트(MeshRender 등)로부터 나만의 동적 재질을 생성합니다.
    if (GetOwner()->GetRenderComponent())
    {
        // 부모인 CRenderComponent에 정의된 CreateDynamicMaterial 호출
        Ptr<AMaterial> pMtrl = GetOwner()->GetRenderComponent()->CreateDynamicMaterial();

        if (nullptr != pMtrl)
        {
            // [가장 중요] 도메인을 TRANSPARENT로 변경해야 알파 블렌딩이 활성화됩니다.
            // 보통 열거형 이름은 RENDER_DOMAIN::DOMAIN_TRANSPARENT 또는 TRANSPARENT 입니다.
            pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
        }
    }

    // 포물선 흡입을 위한 초기 속도 설정 (기존 코드 유지)
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