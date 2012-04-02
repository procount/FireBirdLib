#include                "FBLib_TMSOSDMenu.h"

bool OSDMenuSelectItem(int ItemIndex)
{
  tMenu                *pMenu;

  pMenu = &Menu[CurrentMenuLevel];

  if(pMenu->NrItems == 0) return FALSE;
  if(ItemIndex < 0) ItemIndex = 0;
  if(ItemIndex >= (int)pMenu->NrItems) ItemIndex = (int)pMenu->NrItems - 1;

  if(ItemIndex != (int)pMenu->CurrentSelection)
  {
    if(!pMenu->Item[ItemIndex].Selectable) return FALSE;

    pMenu->CurrentSelection = ItemIndex;
    ListDirty = TRUE;
  }

  return TRUE;
}
