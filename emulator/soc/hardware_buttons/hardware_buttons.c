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
#include <unistd.h>

#include <fireplace/soc/gpio/gpio_alive.h>
#include <fireplace/soc/gpio/exynos_gpio.h>
#include <fireplace/soc/hardware_buttons/hardware_buttons.h>

void trigger_key(enum hardware_key key_type)
{
	keys[key_type] = PRESSED;
	usleep(2500); // Wait 2.5MS for software to register button press before restoring.
	keys[key_type] = RELEASED;
}
