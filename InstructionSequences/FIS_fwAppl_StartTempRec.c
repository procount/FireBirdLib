#include "../libFireBird.h"

inline dword FIS_fwAppl_StartTempRec(void)
{
  static dword          _Appl_StartTempRec = 0;

  if (!_Appl_StartTempRec)
    _Appl_StartTempRec = TryResolve("_Z17Appl_StartTempRecb");

  return _Appl_StartTempRec;
}
