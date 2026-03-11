#include "pch.h"
#include "CCollisionMgr.h"
#include "CCameraMgr.h"
#include "CObjMgr.h"
#include "CUiMgr.h"
#include "CTileMgr.h"
#include "CPlayer.h"
#include "CWall.h"
#include "CKeyMgr.h"
#include "CMouse.h"
#include "CInventory.h"
#include "CCrafting.h"
#include "CCraftingSlot.h"
#include "CSlot.h"
#include "CCraftingSlot.h"
#include "CItem.h"
#include "CEquip.h"
#include "CSoundMgr.h"

CCollisionMgr* CCollisionMgr::m_pInstance = nullptr;

CCollisionMgr::CCollisionMgr() :
	m_pPlayer(nullptr),
	m_pMouse(nullptr),
	m_pMonsterList(nullptr),
	m_pVecTile(nullptr),
	m_pVecWall(nullptr)
{

}

CCollisionMgr::~CCollisionMgr()
{
	Release();
}


void CCollisionMgr::Initialize()
{
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();

	m_pMouse = CUiMgr::Get_Instance()->Get_Cursor();
	
	m_pMonsterList = CObjMgr::Get_Instance()->Get_MonsterList();

	m_pProjectileList = CObjMgr::Get_Instance()->Get_ProjectileList();

	m_pItemList = CObjMgr::Get_Instance()->Get_ItemList();

	m_pBulidingList = CObjMgr::Get_Instance()->Get_BulidingList();

	m_pVecTile = CTileMgr::Get_Instance()->Get_VecTile();
	
	m_pVecWall = CObjMgr::Get_Instance()->Get_VecWall();

	Set_Inventory(CUiMgr::Get_Instance()->Get_Inventory());

	Set_Crafting(CUiMgr::Get_Instance()->Get_Crafting());
}

void CCollisionMgr::Update()  
{
}

void CCollisionMgr::Late_Update()
{
	if(m_pPlayer)
	{
	//	Obj_Tile_Collision_RectEx(m_pPlayer);
	//	Obj_Wall_Collision_RectEx(m_pPlayer);
		
		for(auto& BulidingList : *m_pBulidingList)
		{
			Obj_Idle_Collision_RectEx(BulidingList, m_pPlayer);
		}
		if (m_pVecWall)
		{
			Player_Wall_Collision(m_pPlayer, m_pVecWall);
		}
		if (m_pItemList)
		{
			for(auto& ItemList : *m_pItemList)
			{
				Obj_Idle_Collision_RectEx(ItemList, m_pPlayer);
			}
		}
	}

	for (auto& ProjectileList : *m_pProjectileList)
	{
		Obj_Wall_Collision_RectEx(ProjectileList);
	}


	if(m_pMonsterList)
	{
		for (auto& MonsterList : *m_pMonsterList)
		{	
			Player_Monster_Collision(MonsterList);
			Obj_Attack_Collision_RectEx(m_pPlayer, MonsterList);
			for (auto& ProjectileList : *m_pProjectileList)
			{
				Obj_Attack_Collision_RectEx(MonsterList, ProjectileList);
			}
			Obj_Tile_Collision_RectEx(MonsterList);
			Obj_Wall_Collision_RectEx(MonsterList);
		}
	}

	Mouse_Slot_Collision_Rect();

}

void CCollisionMgr::Release()
{
	m_pPlayer = nullptr;
	m_pMouse = nullptr;
	m_pVecTile = nullptr;
	m_pVecWall = nullptr;
	for (int i = 0; i < SLOT_END; i++)
	{
		m_pSlotList[i].clear();
	}
}


void CCollisionMgr::Set_Inventory(list<CUi*>* _pInventoryList)
{
	if (!_pInventoryList)
		return;
	for (auto& InventoryList : *_pInventoryList)
	{
		if (InventoryList->Get_UiID() == UI_INVENTORY)
		{
			CInventory* pInven = dynamic_cast<CInventory*>(InventoryList);

			for (auto& vecSlot : *pInven->Get_vecSlot())
			{
				m_pSlotList[SLOT_INVEN].push_back(vecSlot);
			}
			for (auto& vecSlot : *pInven->Get_vecQuickSlot())
			{
				m_pSlotList[SLOT_QUICK].push_back(vecSlot);
			}
		}
		else if (InventoryList->Get_UiID() == UI_CRAFTING)
		{
			CCrafting* pCrafting = dynamic_cast<CCrafting*>(InventoryList);

			for (auto& vecSlot : *pCrafting->Get_vecCraftingSlot())
			{
				m_pSlotList[SLOT_CRAFTING].push_back(vecSlot);
			}
		}
		else if (InventoryList->Get_UiID() == UI_EQUIP)
		{
			CEquip* pEquip = dynamic_cast<CEquip*>(InventoryList);

			m_pSlotList[SLOT_EQUIP_HELM].push_back(pEquip->Get_HelmSlot());
			m_pSlotList[SLOT_EQUIP_CHEST].push_back(pEquip->Get_ChestSlot());
			m_pSlotList[SLOT_EQUIP_PANTS].push_back(pEquip->Get_PantsSlot());
		}
	}
}

