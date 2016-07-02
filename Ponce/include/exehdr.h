/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-2015 Hex-Rays
 *      ALL RIGHTS RESERVED.
 *
 *              EXE-file header layout
 *
 */

#ifndef __EXEHDR_H
#define __EXEHDR_H
#pragma pack(push, 1)

struct exehdr
{
    uint16 exe_ident;
#define EXE_ID  0x5A4D          // 'MZ'
#define EXE_ID2 0x4D5A          // 'ZM' (DOS works with this also)
    uint16 PartPag;
    uint16 PageCnt;
    uint16 ReloCnt;
    uint16 HdrSize;
    uint16 MinMem;
    uint16 MaxMem;
    uint16 ReloSS;
    uint16 ExeSP;
    uint16 ChkSum;
    uint16 ExeIP;
    uint16 ReloCS;
    uint16 TablOff;
    uint16 Overlay;
/*
    uint16 res[4];                    // Reserved words
    uint16 oemid;                     // OEM identifier (for e_oeminfo)
    uint16 oeminfo;                   // OEM information; e_oemid specific
    uint16 res2[10];                  // Reserved words
    uint32 lfanew;                    // File address of new exe header
*/
    int32 CalcEXE_Length(void)
    {
      int32 len = PageCnt * 512L - HdrSize * 16;
      if ( PartPag != 0 )
        len -= 512 - PartPag;
      return len;
    }
    void CalcEXE_Pages(int32 len)
    {
      PartPag = uint16(len % 512);
      PageCnt = uint16(len / 512);
      if ( PartPag != 0 )
        PageCnt++;
    }
};

struct exehdr_full: exehdr
{
    uint16 res[4];                    // Reserved words
    uint16 oemid;                     // OEM identifier (for e_oeminfo)
    uint16 oeminfo;                   // OEM information; e_oemid specific
    uint16 res2[10];                  // Reserved words
    uint32 lfanew;                    // File address of new exe header
};

#define PSPsize 0x100
#define PE_PTROFF 0x3C
#pragma pack(pop)
#endif
