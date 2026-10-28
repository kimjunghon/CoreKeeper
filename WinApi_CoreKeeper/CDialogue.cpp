#include "pch.h"
#include "CDialogue.h"
#include "CBmpMgr.h"
#include "CButton.h"
#include "CAbstractFactory.h"
#include "CUiMgr.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CInventory.h"

static const int QUEST_MAX = 5;

CDialogue::CDialogue() :
	m_iQuestNumber(0),
	m_iQuestItemCode(0),
	m_iQuestItemCount(0),
	m_iQuestItemHave(0),
	m_bQuest(true),
	m_bQuesting(false),
	m_bQuestClear(false),
	m_hTalkDC(nullptr),
	m_hQuestDC(nullptr),
	m_hClearDC(nullptr),
	m_hFailDC(nullptr),
	m_hTextDC(nullptr),
	m_hHelpDC(nullptr),
	m_hHelpClearDC(nullptr),
	m_hFontDC(nullptr)
{
	for (int i = BT_START; i < BT_END; i++)
	{
		m_pButton[i] = nullptr;
	}
}

CDialogue::~CDialogue()
{
	Release();
}

void CDialogue::Initialize()
{
	m_tInfo.fCX = 600.f;
	m_tInfo.fCY = 400.f;

	m_tInfo.fX = 400.f;
	m_tInfo.fY = 300.f;

	m_tBmpInfo.fCX = 600.f;
	m_tBmpInfo.fCY = 400.f;

	m_eCurDialogueState = DIA_MAIN;
	m_ePreDialogueState = DIA_END;

	m_bQuest = true;
	Set_Quest(1);

	m_hUiDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Dialogue");
	m_hHelpClearDC = CBmpMgr::Get_Instance()->Find_Image(L"Slime_NPC_Quest_Help_Clear");
	m_hFontDC = CBmpMgr::Get_Instance()->Find_Image(L"FontNumber");

	for (int i = BT_START; i < BT_END; i++)
	{
		m_pButton[i] = CUIFactory<CButton>::Create();
		dynamic_cast<CButton*>(m_pButton[i])->Set_Button((BUTTONID)i);
		m_pButton[i]->Update_Rect();
	}

	__super::Update_Rect();

}

void CDialogue::Update()
{

	// 진행 중인 퀘스트가 있을 때만 목표 아이템 보유 수를 확인
	if (m_bQuesting)
	{
		m_iQuestItemHave = static_cast<CInventory*>(CUiMgr::Get_Instance()->Get_PlayerInventory())->Check_ItemCount(m_iQuestItemCode);
		m_bQuestClear = (m_iQuestItemHave >= m_iQuestItemCount);
	}


	if (m_bOpen)
	{

		__super::Update_Rect();


		m_bRender = true;
	}
	else
	{
		m_bRender = false;
		return;
	}


	for (int i = BT_START; i < BT_END; i++)
	{
		m_pButton[i]->Update();

		if (static_cast<CButton*>(m_pButton[i])->ButtonClick())
		{
			switch (i)
			{
			case BT_START:
				if(m_bQuest)
				{
					if (m_bQuesting)
					{
						if (m_bQuestClear)
						{
							m_eCurDialogueState = DIA_QUEST_CLEAR;
						}
						else
						{
							m_eCurDialogueState = DIA_QUEST_FAIL;
						}
					}
					else
					{
						m_eCurDialogueState = DIA_TALK;
					}
				}
				break;
			case BT_STOP:
				CUiMgr::Get_Instance()->DialogueClose();
				dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_CraftingOpen(false);
				m_eCurDialogueState = DIA_MAIN;
				break;
			case BT_ACCEPT:
				m_eCurDialogueState = DIA_QUEST_START;
				m_bQuesting = true;
				break;
			case BT_REFUSE:
				m_eCurDialogueState = DIA_MAIN;
				break;
			}

			break;
		}
	}
}

void CDialogue::Late_Update()
{
	if(m_bOpen)
	{
		Change_State();
	}
}

