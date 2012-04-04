#include                <string.h>
#include                <sys/types.h>
#include                <sys/stat.h>
#include                <fcntl.h>
#include                <unistd.h>
#include                <stdlib.h>
#include                <stdio.h>
#include                "tap.h"
#include                "Setup_busybox.h"
#include                "../../libFireBird.h"

#define PROGRAM_NAME    "FIS_Test"
#define LOGNAME         "FIS_Test.log"
#define FIRMWAREDAT     "FirmwareTMS.dat"

TAP_ID(1);
TAP_PROGRAM_NAME        (PROGRAM_NAME);
TAP_AUTHOR_NAME         ("FireBird");
TAP_DESCRIPTION         ("");
TAP_ETCINFO             (__DATE__);

char                    LogPuffer[256];
#define DEBUG(...)      {sprintf(LogPuffer, __VA_ARGS__); LogEntry(LOGNAME, PROGRAM_NAME, TRUE, TIMESTAMP_HMS, LogPuffer);}

dword TAP_EventHandler (word event, dword param1, dword param2)
{
  (void) event;
  (void) param2;

  return param1;
}

char                    x[256];

void APIBug_ChangeDir()
{
  int             ret;

  HDD_TAP_PushDir();
  ret = TAP_Hdd_ChangeDir("/DataFiles");  //correct directory
  HDD_TAP_PopDir();
  DEBUG("API check TAP_Hdd_ChangeDir()     : existing directory %s", ret ? "OK" : "failed");

  HDD_TAP_PushDir();
  ret = TAP_Hdd_ChangeDir("/Mp3");        //wrong directory
  HDD_TAP_PopDir();
  DEBUG("API check TAP_Hdd_ChangeDir()     : non-existing directory %s", ret ? "failed" : "OK");
}

void APIBug_fReadWrite()
{
  byte           *buffer;
  TYPE_File      *fp;
  dword           ret;

  HDD_ChangeDir("/ProgramFiles");
  TAP_Hdd_Create(PROGRAM_NAME ".test", ATTR_NORMAL);
  fp = TAP_Hdd_Fopen(PROGRAM_NAME ".test");
  if(fp)
  {
    buffer = TAP_MemAlloc(5000);
    if(buffer)
    {
      ret = TAP_Hdd_Fwrite(buffer, 500, 10, fp);
      if(ret == 10)
      {
        DEBUG("API check TAP_Hdd_Fwrite()        : OK");
      }
      else
      {
        DEBUG("API check TAP_Hdd_Fwrite()        : failed (%d instead of 10)", ret);
      }

      TAP_Hdd_Fseek(fp, 0, 0);
      ret = TAP_Hdd_Fread(buffer, 500, 10, fp);
      if(ret == 10)
      {
        DEBUG("API check TAP_Hdd_Fread()         : OK");
      }
      else
      {
        DEBUG("API check TAP_Hdd_Fread()         : failed (%d instead of 10)", ret);
      }

      TAP_MemFree(buffer);
    }
    else
    {
      DEBUG("API check TAP_Hdd_Fread()         : failed (out of memory)");
    }
    TAP_Hdd_Fclose(fp);
  }
  else
  {
    DEBUG("API check TAP_Hdd_Fread()         : failed (failed to open .test)");
  }

  TAP_Hdd_Delete(PROGRAM_NAME ".test");
}

