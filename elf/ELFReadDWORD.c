#include                "ELF.h"
#include                "FBLib_elf.h"

bool ELFReadDWORD(dword SectionIndex, dword *Data)
{
  TRACEENTER();

  Elf32_Shdr           *pSection = NULL;     //Shortcut to speed up access

  pSection = &SectionHeaders[SectionIndex];

  if(pSection->sh_size != sizeof(dword))
  {
    TRACEEXIT();
    return FALSE;
  }

  if(Data)
  {
    lseek(fTAP, pSection->sh_offset, SEEK_SET);
    if(read(fTAP, Data, pSection->sh_size) != (int)pSection->sh_size)
    {
      TRACEEXIT();

      return FALSE;
    }
  }

  TRACEEXIT();
  return TRUE;
}
