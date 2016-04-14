/** @file
  Intel Processor Power Management ACPI Code.

  Copyright (c) 2007 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

DefinitionBlock(
	"APTST.aml",
	"SSDT",
	0x01,
	"PmRef",
	"ApTst",
	0x3000
	)
{
	External(\_PR.CPU1, DeviceObj)
	External(\_PR.CPU2, DeviceObj)
	External(\_PR.CPU3, DeviceObj)		
	External(\_PR.CPU0._PTC)
	External(\_PR.CPU0._TSS)
	External(PDC0)
	External(CFGD)
        External(MPEN)

	Scope(\_PR.CPU1)
	{
		Name(_TPC, 0)	// All T-States are available

		//
		// T-State Control/Status interface
		//
		Method(_PTC, 0)
		{
			Return(\_PR.CPU0._PTC)
		}

		Method(_TSS, 0)
		{
			Return(\_PR.CPU0._TSS)
		}

		//
		// T-State Dependency
		//
		Method(_TSD, 0)
		{
			//
			// IF four cores are supported/enabled && !(direct access to MSR)
			//    Report 4 processors and SW_ANY as the coordination
			// IF two cores are supported/enabled && !(direct access to MSR)
			//    Report 2 processors and SW_ANY as the coordination type
			//  ELSE
			//    Report 1 processor and SW_ALL as the coordination type (domain 1)
			//
			//  CFGD[23] = Four cores enabled
			//  CFGD[24] = Two or more cores enabled
			//  PDCx[2] = OSPM is capable of direct access to On
			//		Demand throttling MSR
			//

		If(LNot(And(PDC0,4)))
                {
				Return(Package(){	// SW_ANY
					Package(){
						5,	          // # entries.
						0,	          // Revision.
						0,	          // Domain #.
						0xFD,	        // Coord Type- SW_ANY
						MPEN          // # processors.
					}
				})
                }
  		Return(Package(){		// SW_ALL
  			Package(){
  				5,		          // # entries.
  				0,		          // Revision.
  				1,		          // Domain #.
  				0xFC,		        // Coord Type- SW_ALL
  				1               // # processors.
  			}
  		})
		}
	}  // End of CPU1
	
	Scope(\_PR.CPU2)
	{
		Name(_TPC, 0)	// All T-States are available

		//
		// T-State Control/Status interface
		//
		Method(_PTC, 0)
		{
			Return(\_PR.CPU0._PTC)
		}

		Method(_TSS, 0)
		{
			Return(\_PR.CPU0._TSS)
		}

		//
		// T-State Dependency
		//
		Method(_TSD, 0)
		{
			//
			// IF four cores are supported/enabled && !(direct access to MSR)
			//    Report 4 processors and SW_ANY as the coordination
			// IF two cores are supported/enabled && !(direct access to MSR)
			//    Report 2 processors and SW_ANY as the coordination type
			//  ELSE
			//    Report 1 processor and SW_ALL as the coordination type (domain 1)
			//
			//  CFGD[23] = Four cores enabled
			//  CFGD[24] = Two or more cores enabled
			//  PDCx[2] = OSPM is capable of direct access to On
			//		Demand throttling MSR
			//

		If(LNot(And(PDC0,4)))
                {
				Return(Package(){	// SW_ANY
					Package(){
						5,	          // # entries.
						0,	          // Revision.
						0,	          // Domain #.
						0xFD,	        // Coord Type- SW_ANY
						MPEN          // # processors.
					}
				})
                }
  		Return(Package(){		// SW_ALL
  			Package(){
  				5,		          // # entries.
  				0,		          // Revision.
  				1,		          // Domain #.
  				0xFC,		        // Coord Type- SW_ALL
  				1                // # processors.
  			}
  		})
		}
	}  // End of CPU2
	
	Scope(\_PR.CPU3)
	{
		Name(_TPC, 0)	// All T-States are available

		//
		// T-State Control/Status interface
		//
		Method(_PTC, 0)
		{
			Return(\_PR.CPU0._PTC)
		}

		Method(_TSS, 0)
		{
			Return(\_PR.CPU0._TSS)
		}

		//
		// T-State Dependency
		//
		Method(_TSD, 0)
		{
			//
			// IF four cores are supported/enabled && !(direct access to MSR)
			//    Report 4 processors and SW_ANY as the coordination
			// IF two cores are supported/enabled && !(direct access to MSR)
			//    Report 2 processors and SW_ANY as the coordination type
			//  ELSE
			//    Report 1 processor and SW_ALL as the coordination type (domain 1)
			//
			//  CFGD[23] = Four cores enabled
			//  CFGD[24] = Two or more cores enabled
			//  PDCx[2] = OSPM is capable of direct access to On
			//		Demand throttling MSR
			//

		If(LNot(And(PDC0,4)))
                {
				Return(Package(){	// SW_ANY
					Package(){
						5,	          // # entries.
						0,	          // Revision.
						0,	          // Domain #.
						0xFD,	        // Coord Type- SW_ANY
						MPEN          // # processors.
					}
				})
                }
  		Return(Package(){		// SW_ALL
  			Package(){
  				5,		          // # entries.
  				0,		          // Revision.
  				1,		          // Domain #.
  				0xFC,		        // Coord Type- SW_ALL
  				1                // # processors.
  			}
  		})
		}
	}  // End of CPU3
} // End of Definition Block

