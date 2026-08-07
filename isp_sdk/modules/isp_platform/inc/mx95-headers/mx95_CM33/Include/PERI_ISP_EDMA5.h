/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*!
 * @file PERI_ISP_EDMA5.h
 * @version 1.0
 * @date 2025-08-11
 * @brief CMSIS Peripheral Access Layer for EDMA5
 *
 * CMSIS Peripheral Access Layer for EDMA5
 */

#if !defined(EDMA5_H_)
#define EDMA5_H_

/* ----------------------------------------------------------------------------
   -- Device Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Peripheral_access_layer Device Peripheral Access Layer
 * @{
 */


/*
** Start of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic push
  #else
    #pragma push
    #pragma anon_unions
  #endif
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

/* ----------------------------------------------------------------------------
   -- EDMA5 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup EDMA5_Peripheral_Access_Layer EDMA5 Peripheral Access Layer
 * @{
 */
/** DMA5 - Register Layout Typedef */
/*!
 * @brief Number of EDMA channels present on device.
 */

#define EDMA_CHANNEL_COUNT      (32U)
/*!
 * @brief EDMA Transfer Control Descriptor registers.
 */
typedef struct
{
    __IO uint32_t SADDR;                /**< Offset: 0x20, Source Address Register Low */
    __IO uint32_t SADDR_HIGH;           /**< Offset: 0x24, Source Address Register High */
    __IO uint16_t SOFF;                 /**< Offset: 0x28, Signed Source Address Offset Register */
    __IO uint16_t ATTR;                 /**< Offset: 0x2A, Transfer Attributes Register */
    union
    {
        __IO uint32_t NBYTES_MLOFF;     /**< Offset: 0x2C, */
        __IO uint32_t NBYTES_MLOFFNO;   /**< Offset: 0x2C, Transfer Size without Minor Loop Offsets Register */
        __IO uint32_t NBYTES_MLOFFYES;  /**< Offset: 0x2C, Transfer Size with Minor Loop Offsets Register */
    };
    __IO uint32_t SLAST_SDA;            /**< Offset: 0x30, Last Source Address Adjustment / Store DADDR Address Register Low*/
    __IO uint32_t SLAST_SDA_HIGH;       /**< Offset: 0x34, Last Source Address Adjustment / Store DADDR Address Register High */
    __IO uint32_t DADDR;                /**< Offset: 0x38, Destination Address Register Low */
    __IO uint32_t DADDR_HIGH;           /**< Offset: 0x3C, Destination Address Register High */
    __IO uint32_t DLAST_SGA;            /**< Offset: 0x40, Last Destination Address Adjustmen / Scatter Gather Address Register Low */
    __IO uint32_t DLAST_SGA_HIGH;       /**< Offset: 0x44, Last Destination Address Adjustmen / Scatter Gather Address Register High */
    __IO uint16_t DOFF;                 /**< Offset: 0x48, Signed Destination Address Offset Offset Register */
    union
    {
        __IO uint16_t CITER_ELINK;      /**< Offset: 0x4A, Current Major Loop Count */
        __IO uint16_t CITER_ELINKNO;    /**< Offset: 0x4A, Current Major Loop Count (Minor Loop Channel Linking Disabled) Register */
        __IO uint16_t CITER_ELINKYES;   /**< Offset: 0x4A, Current Major Loop Count (Minor Loop Channel Linking Enabled) Register */
    };
    __IO uint16_t TCSR;                 /**< Offset: 0x4C, Control and Status Register */
    union
    {
        __IO uint16_t BITER_ELINK;      /**< Offset: 0x4E, Beginning Major Loop Count */
        __IO uint16_t BITER_ELINKNO;    /**< Offset: 0x4E, Beginning Major Loop Count (Major Loop Channel Linking Disabled)*/
        __IO uint16_t BITER_ELINKYES;   /**< Offset: 0x4E, Beginning Major Loop Count (Major Loop Channel Linking Enabled) */
    };
    uint32_t reserved0[4U];             /* 64 byte alignment */
} EDMA_TCD_TypeDef;

/*!
 * @brief EDMA Management registers
 */
typedef struct
{
    __IO uint32_t CSR;                              /**< Offset: 0x00, Management Page Control Register */
    __I  uint32_t ES;                               /**< Offset: 0x04, Management Page Error Status Register */
    __I  uint32_t INT_LOW;                          /**< OFfset: 0x08, Management Page Interrupt Request Status Register */
    __I  uint32_t reserved0;                        /**< Offset: 0x0C, Reserved */
    __I  uint32_t HRS_LOW;                          /**< Offset: 0x10, Hardware Request Status Register */
    __I  uint32_t reserved1[3U];                    /**< Offset: 0x14, Reserved */
    __IO uint32_t STOPCH;                           /**< Offset: 0x20, Stop Channel Register */
    __I  uint32_t reserved2[3U];                    /**< Offset: 0x24, Reserved */
    __I  uint32_t SSR_LOW;                          /**< Offset: 0x30, Stop Status Register */
    __I  uint32_t reserved3[3U];                    /**< Offset: 0x34, Reserved */
    __IO uint32_t VPAS_LOW;                         /**< Offset: 0x40, Virtual/Physical Address Select Register */
    __IO uint32_t reserved4[47U];                   /**< Offset: 0x44, Reserved */
    __IO uint32_t CHn_GRPRI[EDMA_CHANNEL_COUNT];    /**< Offset: 0x100, Channel Arbitration Group Register */
    __IO uint32_t reserved5[160U];                  /**< Offset: 0x180, Reserved */
    __IO uint32_t CHn_PROT[EDMA_CHANNEL_COUNT];     /**< Offset: 0x400, Channel Protection Register */
    __I  uint32_t reserved6[16096U];                /**< Offset: 0x480, Reserved */
    struct {
        /* Channel control and status registers */
        __IO uint32_t CCSR;                         /**< Offset: 0x00, Channel Control and Status Register */
        __IO uint32_t ES;                           /**< Offset: 0x04, Error Status Register */
        __IO uint32_t INT;                          /**< Offset: 0x08, Channel Interrupt Status Register */
        __IO uint32_t SBR;                          /**< Offset: 0x0C, Channel System Bus Register */
        __IO uint32_t PRI;                          /**< Offset: 0x10, Channel Priority Register */
        __I uint32_t reserved0;                     /**< Offset: 0x14, Reserved */
        __IO uint32_t MATTR;                        /**< Offset: 0x18, Memory Attribute Register */
        __I uint32_t reserved1;                     /**< Offset: 0x1C, Reserved */
        EDMA_TCD_TypeDef TCD;
        __I uint32_t reserved[8168U];               /**< ; off: 0x60 - The TCD size has been increased to 64 byte, thus the
                                                            reserved space offset is shifted by 0x10 */
    } Channel[EDMA_CHANNEL_COUNT];

} EDMA_TypeDef;

/* ----------------------------------------------------------------------------
   -- DMA5 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup EDMA5_Register_Masks EDMA5 Register Masks
 * @{
 */
/**
 * @name EDMA Management Page Control Register (MP_CSR) masks
 * @brief The Management Page Control Register defines the basic operating configuration of the DMA.
 * @note For correct operation, changes to the MP_CSR[ ERCA, GCLC, GMRC] bits must be performed when
 * the DMA channels are inactive; that is, when MP_CSR[ACTIVE] bit is cleared.
 *
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td>ACTIVE</td> <td colspan="2">Reserved</td> <td colspan="5">ACTIVE_ID[28:24]</td> <td colspan="8">VER[23:16]</td>
    </tr>
    <tr>
        <td> r </td> <td colspan="2"> r </td> <td colspan="5"> r </td> <td colspan="8"> r </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="8">Reserved[15:8]</td>
        <td>GMRC</td> <td>GCLC</td> <td>HALT</td> <td>HAE</td> <td>GCPC</td> <td>ERCA</td> <td>EDGB</td> <td>Res.</td>
    </tr>
    </tr>
    <tr>
        <td colspan="8"> r </td> <td> rw </td> <td> rw </td> <td> rw </td> <td> rw </td> <td> rw </td> <td> rw </td> <td> rw </td> <td> r </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_CSR_ACTIVE_Pos             (31U)
#define EDMA_CSR_ACTIVE_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_CSR_ACTIVE_Pos)))
/**
 * DMA Active Status.
 * 0: eDMA is idle.\n
 * 1: eDMA is executing a channel.
 */
#define EDMA_CSR_ACTIVE                 EDMA_CSR_ACTIVE_Msk

#define EDMA_CSR_ACTIVE_ID_Pos          (24U)
#define EDMA_CSR_ACTIVE_ID_Msk          ((uint32_t) ((uint32_t) 0x1FU << (EDMA_CSR_ACTIVE_ID_Pos)))
/**
 * Active channel ID.
 */
#define EDMA_CSR_ACTIVE_ID              EDMA_CSR_ACTIVE_ID_Msk

#define EDMA_CSR_VER_Pos                (16U)
#define EDMA_CSR_VER_Msk                ((uint32_t) ((uint32_t) 0xFFU << (EDMA_CSR_VER_Pos)))
/**
 * eDMA version.
 */
#define EDMA_CSR_VER                    EDMA_CSR_VER_Msk

#define EDMA_CSR_GMRC_Pos               (7U)
#define EDMA_CSR_GMRC_Msk               ((uint32_t) ((uint32_t) 1U << (EDMA_CSR_GMRC_Pos)))
/**
 * Global Master ID Replication Control.
 * 0: Master ID replication is disabled for all channels.\n
 * 1: Master ID replication is available and is controlled by each channel's CHn_SBR[EMI] setting.
 *
 * @note If master ID replication is disabled, the privileged protection level
 * (supervisor mode) for DMA transfers is used.
 */
#define EDMA_CSR_GMRC(x)                (((uint32_t)(((uint32_t)(x)) << EDMA_CSR_GMRC_Pos)) & EDMA_CSR_GMRC_Msk)

#define EDMA_CSR_GCLC_Pos               (6U)
#define EDMA_CSR_GCLC_Msk               ((uint32_t) ((uint32_t) 1U << (EDMA_CSR_GCLC_Pos)))
/**
 * Global Channel Linking Control.
 * 0: Channel linking is disabled for all channels.\n
 * 1: Channel linking is available and controlled by each channel's link settings.
 */
#define EDMA_CSR_GCLC(x)                (((uint32_t)(((uint32_t) (x)) << EDMA_CSR_GCLC_Pos)) & EDMA_CSR_GCLC_Msk)

#define EDMA_CSR_HALT_Pos               (5U)
#define EDMA_CSR_HALT_Msk               ((uint32_t) ((uint32_t) 1U << (EDMA_CSR_HALT_Pos)))
/**
 * Halt DMA Operations.
 * 0: Normal operation\n
 * 1: Stall the start of any new channels. Executing channels are allowed to
 * complete. Channel execution resumes when this bit is cleared.
 */
#define EDMA_CSR_HALT(x)                (((uint32_t)(((uint32_t)(x)) << EDMA_CSR_HALT_Pos)) & EDMA_CSR_HALT_Msk)

#define EDMA_CSR_HAE_Pos                (4U)
#define EDMA_CSR_HAE_Msk                ((uint32_t) ((uint32_t) 1U << (EDMA_CSR_HAE_Pos)))
/**
 * Halt After Error.
 * 0: Normal operation\n
 * 1: Any error causes the HALT bit to set. Subsequently, all service requests
 * are ignored until the HALT bit is cleared.
 */
#define EDMA_CSR_HAE(x)                 (((uint32_t)(((uint32_t)(x)) << EDMA_CSR_HAE_Pos)) & EDMA_CSR_HAE_Msk)

#define EDMA_CSR_GCPC_Pos               (3U)
#define EDMA_CSR_GCPC_Msk               ((uint32_t) ((uint32_t) 1U << (EDMA_CSR_GCPC_Pos)))
/**
 * Global Channel Preemption Control.
 * 0: Preemption is disabled for all channels.\n
 * 1: Preemption is available.
 */
#define EDMA_CSR_GCPC(x)                (((uint32_t)(((uint32_t)(x)) << EDMA_CSR_GCPC_Pos)) & EDMA_CSR_GCPC_Msk)

#define EDMA_CSR_ERCA_Pos               (2U)
#define EDMA_CSR_ERCA_Msk               ((uint32_t) ((uint32_t) 1U << (EDMA_CSR_ERCA_Pos)))
/**
 * Enable Round Robin Channel Arbitration.
 * 0: Round robin channel arbitration is disabled. Fixed priority arbitration
 * is used for channel selection within each group.\n
 * 1: Round robin channel arbitration is enabled. Round robin arbitration is
 * used for channel selection within each group.
 */
#define EDMA_CSR_ERCA(x)                (((uint32_t)(((uint32_t)(x)) << EDMA_CSR_ERCA_Pos)) & EDMA_CSR_ERCA_Msk)

#define EDMA_CSR_EDBG_Pos               (1U)
#define EDMA_CSR_EDBG_Msk               ((uint32_t) ((uint32_t) 1U << (EDMA_CSR_EDBG_Pos)))
/**
 * Enable Debug.
 * 0: Debug mode is disabled. When in debug mode, the DMA continues to operate.\n
 * 1: Debug mode is enabled. When in debug mode, the DMA stalls the start of a
 * new channel. Executing channels are allowed to complete. Channel execution
 * resumes when the system exits debug mode or the EDBG bit is cleared.
 */
#define EDMA_CSR_EDBG(x)                (((uint32_t)(((uint32_t)(x)) << EDMA_CSR_EDBG_Pos)) & EDMA_CSR_EDBG_Msk)
/** @} */ /* MP_CSR definitions */

