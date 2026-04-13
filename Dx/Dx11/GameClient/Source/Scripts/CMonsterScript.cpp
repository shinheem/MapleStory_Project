#include "pch.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include "GameObject.h"
#include "CCollider2D.h"
#include "CTransform.h"

#include "TimeMgr.h"
#include "ItemMgr.h"
#include "AssetMgr.h"
#include "LevelMgr.h"

CMonsterScript::CMonsterScript()
    : CScript(SCRIPT_TYPE::MONSTERSCRIPT)
    , m_eCurState(MON_STATE::STAND)
    , m_fAccTime(0.f)
    , m_iHP(100)
    , m_fSpeed(80.f)
    , m_iDir(-1)
    , m_fGravity(1200.f)
    , m_fVelocityY(0.f)
    , m_bOnGround(false)
    , m_fMaxGroundY(-1000000.f)
    , m_bGroundContact(false)
    , m_pRangeObj()
    , m_pAttackObj()
    , m_pSensorObj()
    , m_vSpawnPos()
    , m_fMoveLimit()
{
}

CMonsterScript::~CMonsterScript() {}

void CMonsterScript::Begin()
{
    ChangeState(MON_STATE::STAND);

    // 생성된 시점의 좌표를 활동 중심지로 기록
    m_vSpawnPos = GetOwner()->Transform()->GetRelativePos();
    m_fMoveLimit = 20.f; // 원하는 활동 범위를 설정하세요.

    // 1. 본체 콜라이더 델리게이트 등록
    Ptr<CCollider2D> col = GetOwner()->Collider2D();
    if (col != nullptr)
    {
        col->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CMonsterScript::OnBeginOverlap);
        col->AddDynamicOverlap(this, (COLLISION_EVENT)&CMonsterScript::OnOverlap);
        col->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CMonsterScript::OnEndOverlap);
    }

    // 2. 자식 오브젝트들 찾기
    m_pRangeObj = GetOwner()->FindChildByName(L"Enermy_Range");
    m_pAttackObj = GetOwner()->FindChildByName(L"Enermy_MonsterAttack");
    Ptr<GameObject> pSensorR = GetOwner()->FindChildByName(L"Sensor_R");
    Ptr<GameObject> pSensorL = GetOwner()->FindChildByName(L"Sensor_L");

    // 3. 자식 레이어 설정 및 델리게이트 수동 등록
    if (m_pRangeObj != nullptr)
    {
        m_pRangeObj->SetLayerIdx((UINT)LAYER_TYPE::Layer_Enermy_Range);
        if (m_pRangeObj->Collider2D() != nullptr)
            m_pRangeObj->Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CMonsterScript::OnBeginOverlap);
    }

    if (m_pAttackObj != nullptr)
    {
        m_pAttackObj->SetLayerIdx((UINT)LAYER_TYPE::Layer_Enermy_MonsterAttack);

        // --- 시작 시 공격 판정 비활성화 ---
        m_pAttackObj->SetActive(false);

        if (m_pAttackObj->Collider2D() != nullptr)
            m_pAttackObj->Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CMonsterScript::OnBeginOverlap);
    }

    if (pSensorR != nullptr)
    {
        pSensorR->SetLayerIdx((UINT)LAYER_TYPE::Layer_Enermy_MonsterSensor);
        if (pSensorR->Collider2D() != nullptr)
            pSensorR->Collider2D()->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CMonsterScript::OnEndOverlap);
    }

    if (pSensorL != nullptr)
    {
        pSensorL->SetLayerIdx((UINT)LAYER_TYPE::Layer_Enermy_MonsterSensor);
        if (pSensorL->Collider2D() != nullptr)
            pSensorL->Collider2D()->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CMonsterScript::OnEndOverlap);
    }
}

void CMonsterScript::ApplyGravity()
{
    if (!m_bOnGround) m_fVelocityY -= m_fGravity * DT;
    else { m_fVelocityY = 0.f; return; }

    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
    vPos.y += m_fVelocityY * DT;
    GetOwner()->Transform()->SetRelativePos(vPos);
}

