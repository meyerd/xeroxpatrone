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

#include "mainpanel.h"

#include <wx/choicebk.h>

MainPanel::MainPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size,
	long style, const wxString& name) : wxPanel(parent, winid, pos, size, style, name) {

  wxStaticText* xChooseText = new wxStaticText(this, wxID_ANY, _T("Choose Adapter Type:"), wxPoint(5, 5), wxSize(390, 25));

  wxChoicebook* xAdapterChoiceBook = new wxChoicebook(this, wxID_ANY, wxPoint(5, 30), wxSize(390, 450));

  UsbAdapter* xUsbPage = new UsbAdapter((wxPanel*)xAdapterChoiceBook, wxPoint(5, 0));
  xAdapterChoiceBook->AddPage(xUsbPage, xUsbPage->GetAdapterName(), true);

  SerialAdapterV2* xCOMv2Page = new SerialAdapterV2((wxPanel*)xAdapterChoiceBook, wxPoint(5, 0));
  xAdapterChoiceBook->AddPage(xCOMv2Page, xCOMv2Page->GetAdapterName(), false);

  SerialAdapterV1* xCOMv1Page = new SerialAdapterV1((wxPanel*)xAdapterChoiceBook, wxPoint(5, 0));
  xAdapterChoiceBook->AddPage(xCOMv1Page, xCOMv1Page->GetAdapterName(), false);


  /* wxArrayString xEEPROMType;
  xEEPROMType.Add(_T("24c02"));
  xEEPROMType.Add(_T("24c04")); */
}


MainPanel::~MainPanel() {

}

void MainPanel::OnChoice(wxCommandEvent& event) {
}

BEGIN_EVENT_TABLE(MainPanel, wxPanel)
  // EVT_CHOICE(ID_MAINPANEL_choice, MainPanel::OnChoice)
END_EVENT_TABLE()