/**
 * @name EDMA Management Page Error Status Register (MP_ES) masks
 * @details The ES provides information concerning the last recorded channel error.
 * Channel errors can be caused by:\n
 * • An illegal setting in the transfer-control descriptor\n
 * • An error termination to a bus master read or write cycle\n
 * • An uncorrectable error occurred while the device was accessing the TCD SRAM\n
 * • A cancel transfer with error request was made via the corresponding cancel transfer bit or input signal\n
 * Upon any error condition, the software must initialize the errant channel's
 * TCD as it is in an incomplete state after an error.
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td>VALID</td> <td rowspan="2" colspan="2">Reserved</td> <td colspan="5">ERRCHN</td> <td colspan="8">0</td>
    </tr>
    <tr>
        <td> r </td> <td colspan="5"> r </td> <td colspan="8"> r </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="5"> 0 </td> <td rowspan="2">Res.</td> <td>UCE</td> <td>ECX</td>
        <td>SAE</td> <td>SOE</td> <td>DAE</td> <td>DOE</td> <td>NCE</td> <td>SGE</td> <td>SBE</td> <td>DBE</td>
    </tr>
    </tr>
    <tr>
        <td colspan="5"> r </td> <td> r </td> <td> r </td> <td> r </td>
        <td> r </td> <td> r </td> <td> r </td> <td> r </td> <td> r </td> <td> r </td> <td> r </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_ES_VLD_Pos                 (31U)                                               /**< Valid bit position */
#define EDMA_ES_VLD_Msk                 ((uint32_t) ((uint32_t) 1U << (EDMA_ES_VLD_Pos)))              /**< Valid bit mask */
/**
 * Valid - logical OR of all ERR status bits.
 * 0: No ERR bits are set.\n
 * 1: At least one ERR bit is set indicating a valid error exists that has not been cleared.
 */
#define EDMA_ES_VLD                     EDMA_ES_VLD_Msk

#define EDMA_ES_ERRCHN_Pos              (24U)                                               /**< Error channel number of canceled channel number bit position */
#define EDMA_ES_ERRCHN_Msk              ((uint32_t) ((uint32_t) 0x1FU << (EDMA_ES_ERRCHN_Pos)))        /**< Error channel number of canceled channel number bit mask */
/**
 * Error Channel Number or Canceled Channel Number.
 * The channel number of the last recorded error or last recorded error canceled transfer.
 */
#define EDMA_ES_ERRCHN                  EDMA_ES_ERRCHN_Msk

#define EDMA_ES_UCE_Pos                 (9U)                                                /**< Uncorrectable TCD error during channel execution bit position */
#define EDMA_ES_UCE_Msk                 ((uint32_t) ((uint32_t) 1U << (EDMA_ES_UCE_Pos)))              /**< Uncorrectable TCD error during channel execution bit mask */
/**
 * Uncorrectable TCD error during channel execution.
 * The UCE bit is set when an uncorrectable ECC error occurs on an access generated
 * by the DMA only. If a CPU access to the TCD causes an uncorrectable ECC error,
 * that access will receive a bus error response.\n
 * 0: No uncorrectable ECC error\n
 * 1: The last recorded error was an uncorrectable TCD RAM error
 *
 * @note When the eDMA receives a RAM error from a peripheral bus access (when the user is
accessing a TCD via the programmer's model), the error is reported as a bus abort. When
the eDMA engine (the execution engine is accessing a TCD) receives a RAM error, it is
recorded in the Error Status register, ES[UCE], along with the channel number.
 */
#define EDMA_ES_UCE                     EDMA_ES_UCE_Msk

#define EDMA_ES_ECX_Pos                 (8U)                                                /**< Transfer canceled bit position */
#define EDMA_ES_ECX_Msk                 ((uint32_t) ((uint32_t) 1U << (EDMA_ES_ECX_Pos)))              /**< Transfer canceled bit mask */
/**
 * Transfer Canceled.
 * A channel was stopped by a cancel/stop transfer command.\n
 * 0: No canceled transfers\n
 * 1: The last recorded entry was a canceled transfer by the error cancel transfer input.
 */
#define EDMA_ES_ECX                     EDMA_ES_ECX_Msk

#define EDMA_ES_SAE_Pos                 (7U)                                                /**< Source address error bit position */
#define EDMA_ES_SAE_Msk                 ((uint32_t) ((uint32_t) 1U << (EDMA_ES_SAE_Pos)))              /**< Source address error bit mask */
/**
 * Source Address Error.
 * 0: No source address configuration error.\n
 * 1: The last recorded error was a configuration error detected in the TCDn_SADDR
 * field. TCDn_SADDR is inconsistent with TCDn_ATTR[SSIZE].
 */
#define EDMA_ES_SAE                     EDMA_ES_SAE_Msk

#define EDMA_ES_SOE_Pos                 (6U)                                                /**< Source offset error bit position */
#define EDMA_ES_SOE_Msk                 ((uint32_t) ((uint32_t) 1U << (EDMA_ES_SOE_Pos)))              /**< Source offset error bit mask */
/**
 * Source Offset Error.
 * 0: No source offset configuration error\n
 * 1: The last recorded error was a configuration error detected in the TCDn_SOFF
 * field. TCDn_SOFF is inconsistent with TCDn_ATTR[SSIZE].
 */
#define EDMA_ES_SOE                     EDMA_ES_SOE_Msk

#define EDMA_ES_DAE_Pos                 (5U)                                                /**< Destination address error bit position */
#define EDMA_ES_DAE_Msk                 ((uint32_t) ((uint32_t) 1U << (EDMA_ES_DAE_Pos)))              /**< Destination address error bit mask */
/**
 * Destination Address Error.
 * 0b - No destination address configuration error\n
 * 1b - The last recorded error was a configuration error detected in the TCDn_DADDR
 * field. TCDn_DADDR is inconsistent with TCDn_ATTR[DSIZE].
 */
#define EDMA_ES_DAE                     EDMA_ES_DAE_Msk

#define EDMA_ES_DOE_Pos                 (4U)                                                /**< Destination offset error bit position */
#define EDMA_ES_DOE_Msk                 ((uint32_t) ((uint32_t) 1U << (EDMA_ES_DOE_Pos)))              /**< Destination offset error bit mask */
/**
 * Destination offset error.
 * 0: no destination offset configuration error\n
 * 1: the last recorded error was a configuration error detected in TCDn_DOFF
 * field. TCDn_DOFF is inconsistent with TCDn_ATTR[DSIZE].
 */
#define EDMA_ES_DOE                     EDMA_ES_DOE_Msk

#define EDMA_ES_NCE_Pos                 (3U)                                                /**< NBYTES/CITER configuration error bit position */
#define EDMA_ES_NCE_Msk                 ((uint32_t) ((uint32_t) 1U << (EDMA_ES_NCE_Pos)))              /**< NBYTES/CITER configuration error bit position */
/**
 * NBYTES/CITER configuration error.
 * 0: no NBYTES/CITER configuration error\n
 * 1: The last recorded error was NBYTES equal to zero or a CITER not equal to BITER
 * error.\n
 * The last recorded error was a configuration error detected in the TCDn_NBYTES
 * or TCDn_CITER fields. TCDn_NBYTES is not a multiple of TCDn_ATTR[SSIZE] and
 * TCDn_ATTR[DSIZE], or TCDn_CITER[CITER] is equal to zero, or TCDn_CITER[ELINK]
 * is not equal to TCDn_BITER[ELINK]
 */
#define EDMA_ES_NCE                     EDMA_ES_NCE_Msk

#define EDMA_ES_SGE_Pos                 (2U)                                                /**< Scatter/Gather configuration error bit position */
#define EDMA_ES_SGE_Msk                 ((uint32_t) ((uint32_t) 1U << (EDMA_ES_SGE_Pos)))              /**< Scatter/Gather configuration error bit mask */
/**
 * Scatter/Gather configuration error.
 * 0: No scatter/gather configuration error
 * 1: The last recorded error was a configuration error detected in the
 * TCDn_DLASTSGA field. This field is checked at the beginning of a scatter/gather
 * operation after major loop completion if TCDn_CSR[ESG] is enabled.
 * TCDn_DLASTSGA is not on a 32 byte boundary.
 */
#define EDMA_ES_SGE                     EDMA_ES_SGE_Msk

#define EDMA_ES_SBE_Pos                 (1U)                                                /**< Source bus error bit position */
#define EDMA_ES_SBE_Msk                 ((uint32_t) ((uint32_t) 1U << (EDMA_ES_SBE_Pos)))              /**< Source bus error bit mask */
/**
 * Source bus error.
 * 0: No source bus error\n
 * 1: The last recorded error was a bus error on a source read
 */
#define EDMA_ES_SBE                     EDMA_ES_SBE_Msk

#define EDMA_ES_DBE_Pos                 (0U)                                                /**< Destination bus error bit position */
#define EDMA_ES_DBE_Msk                 ((uint32_t) ((uint32_t) 1U << (EDMA_ES_DBE_Pos)))              /**< Destination bus error bit mask */
/**
 * Destination bus error.
 * 0: No destination bus error\n
 * 1: The last recorded error was a bus error on a destination write
 */
#define EDMA_ES_DBE                     EDMA_ES_DBE_Msk
/** @} */ /* EDMA ES register */

/**
 * @name EDMA Management Page Interrupt Request Status Register (MP_INT_LOW) masks
 * @details Shows the current state of the interrupt service request for all eDMA channels.
 * <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
        <tr><td colspan="16" style="text-align:center">MP_INT_LOW[31:16]</td>
    </tr>
    <tr>
        <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td>
        <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="16" style="text-align: center">MP_INT_LOW[15:0]</td>
    </tr>
    <tr>
        <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td>
        <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td> <td> R </td>
    </tr>
  </table>
 * @{
 */
/**
 * @brief Get Interrupt Request Status based on provided channel
 * @note channel in range of <0; 31>
 */
/* TODO define each channel separately?? */
#define EDMA_MP_INT_LOW_Msk(channel)    ((uint32_t) ((uint32_t) 1U << (channel)))
/** @} */ /* MP_INT_LOW definitions */

/**
 * @name EDMA Management Page Hardware Request Status Register (MP_HRS_LOW) masks
 * @details The hardware request status register (HRS) shows the current state
 * of the hardware service request signaling as seen by the eDMA's arbitration
 * logic.
 * @{
 */
/**
 * Hardware Request Status.
 * The hardware request status register (HRS) shows the current state of the
 * hardware service request signaling as seen by the eDMA's arbitration logic.
 * @note channel in range of <0; 31>
 */
/* TODO define each channel separately? */
#define EDMA_MP_HRS_LOW_Msk(channel)    ((uint32_t) ((uint32_t) 1U << (channel)))
/** @} */ /* MP_HRS_LOW definitions */

/**
 * @name EDMA Management Page Stop Channel Register (MP_STOPCH) masks
 * @details A write to this register will stop the target channel. If ERR is
 * one, the ECX stop transfer with error bit in the corresponding channel is
 * set. If ERR is zero, the CX stop transfer bit in the corresponding channel
 * is set.
 *
    <table>
        <tr>
            <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
            <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
        </tr>
        <tr>
            <td colspan="16"> Reserved[31:16] </td>
        </tr>
        <tr>
            <td colspan="16"> r </td>
        </tr>
        <tr>
            <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
            <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
        </tr>
        <tr>
            <td colspan="8"> Reserved[15:8] </td>
            <td> ERR </td> <td colspan="2"> Reserved </td> <td colspan="5"> STOPCH[4:1] </td>
        </tr>
        <tr>
            <td colspan="8"> r </td>
            <td> rw </td> <td colspan="2"> r </td> <td colspan="5"> rw </td>
        </tr>
    </table>
 * @{
 */
#define EDMA_MP_STOPCH_ERR_Pos          (7U)
#define EDMA_MP_STOPCH_ERR_Msk          ((uint32_t) ((uint32_t) 1U << (EDMA_MP_STOPCH_ERR_Pos)))
/**
 * Stop channel with error exit.
 * The channel may be stopped with a normal exit or an error exit. If error exit
 * is enabled, the ECX error status flag is set and the optional error interrupt
 * is generated.\n
 * 0: Stop channel without error reporting\n
 * 1: Stop channel with error reporting\n
 */
#define EDMA_MP_STOPCH_ERR(x)           (((uint32_t)(((uint32_t)(x)) << EDMA_MP_STOPCH_ERR_Pos)) & EDMA_MP_STOPCH_ERR_Msk)

#define EDMA_MP_STOPCH_STOPCH_Pos       (0U)
#define EDMA_MP_STOPCH_STOPCH_Msk       ((uint32_t) ((uint32_t) 1U << (EDMA_MP_STOPCH_STOPCH_Pos)))
/**
 * Stop Channel.
 * A write to this register will set corresponding channel's CHn_CSR[CX].
 * This action will stop the target channel once any transfers in progress
 * have finished.
 */
#define EDMA_MP_STOPCH_STOPCH(x)        (((uint32_t)(((uint32_t)(x)) << EDMA_MP_STOPCH_STOPCH_Pos)) & EDMA_MP_STOPCH_STOPCH_Msk)
/** @} */ /** EDMA Stop Channel Register */

/**
 * @name EDMA Management Page Stop Status Register (MP_SSR_LOW) masks
 * @details This register shows the stop status for the lower eDMA channel numbers.
 *
    <table>
        <tr>
            <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
            <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
        </tr>
        <tr>
            <td colspan="16"> CSS[31:16] </td>
        </tr>
        <tr>
            <td colspan="16"> r </td>
        </tr>
        <tr>
            <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
            <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
        </tr>
        <tr>
            <td colspan="16"> CSS[15:0] </td>
        </tr>
        <tr>
            <td colspan="16"> r </td>
        </tr>
    </table>
 * @{
 */
/**
 * Channel Stop Status bit mask.
 * The stop status bit for each respective channel shows if a stop request is pending.\n
 * 0: No channel stop request is pending.\n
 * 1: A channel stop request is pending.
 * @note channel in range of <0; 31>
 */
