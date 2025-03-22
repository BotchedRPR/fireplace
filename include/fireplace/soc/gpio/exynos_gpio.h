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

#ifndef FIREPLACE_EXYNOS_GPIO_H
#define FIREPLACE_EXYNOS_GPIO_H

#include <unicorn/unicorn.h>

/* Masks */
#define CON_MASK(x)	(0xf << ((x) << 2))
#define CON_SFR(x, v)	((v) << ((x) << 2))

#define DAT_MASK(x)	(0x1 << (x))
#define DAT_SET(x)	(0x1 << (x))

#define PULL_MASK(x)	(0xf << ((x) << 2))
#define PULL_MODE(x, v)	((v) << ((x) << 2))

#define DRV_MASK(x)	(0xf << ((x) << 2))
#define DRV_SET(x, m)	((m) << ((x) << 2))

#define RATE_MASK(x)	(0x1 << (x + 16))
#define RATE_SET(x)	(0x1 << (x + 16))

/* Pin configurations */
#define GPIO_INPUT	0x0
#define GPIO_OUTPUT	0x1
#define GPIO_IRQ	0xf
#define GPIO_FUNC(x) (x)

/* Pull mode */
#define GPIO_PULL_NONE	0x0
#define GPIO_PULL_DOWN	0x1
#define GPIO_PULL_UP	0x3

/* Drive Strength level */
#define GPIO_DRV_1X	0x0
#define GPIO_DRV_3X	0x1
#define GPIO_DRV_2X	0x2
#define GPIO_DRV_4X	0x3
#define GPIO_DRV_FAST	0x0
#define GPIO_DRV_SLOW	0x1

/* GPIO pins per bank  */
#define GPIO_PER_BANK	8

struct exynos_gpio_bank {
	uint32_t con;
	uint32_t dat;
	uint32_t pull;
	uint32_t drv;
	uint32_t pdn_con;
	uint32_t pdn_pull;
	uint8_t res1[8];
};

/* Functions */
void exynos_gpio_cfg_pin(uc_engine *uc_s, struct exynos_gpio_bank *bank, int gpio, int cfg);
void exynos_gpio_direction_output(uc_engine *uc_s, struct exynos_gpio_bank *bank, int gpio, int en);
void exynos_gpio_direction_input(uc_engine *uc_s, struct exynos_gpio_bank *bank, int gpio);
void exynos_gpio_set_value(uc_engine *uc_s, struct exynos_gpio_bank *bank, int gpio, int en);
uint32_t exynos_gpio_get_value(struct uc_struct *uc_s, struct exynos_gpio_bank *bank, int gpio);
void exynos_gpio_set_pull(uc_engine *uc_s, struct exynos_gpio_bank *bank, int gpio, int mode);
void exynos_gpio_set_drv(uc_engine *uc_s, struct exynos_gpio_bank *bank, int gpio, int mode);
void exynos_gpio_set_rate(uc_engine *uc_s, struct exynos_gpio_bank *bank, int gpio, int mode);
#endif
