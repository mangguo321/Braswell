/** @file
  This tool generates the BIOS ID binary file using the data from the configuration file.

  Copyright (c) 1999 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "GenBiosId.h"

char          InputFileName[FILE_NAME_SIZE];
char          OutputFileName[FILE_NAME_SIZE];

FILE          *InputFile;
FILE          *OutputFile;

BIOS_ID_IMAGE BiosIdImage = {
  { '$', 'I', 'B', 'I', 'O', 'S', 'I', '$' },
  {
    { ATOU (' '), ATOU (' '), ATOU (' '), ATOU (' '), ATOU (' '), ATOU (' '), ATOU (' ') },
    ATOU (' '),
    ATOU ('.'),
    { ATOU (' '), ATOU (' '), ATOU (' ') },
    ATOU ('.'),
    { ATOU ('0'), ATOU ('0'), ATOU ('0'), ATOU ('0') },
    ATOU ('.'),
    ATOU (' '),
    { ATOU ('0'), ATOU ('0') },
    ATOU ('.'),
    { ATOU ('0'), ATOU ('0'), ATOU ('0'), ATOU ('0'), ATOU ('0'), 
      ATOU ('0'), ATOU ('0'), ATOU ('0'), ATOU ('0'), ATOU ('0') },
    ATOU ('\0')
  }
};

/**
  @param[in] argc     Number of command-line arguments.
  @param[in] argv     Array of command-line arguments.

  @retval  0         The operation completed successfully.
  @retval  1         An error occurred.
  
**/
INTN
main (
  IN  INTN  argc,
  IN  CHAR8 *argv[]
  )
{
  int Status;
  
  Status = 0;
  
  PrintBanner ();
  
  if (argc != 5) {
    PrintUsage ();
    Status = -1;
    goto Done;
  }
  
  if (strcmp (argv[1], "-i") == 0) {
    strcpy (InputFileName, argv[2]);
  } else {
    PrintUsage ();
    Status = -1;
    goto Done;
  }

  if (strcmp (argv[3], "-o") == 0) {
    strcpy (OutputFileName, argv[4]);
  } else {
    PrintUsage ();
    Status = -1;
    goto Done;
  }
  
  InputFile   = NULL;
  OutputFile  = NULL;
  
  InputFile   = fopen (InputFileName, "r");
  if (InputFile == NULL) {
    printf ("Error opening input file: %s\n", InputFileName);
    Status = -1;
    goto Done;
  }

  OutputFile = fopen (OutputFileName, "wb");
  if (OutputFile == NULL) {
    printf ("Error creating output file: %s\n", OutputFileName);
    fclose (InputFile);
    Status = -1;
    goto Done;
  }
  
  if (ParseInputFile () != EFI_SUCCESS) {
    printf ("Invalid config file format: %s\n", InputFileName);
    Status = -1;
    goto Done;
  }

  if (WriteOutputFile () != EFI_SUCCESS) {
    printf ("Can't write output file: %s\n", OutputFileName);
    Status = -1;
    goto Done;
  }
  
  printf ("BIOS ID created: ");
  PrintUnicodeString ((CHAR16 *)(&(BiosIdImage.BiosIdString)));
  printf ("\n");
  printf ("BIOS ID binary file created: %s\n", OutputFileName);
  
Done:
  
  if (InputFile != NULL) {
    fclose (InputFile);
  }
  
  if (OutputFile != NULL) {
    fclose (OutputFile);
  }
  
  return Status;
}

/**
 @param[in]     None

**/
VOID
PrintBanner (
  VOID
  )
{
  printf ("\n");
  printf ("GenBiosId utility, version: v1.0 06/08/2005   \n");
  printf ("Copyright (c) 2005 - 2015, Intel Corporation. All rights reserved.   \n");
  printf ("\n");
}

/**
  @param[in]     None

**/
VOID
PrintUsage (
  VOID
  )
{
  printf ("Usage:                                   \n");
  printf ("GenBiosId -i ConfigFile -o OutputFile \n");
  printf ("\n");
}