/*TODO define each channel separately?? */
#define EDMA_MP_SSR_LOW_CSS_Msk(channel)       ((uint32_t) ((uint32_t) 1U << (channel)))
/** @} */ /* MP_SSR_LOW register */

/**
 * @name EDMA Management Page Virtual/Physical Address Select Register (MP_VPAS_LOW) masks
 * @details This register defines if channel address translation is enabled for
 * the lower eDMA channel numbers.
 *
    <table>
        <tr>
            <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
            <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
        </tr>
        <tr>
            <td colspan="16"> EVA[31:16] </td>
        </tr>
        <tr>
            <td colspan="16"> rw </td>
        </tr>
        <tr>
            <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
            <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
        </tr>
        <tr>
            <td colspan="16"> EVA[15:0] </td>
        </tr>
        <tr>
            <td colspan="16"> rw </td>
        </tr>
    </table>
 * @{
 */
/**
 * Enable Virtual Addressing bit mask.
 * The enable virtual address bit for each respective channel is presented on
 * the system bus when the channel is active. If bit "n" is set, then channel
 * "n" is using virtual addressing. If bit "n" is cleared, then channel "n" is
 * using physical addressing.\n
 * 0: Physical addressing.\n
 * 1: Virtual addressing.
 */
/* TODO define for each channel?? */
#define EDMA_MP_VPAS_LOW_Msk(channel)       ((uint32_t) ((uint32_t) 1U << (channel)))
/** @} */ /* mp_vpas_low register */

/**
 * @name EDMA Management Page Channel Arbitration Group Register (CHn_GRPRI) masks
 * @details The contents of this register defines the arbitration group associated
 * with each channel. Using fixed-priority group arbitration scheme, the arbitration
 * group priorities are evaluated by numeric value from highest group number to lowest;
 * for example, 0 is the lowest priority, 1 is the next higher priority, then 2, 3, etc.
 * The range of the group priority values is limited to the values of 0 through 31.
 * Within each arbitration group, the channel priority, CH n _PRI, assignment determines
 * the highest priority channel.
 *
    <table>
        <tr>
            <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
            <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
        </tr>
        <tr>
            <td colspan="16"> 0[31:16] </td>
        </tr>
        <tr>
            <td colspan="16"> r </td>
        </tr>
        <tr>
            <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
            <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
        </tr>
        <tr>
            <td colspan="10"> 0[15:0] </td>
            <td colspan="6"> GRPRI </td>
        </tr>
        <tr>
            <td colspan="10"> r </td>
            <td colspan="6"> rw </td>
        </tr>
    </table>
 * @{
 */
#define EDMA_MP_CHn_GRPRI_GRPRI_Pos             (0U)
#define EDMA_MP_CHn_GRPRI_GRPRI_Msk             ((uint32_t) ((uint32_t) 0x3FU << EDMA_MP_CHn_GRPRI_GRPRI_Pos))
/**
 * Arbitration group per channel.
 * Fixed-priority arbitration group number.
 */
#define EDMA_MP_CHn_GRPRI_GRPRI(x)              (((uint32_t)(((uint32_t) (x)) << EDMA_MP_CHn_GRPRI_GRPRI_Pos)) & EDMA_MP_CHn_GRPRI_GRPRI_Msk)
/** @} */ /* ch_grpri bit definitions */

/**
 * @name EDMA Management Page Channel Protection Register (CHn_PROT) masks
 * @details The contents of this register defines the protection level presented to the system bus.
 *
    <table>
        <tr>
            <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
            <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
        </tr>
        <tr>
            <td colspan="15"> 0 [31:17] </td> <td> EMI </td>
        </tr>
        <tr>
            <td colspan="15"> r </td> <td> rw </td>
        </tr>
        <tr>
            <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
            <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
        </tr>
        <tr>
            <td> PAL </td> <td> SEC </td> <td> INSTR </td> <td colspan="8"> 0 [12:5] </td>
            <td colspan="5"> MID </td>
        </tr>
        <tr>
            <td> rw </td> <td> rw </td> <td> rw </td> <td colspan="8"> r </td>
            <td colspan="5"> rw </td>
        </tr>
    </table>
 * @{
 */
#define EDMA_MP_CHn_PROT_EMI_Pos        (16U)
#define EDMA_MP_CHn_PROT_EMI_Msk        ((uint32_t) ((uint32_t) 1U << EDMA_MP_CHn_PROT_EMI_Pos))
/**
 * Enable master ID replication.
 * The eDMA master ID replication register allows the eDMA to use the same protection
 * level and system bus ID of the master programming the eDMA's TCD. When enabled,
 * the eDMA uses the master ID and protection level stored in the SBR registers
 * instead of the eDMA's default values. When a master, for example a core,
 * programs a CH n GRPRI, its master ID is captured when that register is written.
 * The EMI bit may be written only if Management Page CSR[GMRC] = 1, Global
 * Master ID Replication Control is enabled; otherwise, the EMI will be forced
 * to zero.\n
 * 0: Master ID replication is disabled\n
 * 1: Master ID replication is enabled
 *
 * @note If master ID replication is disabled, the non secure, user protection
 * level for DMA transfers is used.
 */
#define EDMA_MP_CHn_PROT_EMI(x)         (((uint32_t)(((uint32_t) (x)) << EDMA_MP_CHn_PROT_EMI_Pos)) & EDMA_MP_CHn_PROT_EMI_Msk)

#define EDMA_MP_CHn_PROT_PAL_Pos        (15U)
#define EDMA_MP_CHn_PROT_PAL_Msk        ((uint32_t) ((uint32_t) 1U << EDMA_MP_CHn_PROT_PAL_Pos))
/**
 * Privileged Access Level.
 * DMA's protection level on the system bus when the channel is active.\n
 * 0: User protection level for DMA transfers\n
 * 1: Privileged protection level for DMA transfers
 *
 * @note The value written into this register cannot exceed the security and privilege
 * level of the core or other master writing this register. The order of precedence
 * is SecurePriv>SecureUser>NonsecurePriv>NonsecureUser.
 */
#define EDMA_MP_CHn_PROT_PAL(x)         (((uint32_t)(((uint32_t) (x)) << EDMA_MP_CHn_PROT_PAL_Pos)) & EDMA_MP_CHn_PROT_PAL_Msk)

#define EDMA_MP_CHn_PROT_SEC_Pos        (14U)
#define EDMA_MP_CHn_PROT_SEC_Msk        ((uint32_t) ((uint32_t) 1U << EDMA_MP_CHn_PROT_SEC_Pos))
/**
 * Security Level.
 * DMA's security level on the system bus when the channel is active.\n
 * 0: Non secure protection level for DMA transfers\n
 * 1: Secure protection level for DMA transfers\n
 *
 * @note The value written into this register cannot exceed the security and
 * privilege level of the core or other master writing this register. The order
 * of precedence is SecurePriv > SecureUser > NonsecurePriv > NonsecureUser.
 */
#define EMDA_MP_CHn_PROT_SEC(x)         (((uint32_t)(((uint32_t) (x)) << EDMA_MP_CHn_PROT_SEC_Pos)) & EDMA_MP_CHn_PROT_SEC_Msk)

#define EDMA_MP_CHn_PROT_INSTR_Pos      (13U)
#define EDMA_MP_CHn_PROT_INSTR_Msk      ((uint32_t) ((uint32_t) 1U << EDMA_MP_CHn_PROT_INSTR_Pos))
/**
 * Instruction/Data Access.
 * DMA's transactions type on the system bus when the channel is active.\n
 * 0: Data access for DMA transfers\n
 * 1: Instruction access for DMA transfers\n
 */
#define EDMA_MP_CHn_PROT_INSTR(x)       (((uint32_t)(((uint32_t) (x)) << EDMA_MP_CHn_PROT_INSTR_Pos)) & EDMA_MP_CHn_PROT_INSTR_Msk)

#define EDMA_MP_CHn_PROT_MID_Pos        (0U)
#define EDMA_MP_CHn_PROT_MID_Msk        ((uint32_t) ((uint32_t) 1U << EDMA_MP_CHn_PROT_MID_Pos))
/**
 * Master ID.
 * DMA's master ID on the system bus when the channel is active.
 *
 * @note When master ID replication is enabled, the ID captured in this register
 * reflects the master ID of the core or other master writing the channel's control
 * attributes, lower byte of TCD n _CSR.
 */
#define EDMA_MP_CHn_PROT_MID(x)         (((uint32_t)(((uint32_t) (x)) << EDMA_MP_CHn_PROT_MID_Msk)) & EDMA_MP_CHn_PROT_MID_Msk)
/** @} */ /* CHn_PROT register */

/**
 * @name EDMA Channel Control and Status Register (CHn_CSR) masks
 * @details The DMA hardware request input signal and the enable request bit (ERQ)
 * must be asserted before a channel's hardware service request is accepted.
 * The state of the eDMA enable request bit does not affect a channel service
 * request made explicitly through software or channel linking. The state of the
 * ERQ bit does not affect the channel's START bit. The enable asynchronous DMA
 * request bit (EARQ) does not affect DMA operations. When set, this bit allows
 * the hardware service request enable bit (ERQ) to propagate out of the DMA to
 * the power controller. When cleared, this bit masks the hardware service request
 * enable bit to the power controller.  The EEI bit enables the error interrupt
 * signal for the channel. The DMA error indicator and the error interrupt enable
 * flag must be asserted before an error interrupt request for a given channel
 * is asserted to the interrupt controller.  The DONE bit indicates the channel
 * has completed the major loop. If enabled, the eDMA will generate an interrupt
 * request corresponding to this completed channel.\n
 * The ACTIVE bit indicates the channel was selected by arbitration and is executing
 * the prescribed transfers. Except for dynamic channel linking, the Transfer
 * Control Descriptor should not be modified while a channel is active.
    <table>
        <tr>
            <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
            <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
        </tr>
        <tr>
            <td> ACTIVE </td> <td> DONE </td> <td colspan="8"> Reserved[29:22] </td> <td colspan="6"> SIGNEXT </td>
        </tr>
        <tr>
            <td> r </td> <td> w1c </td> <td colspan="8"> r </td> <td colspan="6"> rw </td>
        </tr>
        <tr>
            <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
            <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
        </tr>
        <tr>
            <td colspan="4"> SWAP[15:12] </td> <td colspan="4"> Reserved[11:8] </td>
            <td> ECX </td> <td> CX </td> <td colspan="3"> Reserved[5:3] </td> <td> EEI </td> <td> EARQ </td> <td> ERQ </td>
        </tr>
        <tr>
            <td colspan="4"> rw </td> <td colspan="4"> r </td>
            <td> rw </td> <td> rw </td> <td colspan="3"> r </td> <td> rw </td> <td> rw </td> <td> rw </td>
        </tr>
    </table>
 * @{
 */
#define EDMA_CHn_CSR_ACTIVE_Pos             (31U)
#define EDMA_CHn_CSR_ACTIVE_Msk             ((uint32_t) ((uint32_t) 1U << EDMA_CHn_CSR_ACTIVE_Pos))
/**
 * Channel Active.
 * This flag indicates the channel is currently in execution. It is set when
 * channel service begins, and is cleared by the eDMA as the minor loop completes
 * or when any error condition is detected.
 */
#define EDMA_CHn_CSR_ACTIVE                 EDMA_CHn_CSR_ACTIVE_Msk

#define EDMA_CHn_CSR_DONE_Pos               (30U)
#define EDMA_CHn_CSR_DONE_Msk               ((uint32_t) ((uint32_t) 1U << EDMA_CHn_CSR_DONE_Pos))
/**
 * Channel Done.
 * This flag indicates the eDMA has completed the major loop. The eDMA engine
 * sets this bit as the CITER count reaches zero. The software clears it, or the
 * hardware when the channel is activated.
 *
 * @note This bit must be cleared to write a 1 to the MAJORELINK bit.
 */
#define EDMA_CHn_CSR_DONE(x)                (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_CSR_DONE_Pos)) & EDMA_CHn_CSR_DONE_Msk)

#define EDMA_CHn_CSR_SIGNEXT_Pos            (16U)
#define EDMA_CHn_CSR_SIGNEXT_Msk            ((uint32_t) ((uint32_t) 0x3FU << EDMA_CHn_CSR_SIGNEXT_Pos))
/**
 * Sign Extension.
 * The data written on each transaction will be sign extended at the bit specified
 * in this field. If 0, sign extension is disabled.\n
 * 0: disabled
 * A non-zero value specifying the sign extend bit position
 */
#define EDMA_CHn_CSR_SIGNEXT(x)             (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_CSR_SIGNEXT_Pos)) & EDMA_CHn_CSR_SIGNEXT_Msk)

#define EDMA_CHn_CSR_SWAP_Pos               (12U)
#define EDMA_CHn_CSR_SWAP_Msk               ((uint32_t) ((uint32_t) 0xFU << EDMA_CHn_CSR_SWAP_Pos))
/**
 * Swap size.
 * Swap data elements within the specified transfer. Swap occurs with respect to
 * the specified transfer size (SSIZE or DSIZE).\n
 * 0x0: disabled
 * 0x1: read with 8-bit swap
 * 0x2: read with 16-bit swap
 * 0x3: read with 32-bit swap
 * 0x9: write with 8-bit swap
 * 0xA: write with 16-bit swap
 * 0xB: write with 32-bit swap
 */
#define EDMA_CHn_CSR_SWAP(x)                (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_CSR_SWAP_Pos)) & EDMA_CHn_CSR_SWAP_Msk)

