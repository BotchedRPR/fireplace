/*
 *   Copyright (c) 2025 Igor Belwon <igor.belwon@mentallysanemainliners.org>

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

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>

#include <unicorn/unicorn.h>

#include <fireplace/soc/fb/fb.h>

int fb_init(struct uc_struct *uc_s)
{
	printf("= fb_init\n");
	return 0;
}

pthread_mutex_t fb_lock = PTHREAD_MUTEX_INITIALIZER;
unsigned char framebuffer[FB_SIZE] = "\x0";

void fb_hook(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user_data)
{
	pthread_mutex_lock(&fb_lock);

	size_t offset = address - FB_ADDRESS;

	if (offset % 4 == 0)
	{
		offset -= 4;

		// Convert from BGRA to RGBA
		uint8_t b = (value) & 0xFF;
		uint8_t g = (value >> 8) & 0xFF;
		uint8_t r = (value >> 16) & 0xFF;
		uint8_t a = (value >> 24) & 0xFF;

		framebuffer[offset + 0] = r;
		framebuffer[offset + 1] = g;
		framebuffer[offset + 2] = b;
		framebuffer[offset + 3] = a;
	}

	pthread_mutex_unlock(&fb_lock);
}