/*******************************************************************************
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************/

 /**
 * @file ccl_features
 * @brief controls what features are compiled into the library.
 */

#ifndef CCL_FEATURES_H
#define CCL_FEATURES_H

#ifdef __cplusplus
extern "C" {
#endif

//To enable feature set it to 1. To disable feature set it to 0.

#define CCL_FEATURE_LOG_FRAME_TIMES 0
#define CCL_FEATURE_LOG_SENS_CFG 0
#define CCL_FEATURE_RUNTIME_CONTROL 0

#define CCL_FEATURE_CAPTURE_CGD 1

#define CCL_FEATURE_CAPTURE_MEMCPY 1

#ifdef IMX95
    #define CCL_FEATURE_CONTEXT_MATCH   1
#endif


#ifdef __cplusplus
}
#endif

#endif /* #ifndef CCL_FEATURES_H */

