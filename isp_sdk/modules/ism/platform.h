/*
 * Copyright 2022-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file platform.h
 * @version 0.1
 * @date 2022-08-30
 */

#ifndef PLATFORM_H_GUARD
#define PLATFORM_H_GUARD

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum state Defines whether a feature should be enabled or disabled
 */
enum feature_state
{
    DISABLED,   /**< Disable feature */
    ENABLED     /**< Enable feature */
};

/* Features enable */
#if !defined DEBUG_PRINT_ENABLE
#warning "DEBUG_PRINT_ENABLE not defined, disabling debug prints by default!"
#define DEBUG_PRINT_ENABLE  0
#endif

#include "ism_config.h"

#if defined(IMX95)
    #if (1 != (defined(A0) + defined(B0)))
        #error "Only one revision must be defined! Define one of [A0, B0]!"
    #endif
#else
    #error "HW platform must be defined! Define one of [IMX95]!"
#endif

#if (1 != (defined(LINUX_EMB) + defined(FREE_RTOS) + defined(ISIM) + defined(SDK_OS_FREE_RTOS) + defined(GHS)))
#error "Exactly one platform must be defined! Define one of [LINUX_EMB, FREE_RTOS, ISIM, SDK_OS_FREE_RTOS, GHS]!"
#endif

#if defined (FREE_RTOS)


#define plt_assert(x)   (void)(x)

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "os.h"
#include "errno.h"

#if defined(IMX95)
    #if defined (A0)
        #include "imx9xm0p_A0.h"
    #elif defined (B0)
        #include "imx9xm0p_B0.h"
    #else
        #error "Device revision not specified. Define one of [A0, B0]!"
    #endif
#else
    #error "M0 FW  only for IMX95 platform!"
#endif

#define ADDRESS_TO_UINT64(x)            ((uint64_t)(uint32_t)(x))

/* ILOG can't be used on non-Linux implementations. We don't include ilog header
at all and just define ILOG as empty expression. */
#define ILOG(_mod, _lev, ...)       ((void) 0)

#if (DEBUG_PRINT_ENABLE == 1)
/* On FreeRTOS there is no simple way to print, so we currently just leave the
print empty. */
#define DEBUG_PRINT(x, ...)     (void) x
#else
#define DEBUG_PRINT(x, ...)     (void) x
#endif

#elif defined(LINUX_EMB)


#define plt_assert(x)   (void)(x)

#include <linux/errno.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/limits.h>
#include <linux/time.h>
#include "port.h"

#if defined(IMX95)
    #if defined (A0)
        #include "imx9x_A0.h"
    #elif defined (B0)
        #include "imx9x_B0.h"
    #else
        #error "Device revision not specified. Define one of [A0, B0]!"
    #endif
#else
    #error "ISM only implemented for Linux on IMX95 platform!"
#endif



#if (DEBUG_PRINT_ENABLE == 1)
#define DEBUG_PRINT                 printk
#else
#define DEBUG_PRINT(x, ...)                 (void) x
#endif

#define UINT8_MAX U8_MAX
#define UINT32_MAX U32_MAX
#define UINT64_MAX U64_MAX

#define taskENTER_CRITICAL()                (void) 0
#define taskEXIT_CRITICAL()                 (void) 0
#define taskENTER_CRITICAL_FROM_ISR()       0
#define taskEXIT_CRITICAL_FROM_ISR(mask)    (void) mask
#define xPortGetTime                        prvGetTimeUs

#define ADDRESS_TO_UINT64(x)            ((uint64_t)(x))

typedef struct q_t * QueueHandle_t;

#elif defined (ISIM)

#define plt_assert(x)   (void)(x)

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include "ilog.h"
#include "errno.h"



#if defined(IMX95)
    #if defined (A0)
        #include "imx9xm0p_A0.h"
    #elif defined (B0)
        #include "imx9xm0p_B0.h"
    #else
        #error "Device revision not specified. Define one of [A0, B0]!"
    #endif
#else
    #error "M0 FW  only for IMX95 platform!"
#endif

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"

#define ADDRESS_TO_UINT64(x)            ((uint64_t)(x))

#if (DEBUG_PRINT_ENABLE == 1)
#define DEBUG_PRINT(x, ...)     printf(x, ...)
#else
#define DEBUG_PRINT(x, ...)     (void) x
#endif

#ifdef EDMA_DESCRIPTORS
#undef EDMA_DESCRIPTORS
extern EDMA_TCD_TypeDef edma_descriptors[16];
#define EDMA_DESCRIPTORS ((EDMA_TCD_TypeDef *) edma_descriptors)
#endif /* EDMA Descriptors*/

#ifdef ISP
#undef ISP
extern ISP_TypeDef isp_virtual;
#define ISP         ((ISP_TypeDef *) &isp_virtual)
#endif /* ISP */

#ifdef EDMA
#undef EDMA
extern EDMA_TypeDef edma_virtual;
#define EDMA        ((EDMA_TypeDef *) &edma_virtual)
#endif /* EDMA */

#ifdef MU0
#undef MU0
extern MU_TypeDef mu0_virtual;
#define MU0         ((MU_TypeDef *) &mu0_virtual)
#endif /* MU0 */

