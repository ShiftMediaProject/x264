/* Substitute for and wrapper around <stdarg.h>.
   Copyright (C) 2008-2013 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.  */

#ifndef __STDARG_H
#define __STDARG_H

#ifndef va_copy
# define va_copy(a,b) ((a) = (b))
#endif

#endif /* __STDARG_H */
