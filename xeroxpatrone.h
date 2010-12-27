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

#ifndef XEROXPATRONE_H_
#define XEROXPATRONE_H_

#include "global.h"

#include <wx/wx.h>

class Logger;
class MainPanel;

class XeroxpatroneApp : public wxApp {
public:

private:
  bool OnInit();
  int OnExit();
};

class XeroxpatroneMainWindow : public wxFrame {
public:
	XeroxpatroneMainWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
	~XeroxpatroneMainWindow();

	void OnQuit(wxCommandEvent& event);
	void OnRescan(wxCommandEvent& event);

	void OnClose(wxCloseEvent& event);
	void OnAbout(wxCommandEvent& event);

	void OnShowLogWindow(wxCommandEvent& event);
	void OnHideLogWindow(wxCommandEvent& event);

	void OnHelp(wxCommandEvent& event);

private:
	Logger* logger;
	MainPanel* mainPanel;

protected:
	DECLARE_EVENT_TABLE();
};

enum {
  ID_MAIN_Quit = 1,
  ID_MAIN_Rescan,
  ID_MAIN_About,
  ID_MAIN_ShowLog,
  ID_MAIN_HideLog,
  ID_MAIN_ShowHelp,
  ID_MAINPANEL_choice,
  ID_MAINPANEL_pagechange,
  ID_HELPWINDOW_Ok,
  ID_USBADAPTERKERNEL_choicebook,
  ID_USBADAPTERKERNEL_chooser,
};

#endif /* XEROXPATRONE_H_ */