#define EDMA_CHn_CSR_ECX_Pos                (7U)
#define EDMA_CHn_CSR_ECX_Msk                ((uint32_t) ((uint32_t) 1U << EDMA_CHn_CSR_ECX_Pos))
/**
 * Stop transferring data and set an error flag.
 * For an active channel, stop any new transactions. Any transactions that have
 * already been issued are allowied to complete. If the channel is inactive, the
 * stop reset will negate a subsequent service request. After stopping the channel,
 * the ECX error status bit will be set and an optional error interrupt generated.
 * This register bit is automatically cleared after the stop request is honored.
 * Once set, it cannot be cleared by the programmer's model.\n
 * 0: No operation\n
 * 1: Stop the channel with error reporting
 */
#define EDMA_CHn_CSR_ECX(x)                 (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_CSR_ECX_Pos)) & EDMA_CHn_CSR_ECX_Msk)

#define EDMA_CHn_CSR_CX_Pos                 (6U)
#define EDMA_CHn_CSR_CX_Msk                 ((uint32_t) ((uint32_t) 1U << EDMA_CHn_CSR_CX_Pos))
/**
 * Stop transferring data.
 * For an active channel, stop any new transactions. Any transactions that have
 * already been issued are allow to complete. If the channel is inactive, the
 * stop reset will negate a subsequent service request. This register bit is
 * automatically cleared after the stop request is honored. Once set, it cannot
 * be cleared by the programmer's model.\n
 * 0: No operation\n
 * 1: Stop the channel, no error reporting
 */
#define EDMA_CHn_CSR_CX(x)                  (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_CSR_CX_Pos)) & EDMA_CHn_CSR_CX_Msk)

#define EDMA_CHn_CSR_EEI_Pos                (2U)
#define EDMA_CHn_CSR_EEI_Msk                ((uint32_t) ((uint32_t) 1U << EDMA_CHn_CSR_EEI_Pos))
/**
 * Enable Error Interrupt.
 * 0: The error signal for corresponding channel does not generate an error interrupt\n
 * 1: The assertion of the error signal for corresponding channel generates an error interrupt
 */
#define EDMA_CHn_CSR_EEI(x)                 (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_CSR_EEI_Pos)) & EDMA_CHn_CSR_EEI_Msk)

#define EDMA_CHn_CSR_EARQ_Pos               (1U)
#define EDMA_CHn_CSR_EARQ_Msk               ((uint32_t) ((uint32_t) 1U << EDMA_CHn_CSR_EARQ_Pos))
/**
 * Enable Asynchronous DMA Request in stop mode for channel.
 * 0: Disable asynchronous DMA request for the channel.\n
 * 1: Enable asynchronous DMA request for the channel.
 */
#define EDMA_CHn_CSR_EARQ(x)                (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_CSR_EARQ_Pos)) & EDMA_CHn_CSR_EARQ_Msk)

#define EDMA_CHn_CSR_ERQ_Pos                (0U)
#define EDMA_CHn_CSR_ERQ_Msk                ((uint32_t) ((uint32_t) 1U << EDMA_CHn_CSR_ERQ_Pos))
/**
 * Enable DMA Request.
 * Disable a channel's hardware service request at the source before clearing
 * the channel's ERQ bit.\n
 * 0: The DMA hardware request signal for the corresponding channel is disabled.\n
 * 1: The DMA hardware request signal for the corresponding channel is enabled.
 */
#define EDMA_CHn_CSR_ERQ(x)                 (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_CSR_ERQ_Pos)) & EDMA_CHn_CSR_ERQ_Msk)
/** @} */ /* chn_csr register */

/**
 * @name Channel Error Status Register (CHn_ES) masks
 *
 * @details The ES provides information concerning the last recorded channel error.
 * Channel errors can be caused by:\n
 * • An illegal setting in the transfer-control descriptor,\n
 * • An error termination to a bus master read or write cycle\n
 * The ERR bit signals the presence of an error for the channel. The eDMA engine
 * signals the occurrence of an error condition by setting the appropriate bit
 * in this register. The outputs of this register are enabled by the contents of
 * the EEI, then logically summed across all channels to form an error interrupt
 * request, which may be routed to the interrupt controller. During the execution
 * of the interrupt-service routine associated with any DMA errors, it is software's
 * responsibility to clear the appropriate bit, negating the error-interrupt request.
 * The normal DMA channel completion indicators (setting the transfer control
 * descriptor DONE flag and the possible assertion of an interrupt request) are
 * not affected when an error is detected. The contents of this ERR register bit
 * can also be polled because a non-zero value indicates the presence of a channel
 * error regardless of the state of the EEI mask. The state of any given channel's
 * error indicators is affected by writes to this register. On writes to the ERR
 * bit, a one clears the channel's error status. A zero has no affect on the
 * channel's current error status.\n
 * An unspecified error, where only the ERR bit is set, indicates a transfer was
 * cancelled with an error or an uncorrectable TCD error occurred . The Management
 * Page Error Status register has full view of the error condition.
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td>ERR</td> <td colspan="15">Reserved</td>
    </tr>
    <tr>
        <td> w1c </td> <td colspan="15"> r </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="6"> Reserved </td> <td>UCE</td> <td>ECX</td>
        <td>SAE</td> <td>SOE</td> <td>DAE</td> <td>DOE</td> <td>NCE</td> <td>SGE</td> <td>SBE</td> <td>DBE</td>
    </tr>
    </tr>
    <tr>
        <td colspan="6"> r </td> <td> r </td> <td> r </td>
        <td> r </td> <td> r </td> <td> r </td> <td> r </td> <td> r </td> <td> r </td> <td> r </td> <td> r </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_CHn_ES_ERR_Pos             (31U)
#define EDMA_CHn_ES_ERR_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_ES_VLD_Pos)))
/**
 * Error In Channel.
 * 0: An error in this channel has not occurred.\n
 * 1: An error in this channel has occurred.
 */
#define EDMA_CHn_ES_ERR(x)              (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_ES_ERR_Pos)) & EDMA_CHn_ES_ERR_Msk)

#define EDMA_CHn_ES_UCE_Pos             (9U)
#define EDMA_CHn_ES_UCE_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_ES_UCE_Pos)))
/**
 * Uncorrectable TCD error during channel execution.
 * The UCE bit is set when an uncorrectable ECC error occurs on an access generated
 * by the DMA only. If a CPU access to the TCD causes an uncorrectable ECC error,
 * that access will receive a bus error response.\n
 * 0: No uncorrectable ECC error\n
 * 1: The last recorded error was an uncorrectable TCD RAM error
 *
 * @note When the eDMA receives a RAM error from a peripheral bus access (when the user is
accessing a TCD via the programmer's model), the error is reported as a bus abort. When
the eDMA engine (the execution engine is accessing a TCD) receives a RAM error, it is
recorded in the Error Status register, ES[UCE], along with the channel number.
 */
#define EDMA_CHn_ES_UCE                 EDMA_CHn_ES_UCE_Msk

#define EDMA_CHn_ES_ECX_Pos             (8U)
#define EDMA_CHn_ES_ECX_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_ES_ECX_Pos)))
/**
 * Transfer Canceled.
 * A channel was stopped by a cancel/stop transfer command.\n
 * 0: No canceled transfers\n
 * 1: The last recorded entry was a canceled transfer by the error cancel transfer input.
 */
#define EDMA_CHn_ES_ECX                 EDMA_CHn_ES_ECX_Msk

#define EDMA_CHn_ES_SAE_Pos             (7U)
#define EDMA_CHn_ES_SAE_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_ES_SAE_Pos)))
/**
 * Source Address Error.
 * 0: No source address configuration error.\n
 * 1: The last recorded error was a configuration error detected in the TCDn_SADDR
 * field. TCDn_SADDR is inconsistent with TCDn_ATTR[SSIZE].
 */
#define EDMA_CHn_ES_SAE                 EDMA_CHn_ES_SAE_Msk

#define EDMA_CHn_ES_SOE_Pos             (6U)
#define EDMA_CHn_ES_SOE_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_ES_SOE_Pos)))
/**
 * Source Offset Error.
 * 0: No source offset configuration error\n
 * 1: The last recorded error was a configuration error detected in the TCDn_SOFF
 * field. TCDn_SOFF is inconsistent with TCDn_ATTR[SSIZE].
 */
#define EDMA_CHn_ES_SOE                 EDMA_CHn_ES_SOE_Msk

#define EDMA_CHn_ES_DAE_Pos             (5U)
#define EDMA_CHn_ES_DAE_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_ES_DAE_Pos)))
/**
 * Destination Address Error.
 * 0b - No destination address configuration error\n
 * 1b - The last recorded error was a configuration error detected in the TCDn_DADDR
 * field. TCDn_DADDR is inconsistent with TCDn_ATTR[DSIZE].
 */
#define EDMA_CHn_ES_DAE                 EDMA_CHn_ES_DAE_Msk

#define EDMA_CHn_ES_DOE_Pos             (4U)
#define EDMA_CHn_ES_DOE_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_ES_DOE_Pos)))
/**
 * Destination offset error.
 * 0: no destination offset configuration error\n
 * 1: the last recorded error was a configuration error detected in TCDn_DOFF
 * field. TCDn_DOFF is inconsistent with TCDn_ATTR[DSIZE].
 */
#define EDMA_CHn_ES_DOE                 EDMA_CHn_ES_DOE_Msk

#define EDMA_CHn_ES_NCE_Pos             (3U)
#define EDMA_CHn_ES_NCE_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_ES_NCE_Pos)))
/**
 * NBYTES/CITER configuration error.
 * 0: no NBYTES/CITER configuration error\n
 * 1: The last recorded error was NBYTES equal to zero or a CITER not equal to BITER
 * error.\n
 * The last recorded error was a configuration error detected in the TCDn_NBYTES
 * or TCDn_CITER fields. TCDn_NBYTES is not a multiple of TCDn_ATTR[SSIZE] and
 * TCDn_ATTR[DSIZE], or TCDn_CITER[CITER] is equal to zero, or TCDn_CITER[ELINK]
 * is not equal to TCDn_BITER[ELINK]
 */
#define EDMA_CHn_ES_NCE                 EDMA_CHn_ES_NCE_Msk

#define EDMA_CHn_ES_SGE_Pos             (2U)
#define EDMA_CHn_ES_SGE_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_ES_SGE_Pos)))
/**
 * Scatter/Gather configuration error.
 * 0: No scatter/gather configuration error
 * 1: The last recorded error was a configuration error detected in the
 * TCDn_DLASTSGA field. This field is checked at the beginning of a scatter/gather
 * operation after major loop completion if TCDn_CSR[ESG] is enabled.
 * TCDn_DLASTSGA is not on a 32 byte boundary.
 */
#define EDMA_CHn_ES_SGE                 EDMA_CHn_ES_SGE_Msk

#define EDMA_CHn_ES_SBE_Pos             (1U)
#define EDMA_CHn_ES_SBE_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_ES_SBE_Pos)))
/**
 * Source bus error.
 * 0: No source bus error\n
 * 1: The last recorded error was a bus error on a source read
 */
#define EDMA_CHn_ES_SBE                 EDMA_CHn_ES_SBE_Msk

#define EDMA_CHn_ES_DBE_Pos             (0U)
#define EDMA_CHn_ES_DBE_Msk             ((uint32_t) ((uint32_t) 1U << (EDMA_ES_DBE_Pos)))
/**
 * Destination bus error.
 * 0: No destination bus error\n
 * 1: The last recorded error was a bus error on a destination write
 */
#define EDMA_CHn_ES_DBE                 EDMA_CHn_ES_DBE_Msk
/** @} */ /* CHn_ES register */

/**
 * @name Channel Interrupt Status Register (CHn_INT) masks
 *
 * @details The INT register bit signals the presence of an interrupt request
 * for the channel. Depending on the appropriate bit setting in the transfer
 * control descriptors, the eDMA engine generates an interrupt on data transfer
 * completion . The outputs of this register are directly routed to the interrupt
 * controller. During the interrupt service routine associated with any given
 * channel, it is the software's responsibility to clear the appropriate bit,
 * negating the interrupt request. On writes to INT, a 1 clears the channel's
 * interrupt request.  A zero has no affect on the channel's current interrupt
 * status.
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td colspan="16">Reserved</td>
    </tr>
    <tr>
        <td colspan="16"> r </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="15"> Reserved </td> <td>INT</td>
    </tr>
    </tr>
    <tr>
        <td colspan="15"> r </td> <td> w1c </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_CHn_INT_INT_Pos                (0U)
#define EDMA_CHn_INT_INT_Msk                ((uint32_t) ((uint32_t) 1U << EDMA_CHn_INT_INT_Pos))
/**
 * Interrupt Request.
 * 0: The interrupt request for corresponding channel is cleared.\n
 * 1: The interrupt request for corresponding channel is active.
 */
#define EDMA_CHn_INT_INT(x)                 (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_INT_INT_Pos)) & EDMA_CHn_INT_INT_Msk)
/** @} */ /* chn_int register */

/**
 * @name Channel System Bus Register (CHn_SBR) masks
 *
 * @details The System Bus Register places identification and protection
 * information on the system bus interface for the eDMA.\n
 * The ATTR register outputs the registers values onto the system bus interface
 * for further decode by the security system.
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td colspan="11">Reserved[31:21]</td> <td colspan="4"> ATTR[20:17] </td> <td> EMI </td>
    </tr>
    <tr>
        <td colspan="11"> r </td> <td colspan="4"> rw </td> <td> r </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td>PAL</td> <td>SEC</td> <td>INSTR</td> <td colspan="8"> Reserved[12:5] </td> <td colspan="5">MID[4:0]</td>
    </tr>
    </tr>
    <tr>
        <td> r </td> <td> r </td> <td> r </td> <td colspan="8"> r </td> <td colspan="5"> r </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_CHn_SBR_ATTR_Pos               (17U)
#define EDMA_CHn_SBR_ATTR_Msk               ((uint32_t) ((uint32_t) 0xFU << EDMA_CHn_SBR_ATTR_Pos))
/**
 * Attribute Output.
 * DMA's system bus attribute output value.
 */
