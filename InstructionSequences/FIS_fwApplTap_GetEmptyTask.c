#include "../libFireBird.h"

inline dword FIS_fwApplTap_GetEmptyTask(void)
{
  static dword          _ApplTap_GetEmptyTask = 0;

  if(!_ApplTap_GetEmptyTask)
  {
    _ApplTap_GetEmptyTask = TryResolve("_Z20ApplTap_GetEmptyTaskv");
  }

  return _ApplTap_GetEmptyTask;
}
