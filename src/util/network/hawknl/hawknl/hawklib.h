/*
  HawkLib.h version 1.0
  Copyright (C) 2003-2004 Phil Frisbie, Jr. (phil@hawksoft.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA  02111-1307, USA.

  Or go to http://www.gnu.org/copyleft/lgpl.html
*/

#ifndef HAWKLIB_H
#define HAWKLIB_H

#if defined (WIN32) || defined (WIN64) || defined (_WIN32_WCE)
#define HL_WINDOWS_APP
#endif

#define HL_EXP extern
#define HL_APIENTRY
#define HL_CALLBACK
#ifdef __GNUC__
#define HL_INLINE extern __inline__
#else
#define HL_INLINE inline /* assuming C99 compliant compiler */
#endif /* __GNUC__ */

#endif /* HAWKLIB_H */

