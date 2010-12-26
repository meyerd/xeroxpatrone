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

#include "usbadapterkernel.h"

#include <wx/choicebk.h>
#include <wx/textfile.h>
#include <wx/regex.h>
#include <wx/dir.h>

UsbAdapterKernel* xUsbAdapterKernel = NULL;

UsbAdapterKernel::UsbAdapterKernel(wxPanel* pPanel, const wxPoint& pos) : ProgrammingAdapter(pPanel, pos),
    xsReadFile(wxT("")), xsWriteFile(wxT("")) {

    xUsbAdapterKernel = this;

    SetAdapterName(_T("USB Adapter (Kernel Driver)"));
    SetAdapterType(USB);

    wxBoxSizer* xTopSizer = new wxBoxSizer(wxVERTICAL);

    /*wxChoice* */xDeviceChooser = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                            wxArrayString());
    xTopSizer->Add(xDeviceChooser, 0, wxEXPAND | wxTOP | wxBOTTOM, 2);

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
    xReadFilePicker->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(UsbAdapterKernel::OnReadFilePickerChanged), NULL, NULL);
    xReadBoxSizer->Add(xReadFilePicker, 0, wxEXPAND | wxALL, 2);
    wxButton* xReadStartButton = new wxButton(xReadPage, wxID_ANY, _T("Read ..."));
    xReadStartButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UsbAdapterKernel::OnReadClick), NULL, NULL);
    xReadBoxSizer->Add(xReadStartButton, 0, wxEXPAND | wxALL, 2);
    /* wxTextCtrl* xReadStatusText = new wxTextCtrl(xReadPage, wxID_ANY, _T("idle ..."), wxDefaultPosition, wxDefaultSize,
                                                 wxTE_MULTILINE | wxTE_READONLY, wxDefaultValidator, _T("xReadStatusText"));
    xReadBoxSizer->Add(xReadStatusText, 1, wxEXPAND | wxTOP | wxBOTTOM, 10); */
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
    xWriteFilePicker->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(UsbAdapterKernel::OnWriteFilePickerChanged), NULL, NULL);
    xWriteBoxSizer->Add(xWriteFilePicker, 0, wxEXPAND | wxALL, 2);
    wxButton* xWriteStartButton = new wxButton(xWritePage, wxID_ANY, _T("Write ..."));
    xWriteStartButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UsbAdapterKernel::OnWriteClick), NULL, NULL);
    xWriteBoxSizer->Add(xWriteStartButton, 0, wxEXPAND | wxALL, 2);
    /* wxTextCtrl* xWriteStatusText = new wxTextCtrl(xWritePage, wxID_ANY, _T("idle ..."), wxDefaultPosition, wxDefaultSize,
                                                 wxTE_MULTILINE | wxTE_READONLY, wxDefaultValidator, _T("xWriteStatusText"));
    xWriteBoxSizer->Add(xWriteStatusText, 1, wxEXPAND | wxTOP | wxBOTTOM, 10); */
    xWritePage->SetAutoLayout(true);
    xWritePage->SetSizer(xWriteBoxSizer);
    xWritePage->Layout();

    SetAutoLayout(true);
    SetSizer(xTopSizer);
    Layout();

    wxLogMessage(_T("UsbAdapterKernel: idle ..."));
}

UsbAdapterKernel::~UsbAdapterKernel() {

}

void UsbAdapterKernel::OnReadFilePickerChanged(wxFileDirPickerEvent& event) {
    xUsbAdapterKernel->xsReadFile = event.GetPath();
}

void UsbAdapterKernel::OnWriteFilePickerChanged(wxFileDirPickerEvent& event) {
    xUsbAdapterKernel->xsWriteFile = event.GetPath();
}

bool UsbAdapterKernel::OnShow() {
#ifdef OS_WINDOWS
    return false;
#else
    xslAdapterNames.Clear();
    iSelectedAdapter = 0;
    /* wxTextFile xProcI2C;
    // look in /proc/sys/i2c
    xProcI2C.Open(_T("/proc/sys/i2c"));
    if(xProcI2C.IsOpened()) {
        wxString xsLine;
        xsLine = xProcI2C.GetFirstLine();
        while(!xProcI2C.Eof()) {
//            wxStringTokenizer xTokenizer(xsLine, _T("\t"));
            // TODO: implement this ...
            xsLine = xProcI2C.GetNextLine();
        }
    }*/
    // look in sysfs
    wxTextFile xMount;
    xMount.Open(_T("/proc/mounts"));
    wxString sysfs = _T("");
    if(xMount.IsOpened()) {
        wxString xsLine;
        xsLine = xMount.GetFirstLine();
        wxRegEx xMountRegex(wxT("^[^ ]* ([^ ]*) ([^ ]*)"));
        while(!xMount.Eof()) {
            if(xMountRegex.Matches(xsLine)) {
                wxString mountpoint = xMountRegex.GetMatch(xsLine, 1);
                wxString fstype = xMountRegex.GetMatch(xsLine, 2);
                fstype.MakeLower();
                if(fstype == wxT("sysfs")) {
                    wxLogMessage(_T("UsbAdapterKernel: Read mount line '%s', found sysfs"), xsLine.c_str());
                    sysfs = mountpoint;
                }
            }
            xsLine = xMount.GetNextLine();
        }
        xMount.Close();
    }
    if(sysfs != wxT("")) {
        sysfs = sysfs + wxT("/class/i2c-dev");
        wxLogMessage(_T("UsbAdapterKernel: Opening directory '%s'"), sysfs.c_str());
        wxDir devs(sysfs);
        if(devs.IsOpened()) {
            wxString devname;
            bool cont = devs.GetFirst(&devname);
            while(cont) {
                wxString fulldir = sysfs + wxT("/") + devname + wxT("/name");
                wxTextFile namefile;
                namefile.Open(fulldir);
                if(namefile.IsOpened()) {
                    wxString name = namefile.GetFirstLine();
                    xslAdapterNames.Add(name + wxT(" (") + devname + wxT(")"));
                    xslAdapterDevices.Add(devname);
                    if(name.Find(wxT("i2c-tiny-usb")) != wxNOT_FOUND) {
                        iSelectedAdapter = xslAdapterNames.GetCount() - 1;
                    }
                }
                cont = devs.GetNext(&devname);
            }
        }
    }

    xDeviceChooser->Clear();
    xDeviceChooser->Append(xslAdapterNames);
    xDeviceChooser->SetSelection(iSelectedAdapter);

    if(xslAdapterNames.GetCount() <= 0)
        return false;
    return true;
#endif
}

void UsbAdapterKernel::OnWriteClick(wxCommandEvent& event) {
    wxLogMessage(_T("UsbAdapterKernel: Starting write ..."));
    if(xUsbAdapterKernel->xsWriteFile != wxT("")) {

    } else {
        wxLogMessage(_T("UsbAdapterKernel: no file selected."));
    }
    wxLogMessage(_T("UsbAdapterKernel: idle ..."));
}

void UsbAdapterKernel::OnReadClick(wxCommandEvent& event) {
    wxLogMessage(_T("UsbAdapterKernel: read clicked."));
    if(xUsbAdapterKernel->xsReadFile != wxT("")) {

    } else {
        wxLogMessage(_T("UsbAdapterKernel: no file selected."));
    }
    wxLogMessage(_T("UsbAdapterKernel: idle ..."));
}

bool UsbAdapterKernel::Init() {

}