void CMonsterScript::Tick()
{
    m_fMaxGroundY = -1000000.f;
    m_bGroundContact = false;

    ApplyGravity();

    m_fAccTime += DT;
    switch (m_eCurState) {
    case MON_STATE::STAND: 
        Update_Stand(); 
        break;
    case MON_STATE::MOVE:   
        Update_Move();  
        break;
    case MON_STATE::HIT:    
        Update_Hit();    
        break;
    case MON_STATE::ATTACK:
    {
        if (m_pAttackObj != nullptr)
        {
            if (m_fAccTime >= 0.5f && m_fAccTime <= 0.7f)
            {
                m_pAttackObj->SetActive(true);
            }
            else
            {
                m_pAttackObj->SetActive(false);
            }
        }

        if (GetOwner()->FlipbookRender()->CheckFinish()) {
            // 공격 종료 시 확실히 비활성화
            if (m_pAttackObj != nullptr)
            {
                m_pAttackObj->SetActive(false);
            }

            ChangeState(MON_STATE::STAND);
        }
    }
        //Update_Attack(); 
        break;
    case MON_STATE::DIE:    
        Update_Die();    
        break;
    }

    if (m_bGroundContact) { m_bOnGround = true; m_fVelocityY = 0.f; }
    else m_bOnGround = false;
}

void CMonsterScript::Update_Move()
{
    // 1. 상태 체크
    if (m_eCurState != MON_STATE::MOVE)
        return;

    Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

    // 2. 이동 수행 (m_iDir: -1이면 오른쪽(+), 1이면 왼쪽(-))
    vPos.x -= m_fSpeed * (float)m_iDir * DT;

    // 3. 활동 범위 제한 (스포너 기준)
    float fLeftLimit = m_vSpawnPos.x - m_fMoveLimit;
    float fRightLimit = m_vSpawnPos.x + m_fMoveLimit;

    bool bTurnAround = false;
    if (vPos.x < fLeftLimit) { vPos.x = fLeftLimit; bTurnAround = true; }
    else if (vPos.x > fRightLimit) { vPos.x = fRightLimit; bTurnAround = true; }

    if (bTurnAround)
    {
        m_iDir *= -1;
        GetOwner()->Transform()->SetRelativePos(vPos);
        ChangeState(MON_STATE::STAND);
        return;
    }

    // 4. 좌표 및 이미지 방향 적용
    GetOwner()->Transform()->SetRelativePos(vPos);

    Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
    vScale.x = abs(vScale.x) * (float)m_iDir;
    GetOwner()->Transform()->SetRelativeScale(vScale);

    // 5. 자식 오브젝트(Range, Attack) 위치 동시 반전
    float fDirSign = (float)-m_iDir;

    // [감지 범위]
    if (m_pRangeObj != nullptr)
    {
        Vec3 vRangePos = m_pRangeObj->Transform()->GetRelativePos();
        float fRangeOffset = 80.f;
        vRangePos.x = fRangeOffset * fDirSign;
        m_pRangeObj->Transform()->SetRelativePos(vRangePos);
    }

    // [실제 공격 판정 범위]
    if (m_pAttackObj != nullptr)
    {
        Vec3 vAttackPos = m_pAttackObj->Transform()->GetRelativePos();
        float fAttackOffset = 50.f;
        vAttackPos.x = fAttackOffset * fDirSign;
        m_pAttackObj->Transform()->SetRelativePos(vAttackPos);
    }

    // 6. 자동 대기 전환
    if (m_fAccTime > 3.f)
    {
        ChangeState(MON_STATE::STAND);
    }
}