#define EDMA_CHn_SBR_ATTR(x)                (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_SBR_ATTR_Pos)) & EDMA_CHn_SBR_ATTR_Msk)

#define EDMA_CHn_SBR_EMI_Pos                (16U)
#define EDMA_CHn_SBR_EMI_Msk                ((uint32_t) ((uint32_t) 1U << EDMA_CHn_SBR_EMI_Pos))
/**
 * Enable Master ID replication.
 * The eDMA master ID replication register allows the eDMA to use the same
 * protection level and system bus ID of the master programming the eDMA's TCD.
 * When enabled, the eDMA uses the master ID and protection level stored in the
 * SBR registers instead of the eDMA's default values. When a master, for example
 * a core, programs a TCD, its master ID is captured when the TCDn_CSR control
 * is written. The protection and security levels are captured when the TCDn_CSR
 * control attributes are written. A scatter/gather operation does not affect the
 * CHn_SBR registers. The EMI bit may be written only if Management Page
 * CSR[GMRC] = 1, Global Master ID Replication Control is enabled; otherwise,
 * the EMI will be forced to zero.\n
 * 0: Master ID replication is disabled\n
 * 1: Master ID replication is enabled
 *
 * @note If master ID replication is disabled, the non secure, privileged
 * protection level for DMA transfers is used.
 */
#define EDMA_CHn_SBR_EMI                        EDMA_CHn_SBR_EMI_Msk

#define EDMA_CHn_SBR_PAL_Pos                (15U)
#define EDMA_CHn_SBR_PAL_Msk                ((uint32_t) ((uint32_t) 1U << EDMA_CHn_SBR_PAL_Pos))
/**
 * Privileged Access Level.
 * DMA's protection level on the system bus when the channel is active.\n
 * 0: User protection level for DMA transfers\n
 * 1: Privileged protection level for DMA transfers
 *
 * @note When master ID replication is enabled, the protection level captured in
 * this register reflects the protection level of the core or other master
 * writing the channel's control attributes, lower byte of TCDn_CSR.
 */
#define EDMA_CHn_SBR_PAL                        EDMA_CHn_SBR_PAL_Msk

#define EDMA_CHn_SBR_SEC_Pos                (14U)
#define EDMA_CHn_SBR_SEC_Msk                ((uint32_t) ((uint32_t) 1U << EDMA_CHn_SBR_SEC_Pos))
/**
 * Security Level.
 * DMA's security level on the system bus when the channel is active.\n
 * 0: Non secure protection level for DMA transfers\n
 * 1: Secure protection level for DMA transfers
 *
 * @note When master ID replication is enabled, the security level captured in
 * this register reflects the security level of the core or other master writing
 * the channel's control attributes, lower byte of TCDn_CSR.
 */
#define EDMA_CHn_SBR_SEC                        EDMA_CHn_SBR_SEC_Msk

#define EDMA_CHn_SBR_INSTR_Pos              (13U)
#define EDMA_CHn_SBR_INSTR_Msk              ((uint32_t) ((uint32_t) 1U << EDMA_CHn_SBR_INSTR_Pos))
/**
 * Instruction/Data Access.
 * DMA's transactions type on the system bus when the channel is active.\n
 * 0: Data access for DMA transfers
 * 1: Instruction access for DMA transfers
 */
#define EDMA_CHn_SBR_INSTR                      EDMA_CHn_SBR_INSTR_Msk

#define EDMA_CHn_SBR_MID_Pos                (0U)
#define EDMA_CHn_SBR_MID_Msk                ((uint32_t) ((uint32_t) 1U << EDMA_CHn_SBR_MID_Pos))
/**
 * Master ID.
 * DMA's master ID on the system bus when the channel is active.
 *
 * @note When master ID replication is enabled, the ID captured in this register
 * reflects the master ID of the core or other master writing the channel's control
 * attributes, lower byte of TCDn_CSR.
 */
#define EDMA_CHn_SBR_MID(x)                 (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_SBR_MID_Pos)) & EDMA_CHn_SBR_MID_Msk)
/** @} */ /* chn_sbr register */

/**
 * @name Channel Priority Register (CHn_PRI) masks
 *
 * @details The contents of these registers define unique priorities associated
 * with each channel within the same channel group. Channel grouping is programmed
 * via the group priority registers MP_GRPRI. The channel priorities within a group
 * are evaluated by numeric value; for example, 0 is the lowest priority, 1 is
 * the next higher priority, then 2, 3, etc. Software must program the channel
 * priorities with unique values; otherwise, channel numbers with the same,
 * non-zero value, will be selected based on channel number with the higher
 * channel number having higher priority. If more than one channel in a group
 * has an Arbitration Priority Level value of zero, then the arbitration mode
 * bit MP_CSR[ERCA] is used to determine the arbitration scheme for all channels
 * with APL=0 within a group. When round-robin channel arbitration is enabled
 * (MP_CSR[ERCA] = 1), all channels with APL=0 within a group will use a
 * round-robin arbitration scheme which will rotate among these channels requesting
 * service without regard to priority. Round-robin provides a fairness mechanism
 * within an arbitration group. When fixed-priority channel arbitration is enabled
 * (MP_CSR[ERCA] = 0), channels with APL=0 will be selected based on channel
 * number with the higher channel number having higher priority.
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td> ECP </td> <td> DPA </td> <td colspan="14">Reserved[29:16]</td>
    </tr>
    <tr>
        <td> rw </td> <td> rw </td> <td colspan="14"> r </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="13"> Reserved[15:3] </td> <td>APL[2:0]</td>
    </tr>
    </tr>
    <tr>
        <td colspan="13"> r </td> <td> rw </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_CHn_PRI_ECP_Pos                (31U)
#define EDMA_CHn_PRI_ECP_Msk                ((uint32_t) ((uint32_t) 1U << EDMA_CHn_PRI_ECP_Pos))
/**
 * Enable Channel Preemption.
 * 0: The channel cannot be suspended by a higher priority channel's service request.\n
 * 1: The channel can be temporarily suspended by the service request of a higher priority channel.
 */
#define EDMA_CHn_PRI_ECP(x)                 (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_PRI_ECP_Pos)) & EDMA_CHn_PRI_ECP_Msk)

#define EDMA_CHn_PRI_DPA_Pos                (30U)
#define EDMA_CHn_PRI_DPA_Msk                ((uint32_t) ((uint32_t) 1U << EDMA_CHn_PRI_DPA_Pos))
/**
 * Disable Preempt Ability.
 * 0: The channel can suspend a lower priority channel.\n
 * 1: The channel cannot suspend any other channel, regardless of channel priority.
 */
#define EDMA_CHn_PRI_DPA(x)                 (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_PRI_DPA_Pos)) & EDMA_CHn_PRI_DPA_Msk)

#define EDMA_CHn_PRI_APL_Pos                (0U)
#define EDMA_CHn_PRI_APL_Msk                ((uint32_t) ((uint32_t) 0x7U << EDMA_CHn_PRI_APL_Pos))
/**
 * Arbitration Priority Level.
 * Channel priority level for arbitration within the assigned arbitration group.
 */
#define EDMA_CHn_PRI_APL(x)                 (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_PRI_APL_Pos)) & EDMA_CHn_PRI_APL_Msk)
/** @} */ /* chn_pri register */

/**
 * @name Channel Memory Attributes Register (CHn_MATTR) masks
 *
 * @details The Memory Attributes Register defines a set of transaction attributes
 * for memory and peripherals. The cache attributes specify how a transaction
 * progresses through the system and how they are handled. Cache stashing is the
 * ability of the DMA to bring in (or stash) a cache line to a specific cache in
 * the core cluster.
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td colspan="16">Reserved[31:16]</td>
    </tr>
    <tr>
        <td colspan="16"> r </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="8"> Reserved[15:8] </td> <td colspan="4">WCACHE</td> <td colspan="4">RCACHE</td>
    </tr>
    </tr>
    <tr>
        <td colspan="8"> r </td> <td colspan="4"> rw </td> <td colspan="4"> rw </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_CHn_MATTR_WCACHE_Pos               (4U)
#define EDMA_CHn_MATTR_WCACHE_Msk               ((uint32_t) ((uint32_t) 0xFU << EDMA_CHn_MATTR_WCACHE_Pos))
/**
 * Write Cache Attributes.
 * This field defines the attributes associated with a write transaction.\n
 * WCACHE[7] 0: no write allocate, 1: write-allocate\n
 * WCACHE[6] 0: no read allocate, 1: read-allocate\n
 * WCACHE[5] 0: non-cacheable, 1: cacheable\n
 * WCACHE[4] 0: non-bufferable, 1: bufferable\n
 */
#define EDMA_CHn_MATTR_WCACHE(x)                (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_MATTR_WCACHE_Pos)) & EDMA_CHn_MATTR_WCACHE_Msk)

#define EDMA_CHn_MATTR_RCACHE_Pos               (0U)
#define EDMA_CHn_MATTR_RCACHE_Msk               ((uint32_t) ((uint32_t) 0xFU << EDMA_CHn_MATTR_RCACHE_Pos))
/**
 * Read Cache Attributes.
 * This field defines the attributes associated with a read transaction.\n
 * RCACHE[3] 0: no write allocate, 1: write-allocate\n
 * RCACHE[2] 0: no read allocate, 1: read-allocate\n
 * RCACHE[1] 0: non-cacheable, 1: cacheable\n
 * RCACHE[0] 0: non-bufferable, 1: bufferable\n
 */
#define EDMA_CHn_MATTR_RCACHE(x)                (((uint32_t)(((uint32_t) (x)) << EDMA_CHn_MATTR_RCACHE_Pos)) & EDMA_CHn_MATTR_RCACHE_Msk)
/** @} */ /* chn_mattr register */

/**
 * @name TCD Source Address Register (TCDn_SADDR) masks
 *
 * @details Memory address bits[31:0] pointing to the source data.
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td colspan="16">SADDR[31:16]</td>
    </tr>
    <tr>
        <td colspan="16">rw</td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="16">SADDR[15:0]</td>
    </tr>
    </tr>
    <tr>
        <td colspan="16"> rw </td>
    </tr>
  </table>
 * @{
 */
/**
 * Source Address bits[31:0].
 * Memory address pointing to the source data.
 */
#define EDMA_TCDn_SADDR_SADDR_Pos               (0U)
#define EDMA_TCDn_SADDR_SADDR_Msk               (0xFFFFFFFFU)
/** @} */ /* tcdn_saddr register */

/**
 * @name TCD Source Address High Register (TCDn_SADDR_HIGH) masks
 *
 * @details Memory address bits[63:32] pointing to the source data.
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td colspan="16">Reserved[31:16]</td>
    </tr>
    <tr>
        <td colspan="16"> r </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="4"> Reserved[15:12] </td> <td colspan="12">SADDR[11:0]</td>
    </tr>
    </tr>
    <tr>
        <td colspan="4"> r </td> <td colspan="12"> rw </td>
    </tr>
  </table>
 * @{
 */
/**
 * Source Address bits [43:32].
 * Memory address pointing to the source data.
 */
#define EDMA_TCDn_SADDR_HIGH_Pos                (0U)
#define EDMA_TCDn_SADDR_HIGH_Msk                ((uint32_t) ((uint32_t) 0xFFFU << EDMA_TCDn_SADDR_HIGH_Pos))
/** @} */ /* chn_int register */

/**
 * @name TCD Signed Source Address Offset Register (TCDn_SOFF) masks
 *
 * @details
  <table>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="16"> SOFF[15:0] </td>
    </tr>
    </tr>
    <tr>
        <td colspan="16"> rw </td>
    </tr>
  </table>
 * @{
 */
/**
 * Source address signed offset.
 * Sign-extended offset applied to the current source address to form the
 * next-state value as each source read is completed.
 */
#define EDMA_TCDn_SOFF_SOFF_Pos                 (0U)
#define EDMA_TCDn_SOFF_SOFF_Msk                 ((uint16_t)((uint16_t) 0xFFFFU << EDMA_TCDn_SOFF_SOFF_Pos))
/** @} */ /* tcdn_soff register */

/**
 * @name TCD Transfer Attributes Register (TCDn_ATTR) masks
 *
  <table>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="5"> SMOD </td> <td colspan="3"> SSIZE </td> <td colspan="5"> DMOD </td> <td colspan="3"> DSIZE </td>
    </tr>
    </tr>
    <tr>
        <td colspan="5"> rw </td> <td colspan="3"> rw </td> <td colspan="5"> DMOD </td> <td colspan="3"> DSIZE </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_ATTR_SMOD_Pos                 (11U)
#define EDMA_TCDn_ATTR_SMOD_Msk                 ((uint16_t) ((uint16_t) 0x1FU << EDMA_TCDn_ATTR_SMOD_Pos))
/**
 * Source address modulo.
 * This field defines a specific address range specified to be the value after
 * SADDR + SOFF calculation is performed on the original register value. Setting
 * this field provides the ability to implement a circular data queue easily.
 * For data queues requiring power-of-2 size bytes, the queue should start at a
 * 0-modulo-size address and the SMOD field should be set to the appropriate
 * value for the queue, freezing the desired number of upper address bits. The
 * value programmed into this field specifies the number of lower address bits
 * allowed to change. For a circular queue application, the SOFF is typically
 * set to the transfer size to implement post-increment addressing with the SMOD
 * function constraining the addresses to a 0-modulo-size range.\n
 * 0: Source address modulo feature is disabled\n
 * non-zero: Source address modulo feature is enabled for any non-zero value [1-31]
 */
#define EDMA_TCDn_ATTR_SMOD(x)                  (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_ATTR_SMOD_Pos)) & EDMA_TCDn_ATTR_SMOD_Msk)

