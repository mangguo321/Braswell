/** @file
  Macros to simplify and abstract the interface to PCI configuration.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _ChvAccess_H_INCLUDED_
#define _ChvAccess_H_INCLUDED_

#include "Cherryview.h"
#include "ChvCommonDefinitions.h"
#include <Library/IoLib.h>

//
// Memory Mapped IO access macros used by MSG BUS LIBRARY
//
#define MmioAddress( BaseAddr, Register ) \
  ( (UINTN)BaseAddr + \
    (UINTN)(Register) \
  )

//
// UINT32
//

#define Mmio32Ptr( BaseAddr, Register ) \
  ( (volatile UINT32 *)MmioAddress( BaseAddr, Register ) )

#define Mmio32( BaseAddr, Register ) \
  *Mmio32Ptr( BaseAddr, Register )

#define Mmio32Or( BaseAddr, Register, OrData ) \
  Mmio32( BaseAddr, Register ) = \
    (UINT32) ( \
      Mmio32( BaseAddr, Register ) | \
      (UINT32)(OrData) \
    )

#define Mmio32And( BaseAddr, Register, AndData ) \
  Mmio32( BaseAddr, Register ) = \
    (UINT32) ( \
      Mmio32( BaseAddr, Register ) & \
      (UINT32)(AndData) \
    )

#define Mmio32AndThenOr( BaseAddr, Register, AndData, OrData ) \
  Mmio32( BaseAddr, Register ) = \
    (UINT32) ( \
      ( Mmio32( BaseAddr, Register ) & \
          (UINT32)(AndData) \
      ) | \
      (UINT32)(OrData) \
    )

//
// UINT16
//

#define Mmio16Ptr( BaseAddr, Register ) \
  ( (volatile UINT16 *)MmioAddress( BaseAddr, Register ) )

#define Mmio16( BaseAddr, Register ) \
  *Mmio16Ptr( BaseAddr, Register )

#define Mmio16Or( BaseAddr, Register, OrData ) \
  Mmio16( BaseAddr, Register ) = \
    (UINT16) ( \
      Mmio16( BaseAddr, Register ) | \
      (UINT16)(OrData) \
    )

#define Mmio16And( BaseAddr, Register, AndData ) \
  Mmio16( BaseAddr, Register ) = \
    (UINT16) ( \
      Mmio16( BaseAddr, Register ) & \
      (UINT16)(AndData) \
    )

#define Mmio16AndThenOr( BaseAddr, Register, AndData, OrData ) \
  Mmio16( BaseAddr, Register ) = \
    (UINT16) ( \
      ( Mmio16( BaseAddr, Register ) & \
          (UINT16)(AndData) \
      ) | \
      (UINT16)(OrData) \
    )

//
// UINT8
//

#define Mmio8Ptr( BaseAddr, Register ) \
  ( (volatile UINT8 *)MmioAddress( BaseAddr, Register ) )

#define Mmio8( BaseAddr, Register ) \
  *Mmio8Ptr( BaseAddr, Register )

#define Mmio8Or( BaseAddr, Register, OrData ) \
  Mmio8( BaseAddr, Register ) = \
    (UINT8) ( \
      Mmio8( BaseAddr, Register ) | \
      (UINT8)(OrData) \
    )

#define Mmio8And( BaseAddr, Register, AndData ) \
  Mmio8( BaseAddr, Register ) = \
    (UINT8) ( \
      Mmio8( BaseAddr, Register ) & \
      (UINT8)(AndData) \
    )

#define Mmio8AndThenOr( BaseAddr, Register, AndData, OrData ) \
  Mmio8( BaseAddr, Register ) = \
    (UINT8) ( \
      ( Mmio8( BaseAddr, Register ) & \
          (UINT8)(AndData) \
        ) | \
      (UINT8)(OrData) \
    )

//
// MSG BUS API
//

#define MSG_BUS_ENABLED     0x000000F0
#define MSGBUS_MASKHI       0xFFFFFF00
#define MSGBUS_MASKLO       0x000000FF

#define MESSAGE_BYTE_EN          BIT4
#define MESSAGE_WORD_EN          BIT4 | BIT5
#define MESSAGE_DWORD_EN         BIT4 | BIT5 | BIT6 | BIT7

#define SIDEBAND_OPCODE          0x78
#define MEMREAD_OPCODE           0x00000000
#define MEMWRITE_OPCODE          0x01000000

#define MsgBusReadCmd(PortId) ((PortId == 0x06 || PortId == 0x12 || PortId == 0x10)? CHV_MBR_GDISPIOREAD_CMD : \
((PortId == 0x04 || PortId == 0x13 || PortId == 0xA9 \
||PortId == 0x0C )? CHV_MBR_PDFXGPIODDRIOREAD_CMD : CHV_MBR_READ_CMD))

#define MsgBusWriteCmd(PortId) ((PortId == 0x06 || PortId == 0x12 || PortId == 0x10)? CHV_MBR_GDISPIOWRITE_CMD : \
((PortId == 0x04 || PortId == 0x13 || PortId == 0xA9 \
||PortId == 0x0C )? CHV_MBR_PDFXGPIODDRIOWRITE_CMD : CHV_MBR_WRITE_CMD))

#define MsgBusCmd(PortId) \
{ \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( (SIDEBAND_OPCODE << 24)  | (PortId <<16)) ; \
}

#define MsgBusCmdWrite(PortId,Dbuff) \
{ \
  Mmio32( EC_BASE, MC_MDR ) = Dbuff; \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( (SIDEBAND_OPCODE << 24)  | (PortId <<16)) ; \
}
//
// UINT32
//

#define MsgBus32Read(PortId, Register, Dbuff)  \
{ \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusReadCmd(PortId)  | ((PortId) <<16)  | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_DWORD_EN); \
  (Dbuff) = (UINT32)Mmio32( EC_BASE, MC_MDR ); \
}

#define MsgBus32Write(PortId, Register,Dbuff) \
{ \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MDR ) = Dbuff; \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusWriteCmd(PortId)  | ((PortId) <<16) | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_DWORD_EN); \
}

#define MsgBus32ReadEx( OPCODE, PORTID, Register, Dbuff, Bar, Dev, Func) \
{ \
  UINT8 Fid = ((Dev <<3) | (Func)) & 0xFF; \
  UINT32 Temp32 = 0; \
  Temp32 = (((Bar&0x7)<<8)|(Fid&0xff)); \
  Mmio32( EC_BASE, MC_MCRXX) = Temp32; \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( (OPCODE << 24) | (PORTID << 16) | ((Register & MSGBUS_MASKLO) << 8) | MESSAGE_DWORD_EN); \
  (Dbuff) = (UINT32)Mmio32( EC_BASE, MC_MDR ); \
}

#define MsgBus32WriteEx(OPCODE, PORTID, Register, Dbuff, Bar, Dev, Func) \
{ \
  UINT8 Fid = ((Dev <<3) | (Func)) & 0xFF; \
  UINT32 Temp32 = 0; \
  Temp32 = (((Bar&0x7)<<8)|(Fid&0xff)); \
  Mmio32( EC_BASE, MC_MCRXX) = Temp32; \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MDR ) = Dbuff; \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( (OPCODE << 24) | (PORTID << 16) | ((Register & MSGBUS_MASKLO) << 8) | MESSAGE_DWORD_EN); \
}

#define GunitMsgBus32Read(PortId, Register, Dbuff)  \
{ \
  Mmio32( EC_BASE + (2<<15), GUNIT_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE + (2<<15), GUNIT_MCR ) = (UINT32)( MsgBusReadCmd(PortId)  | ((PortId) <<16)  | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_DWORD_EN); \
  (Dbuff) = (UINT32)Mmio32( EC_BASE + (2<<15), GUNIT_MDR ); \
}

#define GunitMsgBus32Write(PortId, Register,Dbuff) \
{ \
  Mmio32( EC_BASE + (2<<15), GUNIT_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE + (2<<15), GUNIT_MDR ) = Dbuff; \
  Mmio32( EC_BASE + (2<<15), GUNIT_MCR ) = (UINT32)( MsgBusWriteCmd(PortId)  | ((PortId) <<16) | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_DWORD_EN); \
}

#define MsgBus32Or(PortId, Register, Dbuff, OrData ) \
{ \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusReadCmd(PortId)  | ((PortId) <<16)  | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_DWORD_EN); \
  (Dbuff) = (UINT32)Mmio32( EC_BASE, MC_MDR ); \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MDR ) = (Dbuff | OrData); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusWriteCmd(PortId)  | ((PortId) <<16) | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_DWORD_EN); \
}

#define MsgBus32And(PortId, Register, Dbuff, AndData ) \
{ \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusReadCmd(PortId) | ((PortId) <<16)  | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_DWORD_EN); \
  (Dbuff) = (UINT32)Mmio32( EC_BASE, MC_MDR ); \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MDR ) = (Dbuff & AndData); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusWriteCmd(PortId) | ((PortId) <<16) | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_DWORD_EN); \
}

#define MsgBus32AndThenOr( PortId, Register, Dbuff, AndData, OrData ) \
{ \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusReadCmd(PortId)  | ((PortId) <<16)  | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_DWORD_EN); \
  (Dbuff) = (UINT32)Mmio32( EC_BASE, MC_MDR ); \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MDR ) = ((Dbuff & AndData) | OrData); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusWriteCmd(PortId)  | ((PortId) <<16) | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_DWORD_EN); \
}

//
// UINT16
//

#define MsgBus16Or(PortId, Register, Dbuff, OrData ) \
{ \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusReadCmd(PortId)  | ((PortId) <<16)  | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_WORD_EN); \
  (Dbuff) = (UINT16)Mmio16( EC_BASE, MC_MDR ); \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio16( EC_BASE, MC_MDR ) = (Dbuff | OrData); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusWriteCmd(PortId)  | ((PortId) <<16) | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_WORD_EN); \
}

#define MsgBus16And(PortId, Register, Dbuff, AndData ) \
{ \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusReadCmd(PortId)  | ((PortId) <<16)  | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_WORD_EN); \
  (Dbuff) = (UINT16)Mmio16( EC_BASE, MC_MDR ); \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio16( EC_BASE, MC_MDR ) = (Dbuff & AndData); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusWriteCmd(PortId)  | ((PortId) <<16) | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_WORD_EN); \
}

#define MsgBus16AndThenOr( PortId, Register, Dbuff, AndData, OrData ) \
{ \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusReadCmd(PortId)  | ((PortId) <<16)  | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_WORD_EN); \
  (Dbuff) = (UINT16)Mmio16( EC_BASE, MC_MDR ); \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio16( EC_BASE, MC_MDR ) = ((Dbuff & AndData) | OrData); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusWriteCmd(PortId)  | ((PortId) <<16) | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_WORD_EN); \
}

//
// UINT8
//

#define MsgBus8Or( PortId, Register, Dbuff, OrData ) \
{ \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusReadCmd(PortId)  | ((PortId) <<16)  | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_BYTE_EN); \
  (Dbuff) = (UINT8)Mmio8( EC_BASE, MC_MDR ); \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio8( EC_BASE, MC_MDR ) = (Dbuff | OrData); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusWriteCmd(PortId)  | ((PortId) <<16) | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_BYTE_EN); \
}

#define MsgBus8And( PortId, Register, Dbuff, AndData ) \
{ \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusReadCmd(PortId)  | ((PortId) <<16)  | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_BYTE_EN); \
  (Dbuff) = (UINT8)Mmio8( EC_BASE, MC_MDR ); \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio8( EC_BASE, MC_MDR ) = (Dbuff & AndData); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusWriteCmd(PortId)  | ((PortId) <<16) | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_BYTE_EN); \
}

#define MsgBus8AndThenOr( PortId, Register, Dbuff, AndData, OrData ) \
{ \
    Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusReadCmd(PortId)  | ((PortId) <<16)  | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_BYTE_EN); \
  (Dbuff) = (UINT8)Mmio8( EC_BASE, MC_MDR ); \
  Mmio32( EC_BASE, MC_MCRX) = ( (Register & MSGBUS_MASKHI)); \
  Mmio8( EC_BASE, MC_MDR ) = ((Dbuff & AndData) | OrData); \
  Mmio32( EC_BASE, MC_MCR ) = (UINT32)( MsgBusWriteCmd(PortId) | ((PortId) <<16) | ((Register & MSGBUS_MASKLO)<<8) | MESSAGE_BYTE_EN); \
}

/***************************/
//
// Memory mapped PCI IO
//