void CCollisionMgr::Set_Crafting(list<CUi*>* _pCraftingList)
{
	if (!_pCraftingList)
		return;

	for (auto& CraftingList : *_pCraftingList)
	{
		CCrafting* pCrafting = dynamic_cast<CCrafting*>(CraftingList);

		for (auto& vecSlot : *pCrafting->Get_vecCraftingSlot())
		{
			m_pSlotList[SLOT_CRAFTING].push_back(vecSlot);
		}
	}

}

void CCollisionMgr::Push_CollisionSlotList(CUi* _pSlot, SLOTID _eSlotID)
{
	if (_pSlot)
	{
		m_pSlotList[_eSlotID].push_back(_pSlot);
	}
}

void CCollisionMgr::Obj_Tile_Collision_RectEx(CObj* _pObj)
{
	if (!_pObj || !_pObj->IsRender())
		return;
	for (auto& vecCollision : _pObj->Get_VecCollisionInfo())
	{
		int	x = (int)vecCollision.fX / TILECX;
		int	y = (int)vecCollision.fY / TILECY;

		int		iCompIndex = y * TILEX + x;

		if (0 > iCompIndex || (size_t)iCompIndex >= m_pVecWall->size())
			return;

		int dX[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };
		int dY[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };
		int Dir = 0;
		int iIndex = 0;

		float		fWidth(0.f), fHeight(0.f);

		for (Dir = 0; Dir < 8; Dir++)
		{
			iIndex = iCompIndex + dX[Dir] + (dY[Dir] * TILEX);

			if (iIndex < 0 || (size_t)iIndex >= m_pVecTile->size())
				continue;

			if (Check_Tile_Rect(vecCollision, (*m_pVecTile)[iIndex], &fWidth, &fHeight))
			{
				if (fWidth > fHeight)
				{
					// 상 충돌
					if (vecCollision.fY < (*m_pVecTile)[iIndex]->Get_Info()->fY)
					{
						_pObj->Set_PosY(-fHeight);
					}
					// 하 충돌
					else
					{
						_pObj->Set_PosY(fHeight);
					}
				}
				// 좌 우 충돌
				else
				{
					// 좌 충돌
					if (vecCollision.fX < (*m_pVecTile)[iIndex]->Get_Info()->fX)
					{
						_pObj->Set_PosX(-fWidth);
					}

					// 우 충돌
					else
					{
						_pObj->Set_PosX(fWidth);
					}
				}
			//	return;
			}
		}
	}
}

