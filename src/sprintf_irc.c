/*
 *  ircd-hybrid: an advanced Internet Relay Chat Daemon(ircd).
 *  snprintf.c: Functions for printing to a string.
 *
 *  Copyright (C) 2002 by the past and present ircd coders, and others.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *  USA
 *
 *  $Id$
 */

#include "stdinc.h"
#include "sprintf_irc.h"
#include "irc_string.h"


static const char atoi_tab[4000] = {
    '0','0','0',0, '0','0','1',0, '0','0','2',0, '0','0','3',0, '0','0','4',0,
    '0','0','5',0, '0','0','6',0, '0','0','7',0, '0','0','8',0, '0','0','9',0,
    '0','1','0',0, '0','1','1',0, '0','1','2',0, '0','1','3',0, '0','1','4',0,
    '0','1','5',0, '0','1','6',0, '0','1','7',0, '0','1','8',0, '0','1','9',0,
    '0','2','0',0, '0','2','1',0, '0','2','2',0, '0','2','3',0, '0','2','4',0,
    '0','2','5',0, '0','2','6',0, '0','2','7',0, '0','2','8',0, '0','2','9',0,
    '0','3','0',0, '0','3','1',0, '0','3','2',0, '0','3','3',0, '0','3','4',0,
    '0','3','5',0, '0','3','6',0, '0','3','7',0, '0','3','8',0, '0','3','9',0,
    '0','4','0',0, '0','4','1',0, '0','4','2',0, '0','4','3',0, '0','4','4',0,
    '0','4','5',0, '0','4','6',0, '0','4','7',0, '0','4','8',0, '0','4','9',0,
    '0','5','0',0, '0','5','1',0, '0','5','2',0, '0','5','3',0, '0','5','4',0,
    '0','5','5',0, '0','5','6',0, '0','5','7',0, '0','5','8',0, '0','5','9',0,
    '0','6','0',0, '0','6','1',0, '0','6','2',0, '0','6','3',0, '0','6','4',0,
    '0','6','5',0, '0','6','6',0, '0','6','7',0, '0','6','8',0, '0','6','9',0,
    '0','7','0',0, '0','7','1',0, '0','7','2',0, '0','7','3',0, '0','7','4',0,
    '0','7','5',0, '0','7','6',0, '0','7','7',0, '0','7','8',0, '0','7','9',0,
    '0','8','0',0, '0','8','1',0, '0','8','2',0, '0','8','3',0, '0','8','4',0,
    '0','8','5',0, '0','8','6',0, '0','8','7',0, '0','8','8',0, '0','8','9',0,
    '0','9','0',0, '0','9','1',0, '0','9','2',0, '0','9','3',0, '0','9','4',0,
    '0','9','5',0, '0','9','6',0, '0','9','7',0, '0','9','8',0, '0','9','9',0,
    '1','0','0',0, '1','0','1',0, '1','0','2',0, '1','0','3',0, '1','0','4',0,
    '1','0','5',0, '1','0','6',0, '1','0','7',0, '1','0','8',0, '1','0','9',0,
    '1','1','0',0, '1','1','1',0, '1','1','2',0, '1','1','3',0, '1','1','4',0,
    '1','1','5',0, '1','1','6',0, '1','1','7',0, '1','1','8',0, '1','1','9',0,
    '1','2','0',0, '1','2','1',0, '1','2','2',0, '1','2','3',0, '1','2','4',0,
    '1','2','5',0, '1','2','6',0, '1','2','7',0, '1','2','8',0, '1','2','9',0,
    '1','3','0',0, '1','3','1',0, '1','3','2',0, '1','3','3',0, '1','3','4',0,
    '1','3','5',0, '1','3','6',0, '1','3','7',0, '1','3','8',0, '1','3','9',0,
    '1','4','0',0, '1','4','1',0, '1','4','2',0, '1','4','3',0, '1','4','4',0,
    '1','4','5',0, '1','4','6',0, '1','4','7',0, '1','4','8',0, '1','4','9',0,
    '1','5','0',0, '1','5','1',0, '1','5','2',0, '1','5','3',0, '1','5','4',0,
    '1','5','5',0, '1','5','6',0, '1','5','7',0, '1','5','8',0, '1','5','9',0,
    '1','6','0',0, '1','6','1',0, '1','6','2',0, '1','6','3',0, '1','6','4',0,
    '1','6','5',0, '1','6','6',0, '1','6','7',0, '1','6','8',0, '1','6','9',0,
    '1','7','0',0, '1','7','1',0, '1','7','2',0, '1','7','3',0, '1','7','4',0,
    '1','7','5',0, '1','7','6',0, '1','7','7',0, '1','7','8',0, '1','7','9',0,
    '1','8','0',0, '1','8','1',0, '1','8','2',0, '1','8','3',0, '1','8','4',0,
    '1','8','5',0, '1','8','6',0, '1','8','7',0, '1','8','8',0, '1','8','9',0,
    '1','9','0',0, '1','9','1',0, '1','9','2',0, '1','9','3',0, '1','9','4',0,
    '1','9','5',0, '1','9','6',0, '1','9','7',0, '1','9','8',0, '1','9','9',0,
    '2','0','0',0, '2','0','1',0, '2','0','2',0, '2','0','3',0, '2','0','4',0,
    '2','0','5',0, '2','0','6',0, '2','0','7',0, '2','0','8',0, '2','0','9',0,
    '2','1','0',0, '2','1','1',0, '2','1','2',0, '2','1','3',0, '2','1','4',0,
    '2','1','5',0, '2','1','6',0, '2','1','7',0, '2','1','8',0, '2','1','9',0,
    '2','2','0',0, '2','2','1',0, '2','2','2',0, '2','2','3',0, '2','2','4',0,
    '2','2','5',0, '2','2','6',0, '2','2','7',0, '2','2','8',0, '2','2','9',0,
    '2','3','0',0, '2','3','1',0, '2','3','2',0, '2','3','3',0, '2','3','4',0,
    '2','3','5',0, '2','3','6',0, '2','3','7',0, '2','3','8',0, '2','3','9',0,
    '2','4','0',0, '2','4','1',0, '2','4','2',0, '2','4','3',0, '2','4','4',0,
    '2','4','5',0, '2','4','6',0, '2','4','7',0, '2','4','8',0, '2','4','9',0,
    '2','5','0',0, '2','5','1',0, '2','5','2',0, '2','5','3',0, '2','5','4',0,
    '2','5','5',0, '2','5','6',0, '2','5','7',0, '2','5','8',0, '2','5','9',0,
    '2','6','0',0, '2','6','1',0, '2','6','2',0, '2','6','3',0, '2','6','4',0,
    '2','6','5',0, '2','6','6',0, '2','6','7',0, '2','6','8',0, '2','6','9',0,
    '2','7','0',0, '2','7','1',0, '2','7','2',0, '2','7','3',0, '2','7','4',0,
    '2','7','5',0, '2','7','6',0, '2','7','7',0, '2','7','8',0, '2','7','9',0,
    '2','8','0',0, '2','8','1',0, '2','8','2',0, '2','8','3',0, '2','8','4',0,
    '2','8','5',0, '2','8','6',0, '2','8','7',0, '2','8','8',0, '2','8','9',0,
    '2','9','0',0, '2','9','1',0, '2','9','2',0, '2','9','3',0, '2','9','4',0,
    '2','9','5',0, '2','9','6',0, '2','9','7',0, '2','9','8',0, '2','9','9',0,
    '3','0','0',0, '3','0','1',0, '3','0','2',0, '3','0','3',0, '3','0','4',0,
    '3','0','5',0, '3','0','6',0, '3','0','7',0, '3','0','8',0, '3','0','9',0,
    '3','1','0',0, '3','1','1',0, '3','1','2',0, '3','1','3',0, '3','1','4',0,
    '3','1','5',0, '3','1','6',0, '3','1','7',0, '3','1','8',0, '3','1','9',0,
    '3','2','0',0, '3','2','1',0, '3','2','2',0, '3','2','3',0, '3','2','4',0,
    '3','2','5',0, '3','2','6',0, '3','2','7',0, '3','2','8',0, '3','2','9',0,
    '3','3','0',0, '3','3','1',0, '3','3','2',0, '3','3','3',0, '3','3','4',0,
    '3','3','5',0, '3','3','6',0, '3','3','7',0, '3','3','8',0, '3','3','9',0,
    '3','4','0',0, '3','4','1',0, '3','4','2',0, '3','4','3',0, '3','4','4',0,
    '3','4','5',0, '3','4','6',0, '3','4','7',0, '3','4','8',0, '3','4','9',0,
    '3','5','0',0, '3','5','1',0, '3','5','2',0, '3','5','3',0, '3','5','4',0,
    '3','5','5',0, '3','5','6',0, '3','5','7',0, '3','5','8',0, '3','5','9',0,
    '3','6','0',0, '3','6','1',0, '3','6','2',0, '3','6','3',0, '3','6','4',0,
    '3','6','5',0, '3','6','6',0, '3','6','7',0, '3','6','8',0, '3','6','9',0,
    '3','7','0',0, '3','7','1',0, '3','7','2',0, '3','7','3',0, '3','7','4',0,
    '3','7','5',0, '3','7','6',0, '3','7','7',0, '3','7','8',0, '3','7','9',0,
    '3','8','0',0, '3','8','1',0, '3','8','2',0, '3','8','3',0, '3','8','4',0,
    '3','8','5',0, '3','8','6',0, '3','8','7',0, '3','8','8',0, '3','8','9',0,
    '3','9','0',0, '3','9','1',0, '3','9','2',0, '3','9','3',0, '3','9','4',0,
    '3','9','5',0, '3','9','6',0, '3','9','7',0, '3','9','8',0, '3','9','9',0,
    '4','0','0',0, '4','0','1',0, '4','0','2',0, '4','0','3',0, '4','0','4',0,
    '4','0','5',0, '4','0','6',0, '4','0','7',0, '4','0','8',0, '4','0','9',0,
    '4','1','0',0, '4','1','1',0, '4','1','2',0, '4','1','3',0, '4','1','4',0,
    '4','1','5',0, '4','1','6',0, '4','1','7',0, '4','1','8',0, '4','1','9',0,
    '4','2','0',0, '4','2','1',0, '4','2','2',0, '4','2','3',0, '4','2','4',0,
    '4','2','5',0, '4','2','6',0, '4','2','7',0, '4','2','8',0, '4','2','9',0,
    '4','3','0',0, '4','3','1',0, '4','3','2',0, '4','3','3',0, '4','3','4',0,
    '4','3','5',0, '4','3','6',0, '4','3','7',0, '4','3','8',0, '4','3','9',0,
    '4','4','0',0, '4','4','1',0, '4','4','2',0, '4','4','3',0, '4','4','4',0,
    '4','4','5',0, '4','4','6',0, '4','4','7',0, '4','4','8',0, '4','4','9',0,
    '4','5','0',0, '4','5','1',0, '4','5','2',0, '4','5','3',0, '4','5','4',0,
    '4','5','5',0, '4','5','6',0, '4','5','7',0, '4','5','8',0, '4','5','9',0,
    '4','6','0',0, '4','6','1',0, '4','6','2',0, '4','6','3',0, '4','6','4',0,
    '4','6','5',0, '4','6','6',0, '4','6','7',0, '4','6','8',0, '4','6','9',0,
    '4','7','0',0, '4','7','1',0, '4','7','2',0, '4','7','3',0, '4','7','4',0,
    '4','7','5',0, '4','7','6',0, '4','7','7',0, '4','7','8',0, '4','7','9',0,
    '4','8','0',0, '4','8','1',0, '4','8','2',0, '4','8','3',0, '4','8','4',0,
    '4','8','5',0, '4','8','6',0, '4','8','7',0, '4','8','8',0, '4','8','9',0,
    '4','9','0',0, '4','9','1',0, '4','9','2',0, '4','9','3',0, '4','9','4',0,
    '4','9','5',0, '4','9','6',0, '4','9','7',0, '4','9','8',0, '4','9','9',0,
    '5','0','0',0, '5','0','1',0, '5','0','2',0, '5','0','3',0, '5','0','4',0,
    '5','0','5',0, '5','0','6',0, '5','0','7',0, '5','0','8',0, '5','0','9',0,
    '5','1','0',0, '5','1','1',0, '5','1','2',0, '5','1','3',0, '5','1','4',0,
    '5','1','5',0, '5','1','6',0, '5','1','7',0, '5','1','8',0, '5','1','9',0,
    '5','2','0',0, '5','2','1',0, '5','2','2',0, '5','2','3',0, '5','2','4',0,
    '5','2','5',0, '5','2','6',0, '5','2','7',0, '5','2','8',0, '5','2','9',0,
    '5','3','0',0, '5','3','1',0, '5','3','2',0, '5','3','3',0, '5','3','4',0,
    '5','3','5',0, '5','3','6',0, '5','3','7',0, '5','3','8',0, '5','3','9',0,
    '5','4','0',0, '5','4','1',0, '5','4','2',0, '5','4','3',0, '5','4','4',0,
    '5','4','5',0, '5','4','6',0, '5','4','7',0, '5','4','8',0, '5','4','9',0,
    '5','5','0',0, '5','5','1',0, '5','5','2',0, '5','5','3',0, '5','5','4',0,
    '5','5','5',0, '5','5','6',0, '5','5','7',0, '5','5','8',0, '5','5','9',0,
    '5','6','0',0, '5','6','1',0, '5','6','2',0, '5','6','3',0, '5','6','4',0,
    '5','6','5',0, '5','6','6',0, '5','6','7',0, '5','6','8',0, '5','6','9',0,
    '5','7','0',0, '5','7','1',0, '5','7','2',0, '5','7','3',0, '5','7','4',0,
    '5','7','5',0, '5','7','6',0, '5','7','7',0, '5','7','8',0, '5','7','9',0,
    '5','8','0',0, '5','8','1',0, '5','8','2',0, '5','8','3',0, '5','8','4',0,
    '5','8','5',0, '5','8','6',0, '5','8','7',0, '5','8','8',0, '5','8','9',0,
    '5','9','0',0, '5','9','1',0, '5','9','2',0, '5','9','3',0, '5','9','4',0,
    '5','9','5',0, '5','9','6',0, '5','9','7',0, '5','9','8',0, '5','9','9',0,
    '6','0','0',0, '6','0','1',0, '6','0','2',0, '6','0','3',0, '6','0','4',0,
    '6','0','5',0, '6','0','6',0, '6','0','7',0, '6','0','8',0, '6','0','9',0,
    '6','1','0',0, '6','1','1',0, '6','1','2',0, '6','1','3',0, '6','1','4',0,
    '6','1','5',0, '6','1','6',0, '6','1','7',0, '6','1','8',0, '6','1','9',0,
    '6','2','0',0, '6','2','1',0, '6','2','2',0, '6','2','3',0, '6','2','4',0,
    '6','2','5',0, '6','2','6',0, '6','2','7',0, '6','2','8',0, '6','2','9',0,
    '6','3','0',0, '6','3','1',0, '6','3','2',0, '6','3','3',0, '6','3','4',0,
    '6','3','5',0, '6','3','6',0, '6','3','7',0, '6','3','8',0, '6','3','9',0,
    '6','4','0',0, '6','4','1',0, '6','4','2',0, '6','4','3',0, '6','4','4',0,
    '6','4','5',0, '6','4','6',0, '6','4','7',0, '6','4','8',0, '6','4','9',0,
    '6','5','0',0, '6','5','1',0, '6','5','2',0, '6','5','3',0, '6','5','4',0,
    '6','5','5',0, '6','5','6',0, '6','5','7',0, '6','5','8',0, '6','5','9',0,
    '6','6','0',0, '6','6','1',0, '6','6','2',0, '6','6','3',0, '6','6','4',0,
    '6','6','5',0, '6','6','6',0, '6','6','7',0, '6','6','8',0, '6','6','9',0,
    '6','7','0',0, '6','7','1',0, '6','7','2',0, '6','7','3',0, '6','7','4',0,
    '6','7','5',0, '6','7','6',0, '6','7','7',0, '6','7','8',0, '6','7','9',0,
    '6','8','0',0, '6','8','1',0, '6','8','2',0, '6','8','3',0, '6','8','4',0,
    '6','8','5',0, '6','8','6',0, '6','8','7',0, '6','8','8',0, '6','8','9',0,
    '6','9','0',0, '6','9','1',0, '6','9','2',0, '6','9','3',0, '6','9','4',0,
    '6','9','5',0, '6','9','6',0, '6','9','7',0, '6','9','8',0, '6','9','9',0,
    '7','0','0',0, '7','0','1',0, '7','0','2',0, '7','0','3',0, '7','0','4',0,
    '7','0','5',0, '7','0','6',0, '7','0','7',0, '7','0','8',0, '7','0','9',0,
    '7','1','0',0, '7','1','1',0, '7','1','2',0, '7','1','3',0, '7','1','4',0,
    '7','1','5',0, '7','1','6',0, '7','1','7',0, '7','1','8',0, '7','1','9',0,
    '7','2','0',0, '7','2','1',0, '7','2','2',0, '7','2','3',0, '7','2','4',0,
    '7','2','5',0, '7','2','6',0, '7','2','7',0, '7','2','8',0, '7','2','9',0,
    '7','3','0',0, '7','3','1',0, '7','3','2',0, '7','3','3',0, '7','3','4',0,
    '7','3','5',0, '7','3','6',0, '7','3','7',0, '7','3','8',0, '7','3','9',0,
    '7','4','0',0, '7','4','1',0, '7','4','2',0, '7','4','3',0, '7','4','4',0,
    '7','4','5',0, '7','4','6',0, '7','4','7',0, '7','4','8',0, '7','4','9',0,
    '7','5','0',0, '7','5','1',0, '7','5','2',0, '7','5','3',0, '7','5','4',0,
    '7','5','5',0, '7','5','6',0, '7','5','7',0, '7','5','8',0, '7','5','9',0,
    '7','6','0',0, '7','6','1',0, '7','6','2',0, '7','6','3',0, '7','6','4',0,
    '7','6','5',0, '7','6','6',0, '7','6','7',0, '7','6','8',0, '7','6','9',0,
    '7','7','0',0, '7','7','1',0, '7','7','2',0, '7','7','3',0, '7','7','4',0,
    '7','7','5',0, '7','7','6',0, '7','7','7',0, '7','7','8',0, '7','7','9',0,
    '7','8','0',0, '7','8','1',0, '7','8','2',0, '7','8','3',0, '7','8','4',0,
    '7','8','5',0, '7','8','6',0, '7','8','7',0, '7','8','8',0, '7','8','9',0,
    '7','9','0',0, '7','9','1',0, '7','9','2',0, '7','9','3',0, '7','9','4',0,
    '7','9','5',0, '7','9','6',0, '7','9','7',0, '7','9','8',0, '7','9','9',0,
    '8','0','0',0, '8','0','1',0, '8','0','2',0, '8','0','3',0, '8','0','4',0,
    '8','0','5',0, '8','0','6',0, '8','0','7',0, '8','0','8',0, '8','0','9',0,
    '8','1','0',0, '8','1','1',0, '8','1','2',0, '8','1','3',0, '8','1','4',0,
    '8','1','5',0, '8','1','6',0, '8','1','7',0, '8','1','8',0, '8','1','9',0,
    '8','2','0',0, '8','2','1',0, '8','2','2',0, '8','2','3',0, '8','2','4',0,
    '8','2','5',0, '8','2','6',0, '8','2','7',0, '8','2','8',0, '8','2','9',0,
    '8','3','0',0, '8','3','1',0, '8','3','2',0, '8','3','3',0, '8','3','4',0,
    '8','3','5',0, '8','3','6',0, '8','3','7',0, '8','3','8',0, '8','3','9',0,
    '8','4','0',0, '8','4','1',0, '8','4','2',0, '8','4','3',0, '8','4','4',0,
    '8','4','5',0, '8','4','6',0, '8','4','7',0, '8','4','8',0, '8','4','9',0,
    '8','5','0',0, '8','5','1',0, '8','5','2',0, '8','5','3',0, '8','5','4',0,
    '8','5','5',0, '8','5','6',0, '8','5','7',0, '8','5','8',0, '8','5','9',0,
    '8','6','0',0, '8','6','1',0, '8','6','2',0, '8','6','3',0, '8','6','4',0,
    '8','6','5',0, '8','6','6',0, '8','6','7',0, '8','6','8',0, '8','6','9',0,
    '8','7','0',0, '8','7','1',0, '8','7','2',0, '8','7','3',0, '8','7','4',0,
    '8','7','5',0, '8','7','6',0, '8','7','7',0, '8','7','8',0, '8','7','9',0,
    '8','8','0',0, '8','8','1',0, '8','8','2',0, '8','8','3',0, '8','8','4',0,
    '8','8','5',0, '8','8','6',0, '8','8','7',0, '8','8','8',0, '8','8','9',0,
    '8','9','0',0, '8','9','1',0, '8','9','2',0, '8','9','3',0, '8','9','4',0,
    '8','9','5',0, '8','9','6',0, '8','9','7',0, '8','9','8',0, '8','9','9',0,
    '9','0','0',0, '9','0','1',0, '9','0','2',0, '9','0','3',0, '9','0','4',0,
    '9','0','5',0, '9','0','6',0, '9','0','7',0, '9','0','8',0, '9','0','9',0,
    '9','1','0',0, '9','1','1',0, '9','1','2',0, '9','1','3',0, '9','1','4',0,
    '9','1','5',0, '9','1','6',0, '9','1','7',0, '9','1','8',0, '9','1','9',0,
    '9','2','0',0, '9','2','1',0, '9','2','2',0, '9','2','3',0, '9','2','4',0,
    '9','2','5',0, '9','2','6',0, '9','2','7',0, '9','2','8',0, '9','2','9',0,
    '9','3','0',0, '9','3','1',0, '9','3','2',0, '9','3','3',0, '9','3','4',0,
    '9','3','5',0, '9','3','6',0, '9','3','7',0, '9','3','8',0, '9','3','9',0,
    '9','4','0',0, '9','4','1',0, '9','4','2',0, '9','4','3',0, '9','4','4',0,
    '9','4','5',0, '9','4','6',0, '9','4','7',0, '9','4','8',0, '9','4','9',0,
    '9','5','0',0, '9','5','1',0, '9','5','2',0, '9','5','3',0, '9','5','4',0,
    '9','5','5',0, '9','5','6',0, '9','5','7',0, '9','5','8',0, '9','5','9',0,
    '9','6','0',0, '9','6','1',0, '9','6','2',0, '9','6','3',0, '9','6','4',0,
    '9','6','5',0, '9','6','6',0, '9','6','7',0, '9','6','8',0, '9','6','9',0,
    '9','7','0',0, '9','7','1',0, '9','7','2',0, '9','7','3',0, '9','7','4',0,
    '9','7','5',0, '9','7','6',0, '9','7','7',0, '9','7','8',0, '9','7','9',0,
    '9','8','0',0, '9','8','1',0, '9','8','2',0, '9','8','3',0, '9','8','4',0,
    '9','8','5',0, '9','8','6',0, '9','8','7',0, '9','8','8',0, '9','8','9',0,
    '9','9','0',0, '9','9','1',0, '9','9','2',0, '9','9','3',0, '9','9','4',0,
    '9','9','5',0, '9','9','6',0, '9','9','7',0, '9','9','8',0, '9','9','9',0 };

