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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>

// Include SDL2 first
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// Nuklear
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL2_IMPLEMENTATION

#include <external/nuklear.h>
#include <external/nuklear_sdl_gl2.h>

#include <fireplace/core/core.h>
#include <fireplace/core/emulator.h>
#include <fireplace/soc/uart/uart.h>

#define RESOLUTION_SCALE (0.4)
#define WINDOW_WIDTH (1440 * RESOLUTION_SCALE)
#define WINDOW_HEIGHT (3200 * RESOLUTION_SCALE)

extern pthread_mutex_t main_mutex;
extern pthread_cond_t main_cond;

extern atomic_int sharedState;
extern char uart_buf[];
extern pthread_mutex_t uart_lock;

extern atomic_int line;

void gui_init(void)
{
	/* SDL setup */
	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	return;
}

void *gui_core(void *dummy)
{
	/* Platform */
	SDL_Window *win;
	SDL_GLContext glContext;
	int win_width, win_height;
	int running = 1;

	/* GUI */
	struct nk_context *ctx;
	struct nk_colorf bg;

	state emuState = 0;
	int cline = 0;

	win = SDL_CreateWindow("Fireplace",
			       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	glContext = SDL_GL_CreateContext(win);
	SDL_GetWindowSize(win, &win_width, &win_height);

	/* GUI */
	ctx = nk_sdl_init(win);
	{
		struct nk_font_atlas *atlas;
		nk_sdl_font_stash_begin(&atlas);
		nk_sdl_font_stash_end();
	}

	bg.r = 0.10f, bg.g = 0.10f, bg.b = 0.14f, bg.a = 1.0f;
	while (running)
	{
		/* Input */
		SDL_Event evt;
		nk_input_begin(ctx);
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT)
				goto cleanup;
			nk_sdl_handle_event(&evt);
		}
		nk_input_end(ctx);

		emuState = atomic_load(&sharedState);
		cline = atomic_load(&line);

		/* GUI */
		if (nk_begin(ctx, "Emulator setup", nk_rect(0, 0, 230, 250),
			     NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
				 NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
		{
			nk_layout_row_static(ctx, 30, 80, 1);
			if (emuState != STATE_RUNNING)
			{
				if (nk_button_label(ctx, "Start"))
					create_emulator_thread();
			}

			nk_layout_row_dynamic(ctx, 30, 1);
			switch (emuState)
			{
			case STATE_OFF:
				nk_label_colored(ctx, "Emulator state: off",
						 NK_TEXT_LEFT, nk_rgb(208, 211, 212));
				break;
			case STATE_RUNNING:
				nk_label_colored(ctx, "Emulator state: running",
						 NK_TEXT_LEFT, nk_rgb(9, 255, 0));
				break;
			case STATE_CRASHED:
				nk_label_colored(ctx, "Emulator state: crashed",
						 NK_TEXT_LEFT, nk_rgb(208, 211, 212));
				break;
			default:
				nk_label_colored(ctx, "Emulator state: unknown",
						 NK_TEXT_LEFT, nk_rgb(255, 0, 166));
				break;
			}
		}

		nk_end(ctx);

		if (nk_begin(ctx, "UART window", nk_rect(50, 50, 500, 400),
			     NK_WINDOW_BORDER | NK_WINDOW_MOVABLE |
				 NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
		{
			nk_layout_row_dynamic(ctx, UART_BUF_SIZE * 2, 1);

			pthread_mutex_lock(&uart_lock);
			nk_label_colored_wrap(ctx, uart_buf, nk_rgb(255, 255, 255));
			pthread_mutex_unlock(&uart_lock);

			// This is the worst shit i've ever written
			nk_window_set_scroll(ctx, 0, (line * 27));
		}

		nk_end(ctx);

		/* Draw */
		SDL_GetWindowSize(win, &win_width, &win_height);
		glViewport(0, 0, win_width, win_height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(bg.r, bg.g, bg.b, bg.a);
		nk_sdl_render(NK_ANTI_ALIASING_ON);
		SDL_GL_SwapWindow(win);
	}

cleanup:
	nk_sdl_shutdown();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return dummy;
}