bool CCollisionMgr::Check_Tile_Rect(const INFO& _tInfo, CTile* _pTile, float* _pX, float* _pY)
{
	if (_pTile->Get_TileID() != TILE_NULL)
		return false;

	
	float		fWidth = abs(_tInfo.fX - _pTile->Get_Info()->fX);
	float		fHeight = abs(_tInfo.fY - _pTile->Get_Info()->fY);

	float		fRadiusX = (_tInfo.fCX + _pTile->Get_Info()->fCX) * 0.5f;
	float		fRadiusY = (_tInfo.fCY + _pTile->Get_Info()->fCY) * 0.5f;

	if ((fRadiusX > fWidth) && (fRadiusY > fHeight))
	{
		*_pX = fRadiusX - fWidth;
		*_pY = fRadiusY - fHeight;

		return true;
	}

	return false;
}
void CCollisionMgr::Obj_Wall_Collision_RectEx(CObj* _pObj)
{
	if (!_pObj || !_pObj->IsRender())
		return;

	for (auto& vecCollision : _pObj->Get_VecCollisionInfo())
	{
		int	x = (int)vecCollision.fX / TILECX;
		int	y = (int)vecCollision.fY / TILECY;

		int		iCompIndex = y * TILEX + x;

		if (0 > iCompIndex || (size_t)iCompIndex >= m_pVecWall->size())
			return;

		int dX[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };
		int dY[8] = { 1, 0, -1, 0, 1, 1, -1, -1 };
		int Dir = 0;
		int iIndex = 0;

		float		fWidth(0.f), fHeight(0.f);

		for (Dir = 0; Dir < 8; Dir++)
		{
			iIndex = iCompIndex + dX[Dir] + (dY[Dir] * TILEX);

			if (iIndex < 0 || (size_t)iIndex >= m_pVecWall->size())
				continue;

			if (Check_Wall_Rect(vecCollision, (*m_pVecWall)[iIndex], &fWidth, &fHeight))
			{
				if (_pObj->Get_ObjID() == OBJ_PROJECTILE)
				{
					_pObj->Set_Dead();
				}
				else
				{
					if (fWidth > fHeight)
					{
						// 상 충돌
						if (vecCollision.fY < (*m_pVecWall)[iIndex]->Get_Info().fY)
						{
							_pObj->Set_PosY(-fHeight);
						}
						// 하 충돌
						else
						{
							_pObj->Set_PosY(fHeight);
						}
					}
					// 좌 우 충돌
					else
					{
						// 좌 충돌
						if (vecCollision.fX < (*m_pVecWall)[iIndex]->Get_Info().fX)
						{
							_pObj->Set_PosX(-fWidth);
						}

						// 우 충돌
						else
						{
							_pObj->Set_PosX(fWidth);
						}
					}
				}
				//return;
			}
		}
	}
}

