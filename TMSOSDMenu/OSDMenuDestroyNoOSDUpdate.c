#include                <stdlib.h>
#include                "FBLib_TMSOSDMenu.h"

#undef malloc
#undef free

void OSDMenuDestroyNoOSDUpdate(void)
{
  TRACEENTER();

  if(OSDRgn)
  {
    TAP_Osd_Delete(OSDRgn);
    OSDRgn = 0;
    if(OSDMenuSelectionBarRgn) TAP_Osd_Delete(OSDMenuSelectionBarRgn);
    OSDMenuSelectionBarRgn = 0;
    OSDMenuLastCursor = LCT_NRCURSORS;
  }
  if(Menu[CurrentMenuLevel].Item)
  {
    free(Menu[CurrentMenuLevel].Item);
    Menu[CurrentMenuLevel].Item = NULL;
  }

  OSDMenuFreeStdFonts();

  CallbackProcedure = NULL;

  TRACEEXIT();
}