static char scratch_buffer[32];

/*
 * sprintf_irc
 *
 * sprintf_irc is optimized for the formats: %c, %s, %lu, %d and %u.
 * Where %lu actually equals %l09u (for printing time_t timestamps).
 *
 * sprintf_irc is NOT optimized for any other format and resorts to using
 * the normal sprintf when it encounters a format it doesn't understand
 * (including padding, width, precision etc).
 *
 * The following benchmark was measured on a PPro200 with linux 2.0.30,
 * libc 5.4.28, compiled with gcc-2.7.2.1 with -O3.
 *
 * Format                       sprintf         sprintf_irc     Speed up factor
 *
 * "12345678901234567890"       3.385 us        0.859 us        3.94
 * "%s", buffer (20 chars)      3.780 us        0.547 us        6.91
 * "%c%c%c", c1, c2, c3         3.640 us        0.376 us        9.68
 * "%lu", (time_t)t             5.851 us        0.842 us        6.95
 *
 * Less important:
 * "%d", 31337                  4.487 us        0.852 us        5.27
 * "%u.%u.%u.%u",               9.069 us        2.431 us        3.73
 *
 * Not used:
 * "%03d", 401                  4.348 us
 * "%N"                                         0.216 us        20.13
 *
 * --Run
 */

int
vsprintf_irc(char *str, const char *format, va_list args)

