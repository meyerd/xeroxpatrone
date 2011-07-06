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

#include "helpwindow.h"
#include "xeroxpatrone.h"

#define HELPWINDOW_TEXT \
	"You can find further informations at: https://code.google.com/p/xeroxpatrone/wiki/XeroxpatroneV2"

HelpWindow::HelpWindow(wxWindow* pParent, const wxString& title, const wxPoint& pos, const wxSize& size) :
    wxFrame(pParent, wxID_ANY, title, pos, size) {

  wxBoxSizer* xTopSizer = new wxBoxSizer(wxVERTICAL);

  wxTextCtrl* xHelpText = new wxTextCtrl(this, wxID_ANY, _T(HELPWINDOW_TEXT), wxDefaultPosition,
                                         wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

  xTopSizer->Add(xHelpText, 1, wxEXPAND | wxALL, 2);

  wxButton* xOkButton = new wxButton(this, ID_HELPWINDOW_Ok, _T("OK"));
  //xOkButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HelpWindow::OnOk));
  xTopSizer->Add(xOkButton, 0, wxALIGN_RIGHT | wxALL, 2);

  SetAutoLayout(true);
  SetSizer(xTopSizer);
  Layout();
}


HelpWindow::~HelpWindow() {

}

void HelpWindow::OnOk(wxCommandEvent& WXUNUSED(event)) {
    Close(true);
}

void HelpWindow::OnClose(wxCloseEvent& event) {
  OnCloseWindow(event);
}

BEGIN_EVENT_TABLE(HelpWindow, wxFrame)
    EVT_CLOSE(HelpWindow::OnClose)
    EVT_BUTTON(ID_HELPWINDOW_Ok, HelpWindow::OnOk)
END_EVENT_TABLE()
