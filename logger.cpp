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

#include "logger.h"

Logger::Logger(wxWindow* pParent) : 
	oldLogTarget(NULL) {
	logger = new wxLogWindow(pParent, _T("Log Window"), false, true);
	oldLogTarget = wxLog::GetActiveTarget();
	wxLog::SetActiveTarget(logger);
//#ifdef DEBUG
	ShowLogWindow();
//#endif
}

Logger::~Logger() {
	FinishLogging();
	//SAFE_DELETE(logger); // not neccessary since logwindow is child of main window 
}

bool Logger::SetupLogging() {
	return true;
}

void Logger::FinishLogging() {
	logger->Flush();
	logger->Show(false);
	if(oldLogTarget)
		wxLog::SetActiveTarget(oldLogTarget);
}

void Logger::ShowLogWindow() {
	logger->Show(true);
}

void Logger::HideLogWindow() {
	logger->Show(false);
}

/* void Logger::LogMessage(LogLevel eLogLevel, TCHAR* fmt, va_list data) {
//#ifdef DEBUG
//	TCHAR f[4096];
//	_sntprintf_s(f, 4096, 4096, _T("%s%s"), _T("Error at %s: "), fmt);
//	_ftprintf(stderr, f, AT, data);
//#endif
	if(eLogLevel == LOG_INFO) {
		wxLogInfo(fmt, data);
	} else if(eLogLevel == LOG_WARN) {
		wxLogWarning(fmt, data);
	} else if(eLogLevel == LOG_ERROR) {
		wxLogError(fmt, data);
	} else {
		wxLogFatalError(fmt, data);
	}
}

void Logger::LogInfo(TCHAR* fmt, va_list data) {
	LogMessage(LOG_INFO, fmt, data);
}

void Logger::LogWarn(TCHAR* fmt, va_list data) {
	LogMessage(LOG_WARN, fmt, data);
}

void Logger::LogError(TCHAR* fmt, va_list data) {
	LogMessage(LOG_ERROR, fmt, data);
} */


	
