#include                "FBLib_EPG.h"

bool EPGInfo_FindCurrent(byte SvcType, int SvcNum, TYPE_EPGInfo *EPGData)
{
  TYPE_EvtInfo            *EvtInfo;
  short                   Offset;
  dword                   EventStartTime, EventEndTime;
  TYPE_TapChInfo          chInfo;
  bool                    ret;

  TRACEENTER();

  ret = FALSE;
  if(EPGData)
  {
    TAP_Channel_GetInfo(SvcType, SvcNum, &chInfo);

    memset(EPGData, 0, sizeof(TYPE_EPGInfo));

    EvtInfo = Appl_GetSameTimeEvent(chInfo.satIdx, chInfo.orgNetId, chInfo.tsId, chInfo.svcId);
    if(EvtInfo)
    {
      EventStartTime = UTC2LocalTime(EvtInfo->StartTime, &Offset);
      EventEndTime = UTC2LocalTime(EvtInfo->EndTime, NULL);
      EPGInfo_CopyData(EvtInfo, EPGData, EventStartTime, EventEndTime, Offset);
      ret = TRUE;
    }
  }

  TRACEEXIT();
  return ret;
}
