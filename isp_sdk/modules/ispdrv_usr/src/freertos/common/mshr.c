/*******************************************************************************
 * Copyright 2021-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/
#define _GNU_SOURCE

/******************************************************************************/
/**
 * @file  mshr.c
 * @brief Implements functions to interface with RAM.
 *******************************************************************************/
#include "string.h"
 #include "FreeRTOS.h"
#include "semphr.h"

#include "ilog.h"
#include "mshr.h"


#define MSHR_STREAM_HEAP_BLOCK_SIZE     (100*1024*1024U)
#define BLOCK_ALIGNMENT                 (128U)
#define BLOCK_ALIGNMENT_MASK            (BLOCK_ALIGNMENT - 1)
#define MAX_ALLOCATION_CNT_PER_STREAM   (32U)

DDR_NOINIT uint8_t gHeapLocation[ISP_HWID_CNT * ISPDRV_CAM_PIPE_ID_CNT * MSHR_STREAM_HEAP_BLOCK_SIZE];


/*==============================================================================
*                             DEFINES AND MACROS
==============================================================================*/

/*==============================================================================
*                                  TYPEDEFS
==============================================================================*/

/** @brief info structure for each memory allocation */
typedef struct
{
    /** @brief base address in PHY memory*/
    uint8_t *mStart;
    /** @brief end address in PHY memory */
    uint8_t *mEnd;
}MShr_Allocation_t;

/* control structure for each stream */
typedef struct
{
    MShr_Allocation_t AllocatedMems[MAX_ALLOCATION_CNT_PER_STREAM];
    SemaphoreHandle_t mAccess;
    StaticSemaphore_t mAccessData;
    void* mNextFreeMem;
    void* mFirstNonFreeMem;
} MShr_StreamCtr_t;

/*==============================================================================
*                                  CONSTANTS
==============================================================================*/

/*==============================================================================
*                                  VARIABLES
==============================================================================*/

/* mode of operation (set during init) */
static MShr_Mode_t gMode = MSHR_MODE_UNKNOWN;

/* control block for all streams */
static MShr_StreamCtr_t gStreamCtr[ISP_HWID_CNT][ISPDRV_CAM_PIPE_ID_CNT];

static bool gLockReady = false;
static SemaphoreHandle_t gLock;
static StaticSemaphore_t gLockData;

/*==============================================================================
*                               LOCAL FUNCTIONS
==============================================================================*/

static void InitGlobalLock(void)
{
    gLock = xSemaphoreCreateMutexStatic(&gLockData);
    if (gLock == NULL)
    {
        while (true)
        {
            ;
        }
    }
    gLockReady = true;
}

static inline void ResetAlloc(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    assert(aHwId < ISP_HWID_CNT);
    assert(aCamId < ISPDRV_CAM_PIPE_ID_CNT);
    memset(gStreamCtr[aHwId][aCamId].AllocatedMems, 0, sizeof(gStreamCtr[aHwId][aCamId].AllocatedMems));
    gStreamCtr[aHwId][aCamId].mNextFreeMem = &gHeapLocation[(aHwId + 1) * aCamId * MSHR_STREAM_HEAP_BLOCK_SIZE];
    gStreamCtr[aHwId][aCamId].mFirstNonFreeMem = &gHeapLocation[(aHwId + 1) * (aCamId + 1) * MSHR_STREAM_HEAP_BLOCK_SIZE];
}

/**
 * @brief static initialization of variables
 */
static int MShr_InitVariables(void)
{
    for (uint8_t HwId = 0; HwId < ISP_HWID_CNT; HwId++)
    {
        for (uint8_t CamId = 0; CamId < ISPDRV_CAM_PIPE_ID_CNT; CamId++)
        {
            gStreamCtr[HwId][CamId].mAccess = xSemaphoreCreateMutexStatic(&gStreamCtr[HwId][CamId].mAccessData);
            ResetAlloc(HwId,CamId);
        }
    }

    return 0;
}

static MShr_Allocation_t *GetFreeAllocCtl(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId)
{
    if (aHwId >= ISP_HWID_CNT)
    {
        return NULL;
    }

    MShr_StreamCtr_t *Str = &gStreamCtr[aHwId][aCamId];
    for (int i=0; i<MAX_ALLOCATION_CNT_PER_STREAM; i++)
    {
        if (Str->AllocatedMems[i].mStart == 0 && Str->AllocatedMems[i].mEnd == 0)
        {
            return &Str->AllocatedMems[i];
        }
    }

    return NULL;
}

/**
 * @brief Locates suitable address for allocation, allocates and stores all info for new block
 *
 * @param[in] aHwId - Id of the Hw block
 * @param[in] aCamId - Stream id
 * @param[in] aSize - target size of new block
 *
 * @return Work pointer for the new memory or NULL on error.
 */
