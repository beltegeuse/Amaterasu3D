//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : adrien.gruson@gmail.com
//==========================================================
#pragma once

bool GetSDLChar(SDL_Event& event, char* c)
{
	switch(event.key.keysym.sym)
	{
	case SDLK_q:
		(*c) = 'q'; break;
	case SDLK_w:
			(*c) = 'w'; break;
	case SDLK_e:
			(*c) = 'e'; break;
	case SDLK_r:
			(*c) = 'r'; break;
	case SDLK_t:
			(*c) = 't'; break;
	case SDLK_y:
			(*c) = 'y'; break;
	case SDLK_u:
			(*c) = 'u'; break;
	case SDLK_i:
			(*c) = 'i'; break;
	case SDLK_o:
			(*c) = 'o'; break;
	case SDLK_p:
			(*c) = 'p'; break;
	case SDLK_a:
			(*c) = 'a'; break;
	case SDLK_s:
			(*c) = 's'; break;
	case SDLK_d:
			(*c) = 'd'; break;
	case SDLK_f:
			(*c) = 'f'; break;
	case SDLK_g:
			(*c) = 'g'; break;
	case SDLK_h:
			(*c) = 'h'; break;
	case SDLK_j:
			(*c) = 'j'; break;
	case SDLK_k:
			(*c) = 'k'; break;
	case SDLK_l:
			(*c) = 'l'; break;
	case SDLK_z:
			(*c) = 'z'; break;
	case SDLK_x:
			(*c) = 'x'; break;
	case SDLK_c:
			(*c) = 'c'; break;
	case SDLK_v:
			(*c) = 'v'; break;
	case SDLK_b:
			(*c) = 'b'; break;
	case SDLK_n:
			(*c) = 'n'; break;
	case SDLK_m:
			(*c) = 'm'; break;
	case SDLK_0:
		(*c) = '0'; break;
	case SDLK_1:
		(*c) = '1'; break;
	case SDLK_2:
		(*c) = '2'; break;
	case SDLK_3:
		(*c) = '3'; break;
	case SDLK_4:
		(*c) = '4'; break;
	case SDLK_5:
		(*c) = '5'; break;
	case SDLK_6:
		(*c) = '6'; break;
	case SDLK_7:
		(*c) = '7'; break;
	case SDLK_8:
		(*c) = '8'; break;
	case SDLK_9:
		(*c) = '9'; break;
	case SDLK_SPACE:
		(*c) = ' '; break;
	case SDLK_RETURN:
		(*c) = '\n'; break;
	case SDLK_RETURN2:
		(*c) = '\n'; break;
	case SDLK_HASH:
		(*c) = '#'; break;
	case SDLK_QUOTE:
		(*c) = '"'; break;
	case SDLK_RIGHTPAREN:
		(*c) = ')'; break;
	case SDLK_LEFTPAREN:
		(*c) = '('; break;
	case SDLK_RIGHTBRACKET:
		(*c) = ']'; break;
	case SDLK_LEFTBRACKET:
		(*c) = '['; break;
	case SDLK_SLASH:
		(*c) = '/'; break;
	case SDLK_BACKSPACE:
		(*c) = '\b'; break;
	default:
		return false;
	}
	return true;
}
