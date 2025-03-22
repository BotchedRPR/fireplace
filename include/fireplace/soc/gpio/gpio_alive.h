/*
 *   Copyright (c) 2025 Umer Uddin <umer.uddin@mentallysanemainliners.org>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, version 2.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef FIREPLACE_GPIO_ALIVE_H
#define FIREPLACE_GPIO_ALIVE_H

#include <unicorn/unicorn.h>

#define EXYNOS9830_GPIO_ALIVE_BASE			0x15850000
#define EXYNOS9830_GPA0CON				(EXYNOS9830_GPIO_ALIVE_BASE + 0x0000)

/* Power Button GPIO */
#define EXYNOS9830_GPA2CON				(EXYNOS9830_GPIO_ALIVE_BASE + 0x0040)

/* Display GPIO for reset panel(3HA9 @ UNIV) */
#define EXYNOS9830_GPA3CON				(EXYNOS9830_GPIO_ALIVE_BASE + 0x0060)
#define EXYNOS9830_GPA3DAT				(EXYNOS9830_GPIO_ALIVE_BASE + 0x0064)
#define EXYNOS9830_GPA3PUD				(EXYNOS9830_GPIO_ALIVE_BASE + 0x0068)
#define EXYNOS9830_GPA3DRV				(EXYNOS9830_GPIO_ALIVE_BASE + 0x006C)

/* Maran V2 panel power en : POLED_VCI_EN: GPA1[2]) of 3HA8 */
#define EXYNOS9830_GPA1CON				(EXYNOS9830_GPIO_ALIVE_BASE + 0x0020)
#define EXYNOS9830_GPA1DAT				(EXYNOS9830_GPIO_ALIVE_BASE + 0x0024)

int gpio_alive_init(struct uc_struct*);
void gpio_alive_hook(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user_data);

#endif