#define EDMA_TCDn_ATTR_SSIZE_Pos                (8U)
#define EDMA_TCDn_ATTR_SSIZE_Msk                ((uint16_t) ((uint16_t) 0x7U << EDMA_TCDn_ATTR_SSIZE_Pos))
/**
 * Source data transfer size.
 * This field specifies the size of each read transaction on the system bus.\n
 * 0x0: 8-bit\n
 * 0x1: 16-bit\n
 * 0x2: 32-bit\n
 * 0x3: 64-bit\n
 * 0x4: 16-byte\n
 * 0x5: 32-byte\n
 * 0x6: 64-byte\n
 * 0x7: 128-byte\n
 */
#define EDMA_TCDn_ATTR_SSIZE(x)                 (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_ATTR_SSIZE_Pos)) & EDMA_TCDn_ATTR_SSIZE_Msk)

#define EDMA_TCDn_ATTR_DMOD_Pos                 (3U)
#define EDMA_TCDn_ATTR_DMOD_Msk                 ((uint16_t) ((uint16_t) 0x1FU << EDMA_TCDn_ATTR_DMOD_Pos))
/**
 * Destination address modulo.
 * This field defines a specific address range specified to be the value after
 * DADDR + DOFF calculation is performed on the original register value. Setting
 * this field provides the ability to implement a circular data queue easily.
 * For data queues requiring power-of-2 size bytes, the queue should start at a
 * 0-modulo-size address and the SMOD field should be set to the appropriate
 * value for the queue, freezing the desirednumber of upper address bits. The
 * value programmed into this field specifies the number of lower address bits
 * allowed to change. For a circular queue application, the SOFF is typically
 * set to the transfer size to implement post-increment addressing with the SMOD
 * function constraining the addresses to a 0-modulo-size range.\n
 * 0: Source address modulo feature is disabled\n
 * non-zero: Source address modulo feature is enabled for any non-zero value [1-31]
 */
#define EDMA_TCDn_ATTR_DMOD(x)                  (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_ATTR_DMOD_Pos)) & EDMA_TCDn_ATTR_DMOD_Msk)

#define EDMA_TCDn_ATTR_DSIZE_Pos                (0U)
#define EDMA_TCDn_ATTR_DSIZE_Msk                ((uint16_t) ((uint16_t) 0x7U << EDMA_TCDn_ATTR_DSIZE_Pos))
/**
 * Destination data transfer size.
 * This field specifies the size of each write transaction on the system bus.\n
 * 0x0: 8-bit\n
 * 0x1: 16-bit\n
 * 0x2: 32-bit\n
 * 0x3: 64-bit\n
 * 0x4: 16-byte\n
 * 0x5: 32-byte\n
 * 0x6: 64-byte\n
 * 0x7: 128-byte\n
 */
#define EDMA_TCDn_ATTR_DSIZE(x)                 (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_ATTR_DSIZE_Pos)) & EDMA_TCDn_ATTR_DSIZE_Msk)
/** @} */ /* tcdn_attr register */

/**
 * @name TCD Transfer Size with/without Minor Loop Offsets Register (TCDn_NBYTES_MLOFFNO/TCDn_NBYTES_MLOFFYES) masks
 *
 * @details The TCDn_NBYTES field defines the number of bytes to transfer per
 * service request.\n
 * Minor loop offsets are address offset values added to the final source address
 * (TCDn_SADDR) or destination address (TCDn_DADDR) upon minor loop completion.
 * Minor loop completion is when the channel has finished the service request and
 * has transferred NBYTES. When minor loop offsets are enabled, the minor loop
 * offset value (MLOFF) is added to the final source address (TCDn_SADDR),
 * the final destination address (TCDn_DADDR), or to both prior to the addresses
 * being written back into TCD. If the major loop is complete, the minor loop
 * offset is ignored and the major loop address offsets (TCDn_SLAST_SDA and
 * TCDn_DLAST_SGA) are used to compute the next TCDn_SADDR and TCDn_DADDR values.\n
 *
 * The register has a twofold meaning based on SMLOE[31] / DMLOE[30] bits. If
 * both bits are cleared, bits [29:0] belong to NBYTES. If SMLOE[31] or
 * DMLOE[30] is set, bits [29:10] belong to MLOFF and bits [9:0] belong to NBYTES.\n
  <table>
    <caption>TCDn_MLOFFYES</caption>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td>SMLOE</td> <td>DMLOE</td><td colspan="14">MLOFF[29:16]</td>
    </tr>
    <tr>
        <td>rw</td><td>rw</td><td colspan="14">rw</td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="6"> MLOFF[15:10] </td> <td colspan="10">NBYTES[9:0]</td>
    </tr>
    </tr>
    <tr>
        <td colspan="6"> rw </td> <td colspan="10"> rw </td>
    </tr>
  </table>
  <table>
    <caption>TCDn_MLOFFNO</caption>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td>SMLOE(0)</td> <td>DMLOE(0)</td><td colspan="14">NBYTES[29:16]</td>
    </tr>
    <tr>
        <td>rw</td><td>rw</td><td colspan="14">rw</td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="16"> NBYTES[15:0] </td>
    </tr>
    </tr>
    <tr>
        <td colspan="16"> rw </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_NBYTES_SMLOE_Pos              (31U)
#define EDMA_TCDn_NBYTES_SMLOE_Msk              ((uint32_t) ((uint32_t) 1U << EDMA_TCDn_NBYTES_SMLOE_Pos))
/**
 * Source Minor Loop Offset Enable.
 * Selects whether the minor loop offset is applied to the source address upon minor loop completion.\n
 * 0: The minor loop offset is not applied to the SADDR\n
 * 1: The minor loop offset is applied to the SADDR
 */
#define EDMA_TCDn_NBYTES_SMLOE(x)               (((uint32_t)(((uint32_t) (x)) << EDMA_TCDn_NBYTES_SMLOE_Pos)) & EDMA_TCDn_NBYTES_SMLOE_Msk)

#define EDMA_TCDn_NBYTES_DMLOE_Pos              (30U)
#define EDMA_TCDn_NBYTES_DMLOE_Msk              ((uint32_t) ((uint32_t) 1U << EDMA_TCDn_NBYTES_DMLOE_Pos))
/**
 * Destination Minor Loop Offset Enable.
 * Selects whether the minor loop offset is applied to the destination address upon minor loop completion.\n
 * 0: The minor loop offset is not applied to the DADDR\n
 * 1: The minor loop offset is applied to the DADDR
 */
#define EDMA_TCDn_NBYTES_DMLOE(x)               (((uint32_t)(((uint32_t) (x)) << EDMA_TCDn_NBYTES_DMLOE_Pos)) & EDMA_TCDn_NBYTES_DMLOE_Msk)

#define EDMA_TCDn_NBYTES_MLOFF_Pos              (10U)
#define EDMA_TCDn_NBYTES_MLOFF_Msk              ((uint32_t) ((uint32_t) 0xFFU << EDMA_TCDn_NBYTES_MLOFF_Pos))
/**
 * Minor Loop Offset.
 * If SMLOE or DMLOE is set, this field represents a sign-extended offset applied to the source or
 * destination address to form the next-state value after the minor loop completes.
 *
 * @note The MLOFF bits are valid only in case if SMLOE or DMLOE is set to 1.
 */
#define EDMA_TCDn_NBYTES_MLOFF(x)               (((uint32_t)(((uint32_t) (x)) << EDMA_TCDn_NBYTES_MLOFF_Pos)) & EDMA_TCDn_NBYTES_MLOFF_Msk)

#define EDMA_TCDn_NBYTES_NBYTES_Pos             (0U)
#define EDMA_TCDn_NBYTES_MLOFFNO_NBYTES_Msk     ((uint32_t) ((uint32_t) 0x3FFFFFFFU << EDMA_TCDn_NBYTES_NBYTES_Pos))
#define EDMA_TCDn_NBYTES_MLOFFYES_NBYTES_Msk    ((uint32_t) ((uint32_t) 0x3FFU << EDMA_TCDn_NBYTES_NBYTES_Pos))
/**
 * Number of Bytes to transfer per service request.
 * Number of bytes to be transferred in each service request of the channel.\n
 * As a channel activates, the appropriate TCD content loads into the eDMA
 * engine, and the appropriate reads and writes perform until the minor byte
 * transfer count has transferred. This is an indivisible operation and cannot
 * be halted. It can, however, be stalled by using the bandwidth control field,
 * or via preemption. After the minor count is exhausted, the SADDR and DADDR
 * values are written back into the TCD memory, the major iteration count is
 * decremented and restored in the TCD memory. If the major iteration count is
 * completed, additional processing is performed.
 *
 * @note Use this macro if SMLOE = 0 & DMLOE = 0.
 */
#define EDMA_TCDn_NBYTES_MLOFFNO_NBYTES(x)      (((uint32_t)(((uint32_t) (x)) << EDMA_TCDn_NBYTES_NBYTES_Pos)) & EDMA_TCDn_NBYTES_MLOFFNO_NBYTES_Msk)
/**
 * Number of Bytes to transfer per service request.
 * Number of bytes to be transferred in each service request of the channel.\n
 * As a channel activates, the appropriate TCD content loads into the eDMA
 * engine, and the appropriate reads and writes perform until the minor byte
 * transfer count has transferred. This is an indivisible operation and cannot
 * be halted. It can, however, be stalled by using the bandwidth control field,
 * or via preemption. After the minor count is exhausted, the SADDR and DADDR
 * values are written back into the TCD memory, the major iteration count is
 * decremented and restored in the TCD memory. If the major iteration count is
 * completed, additional processing is performed.
 *
 * @note Use this macro if SMLOE = 1, DMLOE = 1 or both are set to 1.
 */
#define EDMA_TCDn_NBYTES_MLOFFYES_NBYTES(x)     (((uint32_t)(((uint32_t) (x)) << EDMA_TCDn_NBYTES_NBYTES_Pos)) & EDMA_TCDn_NBYTES_MLOFFYES_NBYTES_Msk)
/** @} */ /* tcdn_mloffyes/no register */

/**
 * @name TCD Last Source Address Adjustment / Store DADDR Address Register (TCDn_SLAST_SDA) masks
 *
 * @details Final source address offset or system memory location to store final DADDR; address bits[31:0].
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td colspan="16">SLAST_SDA[31:16]</td>
    </tr>
    <tr>
        <td colspan="16"> rw </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="16"> SLAST_SDA[15:0] </td>
    </tr>
    </tr>
    <tr>
        <td colspan="16"> rw </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_SLAST_SDA_SLAST_SDA_Pos       (0U)
#define EDMA_TCDn_SLAST_SDA_SLAST_SDA_Msk       (0xFFFFFFFFU)
/**
 * Last Source Address Adjustment / Store DADDR Address bits[31:0].
 * Source last address adjustment or the system memory address for destination
 * address (DADDR) storage.\n
 * If TCDn_CSR[ESDA] = 0, then:\n
 * • Adjustment value is added to the source address at the completion of the
 * major iteration count. This value can be used to restore the source address
 * to the initial value or adjust the address to reference the next data
 * structure.\n
 * • This field uses two's complement notation for the final source address
 * adjustment.\n
 * Otherwise:\n
 * • This address points to the 32-bit-aligned memory location where the
 * destination address (DADDR) is to be stored in system memory. By saving the
 * final destination address in system memory via the ESDA feature, you are able
 * to compute the size of a variable destination data buffer by simply subtracting
 * the beginning DADDR from the final, saved DADDR. This feature is used together
 * with the scatter/gather operation to prevent the loss of the final DADDR,
 * which is overwritten during the scatter/gather operation.\n
 * The "Store Destination Address" (SDA) value must be a 32-bit-aligned location
 * because the eDMA forces the lower two address bits of the SLAST_SDA field to
 * zero when ESDA is enabled. The module performs this write operation when the
 * major loop is done; that is, when the major iteration count (CITER) decrements
 * to zero.
 */
#define EDMA_TCDn_SLAST_SDA_SLAST_SDA(x)        ((uint32_t) (x))
/** @} */ /* tcdn_slast_sda register */

/**
 * @name TCD Last Source Address Adjustment / Store DADDR Address High Register (TCDn_SLAST_SDA_HIGH) masks
 *
 * @details Final source address offset or system memory location to store final DADDR; bits[43:32].
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td colspan="16">Reserved</td>
    </tr>
    <tr>
        <td colspan="16"> r </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="4"> Reserved </td> <td colspan="12">SLAST_SDA</td>
    </tr>
    </tr>
    <tr>
        <td colspan="4"> r </td> <td colspan="12"> rw </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_SLAST_SDA_HIGH_SLAST_SDA_Pos      (0U)
#define EDMA_TCDn_SLAST_SDA_HIGH_SLAST_SDA_Msk      ((uint32_t) ((uint32_t) 0xFFFFFFU << EDMA_TCDn_SLAST_SDA_HIGH_SLAST_SDA_Pos))
/**
 * Last Source Address Adjustment / Store DADDR Address bits[43:32].
 * Upper address bits for last source address adjustment or the system memory
 * address for destination address (DADDR) storage overlay.
 */
#define EDMA_TCDn_SLAST_SDA_HIGH_SLAST_SDA(x)       (((uint32_t)(((uint32_t) (x)) << EDMA_TCDn_SLAST_SDA_HIGH_SLAST_SDA_Pos)) & EDMA_TCDn_SLAST_SDA_HIGH_SLAST_SDA_Msk)
/** @} */ /* tcdn_slast_sda_high register */

/**
 * @name TCD Destination Address Register (TCDn_DADDR) masks
 *
 * @details Memory address bits[31:0] pointing to the destination of the data.
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td colspan="16">DADDR</td>
    </tr>
    <tr>
        <td colspan="16"> rw </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="16">DADDR</td>
    </tr>
    </tr>
    <tr>
        <td colspan="16"> rw </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_DADDR_DADDR_Pos               (0U)
#define EDMA_TCDn_DADDR_DADDR_Msk               (0xFFFFFFFFU)
/**
 * Destination Address.
 * Memory address pointing to the destination data.
 */