/**
  Parse the BIOS ID definition file and fill the BIOS ID structure, including timestamp.

  @param[in]     None

  @retval  EFI_SUCCESS             The input file was parsed successfully.
  @retval  EFI_INVALID_PARAMETER   The input file has incorrect format.

**/
EFI_STATUS
ParseInputFile (
  VOID
  )
{
  CHAR8   StrLine[MAX_LINE_SIZE];
  CHAR8   StrFieldName[MAX_LINE_SIZE];
  CHAR8   StrFieldValue[MAX_LINE_SIZE];
  CHAR8   *Str;
  UINT16  FieldsSet;
  
  FieldsSet = 0;
  Str       = NULL;
  
  while (fgets (StrLine, sizeof (StrLine) - 1, InputFile) != NULL) {
    Str = StripLeadingWhiteSpace (StrLine);

    //
    // Check for whitespace string.
    //
    if (Str == NULL) {
      continue;
    }

    //
    // Check for comment string.
    //
    if (*Str == '#') {
      continue;
    }

    if (sscanf (Str, "%s = %s", StrFieldName, StrFieldValue) != 2) {
      continue;
    }
    
    if (strcmp (StrFieldName, NAME_BOARD_ID) == 0) {
      if (strlen (StrFieldValue) == CHARS_BOARD_ID) {
        ConvertToUnicode (StrFieldValue, BiosIdImage.BiosIdString.BoardId);
        FieldsSet |= FIELD_BOARD_ID;
      } else {
        printf ("Error: value of %s should have exactly %d chars\n", NAME_BOARD_ID, CHARS_BOARD_ID);
        return EFI_INVALID_PARAMETER;
      }
    }

    if (strcmp (StrFieldName, NAME_BOARD_REV) == 0) {
      if (strlen (StrFieldValue) == CHARS_BOARD_REV) {
        ConvertToUnicode (StrFieldValue, &(BiosIdImage.BiosIdString.BoardRev));
        FieldsSet |= FIELD_BOARD_REV;
      } else {
        printf ("Error: value of %s should have exactly %d chars\n", NAME_BOARD_REV, CHARS_BOARD_REV);
        return EFI_INVALID_PARAMETER;
      }
    }

    if (strcmp (StrFieldName, NAME_OEM_ID) == 0) {
      if (strlen (StrFieldValue) == CHARS_OEM_ID) {
        ConvertToUnicode (StrFieldValue, BiosIdImage.BiosIdString.OemId);
        FieldsSet |= FIELD_OEM_ID;
      } else {
        printf ("Error: value of %s should have exactly %d chars\n", NAME_OEM_ID, CHARS_OEM_ID);
        return EFI_INVALID_PARAMETER;
      }
    }

    if (strcmp (StrFieldName, NAME_BUILD_TYPE) == 0) {
      if (strlen (StrFieldValue) == CHARS_BUILD_TYPE) {
        ConvertToUnicode (StrFieldValue, &(BiosIdImage.BiosIdString.BuildType));
        FieldsSet |= FIELD_BUILD_TYPE;
      } else {
        printf ("Error: value of %s should have exactly %d chars\n", NAME_BUILD_TYPE, CHARS_BUILD_TYPE);
        return EFI_INVALID_PARAMETER;
      }
    }

    if (strcmp (StrFieldName, NAME_VERSION_MAJOR) == 0) {
      if (strlen (StrFieldValue) == CHARS_VERSION_MAJOR) {
        ConvertToUnicode (StrFieldValue, BiosIdImage.BiosIdString.VersionMajor);
        FieldsSet |= FIELD_VERSION_MAJOR;
      } else {
        printf ("Error: value of %s should have exactly %d chars\n", NAME_VERSION_MAJOR, CHARS_VERSION_MAJOR);
        return EFI_INVALID_PARAMETER;
      }
    }

    if (strcmp (StrFieldName, NAME_VERSION_MINOR) == 0) {
      if (strlen (StrFieldValue) == CHARS_VERSION_MINOR) {
        ConvertToUnicode (StrFieldValue, BiosIdImage.BiosIdString.VersionMinor);
        FieldsSet |= FIELD_VERSION_MINOR;
      } else {
        printf ("Error: value of %s should have exactly %d chars\n", NAME_VERSION_MINOR, CHARS_VERSION_MINOR);
        return EFI_INVALID_PARAMETER;
      }
    }

  }
  
  //
  // Exit if not all fields were supplied.
  //
  if ((FieldsSet & FIELD_ALL) != FIELD_ALL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Fill the timestamp.
  //
  FillTimeStamp (BiosIdImage.BiosIdString.TimeStamp);
  
  return EFI_SUCCESS;
}

/**
  Write the BIOS ID structure to the output file.

  @param[in]

  @retval  EFI_SUCCESS         The output file was created successfully.
  @retval  EFI_DEVICE_ERROR    There was a file write error.

**/
EFI_STATUS
WriteOutputFile (
  VOID
  )
{
  if (fwrite (&BiosIdImage, sizeof (BIOS_ID_IMAGE), 1, OutputFile) != 1) {
    return EFI_DEVICE_ERROR;
  }
  
  return EFI_SUCCESS;
}

/**
  Strip the leading whitespoace off the given ASCII string.

  @param[in]  StrInput      The ASCII string that should be processed.

  @retval                   A pointer to the first non-whitespace character in the given string, 
                            or NULL if the string was all whitespace.

**/
CHAR8 *
StripLeadingWhiteSpace (
  IN CHAR8 *StrInput
  )
{
  if (StrInput == NULL) {
    return NULL;
  }
  
  while (*StrInput != 0) {
    if ((*StrInput == ' ') || (*StrInput == '\t') || (*StrInput == '\n')) {
      ++StrInput;
    } else {
      return StrInput;
    }
  }
  
  return NULL;
}

/**
  Convert the given ASCII string to Unicode without appending terminating 0x0000.

  @param[in]  StrAscii      The source ASCII string, null-terminated.
  @param[out] StrUnicode    The resulted Unicode string will be put here, without the terminating 0x0000.

  @retval   

**/
VOID
ConvertToUnicode (
  IN  CHAR8  *StrAscii,
  OUT CHAR16 *StrUnicode
  )
{
  if ((StrAscii == NULL) || (StrUnicode == NULL)) {
    return;
  }
  
  while (*StrAscii != 0) {
    *StrUnicode = ATOU (*StrAscii);
    ++StrAscii;
    ++StrUnicode;
  }
}

/**
  Print the given Unicode string.

  @param[in]  StrUnicode     The null-terminated Unicode string to print.

  @retval

**/
VOID
PrintUnicodeString (
  IN CHAR16 *StrUnicode
  )
{
  if (StrUnicode == NULL) {
    return;
  }
  
  while (*StrUnicode != 0) {
    putchar ((CHAR8)(*StrUnicode));
    ++StrUnicode;
  }
}

/**
  The function generates the current timestamp in "YYMMDDHHMM" format
  and puts it into the Unicode string supplied, without the null-terminator. 

  @param[in]  StrTimeStampUnicode    The Unicode string which is filled on return with the current timestamp.

  @retval

**/
VOID
FillTimeStamp (
  OUT CHAR16  *StrTimeStampUnicode
  )
{
  struct tm *Time;
  time_t    CurTime;
  CHAR8     StrTime[11];

  //
  // Fetch the current time.
  //
  time (&CurTime);
  Time = localtime (&CurTime);

  sprintf (
    StrTime,
    "%02d%02d%02d%02d%02d",
    (Time->tm_year + 1900) % 100, // Year is 1900-based, need only 2 digits.
    Time->tm_mon + 1,             // Month is zero based.
    Time->tm_mday,
    Time->tm_hour,
    Time->tm_min
    );
  
  ConvertToUnicode (StrTime, StrTimeStampUnicode);
}
