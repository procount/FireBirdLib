#include "../libFireBird.h"

inline dword FIS_fwAppl_EnterNormal(void)
{
  TRACEENTER();

  static dword          _Appl_EnterNormal = 0;

  if(!_Appl_EnterNormal)
    _Appl_EnterNormal = TryResolve("_Z16Appl_EnterNormalh");

  TRACEEXIT();
  return _Appl_EnterNormal;
}