void DumpInfo(void)
{
  tFWDATHeader         *FWDatHeader;
  tToppyInfo           *ToppyInfo;
  tFWInfo              *FWInfo;
  INILOCATION           Loc;
  SYSTEM_TYPE           SystemType = ST_UNKNOWN;
  int                   i;

  //Details about Flash access
  if (InitTAPex())
  {
    DEBUG("Main init = OK");
  }
  else
  {
    DEBUG("Main init failed");
  }

  //Details about Firmware.dat
  DEBUG("");
  HDD_TAP_PushDir();
  Loc = INILocateFile(FIRMWAREDAT, NULL);
  if (Loc == INILOCATION_NotFound)
  {
    HDD_TAP_PopDir();

    DEBUG("%s not found.", FIRMWAREDAT);
  }
  else
  {
    TAP_SPrint(x, "%s found at ", FIRMWAREDAT);
    switch(Loc)
    {
      case INILOCATION_AtCurrentDir:    strcat(x, "current directory"); break;
      case INILOCATION_AtAppName:       strcat(x, "/ProgramFiles/Settings/<appname>"); break;
      case INILOCATION_AtSettings:      strcat(x, "/ProgramFiles/Settings"); break;
      case INILOCATION_AtProgramFiles:  strcat(x, "/ProgramFiles"); break;
      default: strcat(x, "(unhandled)"); break;
    }

    if (!LoadFirmwareDat(&FWDatHeader, &ToppyInfo, &FWInfo))
    {
      HDD_TAP_PopDir();
      DEBUG("%s", x);
      DEBUG("  Failed while trying to load %s", FIRMWAREDAT);
    }
    else
    {
      HDD_TAP_PopDir();
      DEBUG("%s", x);
      DEBUG("  Compile date %d-%02d-%02d", FWDatHeader->CompileDate >> 16, (FWDatHeader->CompileDate >> 8) & 0xff, FWDatHeader->CompileDate & 0xff);
      DEBUG("  %d SysIDs, %d F/W IDs", FWDatHeader->NrOfToppyInfoEntries, FWDatHeader->NrOfFWInfoEntries);
    }

    SystemType = GetSystemType();
    if(ToppyInfo->SystemType == ST_UNKNOWN)
    {
      DEBUG("  Your SysID (%d) is not included in %s!", GetSysID(), FIRMWAREDAT);
    }
    else
    {
      for (i = 0; i < (int)FWDatHeader->NrOfToppyInfoEntries; i++, ToppyInfo++)
        if(ToppyInfo->SysID == GetSysID()) break;

      DEBUG("  SysID         =  %d", ToppyInfo->SysID);
      DEBUG("  Device        =  %s", ToppyInfo->Device);
      DEBUG("  AppType       =  %s", ToppyInfo->AppType);

      switch (ToppyInfo->SystemType)
      {
        case ST_UNKNOWN   : break;
        case ST_S         : DEBUG("  SystemType    =  ST_S"); break;
        case ST_T         : DEBUG("  SystemType    =  ST_T"); break;
        case ST_C         : DEBUG("  SystemType    =  ST_C"); break;
        case ST_T5700     : DEBUG("  SystemType    =  ST_T5700"); break;
        case ST_TMSS      : DEBUG("  SystemType    =  ST_TMSS"); break;
        case ST_TMST      : DEBUG("  SystemType    =  ST_TMST"); break;
        case ST_TMSC      : DEBUG("  SystemType    =  ST_TMSC"); break;
        case ST_T5800     : DEBUG("  SystemType    =  ST_T5800"); break;
        case ST_ST        : DEBUG("  SystemType    =  ST_ST"); break;
        case ST_CT        : DEBUG("  SystemType    =  ST_CT"); break;
        case ST_TF7k7HDPVR: DEBUG("  SystemType    =  ST_TF7k7HDPVR"); break;
        default:;
      }

      switch(ToppyInfo->DisplayType)
      {
        case DT_LED:      DEBUG("  DisplayType   =  LED"); break;
        case DT_VFD5500:  DEBUG("  DisplayType   =  TF5500 dot matrix VFD"); break;
        case DT_VFDMP:    DEBUG("  DisplayType   =  Masterpiece VFD"); break;
      }

      switch(ToppyInfo->RemoteType)
      {
        case RT_5000:     DEBUG("  RemoteType    =  5000"); break;
        case RT_2100:     DEBUG("  RemoteType    =  2100"); break;
        case RT_7100PLUS: DEBUG("  RemoteType    =  7100+"); break;
      }

      DEBUG("  NrRecs        =  %d", ToppyInfo->MaxRecStreams);
      DEBUG("  Rec extension =  %s", GetRecExtension());
    }
  }
  DEBUG("  Timer struct  =  %d", FlashTimerStructSize());

  //Details about the FindInstructionSequences
  DEBUG("");

  DEBUG("Appl_AddSvcName                    = %8.8x", FIS_fwAppl_AddSvcName());
  DEBUG("Appl_CheckRecording                = %8.8x", FIS_fwAppl_CheckRecording());
  DEBUG("Appl_CheckRecording_Tuner          = %8.8x", FIS_fwAppl_CheckRecording_Tuner());
  DEBUG("Appl_ClrTimer                      = %8.8x", FIS_fwAppl_ClrTimer());
  DEBUG("Appl_DeleteRadioSvcName            = %8.8x", FIS_fwAppl_DeleteRadioSvcName());
  DEBUG("Appl_DeleteTvSvcName               = %8.8x", FIS_fwAppl_DeleteTvSvcName());
  DEBUG("Appl_EnterNormal                   = %8.8x", FIS_fwAppl_EnterNormal());
  DEBUG("Appl_ExecProgram                   = %8.8x", FIS_fwAppl_ExecProgram());
  DEBUG("Appl_ExportChData                  = %8.8x", FIS_fwAppl_ExportChData());
  DEBUG("Appl_GetFreeExtRecordSpace         = %8.8x", FIS_fwAppl_GetFreeExtRecordSpace());
  DEBUG("Appl_GetIsExternal                 = %8.8x", FIS_fwAppl_GetIsExternal());
  DEBUG("Appl_GetStreamFormat               = %8.8x", FIS_fwAppl_GetStreamFormat());
  DEBUG("Appl_ImportChData                  = %8.8x", FIS_fwAppl_ImportChData());
  DEBUG("Appl_InitTempRec                   = %8.8x", FIS_fwAppl_InitTempRec());
  DEBUG("Appl_IsTimeShifting                = %8.8x", FIS_fwAppl_IsTimeShifting());
  DEBUG("Appl_PvrList                       = %8.8x", FIS_fwAppl_PvrList());
  DEBUG("Appl_PvrList_SetListType           = %8.8x", FIS_fwAppl_PvrList_SetListType());
  DEBUG("Appl_RestartTimeShiftSvc           = %8.8x", FIS_fwAppl_RestartTimeShiftSvc());
  DEBUG("Appl_SetApplVer                    = %8.8x", FIS_fwAppl_SetApplVer());
  DEBUG("Appl_SetIsExternal                 = %8.8x", FIS_fwAppl_SetIsExternal());
  DEBUG("Appl_SetProviderName               = %8.8x", FIS_fwAppl_SetProviderName());
  DEBUG("Appl_SetTimeShift                  = %8.8x", FIS_fwAppl_SetTimeShift());
  DEBUG("Appl_ShoutCast                     = %8.8x", FIS_fwAppl_ShoutCast());
  DEBUG("Appl_StartPlaybackMedia            = %8.8x", FIS_fwAppl_StartPlaybackMedia());
  DEBUG("Appl_StartTempRec                  = %8.8x", FIS_fwAppl_StartTempRec());
  DEBUG("Appl_StopPlaying                   = %8.8x", FIS_fwAppl_StopPlaying());
  DEBUG("Appl_StopRecPlaying                = %8.8x", FIS_fwAppl_StopRecPlaying());
  DEBUG("Appl_StopTempRec                   = %8.8x", FIS_fwAppl_StopTempRec());
  DEBUG("Appl_TimeToLocal                   = %8.8x", FIS_fwAppl_TimeToLocal());
  DEBUG("Appl_WaitEvt                       = %8.8x", FIS_fwAppl_WaitEvt());
  DEBUG("Appl_WriteFlash                    = %8.8x", FIS_fwAppl_WriteFlash());
  DEBUG("Appl_WriteRecInfo                  = %8.8x", FIS_fwAppl_WriteRecInfo());
  DEBUG("ApplChannel_GetAgc                 = %8.8x", FIS_fwApplChannel_GetAgc());
  DEBUG("ApplChannel_GetBer                 = %8.8x", FIS_fwApplChannel_GetBer());
  DEBUG("ApplHdd_FileCutPaste               = %8.8x", FIS_fwApplHdd_FileCutPaste());
  DEBUG("ApplHdd_FreeSize                   = %8.8x", FIS_fwApplHdd_FreeSize());
  DEBUG("ApplHdd_GetFileInfo                = %8.8x", FIS_fwApplHdd_GetFileInfo());
  DEBUG("ApplHdd_GetInfoFromExternalDevice  = %8.8x", FIS_fwApplHdd_GetInfoFromExternalDevice());
  DEBUG("ApplHdd_RestoreWorkFolder          = %8.8x", FIS_fwApplHdd_RestoreWorkFolder());
  DEBUG("ApplHdd_SaveWorkFolder             = %8.8x", FIS_fwApplHdd_SaveWorkFolder());
  DEBUG("ApplHdd_SelectFolder               = %8.8x", FIS_fwApplHdd_SelectFolder());
  DEBUG("ApplHdd_SetWorkFolder              = %8.8x", FIS_fwApplHdd_SetWorkFolder());
  DEBUG("ApplOsd_DrawJpeg                   = %8.8x", FIS_fwApplOsd_DrawJpeg());
  DEBUG("ApplTap_CallEventHandler           = %8.8x", FIS_fwApplTap_CallEventHandler());
  DEBUG("ApplTap_GetEmptyTask               = %8.8x", FIS_fwApplTap_GetEmptyTask());
  DEBUG("ApplTimer_OptimizeList             = %8.8x", FIS_fwApplTimer_OptimizeList());
  DEBUG("ApplTimeToLocal                    = %8.8x", FIS_fwApplTimeToLocal());
  DEBUG("ApplVfdSendData                    = %8.8x", FIS_fwApplVfdSendData());
  DEBUG("ApplVfdStart                       = %8.8x", FIS_fwApplVfdStart());
  DEBUG("ApplVfdStop                        = %8.8x", FIS_fwApplVfdStop());
  DEBUG("DevEeprom_GetMacAddr               = %8.8x", FIS_fwDevEeprom_GetMacAddr());
  DEBUG("DevEeprom_Info                     = %8.8x", FIS_fwDevEeprom_Info());
  DEBUG("DevFront_PowerOffCancel            = %8.8x", FIS_fwDevFront_PowerOffCancel());
  DEBUG("DevFront_SetIlluminate             = %8.8x", FIS_fwDevFront_SetIlluminate());
  DEBUG("DevHdd_DeviceClose                 = %8.8x", FIS_fwDevHdd_DeviceClose());
  DEBUG("DevHdd_DeviceOpen                  = %8.8x", FIS_fwDevHdd_DeviceOpen());
  DEBUG("Eeprom_DirectWrite                 = %8.8x", FIS_fwEeprom_DirectWrite());
  DEBUG("PowerOff                           = %8.8x", FIS_fwPowerOff());
  DEBUG("PutDevEvt                          = %8.8x", FIS_fwPutDevEvt());
  DEBUG("SetIrCode                          = %8.8x", FIS_fwSetIrCode());

  DEBUG("");

  DEBUG("_applEepromAddr                    = %8.8x", FIS_vEEPROM());
  DEBUG("_applFlashAddr                     = %8.8x", FIS_vFlash());
  DEBUG("_applState                         = %8.8x", FIS_vApplState());
  DEBUG("_AudioTrack                        = %8.8x", FIS_vAudioTrack());
  DEBUG("_BootReason                        = %8.8x", FIS_vBootReason());
  DEBUG("_checkAutoDecTimerId               = %8.8x", FIS_vCheckAutoDecTimerId());
  DEBUG("_curTapTask                        = %8.8x", FIS_vCurTapTask());
  DEBUG("_directSvcNumTimerId               = %8.8x", FIS_vDirectSvcNumTimerId());
  DEBUG("_EEPROM                            = %8.8x", FIS_vEEPROM());
  DEBUG("_EEPROMPin                         = %8.8x", FIS_vEEPROMPin());
  DEBUG("_EtcInfo                           = %8.8x", FIS_vEtcInfo());
  DEBUG("_extPartitionInfo                  = %8.8x", FIS_vExtPartitionInfo());
  DEBUG("_extTsFolder                       = %8.8x", FIS_vExtTsFolder());
  DEBUG("_Flash                             = %8.8x", FIS_vFlash());
  DEBUG("_grid                              = %8.8x", FIS_vGrid());
  DEBUG("_hddDivxFolder                     = %8.8x", FIS_vHddDivxFolder());
  DEBUG("_hddTapFolder                      = %8.8x", FIS_vHddTapFolder());
  DEBUG("_hddTsFolder                       = %8.8x", FIS_vHddTsFolder());
  DEBUG("_iboxTimerId                       = %8.8x", FIS_vIboxTimerId());
  DEBUG("_isAllPartitionInvalid             = %8.8x", FIS_vIsAllPartitionInvalid());
  DEBUG("_isPipActive                       = %8.8x", FIS_vIsPipActive());
  DEBUG("_isPopUpOn                         = %8.8x", FIS_vIsPopUpOn());
  DEBUG("_MACAddress                        = %8.8x", FIS_vMACAddress());
  DEBUG("_nExtPartition                     = %8.8x", FIS_vnExtPartition());
  DEBUG("_nPipSvcNum                        = %8.8x", FIS_vnPipSvcNum());
  DEBUG("_nRadioSvc                         = %8.8x", FIS_vnRadioSvc());
  DEBUG("_nTvSvc                            = %8.8x", FIS_vnTvSvc());
  DEBUG("_OSDMap                            = %8.8x", FIS_vOsdMap());
  DEBUG("_osdOutBuf                         = %8.8x", FIS_vOsdOutBuf());
  DEBUG("_ParentalInfo                      = %8.8x", FIS_vParentalInfo());
  DEBUG("_photoAlbumInfo                    = %8.8x", FIS_vPhotoAlbumInfo());
  DEBUG("_pipH                              = %8.8x", FIS_vPipH());
  DEBUG("_pipSvcNum                         = %8.8x", FIS_vPipSvcNum());
  DEBUG("_pipW                              = %8.8x", FIS_vPipW());
  DEBUG("_pipX                              = %8.8x", FIS_vPipX());
  DEBUG("_pipY                              = %8.8x", FIS_vPipY());
  DEBUG("_pvrRecTsInfo                      = %8.8x", FIS_vPvrRecTsInfo());
  DEBUG("_pvrRecTsPlayInfo                  = %8.8x", FIS_vPvrRecTsPlayInfo());
  DEBUG("_RECSlotAddress0                   = %8.8x", FIS_vRECSlotAddress(0));
  DEBUG("_RECSlotAddress1                   = %8.8x", FIS_vRECSlotAddress(1));
  DEBUG("_RECSlotAddress2                   = %8.8x", FIS_vRECSlotAddress(2));
  DEBUG("_RECSlotAddress3                   = %8.8x", FIS_vRECSlotAddress(3));
  DEBUG("_RECSlotAddress4                   = %8.8x", FIS_vRECSlotAddress(4));
  DEBUG("_selectedPartition                 = %8.8x", FIS_vSelectedPartition);
  DEBUG("_shoutCastInfo                     = %8.8x", FIS_vShoutCastInfo());
  DEBUG("_shoutCastState                    = %8.8x", FIS_vShoutCastState());
  DEBUG("_tapSysOsdCtrl                     = %8.8x", FIS_vTapSysOsdCtrl());
  DEBUG("_tapTask                           = %8.8x", FIS_vTAPTable());
  DEBUG("_timerEditInfo                     = %8.8x", FIS_vTimerEditInfo());
  DEBUG("_timerTempInfo                     = %8.8x", FIS_vTimerTempInfo());
  DEBUG("_topEvent                          = %8.8x", FIS_vTopEvent());
  DEBUG("_vfdBrightTimerId                  = %8.8x", FIS_vVfdBrightTimerId());
  DEBUG("_vfdTimerId                        = %8.8x", FIS_vVfdTimerId());

  DEBUG("");

  DEBUG("Flash");
  DEBUG("_tvSvc                             = %8.8x", FIS_vFlashBlockTVServices());
  DEBUG("_radioSvc                          = %8.8x", FIS_vFlashBlockRadioServices());
  DEBUG("_satInfo                           = %8.8x", FIS_vFlashBlockSatInfo());
  DEBUG("_tpInfo                            = %8.8x", FIS_vFlashBlockTransponderInfo());
  DEBUG("_favGrp                            = %8.8x", FIS_vFlashBlockFavoriteGroup());
  DEBUG("_gameSaveData                      = %8.8x", FIS_vFlashBlockGameSaveData());
  DEBUG("_timeInfo                          = %8.8x", FIS_vFlashBlockTimeInfo());
  DEBUG("_timer                             = %8.8x", FIS_vFlashBlockTimer());
  DEBUG("_svcName                           = %8.8x", FIS_vFlashBlockServiceName());
  DEBUG("_providerInfo                      = %8.8x", FIS_vFlashBlockProviderInfo());
  DEBUG("_otaInfo                           = %8.8x", FIS_vFlashBlockOTAInfo());
  DEBUG("_oldLanIpConfig                    = %8.8x", FIS_vFlashBlockLanIPConfig());
  DEBUG("_serverData                        = %8.8x", FIS_vFlashBlockServerData());
  DEBUG("_networkUpdateConfig               = %8.8x", FIS_vFlashBlockNetworkUpdateConfig());
  DEBUG("_network                           = %8.8x", FIS_vFlashBlockNetwork());
  DEBUG("_autoDec                           = %8.8x", FIS_vFlashBlockAutoDec());
  DEBUG("_dlnaData                          = %8.8x", FIS_vFlashBlockDLNAData());
}

