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

#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

#include <fireplace/core/emulator.h>
#include <fireplace/gui/gui.h>

bool emu_running = false;

static pthread_t ui_thread;
static pthread_t emulator_thread;

void create_emulator_thread(void)
{
	pthread_create(&emulator_thread, NULL, _emulator_init, NULL);

	return;
}

int main(void)
{
	/* Create UI thread here. It'll create the other threads. */
	pthread_create(&ui_thread, NULL, *gui_core, NULL);

	pthread_join(ui_thread, NULL);
	if(emu_running)
		pthread_join(emulator_thread, NULL);

	return 0;
}