static void* MShr_Allocate(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, uint64_t aSize)
{
    MShr_StreamCtr_t *Str = &gStreamCtr[aHwId][aCamId];
    MShr_Allocation_t *FirstFreeCtlStructure;

    // check we have space for this mem before the end of alock block
    if (aSize > ((uint64_t) Str->mFirstNonFreeMem - (uint64_t) Str->mNextFreeMem))
    {
        return false;
    }
    FirstFreeCtlStructure = GetFreeAllocCtl(aHwId, aCamId);
    if (NULL == FirstFreeCtlStructure)
    {
        return false;
    }

    FirstFreeCtlStructure->mStart = Str->mNextFreeMem;
    uintptr_t End = ((uintptr_t )Str->mNextFreeMem) + aSize;
    if (0 != (End & BLOCK_ALIGNMENT_MASK))
    {
        End &= ~BLOCK_ALIGNMENT_MASK;
        End += BLOCK_ALIGNMENT;
    }
    Str->mNextFreeMem = (uint8_t *)End;
    FirstFreeCtlStructure->mEnd = (uint8_t *) (End - 1);

    return FirstFreeCtlStructure->mStart;
}

/**
 * @brief Frees given block if allocated
 *
 * @param[in] aHwId - Id of the Hw block
 * @param[in] aStreamId - Stream id
 * @param[in] apPtr - block to free
 */
static void Mshr_FreeBlock(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, void* apPtr)
{
    MShr_StreamCtr_t *Str = &gStreamCtr[aHwId][aCamId];
    bool Allfreed = true;
    for (int i=0; i<MAX_ALLOCATION_CNT_PER_STREAM; i++)
    {
        if (Str->AllocatedMems[i].mStart == apPtr)
        {
            memset(&Str->AllocatedMems[i], 0, sizeof(MShr_Allocation_t));
        }

        if (Str->AllocatedMems[i].mStart != 0 || Str->AllocatedMems[i].mEnd != 0)
        {
            Allfreed = false;
        }
    }

    if (Allfreed)
    {
        ResetAlloc(aHwId, aCamId);
    }
}

/**
 * @brief Extracts information about allocated block
 *
 * @param[in] aHwId - Id of the Hw block
 * @param[in] aCamId - Stream id
 * @param[in] apPtr - target block
 * @param[out] apOutHandle - global handle for the block (PHY address)
 * @param[out] apSize - size of the block
 *
 * @return True if success, false otherwise (out variables unmodified)
 */
static bool Mshr_GetGhandle(ispdrv_HwId_t aHwId, ispdrv_CamPipeId_t aCamId, void* apPtr, uint64_t* apOutHandle, uint64_t* apSize)
{
    MShr_StreamCtr_t *Str = &gStreamCtr[aHwId][aCamId];
    for (int i=0; i<MAX_ALLOCATION_CNT_PER_STREAM; i++)
    {
        if (Str->AllocatedMems[i].mStart == apPtr)
        {
            if (apOutHandle != NULL)
            {
                *apOutHandle = (uintptr_t)Str->AllocatedMems[i].mStart;
            }
            if (apSize != NULL)
            {
                *apSize = ((uintptr_t)Str->AllocatedMems[i].mEnd) - ((uintptr_t) Str->AllocatedMems[i].mStart) + 1;
            }
            return true;
        }
    }

    return false;
}

/*==============================================================================
*                              GLOBAL FUNCTIONS
==============================================================================*/

bool MShr_Init(MShr_Mode_t aMode)
{
    if (!gLockReady)
    {
        InitGlobalLock();
    }

    if (pdTRUE != xSemaphoreTake(gLock, portMAX_DELAY))
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "xSemaphoreTake fail\n");
        return false;
    }

    bool ret;
    if (gMode != MSHR_MODE_UNKNOWN && gMode != aMode)
    {
        // already initialized to different mode
        ret = false;
    }
    else if (gMode != MSHR_MODE_UNKNOWN && gMode == aMode)
    {
        // already initialized to the requested mode
        ret = true;
    }
    else if (aMode != MSHR_MODE_OWNER)
    {
        // only support owner mode
        ret = false;
    }
    else
    {
        // initialize variables
        gMode = aMode;
        if ( 0 != MShr_InitVariables())
        {
            ret = false;
        }
        else
        {
            ret = true;
        }
    }

    if (pdTRUE != xSemaphoreGive(gLock))
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "xSemaphoreGive fail\n");
        return false;
    }

    return ret;
}

/******************************************************************************/

void MShr_DeInit(void)
{
    if (pdTRUE != xSemaphoreTake(gLock, portMAX_DELAY))
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "xSemaphoreTake fail\n");
        return;
    }

    if (gMode != MSHR_MODE_OWNER)
    {
        // no mode active, nothing to deinit
    }
    else
    {
        for (int hwid = 0; hwid < ISP_HWID_CNT; hwid++)
        {
            for (uint8_t camid = 0; camid < ISPDRV_CAM_PIPE_ID_CNT; camid++)
            {
                ResetAlloc(hwid, camid);
            }
        }
        gMode = MSHR_MODE_UNKNOWN;
    }

    if (pdTRUE != xSemaphoreGive(gLock))
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "xSemaphoreGive fail\n");
        return;
    }
}

