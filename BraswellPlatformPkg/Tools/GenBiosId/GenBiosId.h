/** @file
  Definitions for the GenBiosId tool.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _GEN_BIOS_ID_MPG_
#define _GEN_BIOS_ID_MPG_

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Common/UefiBaseTypes.h>

#define FILE_NAME_SIZE  256
#define MAX_LINE_SIZE   256

#define ATOU(a)         (CHAR16) ((UINT8) (a))

//
// BIOS ID field names.
//
#define NAME_BOARD_ID       "BOARD_ID"
#define NAME_BOARD_REV      "BOARD_REV"
#define NAME_OEM_ID         "OEM_ID"
#define NAME_BUILD_TYPE     "BUILD_TYPE"
#define NAME_VERSION_MAJOR  "VERSION_MAJOR"
#define NAME_VERSION_MINOR  "VERSION_MINOR"

//
// Number of ASCII characters in each field
//
#define CHARS_BOARD_ID      7
#define CHARS_BOARD_REV     1
#define CHARS_OEM_ID        3
#define CHARS_BUILD_TYPE    1
#define CHARS_VERSION_MAJOR 4
#define CHARS_VERSION_MINOR 2

#define FIELD_BOARD_ID      0x0001
#define FIELD_BOARD_REV     0x0002
#define FIELD_OEM_ID        0x0004
#define FIELD_BUILD_TYPE    0x0008
#define FIELD_VERSION_MAJOR 0x0010
#define FIELD_VERSION_MINOR 0x0020

#define FIELD_ALL (     \
  FIELD_BOARD_ID      | \
  FIELD_BOARD_REV     | \
  FIELD_OEM_ID        | \
  FIELD_BUILD_TYPE    | \
  FIELD_VERSION_MAJOR | \
  FIELD_VERSION_MINOR )

//
// BIOS ID string format:
//
// $(BOARD_ID)$(BOARD_REV).$(OEM_ID).$(VERSION_MAJOR).$(BUILD_TYPE)$(VERSION_MINOR).YYMMDDHHMM
//
// Example: "TRFTCRB1.86C.0008.D03.0506081529"
//
// The format should be same as platform BIOS ID definition
//
#pragma pack(1)

typedef struct {
  CHAR16    BoardId[7];             // "TRFTCRB"
  CHAR16    BoardRev;               // "1"
  CHAR16    Dot1;                   // "."
  CHAR16    OemId[3];               // "86C"
  CHAR16    Dot2;                   // "."
  CHAR16    VersionMajor[4];        // "0008"
  CHAR16    Dot3;                   // "."
  CHAR16    BuildType;              // "D"
  CHAR16    VersionMinor[2];        // "03"
  CHAR16    Dot4;                   // "."
  CHAR16    TimeStamp[10];          // "YYMMDDHHMM"
  CHAR16    NullTerminator;         // 0x0000
} BIOS_ID_STRING;

//
// A signature precedes the BIOS ID string in the FV to enable search by external tools.
//
typedef struct {
  UINT8             Signature[8];   // "$IBIOSI$"
  BIOS_ID_STRING    BiosIdString;   // "TRFTCRB1.86C.0008.D03.0506081529"
} BIOS_ID_IMAGE;

#pragma pack()

INTN
main (
  IN  INTN  argc,
  IN  CHAR8 *argv[]
  );

VOID
PrintBanner (
  VOID
  );

VOID
PrintUsage (
  VOID
  );

EFI_STATUS
ParseInputFile (
  VOID
  );

EFI_STATUS
WriteOutputFile (
  VOID
  );

CHAR8 *
StripLeadingWhiteSpace (
  IN CHAR8 *StrInput
  );

VOID
ConvertToUnicode (
  IN  CHAR8  *StrAscii,
  OUT CHAR16 *StrUnicode
  );

VOID
PrintUnicodeString (
  IN CHAR16 *StrUnicode
  );

VOID
FillTimeStamp (
  OUT CHAR16  *StrTimeStampUnicode
  );

#endif // _GEN_BIOS_ID_MPG_
