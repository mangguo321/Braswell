/** @file
  Defines SocSkuLib.

  Copyright (c) 2005 - 2015, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef SOCSKULIB_H_
#define SOCSKULIB_H_

typedef enum {
  SOC_SKU_200 = 0,
  SOC_SKU_320 = 1,
  SOC_SKU_333 = 3,
  SOC_SKU_400 = 4,
  SOC_SKU_360 = 5,
  SOC_SKU_373 = 6,
  SOC_SKU_MAX = 7
} SOC_SKU;

typedef enum {
  SKU_INDEX_SOC200_SOC320_SOC400 = 0,
  SKU_INDEX_SOC333 = 1,
  SKU_INDEX_SOC360 = 2,
  SKU_INDEX_SOC373 = 3,
  SKU_INDEX_MAX = 4,
} SKU_INDEX;

/**
  @todo	add description

**/
SOC_SKU
GetSocSku (
  VOID
  );

/**
  @todo	add description

**/
SKU_INDEX
GetMrcSkuIndex (
  VOID
  );

#endif /* SOCSKULIB_H_ */
