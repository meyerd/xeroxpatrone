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

  wxBoxSizer* xTopSizer = new wxBoxSizer(wxVERTICAL);

  wxStaticText* xChooseText = new wxStaticText(this, wxID_ANY, _T("Choose Adapter Type:"));
  xTopSizer->Add(xChooseText, 0, wxALL, 10);

  wxChoicebook* xAdapterChoiceBook = new wxChoicebook(this, wxID_ANY);

  UsbAdapter* xUsbPage = new UsbAdapter((wxPanel*)xAdapterChoiceBook, wxDefaultPosition);
  xAdapterChoiceBook->AddPage(xUsbPage, xUsbPage->GetAdapterName(), true);

  SerialAdapterV2* xCOMv2Page = new SerialAdapterV2((wxPanel*)xAdapterChoiceBook, wxDefaultPosition);
  xAdapterChoiceBook->AddPage(xCOMv2Page, xCOMv2Page->GetAdapterName(), false);

  SerialAdapterV1* xCOMv1Page = new SerialAdapterV1((wxPanel*)xAdapterChoiceBook, wxDefaultPosition);
  xAdapterChoiceBook->AddPage(xCOMv1Page, xCOMv1Page->GetAdapterName(), false);

  xTopSizer->Add(xAdapterChoiceBook, 1, wxEXPAND | wxALL, 2);
  SetAutoLayout(true);
  SetSizer(xTopSizer);
  Layout();

  /* wxArrayString xEEPROMType;
  xEEPROMType.Add(_T("24c02"));
  xEEPROMType.Add(_T("24c04")); */
}


MainPanel::~MainPanel() {

}

BEGIN_EVENT_TABLE(MainPanel, wxPanel)
END_EVENT_TABLE()
