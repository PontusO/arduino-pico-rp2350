/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PICO_ERROR_H
#define _PICO_ERROR_H

#ifndef __ASSEMBLER__

/*!
 * \brief Common return codes from pico_sdk methods that return a status
 * \ingroup pico_base
 */
enum pico_error_codes {
    PICO_OK = 0,
    PICO_ERROR_NONE = 0,
    PICO_ERROR_TIMEOUT = -1,
    PICO_ERROR_GENERIC = -2,
    PICO_ERROR_NO_DATA = -3,
    PICO_ERROR_NOT_PERMITTED = -4,
    PICO_ERROR_INVALID_ARG = -5,
    PICO_ERROR_IO = -6,
    PICO_ERROR_BADAUTH = -7,
    PICO_ERROR_CONNECT_FAILED = -8,
    PICO_ERROR_INSUFFICIENT_RESOURCES = -9, // unable to allocate a dynamic resource
    PICO_ERROR_INVALID_ADDRESS = -10, // particularly for calling secure functions from NS code with bad buffer pointers
    PICO_ERROR_BAD_ALIGNMENT = -11,
    PICO_ERROR_INVALID_STATE = -12,
    PICO_ERROR_BUFFER_TOO_SMALL = -13,
    PICO_ERROR_PRECONDITION_NOT_MET = -14,
    PICO_ERROR_MODIFIED_DATA = -15,
    PICO_ERROR_INVALID_DATA = -16,
    PICO_ERROR_NOT_FOUND = -17,
    PICO_ERROR_UNSUPPORTED_MODIFICATION = -18,
    PICO_ERROR_LOCK_REQUIRED = -19,
    PICO_ERROR_VERSION_MISMATCH = -20
};

#endif // !__ASSEMBLER__

#endif