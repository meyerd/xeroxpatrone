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

  wxArrayString xAdapterStrArray;
  ProgrammingAdapter* pUsbAdapter = new UsbAdapter();
  vAdapters.push_back(pUsbAdapter);
  xAdapterStrArray.Add(pUsbAdapter->GetName());
//#ifdef OS_WINDOWS
  ProgrammingAdapter* pAdapter = new SerialAdapterV2();
  vAdapters.push_back(pAdapter);
  xAdapterStrArray.Add(pAdapter->GetName());
  pAdapter = new SerialAdapterV1();
  vAdapters.push_back(pAdapter);
  xAdapterStrArray.Add(pAdapter->GetName());
//#endif
  wxChoice *xAdapterChooser = new wxChoice(this, ID_MAINPANEL_choice, wxPoint(5, 30), wxSize(390, 30), xAdapterStrArray);

  /* wxArrayString xEEPROMType;
  xEEPROMType.Add(_T("24c02"));
  xEEPROMType.Add(_T("24c04")); */

  wxChoicebook* xModeChoiceBook = new wxChoicebook(this, wxID_ANY, wxPoint(5, 65), wxSize(390, 450));

  iSelectedAdapter = 0;

  wxPanel* xWritePage = new wxPanel(xModeChoiceBook);
  wxStaticText* xWritePageText = new wxStaticText(xWritePage, wxID_ANY, _T("Write EEPROM"), wxPoint(5, 5), wxSize(380, 25));
  // pWriteOptionsPanel = ((UsbAdapter*)pUsbAdapter)->GetOptionControls(xWritePage, wxPoint(5, 30));

  wxPanel* xReadPage = new wxPanel(xModeChoiceBook);
  wxStaticText* xReadPageText = new wxStaticText(xReadPage, wxID_ANY, _T("Read EEPROM"), wxPoint(5, 5), wxSize(380, 25));
  // pReadOptionsPanel = ((UsbAdapter*)pUsbAdapter)->GetOptionControls(xReadPage, wxPoint(5, 30));

  xModeChoiceBook->AddPage(xWritePage, _T("Write"), true);
  xModeChoiceBook->AddPage(xReadPage, _T("Read"), false);
}


MainPanel::~MainPanel() {
    for(vector<ProgrammingAdapter*>::iterator it = vAdapters.begin(); it != vAdapters.end(); ++it) {
        SAFE_DELETE((*it));
    }
    vAdapters.clear();
}

void MainPanel::OnChoice(wxCommandEvent& event) {
    int iSelection = event.GetInt();
    wxLogMessage(_T("Selected Adapter: %s"), vAdapters[iSelection]->GetName().c_str());
    iSelectedAdapter = iSelection;
    /* if(pReadOptionsPanel) {
        pReadOptionsPanel->Destroy();
    }
    if(pWriteOptionsPanel) {
        pWriteOptionsPanel->Destroy();
    }
    if(vAdapters[iSelection]->GetType() == USB) {
        UsbAdapter* pAdapter = (UsbAdapter*)vAdapters[iSelection];
        pWriteOptionsPanel = pAdapter->GetOptionControls(xWritePage, wxPoint(5, 30));
        pReadOptionsPanel = pAdapter->GetOptionControls(xReadPage, wxPoint(5, 30));
    } else if(vAdapters[iSelection]->GetType() == COMv1) {
        SerialAdapterV1* pAdapter = (SerialAdapterV1*)vAdapters[iSelection];
        pWriteOptionsPanel = pAdapter->GetOptionControls(xWritePage, wxPoint(5, 30));
        pReadOptionsPanel = pAdapter->GetOptionControls(xReadPage, wxPoint(5, 30));
    } else if(vAdapters[iSelection]->GetType() == COMv2) {
        SerialAdapterV2* pAdapter = (SerialAdapterV2*)vAdapters[iSelection];
        pWriteOptionsPanel = pAdapter->GetOptionControls(xWritePage, wxPoint(5, 30));
        pReadOptionsPanel = pAdapter->GetOptionControls(xReadPage, wxPoint(5, 30));
    } */
}

BEGIN_EVENT_TABLE(MainPanel, wxPanel)
  EVT_CHOICE(ID_MAINPANEL_choice, MainPanel::OnChoice)
END_EVENT_TABLE()
