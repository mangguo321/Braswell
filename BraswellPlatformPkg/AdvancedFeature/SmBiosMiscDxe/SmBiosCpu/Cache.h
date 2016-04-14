/** @file
  Include file for record cache subclass data with Smbios protocol.

  Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CACHE_H_
#define _CACHE_H_

#include <IndustryStandard/SmBios.h>
//
// Bit field definitions for return registers of CPUID EAX = 4
//
// EAX
#define CPU_CACHE_TYPE_MASK                0x1F
#define CPU_CACHE_LEVEL_MASK               0xE0
#define CPU_CACHE_LEVEL_SHIFT              5
// EBX
#define CPU_CACHE_LINESIZE_MASK            0xFFF
#define CPU_CACHE_PARTITIONS_MASK          0x3FF000
#define CPU_CACHE_PARTITIONS_SHIFT         12
#define CPU_CACHE_WAYS_MASK                0xFFC00000
#define CPU_CACHE_WAYS_SHIFT               22

#define CPU_CACHE_L1        1
#define CPU_CACHE_L2        2
#define CPU_CACHE_L3        3
#define CPU_CACHE_L4        4
#define CPU_CACHE_LMAX      CPU_CACHE_L4



typedef struct {
  UINT8                         CacheLevel;
  UINT8                         CacheDescriptor;
  UINT16                        CacheSizeinKB;
  CACHE_ASSOCIATIVITY_DATA      Associativity;
  CACHE_TYPE_DATA               SystemCacheType;
} CPU_CACHE_CONVERTER;


typedef struct {
  UINT16                        CacheSizeinKB;
  CACHE_ASSOCIATIVITY_DATA      Associativity;
  CACHE_TYPE_DATA               SystemCacheType;
  //
  // Can extend the structure here.
  //
} CPU_CACHE_DATA;

//
// It is defined for SMBIOS_TABLE_TYPE7.CacheConfiguration.
//
typedef struct {
  UINT16    Level           :3;
  UINT16    Socketed        :1;
  UINT16    Reserved2       :1;
  UINT16    Location        :2;
  UINT16    Enable          :1;
  UINT16    OperationalMode :2;
  UINT16    Reserved1       :6;
} CPU_CACHE_CONFIGURATION_DATA;

CPU_CACHE_CONVERTER mCacheConverter[] = {
  {
    1,
    0x06,
    8,
    CacheAssociativity4Way,
    CacheTypeInstruction
  },
  {
    1,
    0x08,
    16,
    CacheAssociativity4Way,
    CacheTypeInstruction
  },
  {
    1,
    0x0A,
    8,
    CacheAssociativity2Way,
    CacheTypeData
  },
  {
    1,
    0x0C,
    16,
    CacheAssociativity4Way,
    CacheTypeData
  },
  {
    3,
    0x22,
    512,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    3,
    0x23,
    1024,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    3,
    0x25,
    2048,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    3,
    0x29,
    4096,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    1,
    0x2C,
    32,
    CacheAssociativity8Way,
    CacheTypeData
  },
  {
    1,
    0x30,
    32,
    CacheAssociativity8Way,
    CacheTypeInstruction
  },
  {
    2,
    0x39,
    128,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x3B,
    128,
    CacheAssociativity2Way,
    CacheTypeUnified
  },
  {
    2,
    0x3C,
    256,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x41,
    128,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x42,
    256,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x43,
    512,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x44,
    1024,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x45,
    2048,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x49,
    4096,
    CacheAssociativity16Way,
    CacheTypeUnified
  },
  {
    1,
    0x60,
    16,
    CacheAssociativity8Way,
    CacheTypeData
  },
  {
    1,
    0x66,
    8,
    CacheAssociativity4Way,
    CacheTypeData
  },
  {
    1,
    0x67,
    16,
    CacheAssociativity4Way,
    CacheTypeData
  },
  {
    1,
    0x68,
    32,
    CacheAssociativity4Way,
    CacheTypeData
  },
  {
    2,
    0x78,
    1024,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x79,
    128,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x7A,
    256,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x7B,
    512,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x7C,
    1024,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x7D,
    2048,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x7F,
    512,
    CacheAssociativity2Way,
    CacheTypeUnified
  },
  {
    2,
    0x82,
    256,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x83,
    512,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x84,
    1024,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x85,
    2048,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x86,
    512,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x87,
    1024,
    CacheAssociativity8Way,
    CacheTypeUnified
  }
};
#endif

