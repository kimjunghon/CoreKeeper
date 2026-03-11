#include "pch.h"
#include "CBmpMgr.h"

CBmpMgr* CBmpMgr::m_pInstance = nullptr;

CBmpMgr::CBmpMgr()
{
}

CBmpMgr::~CBmpMgr()
{
	Release();
}

void CBmpMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagStringCmp(pImgKey));

	if (iter == m_mapBit.end())
	{
		CMyBmp* pBmp = new CMyBmp;
		pBmp->Load_Bmp(pFilePath);

		m_mapBit.insert({ pImgKey, pBmp });
	}

}

HDC CBmpMgr::Find_Image(const TCHAR* pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagStringCmp(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MemDC();
}

void CBmpMgr::Load_Bmp()
{

	Insert_Bmp(L"../Resource/Shadow.bmp", L"Shadow");
	Insert_Bmp(L"../Resource/Shadow_Mem.bmp", L"Shadow_Mem");
	//Background
	Insert_Bmp(L"../Resource/Title/Title_Background.bmp", L"Title_Background");
	Insert_Bmp(L"../Resource/Title/Title_Text.bmp", L"Title_Text");
	Insert_Bmp(L"../Resource/Play/Background.bmp", L"Play_Background");

	Insert_Bmp(L"../Resource/Play/Slime_Scene.bmp", L"Slime_Scene");
	Insert_Bmp(L"../Resource/Play/Hive_Scene.bmp", L"Hive_Scene");
	Insert_Bmp(L"../Resource/Play/Ending_Scene.bmp", L"Ending_Scene");

	Insert_Bmp(L"../Resource/ClearScene.bmp", L"ClearScene");
	
	
	//Player 기본
	Insert_Bmp(L"../Resource/Player/NoEquip_Miner_R.bmp", L"NoEquip_Miner_R");
	Insert_Bmp(L"../Resource/Player/NoEquip_Miner_L.bmp", L"NoEquip_Miner_L");
	//Player 투구
	Insert_Bmp(L"../Resource/Player/Iron_Helm_Miner_R.bmp", L"Iron_Helm_Miner_R");
	Insert_Bmp(L"../Resource/Player/Iron_Helm_Miner_L.bmp", L"Iron_Helm_Miner_L");
	//Player 투구_상의
	Insert_Bmp(L"../Resource/Player/Iron_HelmChest_Miner_L.bmp", L"Iron_HelmChest_Miner_L");
	Insert_Bmp(L"../Resource/Player/Iron_HelmChest_Miner_R.bmp", L"Iron_HelmChest_Miner_R");
	//Player 투구_하의
	Insert_Bmp(L"../Resource/Player/Iron_HelmPants_Miner_L.bmp", L"Iron_HelmPants_Miner_L");
	Insert_Bmp(L"../Resource/Player/Iron_HelmPants_Miner_R.bmp", L"Iron_HelmPants_Miner_R");
	//Player 상의
	Insert_Bmp(L"../Resource/Player/Iron_Chest_Miner_L.bmp", L"Iron_Chest_Miner_L");
	Insert_Bmp(L"../Resource/Player/Iron_Chest_Miner_R.bmp", L"Iron_Chest_Miner_R");
	//Player 상의_하의
	Insert_Bmp(L"../Resource/Player/Iron_ChestPants_Miner_L.bmp", L"Iron_ChestPants_Miner_L");
	Insert_Bmp(L"../Resource/Player/Iron_ChestPants_Miner_R.bmp", L"Iron_ChestPants_Miner_R");
	//Player 하의
	Insert_Bmp(L"../Resource/Player/Iron_Pants_Miner_L.bmp", L"Iron_Pants_Miner_L");
	Insert_Bmp(L"../Resource/Player/Iron_Pants_Miner_R.bmp", L"Iron_Pants_Miner_R");
	//Player 투구_상의_하의
	Insert_Bmp(L"../Resource/Player/Iron_Full_Miner_L.bmp", L"Iron_Full_Miner_L");
	Insert_Bmp(L"../Resource/Player/Iron_Full_Miner_R.bmp", L"Iron_Full_Miner_R");

	//Monster
	Insert_Bmp(L"../Resource/Monster/Slime.bmp", L"Monster_Slime");
	Insert_Bmp(L"../Resource/Monster/Boss_Slime.bmp", L"Monster_Boss_Slime");
	Insert_Bmp(L"../Resource/Monster/bigLarva_DOWN.bmp", L"Monster_Larva_DOWN");
	Insert_Bmp(L"../Resource/Monster/bigLarva_UP.bmp", L"Monster_Larva_UP");
	Insert_Bmp(L"../Resource/Monster/bigLarva_L.bmp", L"Monster_Larva_L");
	Insert_Bmp(L"../Resource/Monster/bigLarva_R.bmp", L"Monster_Larva_R");
	Insert_Bmp(L"../Resource/Monster/boss_hiveMother.bmp", L"Monster_Boss_Hive");
	Insert_Bmp(L"../Resource/Monster/LarvaEgg.bmp", L"LarvaEgg");

	Insert_Bmp(L"../Resource/Projectile/AcidProjectile.bmp", L"AcidProjectile");

	Insert_Bmp(L"../Resource/Monster/Slime_Name.bmp", L"Slime_Name");
	Insert_Bmp(L"../Resource/Monster/Hive_Name.bmp", L"Hive_Name");

	//Tile
	Insert_Bmp(L"../Resource/Tile/turf_tile.bmp", L"Tile_Turf");
	Insert_Bmp(L"../Resource/Tile/dirt_tile.bmp", L"Tile_Dirt");
	Insert_Bmp(L"../Resource/Tile/clay_tile.bmp", L"Tile_Clay");
	Insert_Bmp(L"../Resource/Tile/null_tile.bmp", L"Tile_Null");
	Insert_Bmp(L"../Resource/Tile/edit_tile.bmp", L"Tile_Edit");

	Insert_Bmp(L"../Resource/Tile/turf_wall_down.bmp", L"Down_Tile_Turf");
	Insert_Bmp(L"../Resource/Tile/dirt_wall_down.bmp", L"Down_Tile_Dirt");
	Insert_Bmp(L"../Resource/Tile/clay_wall_down.bmp", L"Down_Tile_Caly");
	
	//Wall
	Insert_Bmp(L"../Resource/Tile/Turf_Wall.bmp", L"Turf_Wall");
	Insert_Bmp(L"../Resource/Tile/Dirt_Wall.bmp", L"Dirt_Wall");
	Insert_Bmp(L"../Resource/Tile/Clay_Wall.bmp", L"Clay_Wall");
	Insert_Bmp(L"../Resource/Tile/Clay_Wall_Iron.bmp", L"Clay_Wall_Iron");

	//Item
		//Item_Weapon
	Insert_Bmp(L"../Resource/Item/sword_wood_R.bmp", L"sword_wood_R");
	Insert_Bmp(L"../Resource/Item/sword_wood_L.bmp", L"sword_wood_L");
	Insert_Bmp(L"../Resource/Item/sword_wood_UP.bmp", L"sword_wood_UP");
	Insert_Bmp(L"../Resource/Item/sword_wood_DOWN.bmp", L"sword_wood_DOWN");

	Insert_Bmp(L"../Resource/Item/pickaxeWood_L.bmp", L"pickaxeWood_L");
	Insert_Bmp(L"../Resource/Item/pickaxeWood_R.bmp", L"pickaxeWood_R");
	Insert_Bmp(L"../Resource/Item/pickaxeWood_UP.bmp", L"pickaxeWood_UP");
	Insert_Bmp(L"../Resource/Item/pickaxeWood_DOWN.bmp", L"pickaxeWood_DOWN");

	Insert_Bmp(L"../Resource/Item/wood_bow_R.bmp", L"wood_bow_R");
	Insert_Bmp(L"../Resource/Item/wood_bow_L.bmp", L"wood_bow_L");
	
	Insert_Bmp(L"../Resource/Projectile/wood_projectile_R.bmp", L"wood_projectile_R");
	Insert_Bmp(L"../Resource/Projectile/wood_projectile_L.bmp", L"wood_projectile_L");

	Insert_Bmp(L"../Resource/Item/sword_iron_R.bmp", L"sword_iron_R");
	Insert_Bmp(L"../Resource/Item/sword_iron_L.bmp", L"sword_iron_L");
	Insert_Bmp(L"../Resource/Item/sword_iron_UP.bmp", L"sword_iron_UP");
	Insert_Bmp(L"../Resource/Item/sword_iron_DOWN.bmp", L"sword_iron_DOWN");

	Insert_Bmp(L"../Resource/Item/pickaxe_iron_R.bmp", L"pickaxe_iron_R");
	Insert_Bmp(L"../Resource/Item/pickaxe_iron_L.bmp", L"pickaxe_iron_L");
	Insert_Bmp(L"../Resource/Item/pickaxe_iron_UP.bmp", L"pickaxe_iron_UP");
	Insert_Bmp(L"../Resource/Item/pickaxe_iron_DOWN.bmp", L"pickaxe_iron_DOWN");

	Insert_Bmp(L"../Resource/Item/iron_bow_R.bmp", L"iron_bow_R");
	Insert_Bmp(L"../Resource/Item/iron_bow_L.bmp", L"iron_bow_L");

	Insert_Bmp(L"../Resource/Projectile/iron_arrow_projectile_L.bmp", L"iron_projectile_L");
	Insert_Bmp(L"../Resource/Projectile/iron_arrow_projectile_R.bmp", L"iron_projectile_R");
		//Item_Armor
	Insert_Bmp(L"../Resource/Item/Iron_Helm.bmp", L"Iron_Helm");
	Insert_Bmp(L"../Resource/Item/Iron_Chest.bmp", L"Iron_Chest");
	Insert_Bmp(L"../Resource/Item/Iron_Pants.bmp", L"Iron_Pants");
		//Item_Use
	Insert_Bmp(L"../Resource/Item/Turf_Wall_Item.bmp", L"Turf_Wall_Item");
	Insert_Bmp(L"../Resource/Item/Dirt_Wall_Item.bmp", L"Dirt_Wall_Item");
	Insert_Bmp(L"../Resource/Item/Clay_Wall_Item.bmp", L"Clay_Wall_Item");
	Insert_Bmp(L"../Resource/Item/Iron.bmp", L"Iron");
	Insert_Bmp(L"../Resource/Item/Iron_Stone.bmp", L"Iron_Stone");
	Insert_Bmp(L"../Resource/Item/Slime_Core.bmp", L"Slime_Core");
	Insert_Bmp(L"../Resource/Item/Hive_Core.bmp", L"Hive_Core");
	Insert_Bmp(L"../Resource/Item/Slime_Meat.bmp", L"Slime_Meat");
	Insert_Bmp(L"../Resource/Item/Larva_Meat.bmp", L"Larva_Meat");
	Insert_Bmp(L"../Resource/Item/Slime_Food.bmp", L"Slime_Food");
	Insert_Bmp(L"../Resource/Item/Larva_Food.bmp", L"Larva_Food");

	//Buliding
	Insert_Bmp(L"../Resource/Buliding/CraftingTable.bmp", L"CraftingTable");
	Insert_Bmp(L"../Resource/Buliding/Core.bmp", L"Core");
	Insert_Bmp(L"../Resource/Buliding/Slime_Core_Buliding.bmp", L"Slime_Core_Buliding");
	Insert_Bmp(L"../Resource/Buliding/Hive_Core_Buliding.bmp", L"Hive_Core_Buliding");
	Insert_Bmp(L"../Resource/Buliding/CookingPot.bmp", L"CookingPot");
	Insert_Bmp(L"../Resource/Buliding/Core_Buliding.bmp", L"Core_Buliding"); 
	Insert_Bmp(L"../Resource/Buliding/Forge.bmp", L"Forge");
	//UI
	Insert_Bmp(L"../Resource/UI/Cursor.bmp", L"Cursor");
	Insert_Bmp(L"../Resource/UI/inven_background.bmp", L"Inven_Background");
	Insert_Bmp(L"../Resource/UI/UI_Slot.bmp", L"UI_Slot");
	Insert_Bmp(L"../Resource/UI/UI_Quick_Slot.bmp", L"UI_Quick_Slot");
	Insert_Bmp(L"../Resource/UI/UI_Player_HpBar.bmp", L"Player_HpBar");
	Insert_Bmp(L"../Resource/UI/UI_Player_HpBar_Background.bmp", L"UI_Player_HpBar_Background");
	Insert_Bmp(L"../Resource/UI/UI_Boss_HpBar.bmp", L"UI_Boss_HpBar");
	Insert_Bmp(L"../Resource/UI/UI_Boss_HpBar_Background.bmp", L"UI_Boss_HpBar_Background");

	Insert_Bmp(L"../Resource/UI/Crafting_UI.bmp", L"Crafting_UI");
	Insert_Bmp(L"../Resource/UI/FontNumber.bmp", L"FontNumber");
	Insert_Bmp(L"../Resource/UI/HitFontNumber.bmp", L"HitFontNumber");

	Insert_Bmp(L"../Resource/UI/Crafting_iron_helm.bmp", L"Crafting_iron_helm");
	Insert_Bmp(L"../Resource/UI/Crafting_iron_chest.bmp", L"Crafting_iron_chest");
	Insert_Bmp(L"../Resource/UI/Crafting_iron_pants.bmp", L"Crafting_iron_pants");
	Insert_Bmp(L"../Resource/UI/Crafting_pickaxe_iron.bmp", L"Crafting_pickaxe_iron");
	Insert_Bmp(L"../Resource/UI/Crafting_Sword_Iron.bmp", L"Crafting_Sword_Iron");
	Insert_Bmp(L"../Resource/UI/Crafting_iron_bow.bmp", L"Crafting_iron_bow");

	Insert_Bmp(L"../Resource/UI/Crafting_Sword_wood.bmp", L"Crafting_Sword_wood");
	Insert_Bmp(L"../Resource/UI/Crafting_pickaxe_wood.bmp", L"Crafting_pickaxe_wood");
	Insert_Bmp(L"../Resource/UI/Crafting_wood_bow.bmp", L"Crafting_wood_bow");

	Insert_Bmp(L"../Resource/UI/Cooking_Larva_food.bmp", L"Cooking_Larva_food");
	Insert_Bmp(L"../Resource/UI/Cooking_Slime_food.bmp", L"Cooking_Slime_food");

	Insert_Bmp(L"../Resource/UI/EquipUi.bmp", L"EquipUi");
	Insert_Bmp(L"../Resource/UI/Equip_Slot_Chest.bmp", L"Equip_Slot_Chest");
	Insert_Bmp(L"../Resource/UI/Equip_Slot_Helm.bmp", L"Equip_Slot_Helm");
	Insert_Bmp(L"../Resource/UI/Equip_Slot_Pants.bmp", L"Equip_Slot_Pants");

	Insert_Bmp(L"../Resource/UI/Core_Equip.bmp", L"Core_Equip");
	Insert_Bmp(L"../Resource/UI/Core_Slot.bmp", L"Core_Slot");
	Insert_Bmp(L"../Resource/UI/Slime_Core_Slot.bmp", L"Slime_Core_Slot");
	Insert_Bmp(L"../Resource/UI/Hive_Core_Slot.bmp", L"Hive_Core_Slot");

	Insert_Bmp(L"../Resource/UI/Player_Stat.bmp", L"Player_Stat");

	Insert_Bmp(L"../Resource/UI/Minimap_Background.bmp", L"Minimap_Background");
	Insert_Bmp(L"../Resource/UI/Minimap_Full_Background.bmp", L"Minimap_Full_Background");
	Insert_Bmp(L"../Resource/UI/Minimap_Dirt.bmp", L"Minimap_Dirt");
	Insert_Bmp(L"../Resource/UI/Minimap_Turf.bmp", L"Minimap_Turf");
	Insert_Bmp(L"../Resource/UI/Minimap_Clay.bmp", L"Minimap_Clay");
	Insert_Bmp(L"../Resource/UI/Minimap_Null.bmp", L"Minimap_Null");
	Insert_Bmp(L"../Resource/UI/Minimap_Player.bmp", L"Minimap_Player");

	Insert_Bmp(L"../Resource/UI/Forge_Fire.bmp", L"Forge_Fire");

		//Dialogue
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Dialogue.bmp", L"Slime_NPC_Dialogue");
	Insert_Bmp(L"../Resource/UI/Button_Start.bmp", L"Button_Start");
	Insert_Bmp(L"../Resource/UI/Button_Trade.bmp", L"Button_Trade");
	Insert_Bmp(L"../Resource/UI/Button_Stop.bmp", L"Button_Stop");
	Insert_Bmp(L"../Resource/UI/Button_Accept.bmp", L"Button_Accept");
	Insert_Bmp(L"../Resource/UI/Button_Refuse.bmp", L"Button_Refuse");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Talk_1.bmp", L"Slime_NPC_Talk_1");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_1.bmp", L"Slime_NPC_Quest_1");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_Clear_1.bmp", L"Slime_NPC_Quest_Clear_1");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_Fail_1.bmp", L"Slime_NPC_Quest_Fail_1");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Talk_2.bmp", L"Slime_NPC_Talk_2");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_2.bmp", L"Slime_NPC_Quest_2");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_Clear_2.bmp", L"Slime_NPC_Quest_Clear_2");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_Fail_2.bmp", L"Slime_NPC_Quest_Fail_2");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Talk_3.bmp", L"Slime_NPC_Talk_3");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_3.bmp", L"Slime_NPC_Quest_3");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_Clear_3.bmp", L"Slime_NPC_Quest_Clear_3");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_Fail_3.bmp", L"Slime_NPC_Quest_Fail_3");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Talk_4.bmp", L"Slime_NPC_Talk_4");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_4.bmp", L"Slime_NPC_Quest_4");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_Clear_4.bmp", L"Slime_NPC_Quest_Clear_4");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_Fail_4.bmp", L"Slime_NPC_Quest_Fail_4");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Talk_5.bmp", L"Slime_NPC_Talk_5");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_5.bmp", L"Slime_NPC_Quest_5");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_Clear_5.bmp", L"Slime_NPC_Quest_Clear_5");
	Insert_Bmp(L"../Resource/UI/Slime_NPC_Quest_Fail_5.bmp", L"Slime_NPC_Quest_Fail_5");

	Insert_Bmp(L"../Resource/UI/Quest_Help_Clear.bmp", L"Slime_NPC_Quest_Help_Clear");
	Insert_Bmp(L"../Resource/UI/Quest_Help_1.bmp", L"Slime_NPC_Quest_Help_1");
	Insert_Bmp(L"../Resource/UI/Quest_Help_2.bmp", L"Slime_NPC_Quest_Help_2");
	Insert_Bmp(L"../Resource/UI/Quest_Help_3.bmp", L"Slime_NPC_Quest_Help_3");
	Insert_Bmp(L"../Resource/UI/Quest_Help_4.bmp", L"Slime_NPC_Quest_Help_4");
	Insert_Bmp(L"../Resource/UI/Quest_Help_5.bmp", L"Slime_NPC_Quest_Help_5");
	//Effect
	Insert_Bmp(L"../Resource/Effect/Warning.bmp", L"Warning");
	Insert_Bmp(L"../Resource/Effect/Hit_Effect.bmp", L"Hit_Effect");
	Insert_Bmp(L"../Resource/Effect/AcidImpact.bmp", L"AcidImpact");
	Insert_Bmp(L"../Resource/Effect/Egg_Hatching.bmp", L"Egg_Hatching");
	Insert_Bmp(L"../Resource/Effect/Boss_Slime_Death.bmp", L"Boss_Slime_Death");
	Insert_Bmp(L"../Resource/Effect/Death_Effect.bmp", L"Death_Effect");
	Insert_Bmp(L"../Resource/Effect/Boss_Hive_Death.bmp", L"Boss_Hive_Death");
	Insert_Bmp(L"../Resource/Effect/Potal.bmp", L"Potal");
	Insert_Bmp(L"../Resource/Effect/PowerUp.bmp", L"PowerUp");
	
	//NPC
	Insert_Bmp(L"../Resource/NPC/Slime_NPC.bmp", L"Slime_NPC");
	Insert_Bmp(L"../Resource/NPC/QuestIcon.bmp", L"QuestIcon");
	

	//Item Info
		//Wall
	Insert_Bmp(L"../Resource/Info/Turf_Wall_Info.bmp", L"Turf_Wall_Info");
	Insert_Bmp(L"../Resource/Info/Dirt_Wall_Info.bmp", L"Dirt_Wall_Info");
	Insert_Bmp(L"../Resource/Info/Clay_Wall_Info.bmp", L"Clay_Wall_Info");

		//Wood Weapons
	Insert_Bmp(L"../Resource/Info/WoodSword_Info.bmp", L"WoodSword_Info");
	Insert_Bmp(L"../Resource/Info/WoodSword_Crafting_Info.bmp", L"WoodSword_Crafting_Info");

	Insert_Bmp(L"../Resource/Info/WoodPickaxe_Info.bmp", L"WoodPickaxe_Info");
	Insert_Bmp(L"../Resource/Info/WoodPickaxe_Crafting_Info.bmp", L"WoodPickaxe_Crafting_Info");

	Insert_Bmp(L"../Resource/Info/WoodBow_Info.bmp", L"WoodBow_Info");
	Insert_Bmp(L"../Resource/Info/WoodBow_Crafting_Info.bmp", L"WoodBow_Crafting_Info");

		//Slime
	Insert_Bmp(L"../Resource/Info/SlimeFood_Info.bmp", L"SlimeFood_Info");
	Insert_Bmp(L"../Resource/Info/SlimeFood_Crafting_Info.bmp", L"SlimeFood_Crafting_Info");

	Insert_Bmp(L"../Resource/Info/Slime_Info.bmp", L"Slime_Info");
	
	Insert_Bmp(L"../Resource/Info/SlimeCore_Info.bmp", L"SlimeCore_Info");

		//Iron
	Insert_Bmp(L"../Resource/Info/IronStone_Info.bmp", L"IronStone_Info");
	Insert_Bmp(L"../Resource/Info/Iron_Info.bmp", L"Iron_Info");

		//Iron Weapon
	Insert_Bmp(L"../Resource/Info/IronSword_Info.bmp", L"IronSword_Info");
	Insert_Bmp(L"../Resource/Info/IronSword_Crafting_Info.bmp", L"IronSword_Crafting_Info");
	Insert_Bmp(L"../Resource/Info/IronPickaxe_Info.bmp", L"IronPickaxe_Info");
	Insert_Bmp(L"../Resource/Info/IronPickaxe_Crafting_Info.bmp", L"IronPickaxe_Crafting_Info");
	Insert_Bmp(L"../Resource/Info/IronBow_Info.bmp", L"IronBow_Info");
	Insert_Bmp(L"../Resource/Info/IronBow_Crafting_Info.bmp", L"IronBow_Crafting_Info");
		//Iron Armor
	Insert_Bmp(L"../Resource/Info/IronHelm_Info.bmp", L"IronHelm_Info");
	Insert_Bmp(L"../Resource/Info/IronHelm_Crafting_Info.bmp", L"IronHelm_Crafting_Info");
	Insert_Bmp(L"../Resource/Info/IronChest_Info.bmp", L"IronChest_Info");
	Insert_Bmp(L"../Resource/Info/IronChest_Crafting_Info.bmp", L"IronChest_Crafting_Info");
	Insert_Bmp(L"../Resource/Info/IronPants_Info.bmp", L"IronPants_Info");
	Insert_Bmp(L"../Resource/Info/IronPants_Crafting_Info.bmp", L"IronPants_Crafting_Info");
		//Larva
	Insert_Bmp(L"../Resource/Info/LarvaMeat_Info.bmp", L"LarvaMeat_Info");

	Insert_Bmp(L"../Resource/Info/LarvaFood_Info.bmp", L"LarvaFood_Info");
	Insert_Bmp(L"../Resource/Info/LarvaFood_Crafting_Info.bmp", L"LarvaFood_Crafting_Info");

	Insert_Bmp(L"../Resource/Info/HiveCore_Info.bmp", L"HiveCore_Info");


}

void CBmpMgr::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), [](auto& MyPair)
		{
			if (MyPair.second)
			{
				delete MyPair.second;
				MyPair.second = nullptr;
			}
		});

	m_mapBit.clear();
}