bool CCollisionMgr::Check_Wall_Rect(const INFO& _tInfo, CObj* _pWall, float* _pX, float* _pY)
{
	if (dynamic_cast<CWall*>(_pWall)->Get_WallID() == WALL_NULL)
		return false;

	float		fWidth = abs(_tInfo.fX - _pWall->Get_Info().fX);
	float		fHeight = abs(_tInfo.fY - _pWall->Get_Info().fY);

	float		fRadiusX = (_tInfo.fCX + _pWall->Get_Info().fCX) * 0.5f;
	float		fRadiusY = (_tInfo.fCY + _pWall->Get_Info().fCY) * 0.5f;

	if ((fRadiusX > fWidth) && (fRadiusY > fHeight))
	{
		*_pX = fRadiusX - fWidth;
		*_pY = fRadiusY - fHeight;

		return true;
	}

	return false;

}
void CCollisionMgr::Mouse_Slot_Collision_Rect()
{
	if (!m_pMouse)
		return;

	for(int i=0; i<SLOT_END; i++)
	{

		for (auto& vecSlot : (m_pSlotList[i]))
		{	
			if (!vecSlot->IsRender())
			{
				CMouse* pMouse = dynamic_cast<CMouse*>(m_pMouse);

				pMouse->Set_InfoDC(nullptr);

				continue;
			}

			RECT		rcSlot = vecSlot->Get_Rect();
			INFO		tMouseInfo = m_pMouse->Get_Info();
			float fX = CCameraMgr::Get_Instance()->Get_RenderX();
			float fY = CCameraMgr::Get_Instance()->Get_RenderY();

			tMouseInfo.fX -= (LONG)CCameraMgr::Get_Instance()->Get_RenderX();
			tMouseInfo.fY -= (LONG)CCameraMgr::Get_Instance()->Get_RenderY();
		
			if (!(abs(tMouseInfo.fX - vecSlot->Get_Info().fX) < 60.f) && !(abs(tMouseInfo.fY - vecSlot->Get_Info().fY) < 60.f))
				continue;


			if (tMouseInfo.fX > rcSlot.left && tMouseInfo.fX < rcSlot.right && tMouseInfo.fY > rcSlot.top && tMouseInfo.fY < rcSlot.bottom)
			{
				CMouse* pMouse = dynamic_cast<CMouse*>(m_pMouse);

				CObj* pTemp = pMouse->Get_Item();
				
				if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
				{
					if(i == SLOT_EQUIP_CHEST)
					{
						if (pTemp)
						{
							CItem* pItem = dynamic_cast<CItem*>(pTemp);
							if (pItem->Get_ItemClass() == IC_ARMOR_CHEST)
							{
								CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

								pMouse->Set_Item(pSlot->Get_Item());

								pSlot->Set_Item(pTemp);

								CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
								CSoundMgr::Get_Instance()->PlaySound(L"EquipArmor.wav", SOUND_UI, 0.8f);
							}
						}
						else
						{
							CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

							pMouse->Set_Item(pSlot->Get_Item());

							pSlot->Set_Item(pTemp);


							CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
							CSoundMgr::Get_Instance()->PlaySound(L"EquipArmor.wav", SOUND_UI, 0.8f);
						}

					}
					else if (i == SLOT_EQUIP_HELM)
					{
						if (pTemp)
						{
							CItem* pItem = dynamic_cast<CItem*>(pTemp);
							if (pItem->Get_ItemClass() == IC_ARMOR_HELM)
							{
								CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

								pMouse->Set_Item(pSlot->Get_Item());

								pSlot->Set_Item(pTemp);
								CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
								CSoundMgr::Get_Instance()->PlaySound(L"EquipArmor.wav", SOUND_UI, 0.8f);
							}
						}
						else
						{
							CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

							pMouse->Set_Item(pSlot->Get_Item());

							pSlot->Set_Item(pTemp);
							CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
							CSoundMgr::Get_Instance()->PlaySound(L"EquipArmor.wav", SOUND_UI, 0.8f);
						}
					}
					else if (i == SLOT_EQUIP_PANTS)
					{
						if (pTemp)
						{
							CItem* pItem = dynamic_cast<CItem*>(pTemp);
							if (pItem->Get_ItemClass() == IC_ARMOR_PANTS)
							{
								CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

								pMouse->Set_Item(pSlot->Get_Item());

								pSlot->Set_Item(pTemp);
								CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
								CSoundMgr::Get_Instance()->PlaySound(L"EquipArmor.wav", SOUND_UI, 0.8f);
							}
						}
						else
						{
							CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

							pMouse->Set_Item(pSlot->Get_Item());

							pSlot->Set_Item(pTemp);
							CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
							CSoundMgr::Get_Instance()->PlaySound(L"EquipArmor.wav", SOUND_UI, 0.8f);
						}
					}
					else if (i == SLOT_CRAFTING)
					{
						if (!pTemp)
						{
							CCraftingSlot* pSlot = dynamic_cast<CCraftingSlot*>(vecSlot);

							pMouse->Set_Item(pSlot->Crafting());
						}
						else
						{
							CItem* pItem = dynamic_cast<CItem*>(pTemp);

							CCraftingSlot* pSlot = dynamic_cast<CCraftingSlot*>(vecSlot);
							
							if (pItem->Get_ItemCode() == pSlot->Get_CraftingItemCode())
							{
								CObj* pTemp = pSlot->Crafting();
								if(pTemp)
								{
									pItem->IncreaseCount(1);
									pTemp->Set_Dead();
								}
							}
						}
					}
					else if (i == SLOT_SLIME_CORE)
					{
						if (pTemp)
						{
							CItem* pItem = dynamic_cast<CItem*>(pTemp);
							if (pItem->Get_ItemCode() == SLIME_CORE)
							{
								CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

								pMouse->Set_Item(pSlot->Get_Item());

								pSlot->Set_Item(pTemp);
							}
						}
						//else
						//{
						//	CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);
						//
						//	pMouse->Set_Item(pSlot->Get_Item());
						//
						//	pSlot->Set_Item(pTemp);
						//}
					}
					else if (i == SLOT_HIVE_CORE)
					{
						if (pTemp)
						{
							CItem* pItem = dynamic_cast<CItem*>(pTemp);
							if (pItem->Get_ItemCode() == HIVE_CORE)
							{
								CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

								pMouse->Set_Item(pSlot->Get_Item());

								pSlot->Set_Item(pTemp);
							}
						}
					//	else
					//	{
					//		CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);
					//
					//		pMouse->Set_Item(pSlot->Get_Item());
					//
					//		pSlot->Set_Item(pTemp);
					//	}
					}
					else if (i == SLOT_FORGE)
					{
						if (pTemp)
						{
							CItem* pItem = dynamic_cast<CItem*>(pTemp);	
							CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

							if (pItem->Get_ItemCode() == IRON_STONE)
							{
							
								if(pSlot->Get_Item())
								{
									if (dynamic_cast<CItem*>(pSlot->Get_Item())->Get_ItemCode() == IRON_STONE)
									{
										dynamic_cast<CItem*>(pSlot->Get_Item())->IncreaseCount(pItem->Get_ItemCount());
										pMouse->Set_Item(nullptr);
									}
								}
								else
								{
									pMouse->Set_Item(pSlot->Get_Item());

									pSlot->Set_Item(pTemp);
								}
							}

							if (pSlot->Get_Item() || pItem)
							{
								CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
								CSoundMgr::Get_Instance()->PlaySound(L"Inventory.wav", SOUND_UI, 0.8f);
							}
						}
						else
						{
							CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

							pMouse->Set_Item(pSlot->Get_Item());

							pSlot->Set_Item(pTemp);

							CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
							CSoundMgr::Get_Instance()->PlaySound(L"Inventory.wav", SOUND_UI, 0.8f);
						}
					}
					else if (i == SLOT_FORGE_RESULT)
					{
						if (!pTemp)
						{
							CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

							pMouse->Set_Item(pSlot->Get_Item());

							pSlot->Set_Item(pTemp);

							CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
							CSoundMgr::Get_Instance()->PlaySound(L"Inventory.wav", SOUND_UI, 0.8f);
						}
					}
					else
					{

						CItem* pItem = dynamic_cast<CItem*>(pTemp);

						CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);
						CItem* pSlotItem = dynamic_cast<CItem*>(pSlot->Get_Item());

						if(pSlotItem && pItem)
						{
							if (pItem->Get_ItemCode() == pSlotItem->Get_ItemCode())
							{
								dynamic_cast<CInventory*>(CUiMgr::Get_Instance()->Get_PlayerInventory())->Add_Item(pMouse->Get_Item());
								pMouse->Set_Item(nullptr);

								CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
								CSoundMgr::Get_Instance()->PlaySound(L"Inventory.wav", SOUND_UI, 0.8f);
								return;
							}
						}
						
						if (pSlotItem || pItem)
						{
							CSoundMgr::Get_Instance()->StopSound(SOUND_UI);
							CSoundMgr::Get_Instance()->PlaySound(L"Inventory.wav", SOUND_UI, 0.8f);
						}
						
						pMouse->Set_Item(pSlot->Get_Item());

						pSlot->Set_Item(pTemp);
						
					}	
				} //마우스 오버
				else
				{
					if (i == SLOT_CRAFTING)
					{
						CCraftingSlot* pSlot = dynamic_cast<CCraftingSlot*>(vecSlot);

						if (pSlot->IsCrafting())
						{
							pMouse->Set_InfoDC(pSlot->Get_InfoDC(), 1);
						}
						else
						{
							pMouse->Set_InfoDC(pSlot->Get_InfoDC(), 0);
						}
						return;
					}
					else
					{
						CSlot* pSlot = dynamic_cast<CSlot*>(vecSlot);

						if (pSlot->Get_Item())
						{
							CItem* pItem = dynamic_cast<CItem*>(pSlot->Get_Item());

							if (!pItem)
							{
								pMouse->Set_InfoDC(nullptr);
							}

							pMouse->Set_InfoDC(pItem->Get_InfoDC());
							return;
						}
						else
						{
							pMouse->Set_InfoDC(nullptr);
						}
					}
				}
			}
			else
			{
				CMouse* pMouse = dynamic_cast<CMouse*>(m_pMouse);

				pMouse->Set_InfoDC(nullptr);
			}

		}
	}
	CMouse* pMouse = dynamic_cast<CMouse*>(m_pMouse);
	

	if (pMouse->Get_Item())
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			dynamic_cast<CInventory*>(CUiMgr::Get_Instance()->Get_PlayerInventory())->Add_Item(pMouse->Get_Item());
			pMouse->Set_Item(nullptr);
		}
	}
}