void CMonsterScript::ChangeState(MON_STATE _eNextState)
{
    // 이미 죽은 상태라면 다른 상태로 전이 불가 (DIE 상태 고정)
    if (m_eCurState == MON_STATE::DIE && _eNextState != MON_STATE::DIE)
        return;

    // 동일한 상태로의 전이 방지 (단, 피격은 연속 허용)
    if (m_eCurState == _eNextState && _eNextState != MON_STATE::HIT)
        return;

    // =========================================================
    // [핵심] DIE 상태로 전환되는 "그 순간" 경험치 지급 및 아이템 드롭
    // =========================================================
    if (_eNextState == MON_STATE::DIE)
    {
        // 1. 경험치 지급 로직
        Ptr<GameObject> pPlayerObj = LevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Maple_Player");

        if (pPlayerObj != nullptr)
        {
            CPlayerScript* pPlayerScript = pPlayerObj->GetScript<CPlayerScript>().Get();
            if (pPlayerScript != nullptr)
            {
                int iMaxEXP = pPlayerScript->GetMaxExp();
                int iGainAmount = (int)((float)iMaxEXP * 0.05f); // 5% 계산
                pPlayerScript->GainExp(iGainAmount);
            }
        }

        // 2. 아이템 드롭 확률 로직 (rand() % 100 이용)
        int iRand = 0;

        // --- 아이템 1: 장비 아이템 (15% 확률) ---
        iRand = rand() % 100;
        if (iRand < 15)
        {
            Ptr<tItemInfo> pDropItem = new tItemInfo;
            pDropItem->strName = L"Monster_Drop_Equip";
            pDropItem->eType = ITEM_TYPE::EQUIP;
            pDropItem->pIcon = AssetMgr::GetInst()->Load<AMaterial>(L"Material\\Item3.mtrl", L"Material\\Item3.mtrl");
            pDropItem->iCount = 1;

            Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
            vPos.x += -30.f; // 드롭 위치 분산
            ItemMgr::GetInst()->CreateItemDrop(pDropItem, vPos);
        }

        // --- 아이템 2: 소비 아이템 A (50% 확률) ---
        iRand = rand() % 100;
        if (iRand < 50)
        {
            Ptr<tItemInfo> pDropItem = new tItemInfo;
            pDropItem->strName = L"Monster_Drop_Potion_HP";
            pDropItem->eType = ITEM_TYPE::CONSUME;
            pDropItem->pIcon = AssetMgr::GetInst()->Load<AMaterial>(L"Material\\Item2.mtrl", L"Material\\Item2.mtrl");
            pDropItem->iCount = 1;

            Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
            vPos.x += 0.f;
            ItemMgr::GetInst()->CreateItemDrop(pDropItem, vPos);
        }

        // --- 아이템 3: 소비 아이템 B (80% 확률 - 잡템) ---
        iRand = rand() % 100;
        if (iRand < 80)
        {
            Ptr<tItemInfo> pDropItem = new tItemInfo;
            pDropItem->strName = L"Monster_Drop_Potion_MP";
            pDropItem->eType = ITEM_TYPE::CONSUME;
            pDropItem->pIcon = AssetMgr::GetInst()->Load<AMaterial>(L"Material\\Item1.mtrl", L"Material\\Item1.mtrl");
            pDropItem->iCount = 1;

            Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
            vPos.x += 30.f;
            ItemMgr::GetInst()->CreateItemDrop(pDropItem, vPos);
        }
    }
    // =========================================================

    // 상태 변경 시 공격 콜라이더 등은 비활성화
    if (m_pAttackObj != nullptr)
    {
        m_pAttackObj->SetActive(false);
    }

    m_eCurState = _eNextState;
    m_fAccTime = 0.f; // 상태 전환 시 시간 초기화

    // 애니메이션 재생
    if (GetOwner()->FlipbookRender())
    {
        int iRepeat = 0;
        if ((m_eCurState == MON_STATE::STAND || m_eCurState == MON_STATE::MOVE))
            iRepeat = -1;
        else
            iRepeat = 0;

        float fPlaySpeed = 7.f;
        if (m_eCurState == MON_STATE::ATTACK) 
            fPlaySpeed = 5.f;
        if (m_eCurState == MON_STATE::HIT)    fPlaySpeed = 3.f;
        if (m_eCurState == MON_STATE::DIE)    fPlaySpeed = 7.f;

        

        GetOwner()->FlipbookRender()->Play((UINT)m_eCurState, fPlaySpeed, iRepeat);
    }
}

void CMonsterScript::Update_Stand() { if (m_fAccTime > 2.f) { m_iDir = (rand() % 2 == 0) ? 1 : -1; ChangeState(MON_STATE::MOVE); } }
void CMonsterScript::Update_Hit()
{
    // 1. 애니메이션이 끝났는지 확인
    bool bAnimFinished = GetOwner()->FlipbookRender()->CheckFinish();

    // 2. 애니메이션이 끝났더라도 최소 0.5초(혹은 원하는 시간)는 유지함
    // m_fAccTime은 ChangeState에서 0으로 초기화됨
    if (bAnimFinished && m_fAccTime >= 0.2f)
    {
        ChangeState(MON_STATE::STAND);
    }
}