#define EDMA_TCDn_DADDR_DADDR(x)                ((uint32_t) (x))
/** @} */ /* tcdn_daddr register */

/**
 * @name TCD Destination Address High Register (TCDn_DADDR_HIGH) masks
 *
 * @details Memory address bits[43:32] pointing to the destination of the data.
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td colspan="16">RESERVED[31:16]</td>
    </tr>
    <tr>
        <td colspan="16"> r </td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="4">RESERVED[15:12]</td><td colspan="12">DADDR</td>
    </tr>
    </tr>
    <tr>
        <td colspan="4">r</td> <td colspan="12"> rw </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_DADDR_HIGH_DADDR_Pos          (0U)
#define EDMA_TCDn_DADDR_HIGH_DADDR_Msk          ((uint32_t)((uint32_t) 0xFFFU << EDMA_TCDn_DADDR_HIGH_DADDR_Pos))
/**
 * Destination Address.
 * Memory address pointing to the destination data.
 */
#define EDMA_TCDn_DADDR_HIGH_DADDR(x)           (((uint32_t)(((uint32_t) (x)) << EDMA_TCDn_DADDR_HIGH_DADDR_Pos)) & EDMA_TCDn_DADDR_HIGH_DADDR_Msk)
/** @} */ /* tcdn_daddr register */

/**
 * @name TCD Last Destination Address Adjustment / Scatter Gather Address Register (TCDn_DLAST_SGA) masks
 *
 * @details Final destination address offset or system memory location for scatter/gather fetch; address bits[31:0].
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td colspan="16">DLAST_SGA</td>
    </tr>
    <tr>
        <td colspan="16">rw</td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="16">DLAST_SGA</td>
    </tr>
    </tr>
    <tr>
        <td colspan="16">rw</td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_DLAST_SGA_DLAST_SGA_Pos           (0U)
#define EDMA_TCDn_DLAST_SGA_DLAST_SGA_Msk           ((uint32_t) 0xFFFFFFFFU)
/**
 * Final Destination Address Adjustment / Scatter Gather Address.
 * Destination final address adjustment or the memory address for the next
 * transfer control descriptor to be loaded into this channel (Scatter/Gather).\n
 * If (TCDn_CSR[ESG] = 0), then:\n
 * • Adjustment value is added to the destination address at the completion of
 * the major iteration count. This value can be used to restore the destination
 * address to the initial value or adjust the address to reference the next data
 * structure.\n
 * • This field uses two's complement notation for the final destination address
 * adjustment.\n
 * Otherwise:\n
 * • This address points to the beginning of a 0-modulo-32-byte region containing
 * the next transfer control descriptor to be loaded into this channel. This
 * channel reload is performed as the major iteration count completes. The
 * scatter/gather address must be 0-modulo-32-byte, otherwise a configuration
 * error is reported.
 */
#define EDMA_TCDn_DLAST_SGA_DLAST_SGA(x)            (((uint32_t)(((uint32_t) (x)) << EDMA_TCDn_DLAST_SGA_DLAST_SGA_Pos)) & EDMA_TCDn_DLAST_SGA_DLAST_SGA_Msk)
/** @} */ /* tcdn_dlast_sga register */

/**
 * @name TCD Last Destination Address Adjustment / Scatter Gather Address Register (TCDn_DLAST_SGA_HIGH) masks
 *
 * @details Final destination address offset or system memory location for scatter/gather fetch; bits[43:32].
  <table>
    <tr>
        <th> 31 </th> <th> 30 </th> <th> 29 </th> <th> 28 </th> <th> 27 </th> <th> 26 </th> <th> 25 </th> <th> 24 </th>
        <th> 23 </th> <th> 22 </th> <th> 21 </th> <th> 20 </th> <th> 19 </th> <th> 18 </th> <th> 17 </th> <th> 16 </th>
    </tr>
    <tr>
        <td colspan="16">RESERVED[31:16]</td>
    </tr>
    <tr>
        <td colspan="16">r</td>
    </tr>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="4">RESERVED[15:12]</td><td colspan="12">DLAST_SGA[11:0]</td>
    </tr>
    </tr>
    <tr>
        <td colspan="4">r</td> <td colspan="12">rw</td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_DLAST_SGA_HIGH_DLAST_SGA_Pos          (0U)
#define EDMA_TCDn_DLAST_SGA_HIGH_DLAST_SGA_Msk          ((uint32_t) ((uint32_t) 0xFFFFFFU << EDMA_TCDn_DLAST_SGA_HIGH_DLAST_SGA_Pos))
/**
 * Final Destination Address Adjustment / Scatter Gather Address.
 * Destination final address adjustment or the memory address for the next
 * transfer control descriptor to be loaded into this channel (Scatter/Gather).\n
 * If (TCDn_CSR[ESG] = 0), then:\n
 * • Adjustment value is added to the destination address at the completion of
 * the major iteration count. This value can be used to restore the destination
 * address to the initial value or adjust the address to reference the next data
 * structure.\n
 * • This field uses two's complement notation for the final destination address
 * adjustment.\n
 * Otherwise:\n
 * • This address points to the beginning of a 0-modulo-32-byte region containing
 * the next transfer control descriptor to be loaded into this channel. This
 * channel reload is performed as the major iteration count completes. The
 * scatter/gather address must be 0-modulo-32-byte, otherwise a configuration
 * error is reported.
 */
#define EDMA_TCDn_DLAST_SGA_HIGH_DLAST_SGA(x)           (((uint32_t)(((uint32_t) (x)) << EDMA_TCDn_DLAST_SGA_DLAST_SGA_Pos)) & EDMA_TCDn_DLAST_SGA_HIGH_DLAST_SGA_Msk)
/** @} */ /* tcdn_dlast_sga_high register */

/**
 * @name TCD Signed Destination Address Offset Register (TCDn_DOFF) masks
 *
 * @details
  <table>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="16">DOFF[15:0]</td>
    </tr>
    <tr>
        <td colspan="16"> rw </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_DOFF_DOFF_Pos                     (0U)
#define EDMA_TCDn_DOFF_DOFF_Msk                     ((uint16_t) ((uint32_t) 1U << EDMA_TCDn_DOFF_Pos))
/**
 * Destination Address Signed Offset.
 * Sign-extended offset applied to the current destination address to form the
 * next-state value as each destination write is completed.
 */
#define EDMA_TCDn_DOFF_DOFF(x)                      (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_DOFF_Pos)) & EDMA_TCDn_DOFF_Msk)
/** @} */ /* tcdn_daddr register */

/**
 * @name TCD Current Major Loop Count (TCDn_CITER_ELINKNO/TCDn_CITER_ELINKYES) masks
 *
 * @details
  <table>
    <caption>TCDn_CITER_ELINKNO (TCDn_CITER[ELINK] = 0)</caption>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td> ELINK </td> <td colspan="15">CITER[14:0]</td>
    </tr>
    </tr>
    <tr>
        <td> rw </td> <td colspan="15"> rw </td>
    </tr>
  </table>
  <table>
    <caption>TCDn_CITER_ELINKYES (TCDn_CITER[ELINK] = 1)</caption>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td> ELINK </td> <td> RES. </td> <td colspan="5"> ELINKCH </td> <td colspan="9"> CITER </td>
    </tr>
    </tr>
    <tr>
        <td> rw </td> <td> r </td> <td colspan="5"> rw </td> <td colspan="9"> rw </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_CITER_ELINK_Pos               (15U)
#define EDMA_TCDn_CITER_ELINK_Msk               ((uint16_t) ((uint32_t) 1U << EDMA_TCDn_CITER_ELINK_Pos))
/**
 * Enable channel-to-channel linking on minor-loop complete.
 * As the channel completes the minor loop, this flag enables linking to another
 * channel, defined by the LINKCH field. When enabled, an internal mechanism
 * sets the TCDn_CSR[START] bit of the specified channel (LINKCH) upon minor loop
 * completion.\n
 * If channel linking is disabled, the CITER value is extended to 15 bits in
 * place of a link channel number. If the major loop is exhausted, this link
 * mechanism is suppressed in favor of the MAJORELINK channel linking.
 *
 * 0: The channel-to-channel linking is disabled
 * 1: The channel-to-channel linking is enabled
 * @note This bit must be equal to the BITER[ELINK] bit; otherwise, a
 configuration error is reported.
 */
#define EDMA_TCDn_CITER_ELINK(x)                (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CITER_ELINK_Pos)) & EDMA_TCDn_CITER_ELINK_Msk)

#define EDMA_TCDn_CITER_CITER_Pos               (0U)
#define EDMA_TCDn_CITER_ELINKNO_CITER_Msk       ((uint16_t) ((uint32_t) 0x7FFFU << EDMA_TCDn_CITER_CITER_Pos))
#define EDMA_TCDn_CITER_ELINKYES_CITER_Msk      ((uint16_t) ((uint32_t) 0x1FFU << EDMA_TCDn_CITER_CITER_Pos))
/**
 * Current Major Iteration Count.
 * This 9-bit (ELINK = 1) or 15-bit (ELINK = 0) count represents the current
 * major loop count for the channel. It is decremented each time the channel
 * finishes a service request and is written back to the TCD memory. After the
 * major iteration count is exhausted, the channel performs a number of operations,
 * for example, final source and destination address calculations, optionally
 * generating an interrupt to signal channel completion before reloading the
 * CITER field from the Beginning Iteration Count (BITER) field.
 * @note When the CITER field is initially loaded by software, it must be set
 * to the same value as that contained in the BITER field for the proper
 * operation of the optional TCDn_CSR[INTHALF] interrupt.\n
 *
 * @note If the channel is configured to execute a single service request, the
 * initial values of BITER and CITER should be 0x0001.
 *
 * @note Use this macro if ELINK = 0.
 */
#define EDMA_TCDn_CITER_ELINKNO_CITER(x)        (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CITER_CITER_Pos)) & EDMA_TCDn_CITER_ELINKNO_CITER_Msk)
/**
 * Current Major Iteration Count.
 * This 9-bit (ELINK = 1) or 15-bit (ELINK = 0) count represents the current
 * major loop count for the channel. It is decremented each time the channel
 * finishes a service request and is written back to the TCD memory. After the
 * major iteration count is exhausted, the channel performs a number of operations,
 * for example, final source and destination address calculations, optionally
 * generating an interrupt to signal channel completion before reloading the
 * CITER field from the Beginning Iteration Count (BITER) field.
 * @note When the CITER field is initially loaded by software, it must be set
 * to the same value as that contained in the BITER field for the proper
 * operation of the optional TCDn_CSR[INTHALF] interrupt.\n
 *
 * @note If the channel is configured to execute a single service request, the
 * initial values of BITER and CITER should be 0x0001.
 *
 * @note Use this macro if ELINK = 1.
 */
#define EDMA_TCDn_CITER_ELINKYES_CITER(x)       (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CITER_CITER_Pos)) & EDMA_TCDn_CITER_ELINKYES_CITER_Msk)

#define EDMA_TCDn_CITER_LINKCH_Pos              (9U)
#define EDMA_TCDn_CITER_LINKCH_Msk              ((uint16_t) ((uint16_t) 0x1FU << EDMA_TCDn_CITER_LINKCH_Pos))
/**
 * Minor Loop Link Channel Number.
 * If channel-to-channel linking is enabled (ELINK = 1), then after the minor
 * loop is exhausted, the eDMA engine initiates a channel service request to the
 * channel defined by this field by setting that channel’s TCDn_CSR[START] bit.
 *
 * @note Use this macro if ELINK = 1 only.
 */
#define EDMA_TCDn_CITER_LINKCH(x)               (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CITER_LINKCH_Pos)) & EDMA_TCDn_CITER_LINKCH_Msk)
/** @} */ /* tcdn_citer_elinkno/yes register */

/**
 * @name TCD Control and Status Register (TCDn_CSR) masks
 *
 * @details Memory address bits[31:0] pointing to the destination of the data.
  <table>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td colspan="2">TMC</td> <td>RES.</td> <td colspan="5">MAJORLINKCH</td>
        <td>ESDA</td> <td>RES.</td> <td>MAJORELINK</td> <td>ESG</td> <td>DREQ</td> <td>INTHALF</td> <td>INTMAJOR</td> <td>START</td>
    </tr>
    </tr>
    <tr>
        <td colspan="2"> rw </td> <td> r </td> <td colspan="5"> rw </td>
        <td>rw</td> <td>r</td> <td>rw</td> <td>rw</td> <td>rw</td> <td>rw</td> <td>rw</td> <td>rw</td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_CSR_TMC_Pos                   (14U)
#define EDMA_TCDn_CSR_TMC_Msk                   ((uint16_t) ((uint16_t) 3U << EDMA_TCDn_CSR_TMC_Pos))
/**
 * Transfer Mode Control.
 * Transfer Mode Control specifies the direction the DMA will transfer data.
 * For normal data movement operation, Read/Write mode reads from the source
 * address and writes to the destination address. For Read Only mode, data is
 * read from the source address and discarded. Read Only mode may be useful for
 * calculating CRC values on a block of memory. For Write Only mode, the source
 * address is read once and all subsequent writes to the destination uses the
 * data from the first and only read. Write only mode may be used to initialize
 * a block of memory to a specific value.\n
 * 0: Read/Write\n
 * 0: Read Only\n
 * 1: Write Only\n
 * 1: Reserved\n
 */
#define EDMA_TCDn_CSR_TMC(x)                    (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CSR_TMC_Pos)) & EDMA_TCDn_CSR_TMC_Msk)

