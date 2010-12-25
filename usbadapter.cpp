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
#include <wx/filepicker.h>

UsbAdapter::UsbAdapter(wxPanel* pPanel, const wxPoint& pos) : ProgrammingAdapter(pPanel, pos) {
    SetAdapterName(_T("USB Adapter (libusb)"));
    SetAdapterType(USB);

    wxChoicebook* xModeChoiceBook = new wxChoicebook(this, wxID_ANY, wxPoint(0, 5), wxSize(390, 450));

    wxPanel* xWritePage = new wxPanel(xModeChoiceBook);
    wxStaticText* xWritePageText = new wxStaticText(xWritePage, wxID_ANY, _T("Write EEPROM"), wxPoint(0, 5), wxSize(380, 25));

    wxPanel* xReadPage = new wxPanel(xModeChoiceBook);
    wxStaticText* xReadPageText = new wxStaticText(xReadPage, wxID_ANY, _T("Read EEPROM"), wxPoint(0, 5), wxSize(380, 25));

    xModeChoiceBook->AddPage(xWritePage, _T("Write"), true);
    xModeChoiceBook->AddPage(xReadPage, _T("Read"), false);

    /* wxStaticText* xReadNoOptionsText = new wxStaticText(xReadPage, wxID_ANY, _T("No Options"), wxPoint(0, 30), wxSize(380, 25));
    wxStaticText* xWriteNoOptionsText = new wxStaticText(xWritePage, wxID_ANY, _T("No Options"), wxPoint(0, 30), wxSize(380, 25)); */

    /* read page */
    wxStaticText* xChooseReadFile = new wxStaticText(xReadPage, wxID_ANY, _T("Select output file:"), wxPoint(0, 30), wxSize(380, 25));
    wxFilePickerCtrl* xReadFilePicker = new wxFilePickerCtrl(xReadPage, wxID_ANY, wxEmptyString, _T("Select output file"),
                                                             _T("BIN files (*.bin)|*.bin|All files (*.*)|*.*"), wxPoint(0, 60), wxSize(380, 25),
                                                              wxFLP_USE_TEXTCTRL | wxFLP_SAVE | wxFLP_OVERWRITE_PROMPT, wxDefaultValidator,
                                                              _T("xReadFilePicker"));

    /* write page */
    wxStaticText* xChooseWriteFile = new wxStaticText(xWritePage, wxID_ANY, _T("Select input file:"), wxPoint(0, 30), wxSize(380, 25));
    wxFilePickerCtrl* xWriteFilePicker = new wxFilePickerCtrl(xWritePage, wxID_ANY, wxEmptyString, _T("Select input file"),
                                                             _T("BIN files (*.bin)|*.bin|All files (*.*)|*.*"), wxPoint(0, 60), wxSize(380, 25),
                                                              wxFLP_USE_TEXTCTRL | wxFLP_OPEN | wxFLP_FILE_MUST_EXIST, wxDefaultValidator,
                                                              _T("xWriteFilePicker"));
}

UsbAdapter::~UsbAdapter() {

}

bool UsbAdapter::Init() {

}