void CMonsterScript::Update_Attack() {
    // --- 수정: 특정 시간에만 잠깐 활성화 ---
    // 예: 공격 시작 후 0.3초 ~ 0.5초 사이에만 판정 오브젝트를 활성화
    // 이 시간은 몬스터의 휘두르는 모션 타이밍에 맞춰서 숫자를 조절하세요.
    if (m_pAttackObj != nullptr)
    {
        if (m_fAccTime >= 0.5f && m_fAccTime <= 0.7f)
        {
            m_pAttackObj->SetActive(true);
        }
        else
        {
            m_pAttackObj->SetActive(false);
        }
    }

    if (GetOwner()->FlipbookRender()->CheckFinish()) {
        // 공격 종료 시 확실히 비활성화
        if (m_pAttackObj != nullptr) 
            m_pAttackObj->SetActive(false);

        ChangeState(MON_STATE::STAND);
    }
}

void CMonsterScript::Update_Die()
{
    // 1. 애니메이션이 완전히 끝났는지 확인
    bool bAnimFinished = GetOwner()->FlipbookRender()->CheckFinish();

    // 2. 애니메이션이 끝난 후에도 시체가 남아있을 시간 설정
    // m_fAccTime은 ChangeState에서 0으로 초기화되었으므로 DIE 상태 진입 후의 총 시간임
    // 예: 애니메이션 종료 후에도 총 시간이 2.0초가 지날 때까지 대기
    if (bAnimFinished && m_fAccTime >= 0.4f)
    {
        GetOwner()->Destroy();
    }
}

void CMonsterScript::OnOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
    if (_OwnCollider->GetOwner() != GetOwner())
        return;

    GameObject* pOther = _OtherCollider->GetOwner();
    const wstring& strName = pOther->GetName();
    UINT iOtherLayer = pOther->GetLayerIdx();

    Vec3 vMyPos = GetOwner()->Transform()->GetWorldPos();
    Vec3 vMyScale = GetOwner()->Transform()->GetWorldScale();
    Vec3 vOtherPos = pOther->Transform()->GetWorldPos();
    Vec3 vOtherScale = pOther->Transform()->GetWorldScale();

    if (strName.find(L"Wall") != wstring::npos || iOtherLayer == (UINT)LAYER_TYPE::Layer_Tile)
    {
        float fMinDistX = (vMyScale.x + vOtherScale.x) * 0.5f;
        float fDiffX = abs(vMyPos.x - vOtherPos.x);
        float fOverlapX = fMinDistX - fDiffX;

        float fMinDistY = (vMyScale.y + vOtherScale.y) * 0.5f;
        float fDiffY = abs(vMyPos.y - vOtherPos.y);
        float fOverlapY = fMinDistY - fDiffY;

        if (fOverlapX > 0.f && fOverlapX < fOverlapY)
        {
            Vec3 vRelPos = GetOwner()->Transform()->GetRelativePos();

            if (vMyPos.x < vOtherPos.x)
                vRelPos.x -= (fOverlapX + 2.f);
            else
                vRelPos.x += (fOverlapX + 2.f);

            GetOwner()->Transform()->SetRelativePos(vRelPos);
            m_iDir *= -1;
            ChangeState(MON_STATE::STAND);
            return;
        }
    }

    if (strName.find(L"Ground") != wstring::npos || strName.find(L"FootBoard") != wstring::npos || iOtherLayer == (UINT)LAYER_TYPE::Layer_Tile)
    {
        float fGroundTop = vOtherPos.y + (vOtherScale.y * 0.5f);
        float fTargetY = fGroundTop + (vMyScale.y * 0.5f);

        if (vMyPos.y - (vMyScale.y * 0.5f) >= fGroundTop - 25.f)
        {
            if (fTargetY > m_fMaxGroundY)
            {
                m_fMaxGroundY = fTargetY;
                m_bGroundContact = true;

                Vec3 vRelPos = GetOwner()->Transform()->GetRelativePos();
                vRelPos.y += (fTargetY - vMyPos.y);
                GetOwner()->Transform()->SetRelativePos(vRelPos);

                m_bOnGround = true;
                m_fVelocityY = 0.f;
            }
        }
    }
}

