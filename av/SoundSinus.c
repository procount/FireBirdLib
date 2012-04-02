#include "../libFireBird.h"

short                   *soundData = NULL;
dword                   soundDataLength = 0;

void SoundSinus(word freq, dword durationInMilliseconds, word Amplitude)
{
  dword                 samples, memSize, periode, index;
  short                 *target, *source;

  short                 Sinus [360] = {0x0000, 0x023C, 0x0478, 0x06B3, 0x08EE, 0x0B28, 0x0D61, 0x0F99, 0x11D0, 0x1406, 0x163A, 0x186C, 0x1A9D, 0x1CCB, 0x1EF7, 0x2121,
                                      0x2348, 0x256C, 0x278E, 0x29AC, 0x2BC7, 0x2DDF, 0x2FF3, 0x3203, 0x3410, 0x3618, 0x381C, 0x3A1C, 0x3C17, 0x3E0E, 0x4000, 0x41EC,
                                      0x43D4, 0x45B6, 0x4793, 0x496A, 0x4B3C, 0x4D08, 0x4ECD, 0x508D, 0x5246, 0x53F9, 0x55A5, 0x574B, 0x58EA, 0x5A82, 0x5C13, 0x5D9C,
                                      0x5F1F, 0x609A, 0x620D, 0x6379, 0x64DD, 0x6639, 0x678D, 0x68D9, 0x6A1D, 0x6B59, 0x6C8C, 0x6DB7, 0x6ED9, 0x6FF3, 0x7104, 0x720C,
                                      0x730B, 0x7401, 0x74EE, 0x75D2, 0x76AD, 0x777F, 0x7847, 0x7906, 0x79BB, 0x7A67, 0x7B0A, 0x7BA2, 0x7C32, 0x7CB7, 0x7D33, 0x7DA5,
                                      0x7E0D, 0x7E6C, 0x7EC0, 0x7F0B, 0x7F4B, 0x7F82, 0x7FAF, 0x7FD2, 0x7FEB, 0x7FFA, 0x7FFF, 0x7FFA, 0x7FEB, 0x7FD2, 0x7FAF, 0x7F82,
                                      0x7F4B, 0x7F0B, 0x7EC0, 0x7E6C, 0x7E0D, 0x7DA5, 0x7D33, 0x7CB7, 0x7C32, 0x7BA2, 0x7B0A, 0x7A67, 0x79BB, 0x7906, 0x7847, 0x777F,
                                      0x76AD, 0x75D2, 0x74EE, 0x7401, 0x730B, 0x720C, 0x7104, 0x6FF3, 0x6ED9, 0x6DB7, 0x6C8C, 0x6B59, 0x6A1D, 0x68D9, 0x678D, 0x6639,
                                      0x64DD, 0x6379, 0x620D, 0x609A, 0x5F1F, 0x5D9C, 0x5C13, 0x5A82, 0x58EA, 0x574B, 0x55A5, 0x53F9, 0x5246, 0x508D, 0x4ECD, 0x4D08,
                                      0x4B3C, 0x496A, 0x4793, 0x45B6, 0x43D4, 0x41EC, 0x3FFF, 0x3E0E, 0x3C17, 0x3A1C, 0x381C, 0x3618, 0x3410, 0x3203, 0x2FF3, 0x2DDF,
                                      0x2BC7, 0x29AC, 0x278E, 0x256C, 0x2348, 0x2121, 0x1EF7, 0x1CCB, 0x1A9D, 0x186C, 0x163A, 0x1406, 0x11D0, 0x0F99, 0x0D61, 0x0B28,
                                      0x08EE, 0x06B3, 0x0478, 0x023C, 0x0000, 0xFDC4, 0xFB88, 0xF94D, 0xF712, 0xF4D8, 0xF29F, 0xF067, 0xEE30, 0xEBFA, 0xE9C6, 0xE794,
                                      0xE563, 0xE335, 0xE109, 0xDEDF, 0xDCB8, 0xDA94, 0xD872, 0xD654, 0xD439, 0xD221, 0xD00D, 0xCDFD, 0xCBF0, 0xC9E8, 0xC7E4, 0xC5E4,
                                      0xC3E9, 0xC1F2, 0xC000, 0xBE14, 0xBC2C, 0xBA4A, 0xB86D, 0xB696, 0xB4C4, 0xB2F8, 0xB133, 0xAF73, 0xADBA, 0xAC07, 0xAA5B, 0xA8B5,
                                      0xA716, 0xA57E, 0xA3ED, 0xA264, 0xA0E1, 0x9F66, 0x9DF3, 0x9C87, 0x9B23, 0x99C7, 0x9873, 0x9727, 0x95E3, 0x94A7, 0x9374, 0x9249,
                                      0x9127, 0x900D, 0x8EFC, 0x8DF4, 0x8CF5, 0x8BFF, 0x8B12, 0x8A2E, 0x8953, 0x8881, 0x87B9, 0x86FA, 0x8645, 0x8599, 0x84F6, 0x845E,
                                      0x83CE, 0x8349, 0x82CD, 0x825B, 0x81F3, 0x8194, 0x8140, 0x80F5, 0x80B5, 0x807E, 0x8051, 0x802E, 0x8015, 0x8006, 0x8001, 0x8006,
                                      0x8015, 0x802E, 0x8051, 0x807E, 0x80B5, 0x80F5, 0x8140, 0x8194, 0x81F3, 0x825B, 0x82CD, 0x8349, 0x83CE, 0x845E, 0x84F6, 0x8599,
                                      0x8645, 0x86FA, 0x87B9, 0x8881, 0x8953, 0x8A2E, 0x8B12, 0x8BFF, 0x8CF5, 0x8DF4, 0x8EFC, 0x900D, 0x9127, 0x9249, 0x9374, 0x94A7,
                                      0x95E3, 0x9727, 0x9873, 0x99C7, 0x9B23, 0x9C87, 0x9DF3, 0x9F66, 0xA0E1, 0xA264, 0xA3ED, 0xA57E, 0xA716, 0xA8B5, 0xAA5B, 0xAC07,
                                      0xADBA, 0xAF73, 0xB133, 0xB2F8, 0xB4C4, 0xB696, 0xB86D, 0xBA4A, 0xBC2C, 0xBE14, 0xC001, 0xC1F2, 0xC3E9, 0xC5E4, 0xC7E4, 0xC9E8,
                                      0xCBF0, 0xCDFD, 0xD00D, 0xD221, 0xD439, 0xD654, 0xD872, 0xDA94, 0xDCB8, 0xDEDF, 0xE109, 0xE335, 0xE563, 0xE794, 0xE9C6, 0xEBFA,
                                      0xEE30, 0xF067, 0xF29F, 0xF4D8, 0xF712, 0xF94D, 0xFB88, 0xFDC4};

  (void) Amplitude;

#ifdef DEBUG_FIREBIRDLIB
  CallTraceEnter("SoundSinus");
#endif

  samples = durationInMilliseconds * 48;
  memSize = samples * 2;             //Zwei Byte pro Sample (16 bit)

  if ((soundData != NULL) && (soundDataLength < memSize))
  {
    TAP_MemFree(soundData);             //Gebe den beim letzten Ton belegten Speicher wieder frei
    soundData = NULL;
  }

  if (soundData == NULL)
  {
    soundData = (word*) TAP_MemAlloc_Chk("SoundSinus", memSize);
    soundDataLength = memSize;
  }

  if (soundData == NULL)
  {

#ifdef DEBUG_FIREBIRDLIB
    CallTraceExit(NULL);
#endif

    return;
  }
  target = soundData;
  periode = 48000 / freq;            //Anzahl von Samples f�r eine volle Schwingung
  if (periode > samples)
  {
    //Die Dauer ist zu kurz, um auch nur eine einzelne vollst�ndige Welle abzubilden

#ifdef DEBUG_FIREBIRDLIB
    CallTraceExit(NULL);
#endif

    return;
  }

  //Berechne die erste vollst�ndige Welle
  for (index = 0; index < periode; index++)
  {
    (*target) = Sinus [(index * 360) / periode];
    target++;
  }

  //Alle weiteren Wellen k�nnen einfach kopiert werden
  source = soundData;
  for (index = periode; index < samples - periode; index++)
  {
    (*target) = (*source);
    target++;
    source++;
  }

  //Erzeuge ein Fade in
  for (index = 0; index < 480; index ++)
  {
    soundData [index] = soundData [index] / 480 * index;
  }

  //Erzeuge ein Fade out
  for (index = 0; index < 1680; index ++)
  {
    soundData [samples - index] = soundData [samples - index] / 1680 * index;
  }

  TAP_PlayPCM ((void *) soundData, 2 * samples, FREQ_48K, NULL);

#ifdef DEBUG_FIREBIRDLIB
    CallTraceExit(NULL);
#endif
}