#ifdef MU1
#undef MU1
extern MU_TypeDef mu1_virtual;
#define MU1         ((MU_TypeDef *) &mu1_virtual)
#endif /* MU1 */

#ifdef MU2
#undef MU2
extern MU_TypeDef mu2_virtual;
#define MU2         ((MU_TypeDef *) &mu2_virtual)
#endif /* MU2 */

#ifdef MU3
#undef MU3
extern MU_TypeDef mu3_virtual;
#define MU3         ((MU_TypeDef *) &mu3_virtual)
#endif /* MU3 */

#ifdef MU4
#undef MU4
extern MU_TypeDef mu4_virtual;
#define MU4         ((MU_TypeDef *) &mu4_virtual)
#endif /* MU4 */

#ifdef MU5
#undef MU5
extern MU_TypeDef mu5_virtual;
#define MU5         ((MU_TypeDef *) &mu5_virtual)
#endif /* MU5 */

#ifdef MU6
#undef MU6
extern MU_TypeDef mu6_virtual;
#define MU6         ((MU_TypeDef *) &mu6_virtual)
#endif /* MU6 */

#ifdef MU7
#undef MU7
extern MU_TypeDef mu7_virtual;
#define MU7         ((MU_TypeDef *) &mu7_virtual)
#endif /* MU7 */

#ifdef MU8
#undef MU8
extern MU_TypeDef mu8_virtual;
#define MU8         ((MU_TypeDef *) &mu8_virtual)
#endif /* MU */

extern void MU_IRQ_Handler(void);
extern void ISP_IRQ_Handler(void);
extern void EDMA_IRQ_Handler(void);
extern void EDMA_IPI_ERR_IRQ_Handler(void);

#elif (defined(SDK_OS_FREE_RTOS))
#include <assert.h>

#define plt_assert(x)   assert(x)

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "clock.h"
#include <time.h>
#include "imp_definition.h"
#include "errno.h"


#include "paxRegConfig.h"
#include "isp_mems.h"


#if defined(IMX95)
    #include "imx9x_addr.h"
    #include "fsl_common.h"
    #include "PERI_ISP_EDMA5.h"

    #if defined(A0)
        #include "PERI_ISP_A1.h"
    #elif defined(B0)
        #include "PERI_ISP_B0.h"
    #else
        #error HW revision not defined [A0, B0].
    #endif
#else
    #error Hw platform not defined [IMX95].
#endif

#define ADDRESS_TO_UINT64(x)            ((uint64_t)(uint32_t)(x))

#if (DEBUG_PRINT_ENABLE == 1)
#include "fsl_debug_console.h"
#define DEBUG_PRINT(...)     PRINTF(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif


/* Converts time in microseconds to time in ticks. This macro is complementary
to pdMS_TO_TICKS, which is used for milliseconds resolution.
The resolution depends on the configTICK_RATE_HZ.
e.g. configTICK_RATE_HZ = 250
the minimal value would be:
1000 000 / 250 = 4000 us

configTICK_RATE_HZ = 20000
1000 000 / 20000 ~ 50 us
Anything below these values would represent same time delay for given
configTICK_RATE_HZ.
*/
#define pdUS_TO_TICKS( xTimeInUs ) ( ( TickType_t ) ( ( ( TickType_t ) ( xTimeInUs ) * ( TickType_t ) configTICK_RATE_HZ ) / ( TickType_t ) 1000000 ) )

#define xPortGetTime()      xPortGetTimeUs()

#elif defined(GHS)

#include <INTEGRITY.h>
#include <INTEGRITY_assert.h>
#include <stdbool.h>
#include <errno.h>
#include "imp_definition.h"
#include "support.h"

#if defined (A0)
#include "imx9x_A0.h"
#elif defined(B0)
#include "imx9x_B0.h"
#else
#error "Device revision not specified. Define one of [A0, B0]!"
#endif

#define plt_assert(x)   Assert(x)

#if (DEBUG_PRINT_ENABLE == 1)
#define DEBUG_PRINT(x, ...) printf(x, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(x, ...)
#endif

#define taskENTER_CRITICAL()                supp_IsmLockAcquire()
#define taskEXIT_CRITICAL()                 supp_IsmLockRelease()
/* ISM driver is located in VAS, so we don't need special handling for ISR */
#define taskENTER_CRITICAL_FROM_ISR()       0; taskENTER_CRITICAL()
#define taskEXIT_CRITICAL_FROM_ISR(mask)    (void) mask; taskEXIT_CRITICAL()

#define xPortGetTime                        xPortGetTimeUs

#define ADDRESS_TO_UINT64(x)            ((uint64_t)(x))

/* Return error codes */
#define EOK (0)

typedef struct q_t
{
    /**
     * Integrity message queue object
     */
    MessageQueue mq;

    /**
     * Size of each element we store into queue
     */
    size_t item_size;
} QueueHandle_t;


#else
#error "Platform not defined! Define one of [LINUX_EMB, FREE_RTOS, ISIM, SDK_OS_FREE_RTOS, GHS]!"
#endif

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_H_GUARD */