void CMonsterScript::OnBeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
    // 1. 충돌 주체 정보 파악
    GameObject* pOwnObj = _OwnCollider->GetOwner();       // 나 (몬스터 본체 또는 자식)
    GameObject* pOtherObj = _OtherCollider->GetOwner();   // 상대방 (충돌한 물체)
    UINT otherLayer = pOtherObj->GetLayerIdx();           // 상대방의 레이어

    // 2. [피격 로직] 몬스터 본체가 플레이어의 "공격 레이어"에 맞았는가?
    if (pOwnObj == GetOwner())
    {
        if (otherLayer == (UINT)LAYER_TYPE::Layer_PlayerAttack)
        {
            // 피격 상태가 아닐 때만 데미지 처리
            if (m_eCurState != MON_STATE::DIE && m_eCurState != MON_STATE::HIT)
            {
                m_iHP -= 20;
                if (m_iHP <= 0) { m_iHP = 0; ChangeState(MON_STATE::DIE); }
                else ChangeState(MON_STATE::HIT);
            }
            return; // 피격 처리가 끝났으면 종료
        }
    }

    // 3. [추격/인식 로직] 몬스터의 감지 범위(Range)에 플레이어가 들어왔는가?
    // [중요 수정]: 단순히 레이어만 체크하는 게 아니라, 상대방이 플레이어 본체인지 확인해야 함
    if (pOwnObj->GetName().find(L"Range") != wstring::npos)
    {
        // 레이어가 Layer_Player이면서, 동시에 상대방의 부모가 없는(최상단 본체인) 경우만 타겟팅
        // 또는 상대방의 이름이 플레이어 본체 이름과 일치하는지 확인
        if (otherLayer == (UINT)LAYER_TYPE::Layer_Player)
        {
            // [핵심]: 만약 충돌한 상대가 "Player_Attack"이라는 이름을 가졌다면 추격하지 않음
            if (pOtherObj->GetName().find(L"Attack") != wstring::npos)
                return;

            if (m_eCurState != MON_STATE::ATTACK && m_eCurState != MON_STATE::DIE)
            {
                ChangeState(MON_STATE::ATTACK);
            }
            return;
        }
    }

    // 4. [공격 판정] 몬스터의 공격 콜라이더가 플레이어 본체에 닿았는가?
    if (pOwnObj->GetName().find(L"Attack") != wstring::npos && pOwnObj != GetOwner())
    {
        // 몬스터의 공격도 플레이어 본체(Layer_Player)에만 반응해야 함
        if (otherLayer == (UINT)LAYER_TYPE::Layer_Player)
        {
            // 상대방이 자식(공격 이펙트 등)이 아닐 때만 플레이어에게 데미지 전달
            if (pOtherObj->GetName().find(L"Attack") == wstring::npos)
            {
                // CPlayerScript* pPlayer = pOtherObj->GetScript<CPlayerScript>().Get();
                // if(pPlayer) pPlayer->TakeDamage(10);
            }
        }
    }
}

void CMonsterScript::OnEndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
    GameObject* pOwnObj = _OwnCollider->GetOwner();
    if (pOwnObj->GetName() != L"Sensor_R" && pOwnObj->GetName() != L"Sensor_L") return;

    GameObject* pOtherObj = _OtherCollider->GetOwner();
    const wstring& strOtherName = pOtherObj->GetName();

    if (strOtherName.find(L"Ground") != wstring::npos || strOtherName.find(L"FootBoard") != wstring::npos)
    {
        bool bIsRightSensor = (pOwnObj->GetName() == L"Sensor_R");

        if ((m_iDir == 1 && bIsRightSensor) || (m_iDir == -1 && !bIsRightSensor))
        {
            m_iDir *= -1;
            Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
            vPos.x += (m_fSpeed * DT + 15.f) * (float)m_iDir;
            GetOwner()->Transform()->SetRelativePos(vPos);
            ChangeState(MON_STATE::STAND);
        }
    }
}