#define PciCfgPtr(Bus, Device, Function, Register )\
    (UINTN)(Bus << 20) + \
    (UINTN)(Device << 15) + \
    (UINTN)(Function << 12) + \
    (UINTN)(Register)

#define PciCfg32Read_CF8CFC(B,D,F,R) \
  (UINT32)(IoOut32(0xCF8,(0x80000000|(B<<16)|(D<<11)|(F<<8)|(R))),IoIn32(0xCFC))

#define PciCfg32Write_CF8CFC(B,D,F,R,Data) \
  (IoOut32(0xCF8,(0x80000000|(B<<16)|(D<<11)|(F<<8)|(R))),IoOut32(0xCFC,Data))

#define PciCfg32Or_CF8CFC(B,D,F,R,O) \
  PciCfg32Write_CF8CFC(B,D,F,R, \
    (PciCfg32Read_CF8CFC(B,D,F,R) | (O)))

#define PciCfg32And_CF8CFC(B,D,F,R,A) \
  PciCfg32Write_CF8CFC(B,D,F,R, \
    (PciCfg32Read_CF8CFC(B,D,F,R) & (A)))

#define PciCfg32AndThenOr_CF8CFC(B,D,F,R,A,O) \
  PciCfg32Write_CF8CFC(B,D,F,R, \
    (PciCfg32Read_CF8CFC(B,D,F,R) & (A)) | (O))