void CCollisionMgr::Obj_Idle_Collision_RectEx(CObj* _pDst, CObj* _pSrc)
{
	if (!_pDst || !_pDst->IsRender() || !_pSrc || !_pSrc->IsRender())
		return;

	for (auto& vecDstCollision : _pDst->Get_VecCollisionInfo())
	{
		for (auto& vecSrcCollision : _pSrc->Get_VecCollisionInfo())
		{
			float fWidth(0.f);
			float fHeight(0.f);

			if (Check_Obj_Rect(vecDstCollision, vecSrcCollision, &fWidth, &fHeight))
			{
				if (_pDst->Get_ObjID() == OBJ_ITEM)
				{
					CItem* pItem = dynamic_cast<CItem*>(_pDst);
					if (pItem->IsEquip() || pItem->IsInventory())
						return;

					dynamic_cast<CInventory*>(CUiMgr::Get_Instance()->Get_PlayerInventory())->Add_Item(_pDst);
					_pDst->Set_Render(false);
					dynamic_cast<CItem*>(_pDst)->Set_Inventory(true);
					CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT2);
					CSoundMgr::Get_Instance()->PlaySound(L"Get_Item.wav", SOUND_EFFECT2, 0.8f);
				}
				else
				{
					if (fWidth > fHeight)
					{
						if (vecDstCollision.fY < vecSrcCollision.fY)
						{
							_pSrc->Set_PosY(fHeight);
						}

						else
						{
							_pSrc->Set_PosY(-fHeight);
						}
					}
					else
					{
						if (vecDstCollision.fX < vecSrcCollision.fX)
						{
							_pSrc->Set_PosX(fWidth);
						}

						// 우 충돌
						else
						{
							_pSrc->Set_PosX(-fWidth);
						}
					}


				}
			}

		}


	}
}

