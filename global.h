/**
 *
 * Xeroxpatrone
 *
 * http://code.google.com/p/xeroxpatrone
 *
 * Copyright (C) 2010 Dominik Meyer
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#ifdef WIN32
#define OS_WINDOWS
#else
#define OS_LINUX
#endif

#ifdef DEBUG
#define _DEBUG
#endif
#ifdef _DEBUG
#define DEBUG
#endif

#if !defined(UNICODE)
#define UNICODE
#endif
#if !defined(_UNICODE)
#define _UNICODE
#endif

#include "targetver.h"

#include <stdio.h>

#include <stdlib.h>
#include <malloc.h>

#ifdef OS_WINDOWS
#include <tchar.h>
#include <windows.h>
#else
#ifdef UNICODE
#define TCHAR wchar_t
#else
#define TCHAR char
#endif
#endif

#include <string>
#include <string.h>

#include <iostream>

#define SAFE_DELETE(x) {if(x) delete (x); (x) = NULL;}
#define SAFE_DELETE_ARRAY(x) {if(x) delete[] (x); (x) = NULL;}

#if defined(UNICODE) || defined(_UNICODE)
	#define tcout std::wcout
	#define tcerr std::wcerr
#else
	#define tcout std::cout
	#define tcerr std::cerr
#endif 

typedef std::basic_string<TCHAR> tstring;
typedef std::basic_ostream<TCHAR> tostream;
typedef std::basic_ostringstream<TCHAR> tostringstream;
typedef std::basic_istringstream<TCHAR> tistringstream;
typedef std::basic_stringstream<TCHAR> tstringstream;

#ifdef OS_WINDOWS
	#define FILE_SEPARATOR "\\"
#else
	#define FILE_SEPARATOR "/"
#endif

// global objects

// program specific compile time configuration options


#endif /* GLOBAL_H_ */
