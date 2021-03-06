/* $Id: sfstty.h,v 1.2 2002/07/31 16:05:19 dm Exp $ */

/*
 *
 * Copyright (C) 2002 Michael Kaminsky (kaminsky@lcs.mit.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 *
 */

#ifndef _SFSMISC_SFSTTY_H_
#define _SFSMISC_SFSTTY_H_ 1

#include <termios.h>

str windowsizetostring (struct winsize *size);
void leave_raw_mode();
void enter_raw_mode();

#endif /* _SFSMISC_SFSTTY_H_ */
