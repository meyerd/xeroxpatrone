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

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "global.h"

#include <wx/wx.h>
#include <wx/log.h>
#include <cstdarg>

#include "singleton.h"

#ifdef DEBUG
#define ___STR(x) #x
#define __STR(x) ___STR(x)
#define ___L(x) L ## x
#define __L(x) ___L(x)
#define __LOCATION__ L"(" __L(__FILE__) L":" __L(__STR(__LINE__)) L")"
#define AT __FILE__ ":" __STR(__LINE__)
#endif

enum LogLevel_t {
	LOG_INFO = 1,
	LOG_WARN,
	LOG_ERROR,
};

typedef enum LogLevel_t LogLevel;

class Logger {
public:
	Logger(wxWindow* pParent);
	~Logger();
	bool SetupLogging();
	void FinishLogging();
	
	void ShowLogWindow();
	void HideLogWindow();
/*	void LogMessage(LogLevel eLogLevel, TCHAR* fmt, va_list data);
	void LogInfo(TCHAR* fmt, va_list data);
	void LogWarn(TCHAR* fmt, va_list data);
	void LogError(TCHAR* fmt, va_list data); */
private:

	wxLog* oldLogTarget;
	wxLogWindow *logger;
};

#endif
