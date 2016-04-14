/** @file
  SMM PCI config space I/O access utility implementation file, for Ia32

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/SmmIoLib.h>
#include <Protocol/SmmBase2.h>
#include <Library/DebugLib.h>

STATIC
EFI_STATUS
SmmSingleSegmentPciAccess (
  IN EFI_SMM_CPU_IO2_PROTOCOL    *CpuIo,
  IN BOOLEAN                     IsWrite,
  IN SMM_PCI_IO_WIDTH            Width,
  IN SMM_PCI_IO_ADDRESS          *Address,
  IN OUT VOID                    *Buffer
  );

/**
  Read value from the specified PCI config space register
    
  @param[in]  Width   - The width (8, 16 or 32 bits) of accessed pci config space register
  @param[in]  Address - The address of the accessed pci register (bus, dev, func, offset)
  @param[in]  Buffer  - The returned value
            
  @retval  EFI_SUCCESS           - All operations successfully
  @retval  EFI_INVALID_PARAMETER - Width is not valid or dosn't match register address
  @retval  Other error code      - If any error occured when calling libiary functions 
    
**/
EFI_STATUS
SmmPciCfgRead (
  IN SMM_PCI_IO_WIDTH       Width,
  IN SMM_PCI_IO_ADDRESS     *Address,
  IN OUT VOID               *Buffer
  )
{
  EFI_SMM_CPU_IO2_PROTOCOL    *SmmCpuIo;

  ASSERT (gSmst);

  SmmCpuIo = &(gSmst->SmmIo);

  return SmmSingleSegmentPciAccess (SmmCpuIo, FALSE, Width, Address, Buffer);
}

/**
  Write value into the specified PCI config space register
    
  @param[in]  Width   - The width (8, 16 or 32 bits) of accessed pci config space register
  @param[in]  Address - The address of the accessed pci register (bus, dev, func, offset)
  @param[in]  Buffer  - The returned value
            
  @retval  EFI_SUCCESS           - All operations successfully
  @retval  EFI_INVALID_PARAMETER - Width is not valid or dosn't match register address
  @retval  Other error code      - If any error occured when calling libiary functions 
  
**/
EFI_STATUS
SmmPciCfgWrite (
  IN SMM_PCI_IO_WIDTH       Width,
  IN SMM_PCI_IO_ADDRESS     *Address,
  IN OUT VOID               *Buffer
  )
{
  EFI_SMM_CPU_IO2_PROTOCOL    *SmmCpuIo;

  ASSERT (gSmst);

  SmmCpuIo = &(gSmst->SmmIo);
  
  return SmmSingleSegmentPciAccess (SmmCpuIo, TRUE, Width, Address, Buffer);
}

/**
  Access a PCI config space address, including read and write
    
  @param[in]  CpuIo   - The cpu I/O accessing interface provided by EFI runtime sys table
  @param[in]  IsWrite - Indicates whether this operation is a write access or read
  @param[in]  Width   - The width (8, 16 or 32 bits) of accessed pci config space register
  @param[in]  Address - The address of the accessed pci register (bus, dev, func, offset)
  @param[in]  Buffer  - The returned value when this is a reading operation or the data 
            to be written when this is a writing one

  @retval  EFI_SUCCESS           - All operations successfully
  @retval  EFI_INVALID_PARAMETER - Width is not valid or dosn't match register address
  @retval  Other error code      - If any error occured when calling libiary functions 
  
**/
STATIC
EFI_STATUS
SmmSingleSegmentPciAccess (
  IN EFI_SMM_CPU_IO2_PROTOCOL    *CpuIo,
  IN BOOLEAN                     IsWrite,
  IN SMM_PCI_IO_WIDTH            Width,
  IN SMM_PCI_IO_ADDRESS          *Address,
  IN OUT VOID                    *Buffer
  )
{
  EFI_STATUS                        Status;
  PCI_CONFIG_ACCESS_CF8             PciCf8Data;
  UINT64                            PciDataReg;

  //
  // PCI Config access are all 32-bit alligned, but by accessing the
  // CONFIG_DATA_REGISTER (0xcfc) with different widths more cycle types
  // are possible on PCI.
  //
  // To read a byte of PCI config space you load 0xcf8 and 
  // read 0xcfc, 0xcfd, 0xcfe, 0xcff
  //
  // The validation of passed in arguments "Address" will be checked in the 
  // CPU IO functions, so we don't check them here
  //
  
  if (Width >= SmmPciWidthMaximum) {
    return EFI_INVALID_PARAMETER;
  }

  PciCf8Data.Bits.Reg      = Address->Register & 0xfc;
  PciCf8Data.Bits.Func     = Address->Function;
  PciCf8Data.Bits.Dev      = Address->Device;
  PciCf8Data.Bits.Bus      = Address->Bus;
  PciCf8Data.Bits.Reserved = 0;
  PciCf8Data.Bits.Enable   = 1;
  
  Status = CpuIo->Io.Write (CpuIo, SmmPciWidthUint32, 0xcf8, 1, &PciCf8Data);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  PciDataReg = 0xcfc + (Address->Register & 0x03);
  
  if (IsWrite) {
    //
    // This is a Pci write operation, write data into (0xcfc + offset)
    //
    Status = CpuIo->Io.Write (CpuIo, Width, PciDataReg, 1, Buffer);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  } else {
    //
    // This is a Pci Read operation, read returned data from (0xcfc + offset)
    //
    Status = CpuIo->Io.Read (CpuIo, Width, PciDataReg, 1, Buffer);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}