void ExtractSettingsMTD(void)
{
  char                  fn[12];
  int                   fd;
  byte                  Data[2];
  ssize_t               ret;
  int                   i;

  for(i = 0; i < 10; i++)
  {
    TAP_SPrint(fn, "/dev/mtd%d", i);
    fd = open(fn, O_RDONLY);
    if(fd)
    {
      ret = read(fd, Data, 2);
      close(fd);

      if((ret == 2) && (Data[0] == 0x1d) && (Data[1] == 0x1f))
      {
        TAP_SPrint(x, "/bin/busybox dd if=%s of=/mnt/hd/ProgramFiles/mtd%d", fn, i);
        system(x);
        break;
      }
    }
  }
}

void GetMTDInfo(void)
{
  system("cp /proc/mtd /mnt/hd/ProgramFiles/mtd.log");
}

void DumpEEPROM(void)
{
  byte                 *EEPROM;
  TYPE_File            *f;

  #define EEPROMFN      "EEPROM.bin"

  EEPROM = (byte*)FIS_vEEPROM();
  if(!EEPROM)
  {
    DEBUG("EEPROM address not found");
    return;
  }

  TAP_Hdd_ChangeDir("/ProgramFiles");
  TAP_Hdd_Create(EEPROMFN, ATTR_NORMAL);
  f = TAP_Hdd_Fopen(EEPROMFN);
  if(!f)
  {
    DEBUG("Failed to open %s", EEPROMFN);
    return;
  }

  TAP_Hdd_Fwrite(EEPROM, 512, 1, f);
  TAP_Hdd_Fclose(f);
}

