/*
 * Copyright (C) 2013 Huawei Ltd.
 * Author: Jiang Liu <liuj97@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef	__ASM_INSN_H
#define	__ASM_INSN_H
#include <linux/types.h>

/*
 * ARM Architecture Reference Manual for ARMv8 Profile-A, Issue A.a
 * Section C3.1 "A64 instruction index by encoding":
 * AArch64 main encoding table
 *  Bit position
 *   28 27 26 25	Encoding Group
 *   0  0  -  -		Unallocated
 *   1  0  0  -		Data processing, immediate
 *   1  0  1  -		Branch, exception generation and system instructions
 *   -  1  -  0		Loads and stores
 *   -  1  0  1		Data processing - register
 *   0  1  1  1		Data processing - SIMD and floating point
 *   1  1  1  1		Data processing - SIMD and floating point
 * "-" means "don't care"
 */
enum aarch64_insn_encoding_class {
	AARCH64_INSN_CLS_UNKNOWN,	/* UNALLOCATED */
	AARCH64_INSN_CLS_DP_IMM,	/* Data processing - immediate */
	AARCH64_INSN_CLS_DP_REG,	/* Data processing - register */
	AARCH64_INSN_CLS_DP_FPSIMD,	/* Data processing - SIMD and FP */
	AARCH64_INSN_CLS_LDST,		/* Loads and stores */
	AARCH64_INSN_CLS_BR_SYS,	/* Branch, exception generation and
					 * system instructions */
};

enum aarch64_insn_hint_op {
	AARCH64_INSN_HINT_NOP	= 0x0 << 5,
	AARCH64_INSN_HINT_YIELD	= 0x1 << 5,
	AARCH64_INSN_HINT_WFE	= 0x2 << 5,
	AARCH64_INSN_HINT_WFI	= 0x3 << 5,
	AARCH64_INSN_HINT_SEV	= 0x4 << 5,
	AARCH64_INSN_HINT_SEVL	= 0x5 << 5,
};

#define	__AARCH64_INSN_FUNCS(abbr, mask, val)	\
static __always_inline bool aarch64_insn_is_##abbr(u32 code) \
{ return (code & (mask)) == (val); } \
static __always_inline u32 aarch64_insn_get_##abbr##_value(void) \
{ return (val); }

__AARCH64_INSN_FUNCS(b,		0xFC000000, 0x14000000)
__AARCH64_INSN_FUNCS(bl,	0xFC000000, 0x94000000)
__AARCH64_INSN_FUNCS(svc,	0xFFE0001F, 0xD4000001)
__AARCH64_INSN_FUNCS(hvc,	0xFFE0001F, 0xD4000002)
__AARCH64_INSN_FUNCS(smc,	0xFFE0001F, 0xD4000003)
__AARCH64_INSN_FUNCS(brk,	0xFFE0001F, 0xD4200000)
__AARCH64_INSN_FUNCS(hint,	0xFFFFF01F, 0xD503201F)

#undef	__AARCH64_INSN_FUNCS

bool aarch64_insn_is_nop(u32 insn);

enum aarch64_insn_encoding_class aarch64_get_insn_class(u32 insn);

bool aarch64_insn_hotpatch_safe(u32 old_insn, u32 new_insn);

#endif	/* __ASM_INSN_H */