void CDialogue::Render(HDC _hDC)
{
	if (m_bOpen)
	{
		GdiTransparentBlt(_hDC, (int)m_tRect.left, (int)m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			m_hUiDC, 0, 0, (int)m_tBmpInfo.fCX, (int)m_tBmpInfo.fCY, RGB(0, 255, 0));

		GdiTransparentBlt(_hDC, (int)m_tRect.left + 20, (int)m_tRect.top + 210, 500, 150,
			m_hTextDC, 0, 0, 500, 150, RGB(0, 255, 0));

		for (auto& pButton : m_pButton)
		{
			pButton->Render(_hDC);
		}
	}
	else if(!m_bOpen && m_bQuesting)
	{
		if (CUiMgr::Get_Instance()->Get_Inventory()->front()->IsRender())
		{
			return;
		}

		GdiTransparentBlt(_hDC, 600, 200, 200, 100,
			m_bQuestClear ? m_hHelpClearDC : m_hHelpDC, 0, 0, 200, 100, RGB(0, 255, 0));

		if (!m_bQuestClear)
		{
			// 숫자 이미지는 한 자리(0~9)만 있으므로 범위를 제한
			int iCount = m_iQuestItemHave;
			if (iCount > 9)
				iCount = 9;

			GdiTransparentBlt(_hDC, 682, 268, 8, 15,
				m_hFontDC, iCount * 15, 0, 15, 45, RGB(0, 0, 0));
		}
	}

}

void CDialogue::Release()
{
	for (auto& pButton : m_pButton)
	{
		Safe_Delete<CUi*>(pButton);
	}
	
}

int CDialogue::QuestFrame()
{

	if (m_bQuest)
	{
		if (m_bQuesting)
		{
			if (m_bQuestClear)
			{
				return 2;
			}
			else
			{
				return 1;
			}
		}
		return 0;
	}
	else
		return -1;
}

void CDialogue::Quest_Clear()
{
	m_bQuesting = false;
	m_bQuestClear = false;

	if (m_iQuestNumber >= QUEST_MAX)
	{
		m_bQuest = false;
		return;
	}

	Set_Quest(m_iQuestNumber + 1);
}

void CDialogue::Set_Quest(int _iQuestNumber)
{
	// 퀘스트 번호별 목표 아이템과 개수
	static const int iQuestGoal[QUEST_MAX][2] =
	{
		{ TURF_WALL,  5 },	// 1
		{ SLIME_MEAT, 5 },	// 2
		{ SLIME_CORE, 1 },	// 3
		{ IRON_STONE, 5 },	// 4
		{ HIVE_CORE,  1 },	// 5
	};

	m_iQuestNumber = _iQuestNumber;
	m_iQuestItemCode = iQuestGoal[_iQuestNumber - 1][0];
	m_iQuestItemCount = iQuestGoal[_iQuestNumber - 1][1];
	m_iQuestItemHave = 0;

	// 대사 이미지는 "이미지키_퀘스트번호" 규칙으로 찾음
	auto Find_QuestImage = [_iQuestNumber](const TCHAR* _pKeyFormat) -> HDC
	{
		TCHAR szKey[64] = {};
		swprintf_s(szKey, 64, _pKeyFormat, _iQuestNumber);
		return CBmpMgr::Get_Instance()->Find_Image(szKey);
	};

	m_hTalkDC = Find_QuestImage(L"Slime_NPC_Talk_%d");
	m_hQuestDC = Find_QuestImage(L"Slime_NPC_Quest_%d");
	m_hClearDC = Find_QuestImage(L"Slime_NPC_Quest_Clear_%d");
	m_hFailDC = Find_QuestImage(L"Slime_NPC_Quest_Fail_%d");
	m_hHelpDC = Find_QuestImage(L"Slime_NPC_Quest_Help_%d");
}

void CDialogue::Change_State()
{

	if (m_eCurDialogueState != m_ePreDialogueState)
	{
		m_ePreDialogueState = m_eCurDialogueState;

		for (auto& pButton : m_pButton)
		{
			pButton->Set_Render(false);
		}

		switch(m_eCurDialogueState)
		{
		case DIA_MAIN:
			m_pButton[BT_START]->Set_Render(true);
			m_pButton[BT_STOP]->Set_Render(true);
			m_hTextDC = nullptr;
			break;
		case DIA_TALK:
			m_pButton[BT_ACCEPT]->Set_Render(true);
			m_pButton[BT_REFUSE]->Set_Render(true);
			m_hTextDC = m_hTalkDC;
			break;
		case DIA_QUEST_START:
			m_pButton[BT_STOP]->Set_Render(true);
			m_hTextDC = m_hQuestDC;
			break;
		case DIA_QUEST_CLEAR:
			m_pButton[BT_STOP]->Set_Render(true);
			m_hTextDC = m_hClearDC;
			Quest_Clear();
			break;
		case DIA_QUEST_FAIL:
			m_pButton[BT_STOP]->Set_Render(true);
			m_hTextDC = m_hFailDC;
			break;
		}
	}
}

