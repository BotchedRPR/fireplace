/*
 *   Copyright (c) 2025 Igor Belwon <igor.belwon@mentallysanemainliners.org>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdatomic.h>
#include <stdio.h>

#include <unicorn/unicorn.h>

#include <fireplace/core/emulator.h>
#include <fireplace/core/macros.h>
#include <fireplace/soc/memmap.h>
#include <fireplace/soc/soc.h>

#define INT_BIN_PATH "../../../../../fireplace/lk.bin"
#define INT_BIN_ADDR 0xe1900000
#define INT_BIN_SIZE 0x00F00000

/*
 * load_image functions:
 * Copyright (c) 2023, Ivaylo Ivanov <ivo.ivanov@null.net>
 * https://github.com/ivoszbg/LAEmu/blob/main/main.c
 * Modified lightly.
*/

static inline uc_err
load_image(uc_engine *uc, const char *file, uint64_t base, uint64_t *last)
{
	char buf[1024];
	FILE *f;
	long sz;
	uc_err err;
	uint64_t addr = base;

	printf("Opening file: %s\n", file);

	if (!(f = fopen(file, "r")))
		return UC_ERR_HANDLE;

	printf("Opened fine!\n", file);

	fseek(f, 0L, SEEK_END);
	sz = ftell(f);
	fseek(f, 0L, SEEK_SET);

	while (ftell(f) != sz) {
		size_t n = fread(buf, 1, 1024, f);
		*last = addr;
		if ((err = uc_mem_write(uc, addr, buf, n)) != UC_ERR_OK)
			return err;
		addr += n;
	}

	return err;
}

atomic_int sharedState = 0;

static inline int emulator_init(void)
{
	uc_engine *uc;
	uint64_t end;
	int err = 0;

	printf("== Emulator starting ==\n");

#if defined(INT_BIN_PATH)
	printf("Using hardcoded :[ binary path %s\n", INT_BIN_PATH);
#else
	printf("No binary path provided! Bailing out\n");
	return -1;
#endif

	/* We have all we need - setup unicorn context */
	err = uc_open(UC_ARCH_ARM64, UC_MODE_ARM, &uc);
	uc_handle_error("Failed to open UC engine!", err);

	if (memmap_soc(uc, MEMORY_12GB))
	{
		printf("Failed to map memory for SoC!\n");
		return -1;
	}

	err = load_image(uc, INT_BIN_PATH, INT_BIN_ADDR, &end);
	uc_handle_error("Failed to load the initial binary to memory!", err);

	/*
	 * Basic emulator setup is done. Now, init peripherals.
	 */
	err = soc_peripherals_init(uc);
	if (err)
	{
		printf("Failed to initialize peripherals!\n");
		return -1;
	}

	printf("=== All good! Starting emulator! ===\n");
	atomic_store(&sharedState, STATE_RUNNING);
	if ((err = uc_emu_start(uc, INT_BIN_ADDR, end, 0, 0)) != UC_ERR_OK)
	{
		int pc;

		printf("\n------> fireplace exception: %s\n", uc_strerror(err));
		atomic_store(&sharedState, STATE_CRASHED);
		if (uc_reg_read(uc, UC_ARM64_REG_PC, &pc) != UC_ERR_OK)
		{
			printf("Failed to read PC register\n");
		}
		else
		{
			printf("= PC at 0x%x =\n", pc);
		}
		uc_close(uc);
		return -1;
	}

	atomic_store(&sharedState, STATE_OFF);
	return 0;
}

void* _emulator_init(void* dummy)
{
	emulator_init();
	return dummy;
}