/******************************************************************************/

void* MShr_malloc(ispdrv_HwId_t aHwId, Mu_ChannelId_t aStreamId, size_t aSize, ssize_t aHandle)
{
    // this parameter is only needed in user mode, this implementation only supports owner mode
    (void)aHandle;

    if (aHwId >= ISP_HWID_CNT)
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "Invalid HwId\n");
        return NULL;
    }

    if (aStreamId == MU_CHANNEL_ID_MASTER || aStreamId > MU_CHANNEL_COUNT)
    {
        // Master channel is not supported on this platform (we try to be compatible on API but save the allocation that should never be used)
        return NULL;
    }
    ispdrv_CamPipeId_t CamId = (ispdrv_CamPipeId_t)(aStreamId - 1);

    if (pdTRUE != xSemaphoreTake(gStreamCtr[aHwId][CamId].mAccess, portMAX_DELAY))
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "xSemaphoreTake fail\n");
        return NULL;
    }

    void* ret = MShr_Allocate(aHwId, CamId, aSize);

    if (pdTRUE != xSemaphoreGive(gStreamCtr[aHwId][CamId].mAccess))
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "xSemaphoreGive fail\n");
        return NULL;
    }

    return ret;
}

/******************************************************************************/

void MShr_free(ispdrv_HwId_t aHwId, Mu_ChannelId_t aStreamId, void* apPtr)
{
    ILOG(ILOG_MOD_ISP, ILOG_LEV_NFO, "Free invoked %p.\n", apPtr);
    if (apPtr == NULL)
    {
        return;
    }

    if (aHwId >= ISP_HWID_CNT)
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "Invalid HwId\n");
        return;
    }

    if (aStreamId == MU_CHANNEL_ID_MASTER || aStreamId > MU_CHANNEL_COUNT)
    {
        // Master channel is not supported on this platform (we try to be compatible on API but save the allocation that should never be used)
        return;
    }
    ispdrv_CamPipeId_t CamId = (ispdrv_CamPipeId_t)(aStreamId - 1);

    if (pdTRUE != xSemaphoreTake(gStreamCtr[aHwId][CamId].mAccess, portMAX_DELAY))
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "xSemaphoreTake fail\n");
        return;
    }

    Mshr_FreeBlock(aHwId, CamId, apPtr);

    if (pdTRUE != xSemaphoreGive(gStreamCtr[aHwId][CamId].mAccess))
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "xSemaphoreGive fail\n");
        return;
    }
}

/******************************************************************************/

bool MShr_GetGlobalMemHandle(ispdrv_HwId_t aHwId, Mu_ChannelId_t aStreamId, void* apPtr, uint64_t* aOutHandle)
{
    if (apPtr == NULL)
    {
        return false;
    }

    if (aHwId >= ISP_HWID_CNT)
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "Invalid HwId\n");
        return NULL;
    }

    if (aStreamId == MU_CHANNEL_ID_MASTER || aStreamId > MU_CHANNEL_COUNT)
    {
        // Master channel is not supported on this platform (we try to be compatible on API but save the allocation that should never be used)
        return false;
    }
    ispdrv_CamPipeId_t CamId = (ispdrv_CamPipeId_t)(aStreamId - 1);

    if (pdTRUE != xSemaphoreTake(gStreamCtr[aHwId][CamId].mAccess, portMAX_DELAY))
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "xSemaphoreTake fail\n");
        return false;
    }

    int ret;

    ret = Mshr_GetGhandle(aHwId, CamId, apPtr, aOutHandle, NULL);

    if (pdTRUE != xSemaphoreGive(gStreamCtr[aHwId][CamId].mAccess))
    {
        ILOG(ILOG_MOD_GEN, ILOG_LEV_ERR, "xSemaphoreGive fail\n");
        return false;
    }

    return ret;
}

int MShr_Transfer2remote(ispdrv_HwId_t aHwId, Mu_ChannelId_t aStreamId, Mshr_BufDir_t aBufDir, void* apPtr, uint64_t* aOutHandle)
{
    // memory is setup as non-cacheable, non need to handle cache
    return 0;
}

int MShr_Transfer2local(ispdrv_HwId_t aHwId, Mu_ChannelId_t aStreamId, Mshr_BufDir_t aBufDir, void* apPtr)
{
    // memory is setup as non-cacheable, non need to handle cache
    return 0;
}

int Mshr_GetDmaBufLocation(int aFd, uint64_t* apPhyAddr)
{
    (void)aFd;
    (void)apPhyAddr;

    return -ISPDRV_RES_NOT_IMPLEMENTED;
}

void* Mshr_MapPhysAddrCached(Mshr_MmapReq_t *apMmapReq)
{
    (void)apMmapReq;

    return NULL;
}

int Mshr_UnmapPhysAddrCached(Mshr_MmapReq_t *apMmapReq)
{
    (void)apMmapReq;

    return -ISPDRV_RES_NOT_IMPLEMENTED;
}

