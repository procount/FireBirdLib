#include                <sys/types.h>
#include                <sys/shm.h>
#include                "FBLib_tap.h"

void *HDD_TAP_GetStartParameter (void)
{
  int                   shmidParameterBlock;

  shmidParameterBlock = shmget(PARAMBLOCKKEY, sizeof(TYPE_Parametered_Tap), 0);
  if(shmidParameterBlock == -1) return NULL;

  fbl_parametered_tap = (TYPE_Parametered_Tap*)shmat(shmidParameterBlock, 0, 0);
  if((int)fbl_parametered_tap == -1) return NULL;

  return (void*)fbl_parametered_tap->pParameterBlock;
}
