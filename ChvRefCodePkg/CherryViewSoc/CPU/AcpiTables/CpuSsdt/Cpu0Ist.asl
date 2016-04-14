/** @file
  Intel Processor Power Management ACPI Code.

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

DefinitionBlock (
    "CPU0IST.aml",
    "SSDT",
    0x01,
    "PmRef",
    "Cpu0Ist",
    0x3000
    )
{
    External (\_PR.CPU0, DeviceObj)
    External (PDC0)
    External (CFGD)
    External (OSID)
    External(\ADBG, MethodObj)

    Scope(\_PR.CPU0)
    {
        //OperationRegion (DEB0, SystemIO, 0x80, 1)    //DBG
        //Field (DEB0, ByteAcc,NoLock,Preserve)        //DBG
        //{ DBG8, 8,}                    //DBG

        Name(_PPC, 0)        // Initialize as All States Available.

        // NOTE:  For CMP systems; this table is not loaded unless
        //      the required driver support is present.
        //      So, we do not check for those cases here.
        //
        //   CFGD[0] = GV3 Capable/Enabled
        //   PDCx[0]  = OS Capable of Hardware P-State control
        //
        Method(_PCT,0)
        {
            If(LAnd(And(CFGD,0x0001), And(PDC0,0x0001)))
            {
                //Store(0xA0,DBG8) //DBG
                Return(Package()    // Native Mode
                {
                    ResourceTemplate(){Register(FfixedHW, 0, 0, 0)},
                    ResourceTemplate(){Register(FfixedHW, 0, 0, 0)}
                })
            }
            // @NOTE: IO Trap is not supported. Therefore should not expose any IO interface for _PCT
            // For all other cases, report control through the
            // SMI interface.  (The port used for SMM control is fixed up
            // by the initialization code.)
            //
            Return(Package()        // SMM Mode
            {                
               ResourceTemplate(){Register(FfixedHW, 0, 0, 0)},
               ResourceTemplate(){Register(FfixedHW, 0, 0, 0)}
            })
        }


        // NOTE:  For CMP systems; this table is not loaded if MP
        //      driver support is not present or P-State are disabled.
        //
        Method(_PSS,0)
        {
          Return(SPSS)
        }

        Name(SPSS,Package()
        {
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000}
        })

        Name(NPSS,Package()
        {
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000},
            Package(){0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000}
        })

        // The _PSD object provides information to the OSPM related
        // to P-State coordination between processors in a multi-processor
        // configurations.
        //
        Method(_PSD,0)
        {
            //
            // IF CMP is supported/enabled
            //   IF quad core processor
            //     IF PDC[11]
            //         Report 4 processors and HW_ALL as the coordination type
            //     ELSE
            //         Report 4 processors and SW_ALL as the coordination type
            //   ELSE
            //     IF PDC[11]
            //         Report 2 processors and HW_ALL as the coordination type
            //     ELSE
            //         Report 2 processors and SW_ALL as the coordination type
            // ELSE
            //    Report 1 processor and SW_ALL as the coordination type
            //    (Domain 0)
            //
            //   CFGD[24] = Two or more cores enabled
            //   CFGD[23] = Four cores enabled
            //   PDCx[11] = Hardware coordination with hardware feedback
            //

            If(And(CFGD,0x1000000))    // CMP Enabled.
            {
              If(And(CFGD,0x800000))    // Four Cores
              { 
                If(And(PDC0,0x0800))
                {
                    Return(Package(){    // HW_ALL
                        Package(){
                            5,              // # entries.
                            0,              // Revision.
                            0,              // Domain #.
                            0xFE,            // Coord Type- HW_ALL.
                            4                // # processors.
                        }
                    })
                } // If(And(PDC0,0x0800))
      
                Return(Package(){        // SW_ALL
                    Package(){
                        5,                  // # entries.
                        0,                  // Revision.
                        0,                  // Domain #.
                        0xFC,                // Coord Type- SW_ALL.
                        4                    // # processors.
                    }
                })
              }  // If(And(CFGD,0x800000))  

              If(And(PDC0,0x0800))
              {
                 Return(Package(){    // HW_ALL
                      Package(){
                          5,              // # entries.
                          0,              // Revision.
                          0,              // Domain #.
                          0xFE,            // Coord Type- HW_ALL.
                          2                // # processors.
                      }
                  })
               } // If(And(PDC0,0x0800))
                Return(Package(){        // SW_ALL
                    Package(){
                        5,                  // # entries.
                        0,                  // Revision.
                        0,                  // Domain #.
                        0xFC,                // Coord Type- SW_ALL.
                        2                    // # processors.
                    }
                })
            }    // If(And(CFGD,0x1000000))    // CMP Enabled.
            
            Return(Package(){              // SW_ALL
                Package(){
                    5,                        // # entries.
                    0,                        // Revision.
                    0,                        // Domain #.
                    0xFC,                      // Coord Type- SW_ALL.
                    4                          // # processors.
                }
            })
        } // Method(_PSD,0)
    } // Scope(\_PR.CPU0)
} // End of Definition Block


