#ifndef AREG_BASE_GEGLOBAL_H
#define AREG_BASE_GEGLOBAL_H
/************************************************************************
 * This file is part of the AREG SDK core engine.
 * AREG SDK is dual-licensed under Free open source (Apache version 2.0
 * License) and Commercial (with various pricing models) licenses, depending
 * on the nature of the project (commercial, research, academic or free).
 * You should have received a copy of the AREG SDK license description in LICENSE.txt.
 * If not, please contact to info[at]aregtech.com
 *
 * \copyright   (c) 2017-2023 Aregtech UG. All rights reserved.
 * \file        areg/base/GEGlobal.h
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan
 * \brief       AREG Platform, global include header file
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/

/**
 * \brief   This header file is included in every file of a project.
 *          The header file contains macro defines for MC Windows OS,
 *          includes standard headers, header of switches 
 *          to use import / exports and standard global macro.
 *          Anything global, which is relevant for all includes,
 *          should be included and/or defined here.
 **/

#include "system/GEPlatform.h"

#define OUTPUT_DEBUG_LEVEL_DEBUG    4
#define OUTPUT_DEBUG_LEVEL_INFO     3
#define OUTPUT_DEBUG_LEVEL_WARN     2
#define OUTPUT_DEBUG_LEVEL_ERROR    1
#define OUTPUT_DEBUG_LEVEL_NONE     0
#define OUTPUT_DEBUG_LEVEL_ALL      OUTPUT_DEBUG_LEVEL_DEBUG

#ifdef DEBUG
    #if !defined(OUTPUT_DEBUG_LEVEL)
        #define OUTPUT_DEBUG_LEVEL      OUTPUT_DEBUG_LEVEL_NONE
    #elif   (OUTPUT_DEBUG_LEVEL > OUTPUT_DEBUG_LEVEL_ALL)
        #define OUTPUT_DEBUG_LEVEL      OUTPUT_DEBUG_LEVEL_ALL
    #endif // !OUTPUT_DEBUG_LEVEL
#else   // !DEBUG
    #undef  OUTPUT_DEBUG_LEVEL
    #define OUTPUT_DEBUG_LEVEL          OUTPUT_DEBUG_LEVEL_NONE
#endif // DEBUG

// enable using safe POSIX calls.
#ifdef __STDC_LIB_EXT1__
    #ifndef __STDC_WANT_LIB_EXT1__
        #define __STDC_WANT_LIB_EXT1__      1
    #endif  // __STDC_WANT_LIB_EXT1__
#else   // __STDC_LIB_EXT1__
        #define __STDC_WANT_LIB_EXT1__      0
#endif  // __STDC_LIB_EXT1__

 /**
 * \brief   Basic types
 **/
#include "areg/base/GETypes.h"

/**
 * \brief   AREG core Switches and MACRO
 **/
#include "areg/base/GESwitches.h"
#include "areg/base/GEMacros.h"

// By default, no AREG extended features.
#ifndef AREG_EXTENDED
    #pragma message("The AREG_EXTENDED is not defined, setting default value 0")
    #define AREG_EXTENDED   0
#endif  // AREG_EXTENDED

// By default, compile with logs
#ifndef AREG_LOGS
    #pragma message("The AREG_LOGS is not defined, setting default value 1")
    #define AREG_LOGS       1
#endif  // AREG_LOGS

#ifdef DEBUG
    #if (defined(OUTPUT_DEBUG) && (OUTPUT_DEBUG != 0)) 
        
        #undef  OUTPUT_DEBUG
        #define OUTPUT_DEBUG    1

    #else // !(defined(OUTPUT_DEBUG) && (OUTPUT_DEBUG != 0))

        #undef  OUTPUT_DEBUG
    
    #endif  // !(defined(OUTPUT_DEBUG) && (OUTPUT_DEBUG != 0))
#else   // _NDEBUG
        #undef  OUTPUT_DEBUG
#endif  // _DEBUG

#endif  // AREG_BASE_GEGLOBAL_H
