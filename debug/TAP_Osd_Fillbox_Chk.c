#include                <stdio.h>
#include                "../libFireBird.h"

int TAP_Osd_FillBox_Chk(char *Comment, word rgn, dword x, dword y, dword w, dword h, dword color)
{
  TRACEENTER();

  int                   ret;

  if(Comment)
  {
    if(!isOSDRegionAlive(rgn)) LogEntryFBLibPrintf(TRUE, "TAP_Osd_FillBox_Chk Warning: rgn(%d) points to an undefined region @ %s", rgn, Comment);
    if(x > 719) LogEntryFBLibPrintf(TRUE, "TAP_Osd_FillBox_Chk Warning: x(%d) out of range @ %s", x, Comment);
    if(y > 575) LogEntryFBLibPrintf(TRUE, "TAP_Osd_FillBox_Chk Warning: y(%d) out of range @ %s", y, Comment);
    if((x + w) > 719) LogEntryFBLibPrintf(TRUE, "TAP_Osd_FillBox_Chk Warning: x(%d) + w(%d) out of range @ %s", x, w, Comment);
    if((y + h) > 575) LogEntryFBLibPrintf(TRUE, "TAP_Osd_FillBox_Chk Warning: y(%d) + h(%d) out of range @ %s", y, h, Comment);
  }

  ret = TAP_Osd_FillBox(rgn, x, y, w, h, color);

  if(Comment && ret) LogEntryFBLibPrintf(TRUE, "TAP_Osd_FillBox_Chk Warning: TAP_Osd_Create() returned %d @ %s", ret, Comment);

  TRACEEXIT();
  return ret;
}