void DumpFlashPointer(dword SectionStart, dword SectionEnd, dword FlashStart, dword FlashEnd, char *SectionName)
{
  dword                *i, d;
  char                  s[100];

  for(i = (dword*)SectionStart; i < (dword*)SectionEnd; i++)
  {
    d = *i;
    if((d >= FlashStart) && (d <= FlashEnd))
    {
      s[0] = '\0';
      if(d == FIS_vFlashBlockAutoDec())             TAP_SPrint(s, "(_autoDec)");
      if(d == FIS_vFlashBlockDLNAData())            TAP_SPrint(s, "(_dlnaData)");
      if(d == FIS_vFlashBlockFavoriteGroup())       TAP_SPrint(s, "(_favGrp)");
      if(d == FIS_vFlashBlockGameSaveData())        TAP_SPrint(s, "(_gameSaveData)");
      if(d == FIS_vFlashBlockLanIPConfig())         TAP_SPrint(s, "(_oldLanIpConfig)");
      if(d == FIS_vFlashBlockNetwork())             TAP_SPrint(s, "(_network)");
      if(d == FIS_vFlashBlockNetworkUpdateConfig()) TAP_SPrint(s, "(_networkUpdateConfig)");
      if(d == FIS_vFlashBlockOTAInfo())             TAP_SPrint(s, "(_otaInfo)");
      if(d == FIS_vFlashBlockProviderInfo())        TAP_SPrint(s, "(_providerInfo)");
      if(d == FIS_vFlashBlockRadioServices())       TAP_SPrint(s, "(_radioSvc)");
      if(d == FIS_vFlashBlockSatInfo())             TAP_SPrint(s, "(_satInfo)");
      if(d == FIS_vFlashBlockServerData())          TAP_SPrint(s, "(_serverData)");
      if(d == FIS_vFlashBlockServiceName())         TAP_SPrint(s, "(_svcName)");
      if(d == FIS_vFlashBlockTVServices())          TAP_SPrint(s, "(_tvSvc)");
      if(d == FIS_vFlashBlockTimeInfo())            TAP_SPrint(s, "(_timeInfo)");
      if(d == FIS_vFlashBlockTimer())               TAP_SPrint(s, "(_timer)");
      if(d == FIS_vFlashBlockTransponderInfo())     TAP_SPrint(s, "(_tpInfo)");
      DEBUG("Flash pointer %8.8p (%s) -> abs=0x%8.8x rel=%8.8x %s", i, SectionName, d, d - FlashStart, s);
    }
  }
}