{
  char c;
  int bytes = 0;

  while ((c = *format++))
    {
      if (c == '%')
	{
	  c = *format++; /* May never be '\0' ! */

	  if (c == 's')
	    {
	      const char *p1 = va_arg(args, const char *);
	      if ((*str = *p1))
		{
		  ++bytes;
		  while ((*++str = *++p1))
		    ++bytes;
		}
	      
	      continue;
	    }

	  if (c == 'c')
	    {
	      *str++ = (char) va_arg(args, int);
	      ++bytes;

	      continue;
	    }

	  /*
	   * Prints time_t value in interval
	   * [ 100000000 , 4294967295 ]
	   * Actually prints like "%09lu"
	   */
	  if (c == 'l' && *format == 'u')
	    {
	      unsigned long v1, v2;
	      const char *ap;

	      ++format;
	      v1 = va_arg(args, unsigned long);
	      if (v1 == 0)
	      {
	        *str++ = '0';
	        ++bytes;
	        continue;
	      }
	      if (v1 > 999999999L)
		{
		  v2 = v1 / 1000000000;
		  v1 -= v2 * 1000000000;
		  *str++ = '0' + v2;
		  ++bytes;
		}

	      v2 = v1 / 1000000;
	      v1 -= v2 * 1000000;
	      ap = atoi_tab + (v2 << 2);
	      *str++ = *ap++;
	      *str++ = *ap++;
	      *str++ = *ap;
	      v2 = v1 / 1000;
	      v1 -= v2 * 1000;
	      ap = atoi_tab + (v2 << 2);
	      *str++ = *ap++;
	      *str++ = *ap++;
	      *str++ = *ap;
	      ap = atoi_tab + (v1 << 2);
	      *str++ = *ap++;
	      *str++ = *ap++;
	      *str++ = *ap;

	      bytes += 9;

	      continue;
	    }
	  if (c == 't')
	    {
	      unsigned int v1;

	      v1 = va_arg(args,int);
                            
	      *str++ = (v1/10) + '0';
	      *str++ = v1%10 + '0';

	      bytes += 2;

	      continue;
	    }

	  if (c == 'd')
	    {
	      unsigned int v1, v2;
	      const char *ap;
	      char *s = &scratch_buffer[sizeof(scratch_buffer) - 2];
	      
	      v1 = va_arg(args, int);
	      if ((int)v1 <= 0)
		{
		  if (v1 == 0)
		    {
		      *str++ = '0';
		      ++bytes;
		      continue;
		    }
		  *str++ = '-';
		  ++bytes;
		  v1 = -v1;
		}

	      do
		{
		  v2 = v1 / 1000;
		  ap = atoi_tab + 2 + ((v1 - 1000 * v2) << 2);
		  *s-- = *ap--;
		  *s-- = *ap--;
		  *s-- = *ap;
		}
	      while ((v1 = v2) > 0);

	      while ('0' == *++s);

	      *str = *s;
	      ++bytes;

	      while ((*++str = *++s))
		++bytes;

	      continue;
	    }

	  if (c == 'u')
	    {
	      unsigned int v1, v2;
	      const char *ap;
	      char *s = &scratch_buffer[sizeof(scratch_buffer) - 2];
	      
	      v1 = va_arg(args, unsigned int);
	      if (v1 == 0)
		{
		  *str++ = '0';
		  ++bytes;
		  continue;
		}

	      do
		{
		  v2 = v1 / 1000;
		  ap = atoi_tab + 2 + ((v1 - 1000 * v2) << 2);
		  *s-- = *ap--;
		  *s-- = *ap--;
		  *s-- = *ap;
		}
	      while ((v1 = v2) > 0);
	      
	      while ('0' == *++s);

	      *str = *s;
	      ++bytes;

	      while ((*++str = *++s))
		++bytes;

	      continue;
	    }

	  if (c != '%')
	    {
	      int ret;
	      
	      format -= 2;
	      ret = vsprintf(str, format, args);
	      str += ret;
	      bytes += ret;

	      break;
	    }
	}

      *str++ = c;
      ++bytes;
    }

  *str = '\0';

  return (bytes);
} /* vsprintf_irc() */

int
ircsprintf(char *str, const char *format, ...)
{
  va_list args;
  int bytes;

  va_start(args, format);

  bytes = vsprintf_irc(str, format, args);

  va_end(args);

  return (bytes);
} /* ircsprintf() */

