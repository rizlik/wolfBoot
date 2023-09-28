/* stage1.h
 *
 * Copyright (C) 2023 wolfSSL Inc.
 *
 * This file is part of wolfBoot.
 *
 * wolfBoot is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfBoot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */
#ifndef STAGE1_H
#define STAGE1_H

#include <stdint.h>

struct stage2_parameter {
#if WOLFBOOT_FSP
    uint32_t hobList;
    uint32_t page_table;
    uint32_t tolum;
#ifdef WOLFBOOT_TPM_SEAL
    uint32_t tpm_policy;
    uint16_t tpm_policy_size;
#endif
#endif
} __attribute__((packed));

/* implemented in src/boot_x86_fsp_payload.c */
struct stage2_parameter *stage2_get_parameters();

#if defined(WOLFBOOT_TPM_SEAL)
int stage2_get_tpm_policy(const uint8_t **policy, uint16_t *policy_size);
#endif /* defined(WOLFBOOT_TPM_SEAL) */

#endif /* STAGE1_H */
