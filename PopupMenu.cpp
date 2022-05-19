#include "stdafx.h"

#include "PopupMenu.h"
#include "resource.h"

#define SKINMENU_USE

void CmdRouteMenu(CWnd* pWnd,CMenu* pPopupMenu)
{
	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	state.m_pParentMenu = pPopupMenu;
	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();

	for (state.m_nIndex = 0; 
	     state.m_nIndex < state.m_nIndexMax; 
	     state.m_nIndex++) 
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);

		// menu separator or invalid cmd - ignore it
		if (state.m_nID == 0) continue; 

		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to child menu if so
			CMenu* pSub=pPopupMenu->GetSubMenu(state.m_nIndex);
			if(pSub) CmdRouteMenu(pWnd,pSub);
		}
		else 
		{
			// normal menu item, Auto disable if command is 
			// _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(pWnd, FALSE);
		}
	}
}


BOOL PopupMenu(UINT nMenuID, UINT nSubMenuPos, CWnd* pParentWnd)
{
	vector<UINT> disableMenus;
	return PopupMenu(nMenuID, nSubMenuPos, disableMenus, pParentWnd);
}


void MakeMenuStyle(MENUMEMBER &menustyle) 
{
	LOGFONT lf = {0};
	lf.lfHeight = -13;
	lf.lfWeight = FW_LIGHT;

	_tcscpy_s(lf.lfFaceName, "consolas");

	menustyle.m_nItemHeight = 22;
	menustyle.m_nIconAreaWidth = 22;

	// Noraml
	memcpy(&menustyle.m_NormalItemProp.m_lfItem, &lf,  sizeof(LOGFONT));
	menustyle.m_NormalItemProp.m_crItemBk = RGB(255, 255, 255);

	// Highlight
	memcpy(&menustyle.m_HighlightItemProp.m_lfItem, &lf,  sizeof(LOGFONT));
	menustyle.m_HighlightItemProp.m_crItemBk = RGB(63, 111, 248);

	// Disable
	memcpy(&menustyle.m_DisabledItemProp.m_lfItem, &lf,  sizeof(LOGFONT));
	menustyle.m_DisabledItemProp.m_crItemBk = RGB(218, 218, 218);
}

typedef struct _MenuIconInfo{
	UINT menuID;
	UINT resourceID;
} MenuIconInfo;

/*MenuIconInfo menuIconInf[] = {
	{ID_CLIENT_LOTTERY, IDI_ICON_LOTTERY},
	{ID_CLIENT_TEST, IDI_ICON_REQ},
};*/

void SetIcon(CSkinMenu *pSkinMenu)
{
	/*int cnt = sizeof(menuIconInf) / sizeof(menuIconInf[0]);

	for (int i = 0; i < cnt; i++)
	{
		if (pSkinMenu->GetMenuState(menuIconInf[i].menuID, MF_BYCOMMAND) != 0xFFFFFFFF)
		{
			HICON hIcon = AfxGetApp()->LoadIcon(menuIconInf[i].resourceID);
			pSkinMenu->SetIcon(menuIconInf[i].menuID, &hIcon, FALSE);
			DestroyIcon(hIcon);
		}
	}*/
}

BOOL PopupMenu(CMenu* pPopupMenu, CWnd* pParentWnd)
{
#ifndef SKINMENU_USE
	CmdRouteMenu(pParentWnd, pPopupMenu);
	POINT pt;
	::GetCursorPos(&pt);
	if (pParentWnd != NULL)
	{
		pParentWnd->SetForegroundWindow();
	}

	BOOL bRes = pPopupMenu->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, pParentWnd, NULL);
	if (pParentWnd != NULL)
		pParentWnd->PostMessage(WM_NULL, 0, 0);
	return bRes;
#else
	MENUMEMBER menustyle;
	MakeMenuStyle(menustyle);

	CSkinMenu skinMenu;
	skinMenu.BindMenuStyle(&menustyle); // CkinMenu.m_hMenu == NULL

	skinMenu.CreatePopupMenu();
	skinMenu.CopyMenu(pPopupMenu->m_hMenu, &skinMenu);
	SetIcon(&skinMenu);

	POINT pt;
	::GetCursorPos(&pt);	
	BOOL bRes = skinMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y, pParentWnd);
	return bRes;
#endif
}

BOOL PopupMenu(UINT nMenuID, UINT nSubMenuPos, vector<UINT> &disableMenus, CWnd* pParentWnd, BOOL bRemove)
{
	CMenu menu;
	if (!menu.LoadMenu(nMenuID))
		return FALSE;

	CMenu* pSub = menu.GetSubMenu(nSubMenuPos);
	if (pSub == NULL)
		return FALSE;

	for	(size_t i = 0; i < disableMenus.size() ; i++)
	{
		if (!bRemove)
			pSub->EnableMenuItem(disableMenus[i], MF_BYCOMMAND | MF_DISABLED | MF_GRAYED); // SkinMenu
		else
			pSub->RemoveMenu(disableMenus[i], MF_BYCOMMAND);
	}	
	return PopupMenu(pSub, pParentWnd);
}

void DisableMenus(vector<UINT> &disableMenu)
{
	/*if (!FUNCTION_MYDESKTOP) disableMenu.push_back(ID_MYDESKTOP);
	if (!FUNCTION_MYSCREEN) disableMenu.push_back(ID_MYSCREEN);
	if (!FUNCTION_OTHERSCREEN) disableMenu.push_back(ID_OTHERSCREEN);
	if (!FUNCTION_REMOTEDESKTOP) disableMenu.push_back(ID_REMOTEDESKTOP);
	if (!FUNCTION_VOICE) disableMenu.push_back(ID_VOICE);
	if (!FUNCTION_VIDEO) disableMenu.push_back(ID_VIDEO);
	if (!FUNCTION_VOICE_BROADCAST) disableMenu.push_back(ID_VOICE_BROADCAST);
	if (!FUNCTION_VIDEO_BROADCAST) disableMenu.push_back(ID_VIDEO_BROADCAST);*/	
}

void DisableMenus2(vector<UINT> &disableMenu)
{
	/*if (!FUNCTION_MYSCREEN) disableMenu.push_back(ID_MYSCREEN);
	if (!FUNCTION_MYDESKTOP) disableMenu.push_back(ID_MYDESKTOP);
 	disableMenu.push_back(ID_FOLDER);
	if (!FUNCTION_VOICE_BROADCAST) disableMenu.push_back(ID_VOICE_BROADCAST);
	if (!FUNCTION_VIDEO_BROADCAST) disableMenu.push_back(ID_VIDEO_BROADCAST);
	if (!FUNCTION_VOICE) disableMenu.push_back(ID_VOICE);
	if (!FUNCTION_VIDEO) disableMenu.push_back(ID_VIDEO);
	if (!FUNCTION_OTHERSCREEN) disableMenu.push_back(ID_OTHERSCREEN);
	if (!FUNCTION_REMOTEDESKTOP) disableMenu.push_back(ID_REMOTEDESKTOP);*/
}