//
// Device 0, Function 0
//
#define McD0PciCfg64(Register)                              MmPci64           (0, MC_BUS, 0, 0, Register)
#define McD0PciCfg64Or(Register, OrData)                    MmPci64Or         (0, MC_BUS, 0, 0, Register, OrData)
#define McD0PciCfg64And(Register, AndData)                  MmPci64And        (0, MC_BUS, 0, 0, Register, AndData)
#define McD0PciCfg64AndThenOr(Register, AndData, OrData)    MmPci64AndThenOr  (0, MC_BUS, 0, 0, Register, AndData, OrData)

#define McD0PciCfg32(Register)                              MmPci32           (0, MC_BUS, 0, 0, Register)
#define McD0PciCfg32Or(Register, OrData)                    MmPci32Or         (0, MC_BUS, 0, 0, Register, OrData)
#define McD0PciCfg32And(Register, AndData)                  MmPci32And        (0, MC_BUS, 0, 0, Register, AndData)
#define McD0PciCfg32AndThenOr(Register, AndData, OrData)    MmPci32AndThenOr  (0, MC_BUS, 0, 0, Register, AndData, OrData)

#define McD0PciCfg16(Register)                              MmPci16           (0, MC_BUS, 0, 0, Register)
#define McD0PciCfg16Or(Register, OrData)                    MmPci16Or         (0, MC_BUS, 0, 0, Register, OrData)
#define McD0PciCfg16And(Register, AndData)                  MmPci16And        (0, MC_BUS, 0, 0, Register, AndData)
#define McD0PciCfg16AndThenOr(Register, AndData, OrData)    MmPci16AndThenOr  (0, MC_BUS, 0, 0, Register, AndData, OrData)

