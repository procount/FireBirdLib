#include "../libFireBird.h"

inline dword FIS_vPipW(void)
{
  static dword          _pipW = 0;

  if (!_pipW)
    _pipW = TryResolve("_pipW");

  return _pipW;
}