void CCollisionMgr::Obj_Attack_Collision_RectEx(CObj* _pDst, CObj* _pSrc)
{
	if (!_pDst || !_pDst->IsRender()|| !_pSrc || !_pSrc->IsRender())
		return;

	
	if (!_pSrc->IsAttack())
		return;
	
	DIRECTION eDirection;
	float fAtkPower(0.f), fMass(0.f), fSpeed(0.f);

	for (auto& vecDstCollision : _pDst->Get_VecCollisionInfo())
	{
		for (auto& vecSrcCollision : _pSrc->Get_VecCollisionInfo())
		{

			float fWidth(0.f);
			float fHeight(0.f);
			if (Check_Obj_Rect(vecDstCollision, vecSrcCollision, &fWidth, &fHeight))
			{
				fAtkPower = _pSrc->Get_AtkPower();
				fMass = _pSrc->Get_Mass();
				fSpeed = _pSrc->Get_Speed();
				// 상 충돌
				if (fWidth > fHeight)
				{
					if (vecDstCollision.fY < vecSrcCollision.fY)
					{
						eDirection = DIR_UP;
					}
					// 하 충돌
					else
					{
						eDirection = DIR_DOWN;
					}
				}
				// 좌 우 충돌
				else
				{
					// 좌 충돌
					if (vecDstCollision.fX < vecSrcCollision.fX)
					{
						eDirection = DIR_RIGHT;
					}
				
					// 우 충돌
					else
					{
						eDirection = DIR_LEFT;
					}
				}

				if(_pDst->Get_ObjID() == OBJ_ITEM)
				{
					dynamic_cast<CInventory*>(CUiMgr::Get_Instance()->Get_PlayerInventory())->Add_Item(_pDst);
			//		_pDst->Set_Dead();
				}
				else
				{
					_pDst->Hit(fAtkPower, fMass, fSpeed, eDirection);
				}

				if (_pSrc->Get_ObjID() == OBJ_PROJECTILE)
					_pSrc->Set_Dead();

				return;
			}

		}
	
			
	}
}

bool CCollisionMgr::Check_Obj_Rect(const INFO& _tDst, const INFO& _tSrc, float* _pX, float* _pY)
{
	
	float		fWidth = abs(_tDst.fX - _tSrc.fX);
	float		fHeight = abs(_tDst.fY - _tSrc.fY);

	float		fRadiusX = (_tDst.fCX + _tSrc.fCX) * 0.5f;
	float		fRadiusY = (_tDst.fCY + _tSrc.fCY) * 0.5f;

	if ((fRadiusX > fWidth) && (fRadiusY > fHeight))
	{
		*_pX = fRadiusX - fWidth;
		*_pY = fRadiusY - fHeight;

		return true;
	}

	return false;
}

