#include "../libFireBird.h"

inline dword FIS_vhddRecordFile(void)
{
  static dword          vhddRecordFile = 0;

  if(!vhddRecordFile) vhddRecordFile = (dword)TryResolve("_hddRecordFile");

  return vhddRecordFile;
}
