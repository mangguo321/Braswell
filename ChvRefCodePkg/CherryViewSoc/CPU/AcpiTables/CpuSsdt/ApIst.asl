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
	"APIST.aml",
	"SSDT",
	1,
	"PmRef",
	"ApIst",
	0x3000
	)
{
	External(\_PR.CPU0._PSS, MethodObj)
	External(\_PR.CPU0._PCT, MethodObj)
	External(\_PR.CPU0._PPC, IntObj)
	External(\_PR.CPU0._PSD, MethodObj)
	External(\_PR.CPU1, DeviceObj)
	External(\_PR.CPU2, DeviceObj)
	External(\_PR.CPU3, DeviceObj)
	External (CFGD)
	External (PDC0)

	Scope(\_PR.CPU1)
	{
		Method(_PPC,0)
		{
			Return(\_PR.CPU0._PPC)	// Return P0 _PPC value.
		}

		Method(_PCT,0)
		{
			Return(\_PR.CPU0._PCT)	// Return P0 _PCT.
		}

		Method(_PSS,0)
		{
			//Return the same table as CPU0 for CMP cases.
			Return(\_PR.CPU0._PSS)
		}

		// The _PSD object provides information to the OSPM related
		// to P-State coordination between processors in a multi-processor
		// configurations.
		//
		Method(_PSD,0)
		{
			Return(\_PR.CPU0._PSD)	// Return P0 _PSD.			
		}
	}
	
	Scope(\_PR.CPU2)
	{
		Method(_PPC,0)
		{
			Return(\_PR.CPU0._PPC)	// Return P0 _PPC value.
		}

		Method(_PCT,0)
		{
			Return(\_PR.CPU0._PCT)	// Return P0 _PCT.
		}

		Method(_PSS,0)
		{
			//Return the same table as CPU0 for CMP cases.
			Return(\_PR.CPU0._PSS)
		}

		// The _PSD object provides information to the OSPM related
		// to P-State coordination between processors in a multi-processor
		// configurations.
		//
		Method(_PSD,0)
		{
			Return(\_PR.CPU0._PSD)	// Return P0 _PSD.			
		}
	}
	
	Scope(\_PR.CPU3)
	{
		Method(_PPC,0)
		{
			Return(\_PR.CPU0._PPC)	// Return P0 _PPC value.
		}

		Method(_PCT,0)
		{
			Return(\_PR.CPU0._PCT)	// Return P0 _PCT.
		}

		Method(_PSS,0)
		{
			//Return the same table as CPU0 for CMP cases.
			Return(\_PR.CPU0._PSS)
		}

		// The _PSD object provides information to the OSPM related
		// to P-State coordination between processors in a multi-processor
		// configurations.
		//
		Method(_PSD,0)
		{
			Return(\_PR.CPU0._PSD)	// Return P0 _PSD.			
		}
	}
} // End of Definition Block