void CCollisionMgr::Player_Monster_Collision(CObj* _pMonster)
{
	if (!_pMonster || !_pMonster->IsRender())
		return;


	if (!m_pPlayer->IsAttack())
		return;

	DIRECTION eDirection;
	float fAtkPower(0.f), fMass(0.f), fSpeed(0.f);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
	
	if (!pPlayer)
		return;

	switch (pPlayer->Get_Dir())
	{
	case DIR_UP:
		eDirection = DIR_DOWN;
		break;
	case DIR_DOWN:
		eDirection = DIR_UP;
		break;
	case DIR_LEFT:
		eDirection = DIR_RIGHT;
		break;
	case DIR_RIGHT:
		eDirection = DIR_LEFT;
		break;
	}

	for (auto& vecMonsterCollision : _pMonster->Get_VecCollisionInfo())
	{
		float fWidth(0.f);
		float fHeight(0.f);

		if(pPlayer->IsEquipWeapon())
		{
			for(auto& vecWeaponCollision : pPlayer->Get_EquipItem()->Get_VecCollisionInfo())
			{
				if (Check_Obj_Rect(vecWeaponCollision, vecMonsterCollision, &fWidth, &fHeight))
				{
					fAtkPower = m_pPlayer->Get_AtkPower();
					fMass = m_pPlayer->Get_Mass();
					fSpeed = m_pPlayer->Get_Speed();

					_pMonster->Hit(fAtkPower, fMass, fSpeed, eDirection);
					return;
				}
			}
		}
		else
		{
			for (auto& vecPlayerCollision : pPlayer->Get_VecCollisionInfo())
			{
				if (Check_Obj_Rect(vecPlayerCollision, vecMonsterCollision, &fWidth, &fHeight))
				{
					fAtkPower = m_pPlayer->Get_AtkPower();
					fMass = m_pPlayer->Get_Mass();
					fSpeed = m_pPlayer->Get_Speed();

					_pMonster->Hit(fAtkPower, fMass, fSpeed, eDirection);
					return;
				}
			}
		}
	}


	
}

void CCollisionMgr::Player_Wall_Collision(CObj* _pPlayer, vector<CObj*>* _pVecWall)
{
	if (!_pPlayer || !_pPlayer->IsAttack() || !_pVecWall)
		return;
	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pPlayer);
	
	if (!pPlayer->Get_CanMining())
		return;

	for(auto& vecCollision : _pPlayer->Get_VecCollisionInfo())
	{
		int	x = (int)vecCollision.fX / TILECX;
		int	y = (int)vecCollision.fY / TILECY;

		int		iCompIndex = y * TILEX + x;

		int dX(0);
		int dY(0);

		if (0 > iCompIndex || (size_t)iCompIndex >= m_pVecWall->size())
			return;

		switch (pPlayer->Get_Dir())
		{
		case DIR_LEFT:
			dX = -1;
			dY = 0;
			break;
		case DIR_RIGHT:
			dX = 1;
			dY = 0;
			break;
		case DIR_UP:
			dX = 0;
			dY = -1;
			break;
		case DIR_DOWN:
			dX = 0;
			dY = 1;
			break;
		}

		int iIndex = 0;

		CObj* pTargetWall = nullptr;
		

		iIndex = iCompIndex + dX + (dY * TILEX);

		if (iIndex < 0 || (size_t)iIndex >= m_pVecWall->size())
			return;
			

		if (dynamic_cast<CWall*>((*m_pVecWall)[iIndex])->Get_WallID() == WALL_NULL)
			return;

		for(auto& vecWallCollision : (*m_pVecWall)[iIndex]->Get_VecCollisionInfo())
		{
			if (Find_Target_Wall(vecCollision, vecWallCollision))
			{
				pTargetWall = (*m_pVecWall)[iIndex];
				break;
			}
		}

		if (pTargetWall != nullptr)
		{
			pPlayer->Set_CanMining(false);

			float fMiningPower = pPlayer->Get_MiningPower();
			pTargetWall->Hit(fMiningPower, 0.f, 0.f, DIR_END);
		}
	}
}

bool CCollisionMgr::Find_Target_Wall(const INFO& _tCollisionInfo, const INFO& _tWallCollisionInfo)
{
	float	fWidth = abs(_tCollisionInfo.fX - _tWallCollisionInfo.fX);
	float 	fHeight = abs(_tCollisionInfo.fY - _tWallCollisionInfo.fY);
		
	float		fRadiusX = (_tCollisionInfo.fCX + _tWallCollisionInfo.fCX) * 0.5f;
	float		fRadiusY = (_tCollisionInfo.fCY + _tWallCollisionInfo.fCY) * 0.5f;

	if ((fRadiusX > fWidth - 10.f) && (fRadiusY > fHeight - 10.f))
	{
		return true;  
	}
	else
		return false;

}