void SearchForFlashPointer(void)
{
  dword                 FlashStart, FlashEnd;
  dword                 SectionIndex, SectionStart, SectionSize, SectionEnd;

  FlashStart = FIS_vFlashBlockTVServices();
  FlashEnd = FlashStart + 0x200000;

  if(ELFOpenAbsFile("/root/pvr") && ELFReadELFHeader() && ELFReadSectionHeaders() && ELFReadShstrtabSection())
  {
    SectionIndex = ELFGetSectionIndex(".sbss");
    if(SectionIndex)
    {
      ELFGetSectionAddress(SectionIndex, &SectionStart, &SectionSize);
      SectionEnd = SectionStart + SectionSize;
      DumpFlashPointer(SectionStart, SectionEnd, FlashStart, FlashEnd, ".sbss");
    }

    SectionIndex = ELFGetSectionIndex(".bss");
    if(SectionIndex)
    {
      ELFGetSectionAddress(SectionIndex, &SectionStart, &SectionSize);
      SectionEnd = SectionStart + SectionSize;
      DumpFlashPointer(SectionStart, SectionEnd, FlashStart, FlashEnd, ".bss");
    }
  }
  else
  {
    TAP_PrintNet("ELFOpenAbsFile() failed on /root/pvr\n");
  }

  ELFCleanup();
}