#define EDMA_TCDn_CSR_MAJORLINKCH_Pos           (8U)
#define EDMA_TCDn_CSR_MAJORLINKCH_Msk           ((uint16_t) ((uint16_t) 0x1FU << EDMA_TCDn_CSR_MAJORLINKCH_Pos))
/**
 * Major loop link channel number.
 * If (MAJORELINK = 0) then:\n
 * • No channel-to-channel linking, or chaining, is performed after the major
 * loop counter is exhausted.\n
 * Otherwise:\n
 * • After the major loop counter is exhausted, the eDMA engine initiates a
 * channel service request at * the channel defined by this field by setting
 * that channel’s TCDn_CSR[START] bit.\n
 */
#define EDMA_TCDn_CSR_MAJORLINKCH(x)            (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CSR_MAJORLINKCH_Pos)) & EDMA_TCDn_CSR_MAJORLINKCH_Msk)

#define EDMA_TCDn_CSR_ESDA_Pos                  (7U)
#define EDMA_TCDn_CSR_ESDA_Msk                  ((uint16_t) ((uint16_t) 1U << EDMA_TCDn_CSR_ESDA_Pos))
/**
 * Enable store destination address.
 * As the channel completes the major loop by either the current iteration
 * counter (CITER) decrementing to zero , this bit enables writing the destination
 * address (DADDR) to the address pointed to by the SLAST_SDA field. The value
 * written to system memory is the last DADDR value prior to the DLAST_SGA offset
 * being applied or overwritten by an enabled scatter/gather operation. When the
 * SDA bit is set, the SLAST_SDA contains the write pointer instead of the final
 * source address offset. Because this is a pointer and not a final offset, a
 * last source address offset of zero is applied to SADDR instead of the
 * SLAST_SDA value.\n
 * 0: The store destination address to system memory operation is disabled.\n
 * 1: The store destination address to system memory operation is enabled.
 */
#define EDMA_TCDn_CSR_EDSA(x)                   (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CSR_ESDA_Pos)) & EDMA_TCDn_CSR_ESDA_Msk)

#define EDMA_TCDn_CSR_MAJORELINK_Pos            (5U)
#define EDMA_TCDn_CSR_MAJORELINK_Msk            ((uint16_t) ((uint16_t) 1U << EDMA_TCDn_CSR_MAJORELINK_Pos))
/**
 * Enable channel-to-channel linking on major loop complete.
 * As the channel completes the major loop, this flag enables the linking to
 * another channel, defined by MAJORLINKCH. The link target channel initiates
 * a channel service request via an internal mechanism that sets the
 * TCDn_CSR[START] bit of the specified channel.\n
 * 0: The channel-to-channel linking is disabled.\n
 * 1: The channel-to-channel linking is enabled.
 *
 * @note To support the dynamic linking coherency model, this field is forced to
 * zero when written to while the TCDn_CSR[DONE] bit is set. This field and the
 * MAJORLINKCH must be written at the same time when attempting a dynamic
 * channel link.
 */
#define EDMA_TCDn_CSR_MAJORELINK(x)             (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CSR_MAJORELINK_Pos)) & EDMA_TCDn_CSR_MAJORELINK_Msk)

#define EDMA_TCDn_CSR_ESG_Pos                   (4U)
#define EDMA_TCDn_CSR_ESG_Msk                   ((uint16_t) ((uint16_t) 1U << EDMA_TCDn_CSR_ESG_Pos))
/**
 * Enable Scatter/Gather processing.
 * As the channel completes the major loop, this flag enables scatter/gather
 * processing in the current channel. If enabled, the eDMA engine uses DLASTSGA
 * as a memory pointer to a 0-modulo-32 address containing a 32-byte data
 * structure loaded as the transfer control descriptor into the local memory.\n
 * 0: The current channel’s TCD is normal format.\n
 * 1: The current channel’s TCD specifies a scatter gather format. The DLASTSGA
 * field provides a memory pointer to the next TCD to be loaded into this
 * channel after the major loop completes its execution.
 * @note Any changes made to this field while the channel is executing will not
 * be recognized until the next time the channel is activated.
 */
#define EDMA_TCDn_CSR_ESG(x)                    (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CSR_ESG_Pos)) & EDMA_TCDn_CSR_ESG_Msk)

#define EDMA_TCDn_CSR_DREQ_Pos                  (3U)
#define EDMA_TCDn_CSR_DREQ_Msk                  ((uint16_t) ((uint16_t) 1U << EDMA_TCDn_CSR_DREQ_Pos))
/**
 * Disable request.
 * If this flag is set, the eDMA hardware automatically clears the corresponding
 * ERQ bit when the current major iteration count reaches zero.\n
 * 0: No operation. The channel’s ERQ bit is not affected.\n
 * 1: Clear the ERQ bit upon major loop completion, thus disabling hardware
 * service requests. The channel’s ERQ bit is cleared when the major loop is
 * complete.
 */
#define EDMA_TCDn_CSR_DREQ(x)                   (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CSR_DREQ_Pos)) & EDMA_TCDn_CSR_DREQ_Msk)

#define EDMA_TCDn_CSR_INTHALF_Pos               (2U)
#define EDMA_TCDn_CSR_INTHALF_Msk               ((uint16_t) ((uint16_t) 1U << EDMA_TCDn_CSR_INTHALF_Pos))
/**
 * Enable an interrupt when major counter is half complete.
 * If this flag is set, the channel generates an interrupt request by setting
 * the appropriate bit in the INT register when the current major iteration
 * count reaches the halfway point. Specifically, the comparison performed by
 * the eDMA engine is (CITER == (BITER/2)). This halfway point interrupt request
 * is provided to support double-buffered, also known as ping-pong, schemes or
 * other types of data movement where the processor needs an early indication of
 * the transfer’s progress.\n
 * 0: The half-point interrupt is disabled.\n
 * 1: The half-point interrupt is enabled.
 *
 * @note If BITER = 1, do not use INTHALF. Use INTMAJOR instead.
 */
#define EDMA_TCDn_CSR_INTHALF(x)                (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CSR_INTHALF_Pos)) & EDMA_TCDn_CSR_INTHALF_Msk)

#define EDMA_TCDn_CSR_INTMAJOR_Pos              (1U)
#define EDMA_TCDn_CSR_INTMAJOR_Msk              ((uint16_t) ((uint16_t) 1U << EDMA_TCDn_CSR_INTMAJOR_Pos))
/**
 * Enable an interrupt when major iteration count completes.
 * If this flag is set, the channel generates an interrupt request by setting
 * the appropriate bit in the INT when the current major iteration count (CITER)
 * reaches zero.\n
 * 0: The end-of-major loop interrupt is disabled.\n
 * 1: The end-of-major loop interrupt is enabled.
 */
#define EDMA_TCDn_CSR_INTMAJOR(x)               (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CSR_INTMAJOR_Pos)) & EDMA_TCDn_CSR_INTMAJOR_Msk)

#define EDMA_TCDn_CSR_START_Pos                 (0U)
#define EDMA_TCDn_CSR_START_Msk                 ((uint16_t) ((uint16_t) 1U << EDMA_TCDn_CSR_START_Pos))
/**
 * Channel Start.
 * If this flag is set, the channel is requesting service. The eDMA hardware
 * automatically clears this flag after the channel begins execution.\n
 * 0: The channel is not explicitly started.
 * 1: The channel is explicitly started via a software initiated service request.
 */
#define EDMA_TCDn_CSR_START(x)                  (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_CSR_START_Pos)) & EDMA_TCDn_CSR_START_Msk)
/** @} */ /* tcdn_daddr register */

/**
 * @name TCD Beginning Major Loop Count (TCDn_BITER_ELINKNO/TCDn_BITER_ELINKYES) masks
 *
 * @details
  <table>
    <caption>TCDn_BITER_ELINKNO (TCDn_BITER[ELINK] = 0)</caption>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td> ELINK </td> <td colspan="15">BITER[14:0]</td>
    </tr>
    </tr>
    <tr>
        <td> rw </td> <td colspan="15"> rw </td>
    </tr>
  </table>
  <table>
    <caption>TCDn_BITER_ELINKYES (TCDn_BITER[ELINK] = 1)</caption>
    <tr>
        <th> 15 </th> <th> 14 </th> <th> 13 </th> <th> 12 </th> <th> 11 </th> <th> 10 </th> <th> 9 </th> <th> 8 </th>
        <th> 7 </th> <th> 6 </th> <th> 5 </th> <th> 4 </th> <th> 3 </th> <th> 2 </th> <th> 1 </th> <th> 0 </th>
    </tr>
    <tr>
        <td> ELINK </td> <td> RES. </td> <td colspan="5"> ELINKCH[13:9] </td> <td colspan="9"> BITER[8:0] </td>
    </tr>
    </tr>
    <tr>
        <td> rw </td> <td> r </td> <td colspan="5"> rw </td> <td colspan="9"> rw </td>
    </tr>
  </table>
 * @{
 */
#define EDMA_TCDn_BITER_ELINK_Pos               (15U)
#define EDMA_TCDn_BITER_ELINK_Msk               ((uint32_t) ((uint32_t) 1U << EDMA_TCDn_BITER_ELINK_Pos))
/**
 * Enables channel-to-channel linking on minor loop complete.
 * As the channel completes the minor loop, this flag enables the linking to
 * another channel, defined by BITER[LINKCH]. The link target channel initiates
 * a channel service request via an internal mechanism that sets the
 * TCDn_CSR[START] bit of the specified channel. If channel linking disables,
 * the BITER value extends to 15 bits in place of a link channel number. If the
 * major loop is exhausted, this link mechanism is suppressed in favor of the
 * MAJORELINK channel linking.\n
 * 0: The channel-to-channel linking is disabled\n
 * 1: The channel-to-channel linking is enabled
 *
 * @note When the software loads the TCD, this field must be set equal to the
 * corresponding CITER field; otherwise, a configuration error is reported. As
 * the major iteration count is exhausted, the contents of this field are
 * reloaded into the CITER field.
 */
#define EDMA_TCDn_BITER_ELINK(x)                (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_BITER_ELINK_Pos)) & EDMA_TCDn_CITER_ELINK_Msk)

#define EDMA_TCDn_BITER_BITER_Pos               (0U)
#define EDMA_TCDn_BITER_ELINKNO_BITER_Msk       ((uint16_t) ((uint16_t) 0x7FFFU << EDMA_TCDn_BITER_BITER_POS))
#define EDMA_TCDn_BITER_ELINKYES_BITER_Msk      ((uint16_t) ((uint16_t) 0x1FFU << EDMA_TCDn_BITER_BITER_Pos))
/**
 * Starting Major Iteration Count.
 * As the transfer control descriptor is first loaded by software, this 9-bit
 * (ELINK = 1) or 15-bit (ELINK = 0) field should be equal to the value in the
 * CITER field. As the major iteration count is exhausted, the content of this
 * field is reloaded into the CITER field.
 *
 * @note When the software loads the TCD, this field must be set equal to the
 * corresponding CITER field for proper operation of the optional
 * TCDn_CSR[INTHALF] interrupt. As the major iteration count is exhausted, the
 * contents of this field is reloaded into the CITER field. If the channel is
 * configured to execute a single service request, the initial values of BITER
 * and CITER should be 0x0001.
 *
 * @note Use this macro if ELINK = 0.
 */
#define EDMA_TCDn_BITER_ELINKNO_BITER(x)        (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_BITER_BITER_Pos)) & EDMA_TCDn_BITER_ELINKNO_BITER_Msk)
/**
 * Starting Major Iteration Count.
 * As the transfer control descriptor is first loaded by software, this 9-bit
 * (ELINK = 1) or 15-bit (ELINK = 0) field should be equal to the value in the
 * CITER field. As the major iteration count is exhausted, the content of this
 * field is reloaded into the CITER field.
 *
 * @note When the software loads the TCD, this field must be set equal to the
 * corresponding CITER field for proper operation of the optional
 * TCDn_CSR[INTHALF] interrupt. As the major iteration count is exhausted, the
 * contents of this field is reloaded into the CITER field. If the channel is
 * configured to execute a single service request, the initial values of BITER
 * and CITER should be 0x0001.
 *
 * @note Use this macro if ELINK = 1.
 */
#define EDMA_TCDn_BITER_ELINKYES_BITER(x)       (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_BITER_BITER_Pos)) & EDMA_TCDn_BITER_ELINKYES_BITER_Msk)

#define EDMA_TCDn_BITER_LINKCH_Pos              (9U)
#define EDMA_TCDn_BITER_LINKCH_Msk              ((uint16_t) ((uint16_t) 0x1FU << EDMA_TCDn_BITER_LINKCH_Pos))
/**
 * Link Channel Number.
 * If channel-to-channel linking is enabled (ELINK = 1), then after the minor
 * loop is exhausted, the eDMA engine initiates a channel service request at
 * the channel defined by this field by setting that channel’s TCDn_CSR[START]
 * bit.
 *
 * @note As the major iteration count is exhausted, the contents of this field
 * are reloaded into the CITER field.
 *
 * @note Use this macro if ELINK = 1 only.
 */
#define EDMA_TCDn_BITER_LINKCH(x)               (((uint16_t)(((uint16_t) (x)) << EDMA_TCDn_BITER_LINKCH_Pos)) & EDMA_TCDn_CITER_LINKCH_Msk)
/** @} */ /* tcdn_biter_elinkno/yes register */

/*!
 * @}
 */ /* end of group EDMA5_Register_Masks */


/*!
 * @}
 */ /* end of group DMA5_Peripheral_Access_Layer */


/*!
 * @}
 */ /* end of group Peripheral_access_layer */

/*
** End of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic pop
  #else
    #pragma pop
  #endif
#elif defined(__GNUC__)
  /* leave anonymous unions enabled */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=default
#else
  #error Not supported compiler type
#endif

#endif  /* EDMA5_H_ */