/** @file 
  SMM I/O access utility implementation file, for Ia32

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

//
// Include files
//
#include "Library/SmmIoLib.h"

/**
  Do a one byte IO read

  @param[in]   Address         IO address to read 
  
  @return     Data read

**/
UINT8
SmmIoRead8 (
  IN  UINT16    Address
  )  
{
  UINT8   Buffer;
  
  ASSERT (mSmst);

  mSmst->SmmIo.Io.Read (
                    &mSmst->SmmIo, 
                    SMM_IO_UINT8, 
                    Address, 
                    1, 
                    &Buffer
                    );

  return Buffer;
}

/**
  Do a one byte IO write

  @param[in]   Address        IO address to write
  @param[in]   Data           Data to write

**/
VOID
SmmIoWrite8 (
  IN  UINT16    Address,
  IN  UINT8     Data
  )
{
  ASSERT (mSmst);

  mSmst->SmmIo.Io.Write (
                    &mSmst->SmmIo, 
                    SMM_IO_UINT8, 
                    Address, 
                    1, 
                    &Data
                    );
}

/**
  Do a two byte IO read

  @param[in]   Address        IO address to read 
  
  @return      Data read

**/
UINT16
SmmIoRead16 (
  IN  UINT16    Address
  )  
{
  UINT16      Buffer;
  
  ASSERT (mSmst);

  mSmst->SmmIo.Io.Read (
                    &mSmst->SmmIo, 
                    SMM_IO_UINT16, 
                    Address,
                    1, 
                    &Buffer 
                    );

  return Buffer;
}

/**
  Do a two byte IO write

  @param[in]   Address        IO address to write
  @param[in]   Data           Data to write

**/
VOID
SmmIoWrite16 (
  IN  UINT16    Address,
  IN  UINT16    Data
  )
{
  ASSERT (mSmst);

  mSmst->SmmIo.Io.Write ( 
                    &mSmst->SmmIo, 
                    SMM_IO_UINT16, 
                    Address,
                    1, 
                    &Data 
                    );
}

/**
  Do a four byte IO read

  @param[in]   Address      IO address to read 
  
  @retval      Data read

**/
UINT32
SmmIoRead32 (
  IN  UINT16    Address
  )    
{
  UINT32        Buffer;

  ASSERT (mSmst);

  mSmst->SmmIo.Io.Read ( 
                    &mSmst->SmmIo, 
                    SMM_IO_UINT32, 
                    Address,
                    1, 
                    &Buffer 
                    );

  return Buffer;
}

/**
  Do a four byte IO write

  @param[in]    Address     IO address to write
  @param[in]    Data        Data to write

**/
VOID
SmmIoWrite32 (
  IN  UINT16    Address,
  IN  UINT32    Data
  )
{
  ASSERT (mSmst);

  mSmst->SmmIo.Io.Write ( 
                    &mSmst->SmmIo, 
                    SMM_IO_UINT32, 
                    Address,
                    1, 
                    &Data 
                    );
}

/**
  Do a one byte Memory write

  @param[in]  Dest    Memory address to write
  @param[in]  Data    Data to write

**/
VOID
SmmMemWrite8 (
  IN  UINT64    Dest,
  IN  UINT8     Data
  )
{
  mSmst->SmmIo.Mem.Write (
                     &mSmst->SmmIo, 
                     SMM_IO_UINT8, 
                     Dest, 
                     1, 
                     &Data
                     );

  return ;
}

/**
  Do a one byte Memory read

  @param[in]   Dest     Memory address to read

  @Return     Data read

**/
UINT8
SmmMemRead8 (
  IN  UINT64    Dest
  )
{
  UINT8 Data;
  mSmst->SmmIo.Mem.Read (
                     &mSmst->SmmIo, 
                     SMM_IO_UINT8, 
                     Dest, 
                     1, 
                     &Data
                     );

  return Data;
}

/**
  Do a two bytes Memory write

  @param[in]   Dest   Memory address to write
  @param[in]   Data   Data to write

**/
VOID
SmmMemWrite16 (
  IN  UINT64    Dest,
  IN  UINT16    Data
  )
{
  mSmst->SmmIo.Mem.Write (
                     &mSmst->SmmIo, 
                     SMM_IO_UINT16, 
                     Dest, 
                     1, 
                     &Data
                     );

  return ;
}

/**
  Do a two bytes Memory read

  @param[in]  Dest    Memory address to read 

  @return    Data read

**/
UINT16
SmmMemRead16 (
  IN  UINT64    Dest
  )
{
  UINT16 Data;
  mSmst->SmmIo.Mem.Read (
                     &mSmst->SmmIo, 
                     SMM_IO_UINT16, 
                     Dest, 
                     1, 
                     &Data
                     );

  return Data;
}

/**
  Do a four bytes Memory write

  @param[in]   Dest       Memory address to write
  @param[in]   Data       Data to write

**/
VOID
SmmMemWrite32 (
  IN  UINT64    Dest,
  IN  UINT32    Data
  )
{
  mSmst->SmmIo.Mem.Write (
                     &mSmst->SmmIo, 
                     SMM_IO_UINT32, 
                     Dest, 
                     1, 
                     &Data
                     );

  return ;
}

/**
  Do a four bytes Memory read

  @param[in]    Dest    Memory address to read 

  @return    Data read

**/
UINT32
SmmMemRead32 (
  IN  UINT64    Dest
  )
{
  UINT32 Data;
  mSmst->SmmIo.Mem.Read (
                     &mSmst->SmmIo, 
                     SMM_IO_UINT32, 
                     Dest, 
                     1, 
                     &Data
                     );

  return Data;
}

/**
  Do a four bytes Memory read, then AND with Data, then write back to the same address 

  @param[in]  Dest    Memory address to write
  @param[in]  Data    Data to do AND

**/
VOID
SmmMemAnd32 (
  IN  UINT64    Dest,
  IN  UINT32    Data
  )
{
  UINT32 Data_And;
  mSmst->SmmIo.Mem.Read (
                     &mSmst->SmmIo, 
                     SMM_IO_UINT32, 
                     Dest, 
                     1, 
                     &Data_And
                     );
  Data_And&=Data;
  mSmst->SmmIo.Mem.Write (
                     &mSmst->SmmIo, 
                     SMM_IO_UINT32, 
                     Dest, 
                     1, 
                     &Data_And
                     );

  return ;
}