void CopyInf(void)
{
  TYPE_FolderEntry      FolderEntry;
  int                   NrFiles, Got, i;

  HDD_TAP_PushDir();
  TAP_Hdd_ChangeDir("/DataFiles");
  NrFiles = TAP_Hdd_FindFirst(&FolderEntry, "inf");
  Got = 0;
  for (i = 0; i < NrFiles; i++)
  {
    if(FolderEntry.attr == ATTR_NORMAL)
    {
      Got++;
      TAP_SPrint(x, "cp '/mnt/hd/DataFiles/%s' /mnt/hd/ProgramFiles/inf%2.2d.inf", FolderEntry.name, Got);
      system(x);
      if(Got == 10) break;
    }
    TAP_Hdd_FindNext(&FolderEntry);
  }
  HDD_TAP_PopDir();
}

void PackAndDelete(void)
{
  #define FILELIST      "EEPROM.bin FIS_Test.log inf*.inf mtd.log mtd?"
  #define ROOTDIR       "/mnt/hd/ProgramFiles"

  TAP_SPrint(x, "cd "ROOTDIR"; ./busybox tar cvf FIS_Test.tar %s", FILELIST);
  system(x);

  TAP_SPrint(x, "cd "ROOTDIR";rm %s", FILELIST);
  system(x);

  system("cd "ROOTDIR"; ./busybox gzip FIS_Test.tar");
  rename(ROOTDIR"/FIS_Test.tar.gz", ROOTDIR"/FIS_Test.tgz");
}

int TAP_Main (void)
{
  word                  SystemID;

  InstallBusybox();

  SystemID = GetSysID();

  DEBUG("SysID=%d, ApplVer=%s (0x%4.4x)", SystemID, GetApplVer(), TAP_GetVersion());
  DEBUG("Built with FBLib version %s", __FBLIB_VERSION__);
  DEBUG("");

  APIBug_ChangeDir();
  APIBug_fReadWrite();

  DumpInfo();
  DEBUG("");
  ExtractSettingsMTD();
  GetMTDInfo();
  DumpEEPROM();
  SearchForFlashPointer();
  CopyInf();
  PackAndDelete();

  return 0;
}