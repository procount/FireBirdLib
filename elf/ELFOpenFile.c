#include                <stdio.h>
#include                "ELF.h"
#include                "FBLib_elf.h"

int                      fTAP = 0;
Elf32_Ehdr              *ELFHeader = NULL;
Elf32_Shdr              *SectionHeaders = NULL;
char                    *shstrtab = NULL;
char                    *strtab = NULL;
Elf32_Sym               *symtab = NULL;

bool ELFOpenFile(char *FileName)
{
  char                  CurrentFile[512];

  memset(CurrentFile, 0, 512);
  TAP_SPrint(CurrentFile, "%s", TAPFSROOT);
  HDD_TAP_GetCurrentDir(&CurrentFile[strlen(CurrentFile)]);
  TAP_SPrint(&CurrentFile[strlen(CurrentFile)], "/%s", FileName);

  return ELFOpenAbsFile(CurrentFile);
}
