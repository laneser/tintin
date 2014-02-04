/******************************************************************************
*   TinTin++                                                                  *
*   Copyright (C) 2005 (See CREDITS file)                                     *
*                                                                             *
*   This program is protected under the GNU GPL (See COPYING)                 *
*                                                                             *
*   This program is free software; you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation; either version 2 of the License, or         *
*   (at your option) any later version.                                       *
*                                                                             *
*   This program is distributed in the hope that it will be useful,           *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*   GNU General Public License for more details.                              *
*                                                                             *
*   You should have received a copy of the GNU General Public License         *
*   along with this program; if not, write to the Free Software               *
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA *
*******************************************************************************/

/******************************************************************************
*   file: echo.c - funtions related to localecho                              *
*           (T)he K(I)cki(N) (T)ickin D(I)kumud Clie(N)t ++ 2.00              *
*                     coded by Igor van den Hoven 2005                        *
******************************************************************************/


#include "tintin.h"

/*
#include <termios.h>
*/

void echo_off(struct session *ses)
{
/*
	struct termios io;

	tcgetattr(STDIN_FILENO, &io);

	DEL_BIT(io.c_lflag, ECHO);

	tcsetattr(STDIN_FILENO, TCSADRAIN, &io);
*/
	DEL_BIT(ses->telopts, TELOPT_FLAG_ECHO);

	check_character_mode(ses);

	readline_echoing_p = FALSE;
}

void echo_on(struct session *ses)
{
/*
	struct termios io;

	tcgetattr(STDIN_FILENO, &io);

	SET_BIT(io.c_lflag, ECHO);

	tcsetattr(STDIN_FILENO, TCSADRAIN, &io);
*/
	SET_BIT(ses->telopts, TELOPT_FLAG_ECHO);

	check_character_mode(ses);

	readline_echoing_p = TRUE;
}

void check_character_mode(struct session *ses)
{
	if ((HAS_BIT(ses->telopts, TELOPT_FLAG_SGA) && !HAS_BIT(ses->telopts, TELOPT_FLAG_ECHO))
	||  (HAS_BIT(ses->flags, SES_FLAG_CONVERTMETA)))
	{
		if (!HAS_BIT(gts->flags, SES_FLAG_PREPPED))
		{
			SET_BIT(gts->flags, SES_FLAG_PREPPED);

			rl_initialize();
			rl_prep_terminal(1);

			commandloop();
		}
	}
	else
	{
		if (HAS_BIT(gts->flags, SES_FLAG_PREPPED))
		{
			DEL_BIT(gts->flags, SES_FLAG_PREPPED);

			rl_initialize();
			rl_deprep_terminal();

			commandloop();
		}
	}
}
