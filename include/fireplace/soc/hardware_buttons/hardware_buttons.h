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

#ifndef FIREPLACE_HARDWARE_BUTTONS_H
#define FIREPLACE_HARDWARE_BUTTONS_H

extern int keys[3];

enum hardware_key
{
	POWER,
	VOL_UP,
	VOL_DOWN
};

enum key_state
{
	PRESSED,
	RELEASED
};

void trigger_key(enum hardware_key key_type);

#endif
