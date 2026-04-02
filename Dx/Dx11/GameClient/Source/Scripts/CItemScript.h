#pragma once
#include "CScript.h"

struct tItemInfo;

class CItemScript :
    public CScript
{
private:
    GameObject*          m_pMaster;  // 빨려 들어갈 대상 (플레이어)

    Ptr<tItemInfo>       m_ItemInfo; // 이 오브젝트가 어떤 아이템인지 들고 있음

    float                m_fAlpha;
    float                m_fGetGravity;  // 흡입 중 적용될 가속도 가중치
    float                m_fGetSpeed;    // 빨려 들어가는 속도
    float                m_fVelocity;
    float                m_fFloatingTime; // 누적 시간
    float                m_fBaseY;       // 착지 기준점
    float                m_fFloatTime;   // Sin 계산용
    float                m_fRotZ;        // 회전값

    bool                 m_bGet;         // 습득 중인지 여부
    bool                 m_bGround;

    Vec3                 m_prevPos;
    Vec3                 m_vOriginPos;    // 처음 생성된 기준 위치
    Vec3                 m_vVelocity;    // 현재 속도 (X, Y 모두 포함)
    Vec3                 m_vGetVelocity; // 흡입 시 초기 속도

public:
    virtual void Tick() override;
    virtual void Begin() override;

    void GetItem(GameObject* _pPlayer);
    void SetItemInfo(Ptr<tItemInfo> _pInfo) { m_ItemInfo = _pInfo; }
    Ptr<tItemInfo> GetItemInfo() { return m_ItemInfo; }

private:
    void OnBeginOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol);
    void OnOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol);
    void OnEndOverlap(CCollider2D* _MyCol, CCollider2D* _OtherCol);
    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}
    CLONE(CItemScript)
public:
    CItemScript();
    virtual ~CItemScript();
};

