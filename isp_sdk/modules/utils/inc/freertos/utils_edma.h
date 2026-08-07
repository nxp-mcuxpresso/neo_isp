/*******************************************************************************
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

#ifndef UTILS_EDMA_H
#define UTILS_EDMA_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/**
 * @file  utils_edma.h
 * @brief FreeRTOS EDMA interface
 *
 *******************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*==============================================================================
*                             FUNCTIONS PROTOTYPES
==============================================================================*/

/******************************************************************************/
/**
 * @brief Initialize the EDMA module, called only once after start
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
void utils_EdmaOpen();

/******************************************************************************/
/**
 * @brief Close the EDMA module
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
void utils_EdmaClose();

/******************************************************************************/
/**
 * @brief Perform a DMA transfer from source to destination, same syntax as memcpy
 *
 * @param dest Pointer to the destination buffer
 * @param src Pointer to the source buffer
 * @param n Number of bytes to transfer
 *
 * @returns 0 if all ok, -1 otherwise
 *******************************************************************************/
int utils_EdmaTransfer(void* dest, const void* src, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef UTILS_EDMA_H */
