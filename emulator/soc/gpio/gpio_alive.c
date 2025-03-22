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

#include <fireplace/soc/gpio/gpio_alive.h>
#include <fireplace/soc/gpio/exynos_gpio.h>
#include <fireplace/soc/hardware_buttons/hardware_buttons.h>

pthread_mutex_t gpio_alive_lock = PTHREAD_MUTEX_INITIALIZER;

struct exynos_gpio_bank *bank_volume = (struct exynos_gpio_bank *)EXYNOS9830_GPA0CON;
struct exynos_gpio_bank *bank_power = (struct exynos_gpio_bank *)EXYNOS9830_GPA2CON;

int keys[3] = {1, 1, 1};

int gpio_alive_init(struct uc_struct *uc_s)
{
	printf("= gpio_alive_init\n");
	printf("= initializing button pins...\n");

	/* Setup pullups */
	exynos_gpio_set_pull(uc_s, bank_volume, 3, GPIO_PULL_UP);
	exynos_gpio_set_pull(uc_s, bank_volume, 4, GPIO_PULL_UP);
	exynos_gpio_set_pull(uc_s, bank_power, 4, GPIO_PULL_UP);

	/* Pins are buttons, so input. */
	exynos_gpio_cfg_pin(uc_s, bank_volume, 3, GPIO_INPUT);
	exynos_gpio_cfg_pin(uc_s, bank_volume, 4, GPIO_INPUT);
	exynos_gpio_cfg_pin(uc_s, bank_power, 4, GPIO_INPUT);

	/* Set initial GPIO values. */
	exynos_gpio_set_value(uc_s, bank_volume, 3, 1);
	exynos_gpio_set_value(uc_s, bank_volume, 4, 1);
	exynos_gpio_set_value(uc_s, bank_power, 4, 1);

	printf("= intialized button pins!\n");
	return 0;
}

void gpio_alive_hook(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user_data)
{
	pthread_mutex_lock(&gpio_alive_lock);

	if(type == UC_MEM_READ)
	{
		exynos_gpio_set_value(uc, bank_volume, 3, keys[VOL_UP]);
		exynos_gpio_set_value(uc, bank_volume, 4, keys[VOL_DOWN]);
		exynos_gpio_set_value(uc, bank_power, 3, keys[POWER]);
	}

	pthread_mutex_unlock(&gpio_alive_lock);
}
