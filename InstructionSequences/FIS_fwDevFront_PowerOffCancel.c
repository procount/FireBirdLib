#include "../libFireBird.h"

inline dword FIS_fwDevFront_PowerOffCancel(void)
{
  TRACEENTER();

  static dword          fwDevFront_PowerOffCancel = 0;

  if(!fwDevFront_PowerOffCancel)
    fwDevFront_PowerOffCancel = TryResolve("DevFront_PowerOffCancel");

  TRACEEXIT();
  return fwDevFront_PowerOffCancel;
}
