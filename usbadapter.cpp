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

#include "usbadapter.h"

#include <wx/choicebk.h>

UsbAdapter::UsbAdapter(wxPanel* pPanel, const wxPoint& pos) : ProgrammingAdapter(pPanel, pos) {
    SetAdapterName(_T("USB Adapter (libusb)"));
    SetAdapterType(USB);

    wxBoxSizer* xTopSizer = new wxBoxSizer(wxVERTICAL);

    wxChoicebook* xModeChoiceBook = new wxChoicebook(this, wxID_ANY);

    wxPanel* xWritePage = new wxPanel(xModeChoiceBook);
    wxPanel* xReadPage = new wxPanel(xModeChoiceBook);

    xModeChoiceBook->AddPage(xWritePage, _T("Write"), true);
    xModeChoiceBook->AddPage(xReadPage, _T("Read"), false);

    xTopSizer->Add(xModeChoiceBook, 1, wxEXPAND, 0);

    /* wxStaticText* xReadNoOptionsText = new wxStaticText(xReadPage, wxID_ANY, _T("No Options"), wxPoint(0, 30), wxSize(380, 25));
    wxStaticText* xWriteNoOptionsText = new wxStaticText(xWritePage, wxID_ANY, _T("No Options"), wxPoint(0, 30), wxSize(380, 25)); */

    /* read page */
    wxBoxSizer* xReadBoxSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* xReadPageText = new wxStaticText(xReadPage, wxID_ANY, _T("Read EEPROM"));
    xReadBoxSizer->Add(xReadPageText, 0, wxALL, 10);
    wxStaticText* xChooseReadFile = new wxStaticText(xReadPage, wxID_ANY, _T("Select output file:"));
    xReadBoxSizer->Add(xChooseReadFile, 0, wxALL, 2);
    wxFilePickerCtrl* xReadFilePicker = new wxFilePickerCtrl(xReadPage, wxID_ANY, wxEmptyString, _T("Select output file"),
                                                             _T("BIN files (*.bin)|*.bin|All files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize,
                                                              wxFLP_USE_TEXTCTRL | wxFLP_SAVE | wxFLP_OVERWRITE_PROMPT, wxDefaultValidator,
                                                              _T("xReadFilePicker"));
    xReadBoxSizer->Add(xReadFilePicker, 0, wxEXPAND | wxALL, 2);
    wxButton* xReadStartButton = new wxButton(xReadPage, wxID_ANY, _T("Read ..."));
    xReadStartButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UsbAdapter::OnReadClick), NULL, NULL);
    xReadBoxSizer->Add(xReadStartButton, 0, wxEXPAND | wxALL, 2);
    wxTextCtrl* xReadStatusText = new wxTextCtrl(xReadPage, wxID_ANY, _T("idle ..."), wxDefaultPosition, wxDefaultSize,
                                                 wxTE_MULTILINE | wxTE_READONLY, wxDefaultValidator, _T("xReadStatusText"));
    xReadBoxSizer->Add(xReadStatusText, 1, wxEXPAND | wxTOP | wxBOTTOM, 10);
    xReadPage->SetAutoLayout(true);
    xReadPage->SetSizer(xReadBoxSizer);
    xReadPage->Layout();


    /* write page */
    wxBoxSizer* xWriteBoxSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* xWritePageText = new wxStaticText(xWritePage, wxID_ANY, _T("Write EEPROM"));
    xWriteBoxSizer->Add(xWritePageText, 0, wxALL, 10);
    wxStaticText* xChooseWriteFile = new wxStaticText(xWritePage, wxID_ANY, _T("Select input file:"));
    xWriteBoxSizer->Add(xChooseWriteFile, 0, wxALL, 2);
    wxFilePickerCtrl* xWriteFilePicker = new wxFilePickerCtrl(xWritePage, wxID_ANY, wxEmptyString, _T("Select input file"),
                                                             _T("BIN files (*.bin)|*.bin|All files (*.*)|*.*"), wxDefaultPosition, wxDefaultSize,
                                                              wxFLP_USE_TEXTCTRL | wxFLP_OPEN | wxFLP_FILE_MUST_EXIST, wxDefaultValidator,
                                                              _T("xWriteFilePicker"));
    xWriteBoxSizer->Add(xWriteFilePicker, 0, wxEXPAND | wxALL, 2);
    wxButton* xWriteStartButton = new wxButton(xWritePage, wxID_ANY, _T("Write ..."));
    xWriteStartButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UsbAdapter::OnWriteClick), NULL, NULL);
    xWriteBoxSizer->Add(xWriteStartButton, 0, wxEXPAND | wxALL, 2);
    wxTextCtrl* xWriteStatusText = new wxTextCtrl(xWritePage, wxID_ANY, _T("idle ..."), wxDefaultPosition, wxDefaultSize,
                                                 wxTE_MULTILINE | wxTE_READONLY, wxDefaultValidator, _T("xWriteStatusText"));
    xWriteBoxSizer->Add(xWriteStatusText, 1, wxEXPAND | wxTOP | wxBOTTOM, 10);
    xWritePage->SetAutoLayout(true);
    xWritePage->SetSizer(xWriteBoxSizer);
    xWritePage->Layout();

    SetAutoLayout(true);
    SetSizer(xTopSizer);
    Layout();
}

UsbAdapter::~UsbAdapter() {

}


void UsbAdapter::OnWriteClick(wxCommandEvent& event) {
 /*   xWriteStatusText->Clear();
    xWriteStatusText->AppendText(_T("write clicked.\n"));
    xWriteStatusText->AppendText(_T("\nidle ...")); */

    wxLogMessage(_T("UsbAdapter: write clicked."));
}

void UsbAdapter::OnReadClick(wxCommandEvent& event) {
  /*  xReadStatusText->Clear();
    xReadStatusText->AppendText(_T("read clicked.\n"));
    xReadStatusText->AppendText(_T("\nidle ...")); */

    wxLogMessage(_T("UsbAdapter: read clicked."));
}

bool UsbAdapter::Init() {

}