#define McD0PciCfg8(Register)                               MmPci8            (0, MC_BUS, 0, 0, Register)
#define McD0PciCfg8Or(Register, OrData)                     MmPci8Or          (0, MC_BUS, 0, 0, Register, OrData)
#define McD0PciCfg8And(Register, AndData)                   MmPci8And         (0, MC_BUS, 0, 0, Register, AndData)
#define McD0PciCfg8AndThenOr( Register, AndData, OrData )   MmPci8AndThenOr   (0, MC_BUS, 0, 0, Register, AndData, OrData)

//
// Device 2, Function 0
//
#define McD2PciCfg64(Register)                              MmPci64           (0, MC_BUS, 2, 0, Register)
#define McD2PciCfg64Or(Register, OrData)                    MmPci64Or         (0, MC_BUS, 2, 0, Register, OrData)
#define McD2PciCfg64And(Register, AndData)                  MmPci64And        (0, MC_BUS, 2, 0, Register, AndData)
#define McD2PciCfg64AndThenOr(Register, AndData, OrData)    MmPci64AndThenOr  (0, MC_BUS, 2, 0, Register, AndData, OrData)

#define McD2PciCfg32(Register)                              MmPci32           (0, MC_BUS, 2, 0, Register)
#define McD2PciCfg32Or(Register, OrData)                    MmPci32Or         (0, MC_BUS, 2, 0, Register, OrData)
#define McD2PciCfg32And(Register, AndData)                  MmPci32And        (0, MC_BUS, 2, 0, Register, AndData)
#define McD2PciCfg32AndThenOr(Register, AndData, OrData)    MmPci32AndThenOr  (0, MC_BUS, 2, 0, Register, AndData, OrData)

#define McD2PciCfg16(Register)                              MmPci16           (0, MC_BUS, 2, 0, Register)
#define McD2PciCfg16Or(Register, OrData)                    MmPci16Or         (0, MC_BUS, 2, 0, Register, OrData)
#define McD2PciCfg16And(Register, AndData)                  MmPci16And        (0, MC_BUS, 2, 0, Register, AndData)
#define McD2PciCfg16AndThenOr(Register, AndData, OrData)    MmPci16AndThenOr  (0, MC_BUS, 2, 0, Register, AndData, OrData)

#define McD2PciCfg8(Register)                               MmPci8            (0, MC_BUS, 2, 0, Register)
#define McD2PciCfg8Or(Register, OrData)                     MmPci8Or          (0, MC_BUS, 2, 0, Register, OrData)
#define McD2PciCfg8And(Register, AndData)                   MmPci8And         (0, MC_BUS, 2, 0, Register, AndData)

//
// IO
//

#ifndef IoIn8

#define IoIn8(Port) \
  IoRead8(Port)

#define IoIn16(Port) \
  IoRead16(Port)

#define IoIn32(Port) \
  IoRead32(Port)

#define IoOut8(Port, Data) \
  IoWrite8(Port, Data) 

#define IoOut16(Port, Data) \
  IoWrite16(Port, Data)

#define IoOut32(Port, Data) \
  IoWrite32(Port, Data)

#endif

#endif
