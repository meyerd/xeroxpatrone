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

MainPanel* xMainPanel = NULL;

MainPanel::MainPanel(wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size,
	long style, const wxString& name) : wxPanel(parent, winid, pos, size, style, name) {

  xMainPanel = this;

  wxBoxSizer* xTopSizer = new wxBoxSizer(wxVERTICAL);

  wxStaticText* xChooseText = new wxStaticText(this, wxID_ANY, _T("Choose Adapter Type:"));
  xTopSizer->Add(xChooseText, 0, wxALL, 10);

  wxChoicebook* xAdapterChoiceBook = new wxChoicebook(this, ID_MAINPANEL_pagechange);

  UsbAdapter* xUsbPage = new UsbAdapter((wxPanel*)xAdapterChoiceBook, wxDefaultPosition);
#ifdef OS_WINDOWS
  xAdapterChoiceBook->AddPage(xUsbPage, xUsbPage->GetAdapterName(), true);
#else
  xAdapterChoiceBook->AddPage(xUsbPage, xUsbPage->GetAdapterName(), true);
#endif
  if(!xUsbPage->OnShow()) {
    wxLogMessage(_T("MainPanel: %s OnShow Event failed."), xUsbPage->GetAdapterName().c_str());
  }

#ifndef OS_WINDOWS
  UsbAdapterKernel* xUsbKernelPage = new UsbAdapterKernel((wxPanel*)xAdapterChoiceBook, wxDefaultPosition);
  xAdapterChoiceBook->AddPage(xUsbKernelPage, xUsbKernelPage->GetAdapterName(), false);
  if(!xUsbKernelPage->OnShow()) {
    wxLogMessage(_T("MainPanel: %s OnShow Event failed."), xUsbKernelPage->GetAdapterName().c_str());
  }
#endif

#ifdef OS_WINDOWS
  SerialAdapterV2* xCOMv2Page = new SerialAdapterV2((wxPanel*)xAdapterChoiceBook, wxDefaultPosition);
  xAdapterChoiceBook->AddPage(xCOMv2Page, xCOMv2Page->GetAdapterName(), false);
  if(!xCOMv2Page->OnShow()) {
    wxLogMessage(_T("MainPanel: %s OnShow Event failed."), xCOMv2Page->GetAdapterName().c_str());
  }

  SerialAdapterV1* xCOMv1Page = new SerialAdapterV1((wxPanel*)xAdapterChoiceBook, wxDefaultPosition);
  xAdapterChoiceBook->AddPage(xCOMv1Page, xCOMv1Page->GetAdapterName(), false);
  if(!xCOMv1Page->OnShow()) {
    wxLogMessage(_T("MainPanel: %s OnShow Event failed."), xCOMv1Page->GetAdapterName().c_str());
  }
#endif

  // xAdapterChoiceBook->Connect(wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, wxNotebookEventHandler(MainPanel::OnChangeNotebookPage));
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

void MainPanel::OnChangeNotebookPage(wxNotebookEvent& event) {
    // we get events propagated from the nested choicebooks, this is bad ...
    // return;
    // int iOldPage = event.GetOldSelection();
    int iNewPage = event.GetSelection();
    wxLogMessage(_T("MainPanel: changed tab to %i"), iNewPage);
    wxChoicebook* xAdapterChoiceBook = NULL;
    wxObject* xObj = event.GetEventObject();
    if(xObj->IsSameAs(*xMainPanel)) {
        xAdapterChoiceBook = dynamic_cast<wxChoicebook*>(xObj);
        if(xAdapterChoiceBook) {
            ProgrammingAdapter* pAdapter = (ProgrammingAdapter*)xAdapterChoiceBook->GetPage(iNewPage);
            if(!pAdapter->OnShow()) {
                wxLogMessage(_T("MainPanel: %s OnShow Event failed."), pAdapter->GetAdapterName().c_str());
            }
        }
    }
}

BEGIN_EVENT_TABLE(MainPanel, wxPanel)
    EVT_NOTEBOOK_PAGE_CHANGED(ID_MAINPANEL_pagechange, MainPanel::OnChangeNotebookPage)
END_EVENT_TABLE()
