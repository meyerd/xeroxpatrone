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

#include "xeroxpatrone.h"
#include "logger.h"
#include "mainpanel.h"
#include "helpwindow.h"

bool XeroxpatroneApp::OnInit() {
#if defined(OS_WINDOWS) && defined(DEBUG)
  //_CrtSetBreakAlloc(1395);
#endif

  wxImage::AddHandler(new wxPNGHandler);

  XeroxpatroneMainWindow *mainFrame = new XeroxpatroneMainWindow(_T("Xeroxpatrone"), wxPoint(300, 100),
        wxSize(400, 600));
  mainFrame->Show(true);
  SetTopWindow(mainFrame);
  return true;
}

int XeroxpatroneApp::OnExit() {
	return 0;
}

XeroxpatroneMainWindow::XeroxpatroneMainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
  : wxFrame(NULL, -1, title, pos, size),
	logger(NULL), mainPanel(NULL) {

   logger = new Logger(this);
   logger->SetupLogging();

   wxMenu *mainMenuFile = new wxMenu;
   mainMenuFile->Append(ID_MAIN_Rescan, _T("&Scan for devices again ..."));
   mainMenuFile->AppendSeparator();
   mainMenuFile->Append(ID_MAIN_Quit, _T("&Quit..."));

   wxMenu *mainMenuQuestionmark = new wxMenu;
//#ifdef DEBUG
   mainMenuQuestionmark->Append(ID_MAIN_ShowLog, _T("&Show Log Window"));
   mainMenuQuestionmark->Append(ID_MAIN_HideLog, _T("&Hide Log Window"));
   mainMenuQuestionmark->AppendSeparator();
//#endif
   mainMenuQuestionmark->Append(ID_MAIN_ShowHelp, _T("&Help"));
   mainMenuQuestionmark->AppendSeparator();
   mainMenuQuestionmark->Append(ID_MAIN_About, _T("&About..."));

   wxMenuBar *mainMenu = new wxMenuBar;
   mainMenu->Append(mainMenuFile, _T("&File"));
   mainMenu->Append(mainMenuQuestionmark, _T("&?"));

   SetMenuBar(mainMenu);

   wxBoxSizer* xTopSizer = new wxBoxSizer(wxVERTICAL);
   mainPanel = new MainPanel(this);
   xTopSizer->Add(mainPanel, 1, wxEXPAND, 0);
   SetAutoLayout(true);
   SetSizer(xTopSizer);
   Layout();

   wxLogMessage(_T("Xeroxpatrone initalized ..."));
}

XeroxpatroneMainWindow::~XeroxpatroneMainWindow() {
	SAFE_DELETE(logger);
}

void XeroxpatroneMainWindow::OnQuit(wxCommandEvent& WXUNUSED(event)) {
  Close(true);
}

void XeroxpatroneMainWindow::OnClose(wxCloseEvent& event) {
  OnCloseWindow(event);
}

void XeroxpatroneMainWindow::OnRescan(wxCommandEvent& WXUNUSED(event)) {
    if(mainPanel) {
        mainPanel->Destroy();
    }
    wxBoxSizer* xTopSizer = new wxBoxSizer(wxVERTICAL);
    mainPanel = new MainPanel(this);
    xTopSizer->Add(mainPanel, 1, wxEXPAND, 0);
    SetAutoLayout(true);
    SetSizer(xTopSizer);
    Layout();;
}

void XeroxpatroneMainWindow::OnAbout(wxCommandEvent& WXUNUSED(event)) {
  wxMessageBox(_T("Xeroxpatrone.\n\nUses the wxWindows Toolkit."),
	  _T("About"), wxOK | wxICON_INFORMATION, this);
}

void XeroxpatroneMainWindow::OnShowLogWindow(wxCommandEvent& WXUNUSED(event)) {
	logger->ShowLogWindow();
}

void XeroxpatroneMainWindow::OnHideLogWindow(wxCommandEvent& WXUNUSED(event)) {
	logger->HideLogWindow();
}

void XeroxpatroneMainWindow::OnHelp(wxCommandEvent& WXUNUSED(event)) {
    HelpWindow* pHelpWindow = new HelpWindow(this, _T("Help"), wxDefaultPosition, wxDefaultSize);
    pHelpWindow->Show(true);
}

BEGIN_EVENT_TABLE(XeroxpatroneMainWindow, wxFrame)
  EVT_MENU(ID_MAIN_Quit, XeroxpatroneMainWindow::OnQuit)
  EVT_MENU(ID_MAIN_Rescan, XeroxpatroneMainWindow::OnRescan)
  EVT_CLOSE(XeroxpatroneMainWindow::OnClose)
  EVT_MENU(ID_MAIN_ShowLog, XeroxpatroneMainWindow::OnShowLogWindow)
  EVT_MENU(ID_MAIN_HideLog, XeroxpatroneMainWindow::OnHideLogWindow)
  EVT_MENU(ID_MAIN_About, XeroxpatroneMainWindow::OnAbout)
  EVT_MENU(ID_MAIN_ShowHelp, XeroxpatroneMainWindow::OnHelp)
END_EVENT_TABLE()

IMPLEMENT_APP(XeroxpatroneApp);
