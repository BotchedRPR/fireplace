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
#include <fireplace/gui/gui.h>
#include <fireplace/soc/fb/fb.h>
#include <fireplace/soc/uart/uart.h>
#include <fireplace/soc/hardware_buttons/hardware_buttons.h>

#define RESOLUTION_SCALE (0.4)
#define WINDOW_WIDTH (1440 * RESOLUTION_SCALE)
#define WINDOW_HEIGHT (3200 * RESOLUTION_SCALE)

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

void blit_window(struct nk_context *ctx)
{
	state emuState = 0;

	emuState = atomic_load(&sharedState);

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
}

void blit_uart_window(struct nk_context *ctx)
{
	atomic_load(&line);

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
}

void blit_hardware_button_window(struct nk_context *ctx)
{
	state emuState = 0;

	emuState = atomic_load(&sharedState);

	if(nk_begin(ctx, "Hardware Button Control", nk_rect(25, 25, 321, 83),
		    NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
	{
		nk_layout_row_static(ctx, 40, 100, 3);

		if (nk_button_label(ctx, "Power"))
		{
			if(emuState == STATE_RUNNING)
				trigger_key(POWER);
		}

		if (nk_button_label(ctx, "Volume Up"))
		{
			if(emuState == STATE_RUNNING)
				trigger_key(VOL_UP);
		}

		if (nk_button_label(ctx, "Volume Down"))
		{
			if(emuState == STATE_RUNNING)
				trigger_key(VOL_DOWN);
		}
	}

	nk_end(ctx);
}

static GLuint fb_texture = 0;

static GLuint prepare_fb_texture()
{
	if (fb_texture == 0)
	{
		glGenTextures(1, &fb_texture);
	}

	glBindTexture(GL_TEXTURE_2D, fb_texture);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 1440);

	pthread_mutex_lock(&fb_lock);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FB_WIDTH, FB_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer);

	pthread_mutex_unlock(&fb_lock);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return fb_texture;
}

void render_ogl(SDL_Window *win, int win_width, int win_height)
{
	SDL_GetWindowSize(win, &win_width, &win_height);
	glViewport(0, 0, win_width, win_height);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, prepare_fb_texture());

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1.0f, -1.0f); // Bottom-left
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, -1.0f); // Bottom-right
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f, 1.0f); // Top-right
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1.0f, 1.0f); // Top-left
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	nk_sdl_render(NK_ANTI_ALIASING_ON);
	SDL_GL_SwapWindow(win);
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

		blit_window(ctx);
		blit_uart_window(ctx);
		blit_hardware_button_window(ctx);

		render_ogl(win, win_height, win_width);
	}

cleanup:
	nk_sdl_shutdown();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return dummy;
}
