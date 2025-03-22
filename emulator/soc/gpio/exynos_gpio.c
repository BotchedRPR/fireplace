/*
 *   Copyright (c) 2025 Umer Uddin <umer.uddin@mentallysanemainliners.org>

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

#include <fireplace/soc/gpio/exynos_gpio.h>

void exynos_gpio_dat_mask(struct exynos_gpio_bank *bank, unsigned int *dat) // Unused on 9830
{
}

static inline unsigned long exynos_gpio_base(int nr) // Unused on 9830
{
	return 0;
}

void exynos_gpio_cfg_pin(struct uc_struct *uc_s, struct exynos_gpio_bank *bank, int gpio, int cfg)
{
	uint32_t value;

	uc_mem_read(uc_s, (uint64_t)&bank->con, &value, sizeof(value));
	value &= ~CON_MASK(gpio);
	value |= CON_SFR(gpio, cfg);
	uc_mem_write(uc_s, (uint64_t)&bank->con, &value, sizeof(value));
}

void exynos_gpio_direction_output(struct uc_struct *uc_s, struct exynos_gpio_bank *bank, int gpio, int en)
{
	uint32_t value;

	exynos_gpio_cfg_pin(uc_s, bank, gpio, GPIO_OUTPUT);

	uc_mem_read(uc_s, (uint64_t)&bank->dat, &value, sizeof(value));
	value &= ~DAT_MASK(gpio);
	if (en)
		value |= DAT_SET(gpio);
	exynos_gpio_dat_mask(bank, &value);
	uc_mem_write(uc_s, (uint64_t)&bank->dat, &value, sizeof(value));
}

void exynos_gpio_direction_input(struct uc_struct *uc_s, struct exynos_gpio_bank *bank, int gpio)
{
	exynos_gpio_cfg_pin(uc_s, bank, gpio, GPIO_INPUT);
}

void exynos_gpio_set_value(struct uc_struct *uc_s, struct exynos_gpio_bank *bank, int gpio, int en)
{
	uint32_t value;

	uc_mem_read(uc_s, (uint64_t)&bank->dat, &value, sizeof(value));
	value &= ~DAT_MASK(gpio);
	if (en)
		value |= DAT_SET(gpio);
	exynos_gpio_dat_mask(bank, &value);
	uc_mem_write(uc_s, (uint64_t)&bank->dat, &value, sizeof(value));
}

uint32_t exynos_gpio_get_value(struct uc_struct *uc_s, struct exynos_gpio_bank *bank, int gpio)
{
	uint32_t value;

	uc_mem_read(uc_s, (uint64_t)&bank->dat, &value, sizeof(value));
	return !!(value & DAT_MASK(gpio));
}

void exynos_gpio_set_pull(struct uc_struct *uc_s, struct exynos_gpio_bank *bank, int gpio, int mode)
{
	uint32_t value;

	uc_mem_read(uc_s, (uint64_t)&bank->pull, &value, sizeof(value));
	value &= ~PULL_MASK(gpio);

	switch (mode) {
	case GPIO_PULL_NONE:
	case GPIO_PULL_DOWN:
	case GPIO_PULL_UP:
		value |= PULL_MODE(gpio, mode);
		break;
	default:
		break;
	}

	uc_mem_write(uc_s, (uint64_t)&bank->pull, &value, sizeof(value));
}

void exynos_gpio_set_drv(struct uc_struct *uc_s, struct exynos_gpio_bank *bank, int gpio, int mode)
{
	uint32_t value;

	uc_mem_read(uc_s, (uint64_t)&bank->drv, &value, sizeof(value));
	value &= ~DRV_MASK(gpio);

	value |= DRV_SET(gpio, mode);

	uc_mem_write(uc_s, (uint64_t)&bank->drv, &value, sizeof(value));
}

void exynos_gpio_set_rate(struct uc_struct *uc_s, struct exynos_gpio_bank *bank, int gpio, int mode)
{
	uint32_t value;

	uc_mem_read(uc_s, (uint64_t)&bank->drv, &value, sizeof(value));
	value &= ~RATE_MASK(gpio);

	switch (mode) {
	case GPIO_DRV_FAST:
	case GPIO_DRV_SLOW:
		value |= RATE_SET(gpio);
		break;
	default:
		return;
	}

	uc_mem_write(uc_s, (uint64_t)&bank->drv, &value, sizeof(value));
}

struct exynos_gpio_bank *exynos_gpio_get_bank(unsigned gpio)
{
	int bank = gpio / GPIO_PER_BANK;

	bank *= sizeof(struct exynos_gpio_bank);

	return (struct exynos_gpio_bank *)(exynos_gpio_base(gpio) + bank);
}

int exynos_gpio_get_pin(unsigned gpio)
{
	return gpio % GPIO_PER_BANK;
}
