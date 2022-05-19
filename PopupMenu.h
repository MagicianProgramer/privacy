#ifndef __POPUPMENU_H__
#define __POPUPMENU_H__

#include <vector>
using namespace std;

BOOL PopupMenu(UINT nMenuID, UINT nSubMenuPos, CWnd* pParentWnd);
BOOL PopupMenu(CMenu* pPopupMenu, CWnd* pParentWnd);
BOOL PopupMenu(UINT nMenuID, UINT nSubMenuPos, vector<UINT> &disableMenus, CWnd* pParentWnd, BOOL bRemove = FALSE);

void DisableMenus(vector<UINT> &disableMenu);
void DisableMenus2(vector<UINT> &disableMenu);
void MakeMenuStyle(MENUMEMBER &menustyle);
#endif