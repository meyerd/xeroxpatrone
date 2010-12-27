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

#include <wx/textfile.h>
#include <wx/regex.h>
#include <wx/dir.h>

#include "xeroxpatrone.h"

#ifndef OS_WINDOWS
// #include <linux/i2c-dev.h>
#include "i2c-dev.h" // stolen from eeprog (http://codesing.org/eeprog.html)
#endif

UsbAdapterKernel* xUsbAdapterKernel = NULL;

UsbAdapterKernel::UsbAdapterKernel(wxPanel* pPanel, const wxPoint& pos) : ProgrammingAdapter(pPanel, pos),
    xsReadFile(wxT("")), xsWriteFile(wxT("")) {

    xUsbAdapterKernel = this;

    SetAdapterName(_T("USB Adapter (Kernel Driver)"));
    SetAdapterType(USB);

    wxBoxSizer* xTopSizer = new wxBoxSizer(wxVERTICAL);

    /*wxChoice* */xDeviceChooser = new wxChoice(this, ID_USBADAPTERKERNEL_chooser, wxDefaultPosition, wxDefaultSize);
    xTopSizer->Add(xDeviceChooser, 0, wxEXPAND | wxTOP | wxBOTTOM, 2);

    wxChoicebook* xModeChoiceBook = new wxChoicebook(this, ID_USBADAPTERKERNEL_choicebook);
    // xModeChoiceBook->Connect(wxEVT_COMMAND_BOOKCTRL_PAGE_CHANGED, wxNotebookEventHandler(UsbAdapterKernel::OnChangeNotebookPage));

    wxPanel* xWritePage = new wxPanel(xModeChoiceBook);
    wxPanel* xReadPage = new wxPanel(xModeChoiceBook);

    xModeChoiceBook->AddPage(xWritePage, _T("Write"), false);
    xModeChoiceBook->AddPage(xReadPage, _T("Read"), true);


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
    /*wxTextCtrl* xWriteStatusText = new wxTextCtrl(xWritePage, wxID_ANY, _T("idle ..."), wxDefaultPosition, wxDefaultSize,
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

void UsbAdapterKernel::OnChangeNotebookPage(wxNotebookEvent& event) {
    // catch event and prevent to propagate up to main panel
    event.Skip();
    return;
}

void UsbAdapterKernel::OnDeviceChoice(wxCommandEvent& event) {
    iSelectedAdapter = event.GetSelection();
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
    wxButton* pButton = (wxButton*)event.GetEventObject();
    pButton->Disable();
    pButton->SetLabel(_T("Writing ..."));
    pButton->Update();
    wxLogMessage(_T("UsbAdapterKernel: Starting write ..."));
    if(xUsbAdapterKernel->xsWriteFile != wxT("")) {
        if(xUsbAdapterKernel->Init()) {
            wxFile xInput;
            // wxLogMessage(_T("UsbAdapterKernel: output file '%s'"), xUsbAdapterKernel->xsReadFile.c_str());
            xInput.Open(xUsbAdapterKernel->xsWriteFile, wxFile::read);
            if(xInput.IsOpened()) {
                size_t fsize = 0;
                xInput.SeekEnd(0);
                fsize = xInput.Tell();
                xInput.Seek(0);
                if(fsize < 256) {
                    wxLogMessage(_T("UsbAdapterKernel: input file too short (%i < 256 bytes)"), fsize);
                    xInput.Close();
                    goto out_error;
                }
                wxLogMessage(_T("UsbAdapterKernel: input file size %i bytes"), fsize);
                // start reading from address 0x0
                int fd = xUsbAdapterKernel->xDeviceFile.fd();

                int write_addr_counter = 0;
                __u8 write_addr = 0x0;
                __u8 buf = 0x0;
                int tmp = 0;
                for(write_addr_counter = 0; write_addr_counter < 256; write_addr_counter += 1) {
                    write_addr = (__u8)write_addr_counter;
                    tmp = xInput.Read(&buf, 1);
                    if(tmp < 1) {
                        wxLogMessage(_T("UsbAdapterKernel: error reading from input file at byte %i"), write_addr_counter);
                        goto out_error;
                    }
                    if(i2c_smbus_write_byte_data(fd, write_addr, buf) < 0) {
                        wxLogMessage(_T("UsbAdapterKernel: error writing byte to EEPROM at byte %i"), write_addr_counter);
                        goto out_error;
                    }
                }
                xInput.Close();
            } else {
                wxLogMessage(_T("UsbAdapterKernel: error opening input file '%s'"), xUsbAdapterKernel->xsReadFile.c_str());
                goto out_error;
            }
        } else {
            wxLogMessage(_T("UsbAdapterKernel: device initialization failed."));
            goto out_error;
        }
    } else {
        wxLogMessage(_T("UsbAdapterKernel: no file selected."));
        goto out_error;
    }
    wxLogMessage(_T("UsbAdapterKernel: idle ..."));
    wxMessageBox(_T("Wrote 256 bytes."), _T("Success"), wxICON_INFORMATION);
goto out;
out_error:
    wxMessageBox(_T("Error writing EEPROM (consult log window)"), _T("Error"), wxICON_ERROR);
out:
    wxLogMessage(_T("UsbAdapterKernel: idle ..."));
    pButton->Enable();
    pButton->SetLabel(_T("Write"));
    pButton->Update();
}

void UsbAdapterKernel::OnReadClick(wxCommandEvent& event) {
    wxButton* pButton = (wxButton*)event.GetEventObject();
    pButton->Disable();
    pButton->SetLabel(_T("Reading ..."));
    pButton->Update();
    wxLogMessage(_T("UsbAdapterKernel: Starting read ..."));
    if(xUsbAdapterKernel->xsReadFile != wxT("")) {
        if(xUsbAdapterKernel->Init()) {
            wxFile xOutput;
            // wxLogMessage(_T("UsbAdapterKernel: output file '%s'"), xUsbAdapterKernel->xsReadFile.c_str());
            xOutput.Open(xUsbAdapterKernel->xsReadFile, wxFile::write);
            if(xOutput.IsOpened()) {
                // start reading from address 0x0
                int fd = xUsbAdapterKernel->xDeviceFile.fd();
                __u8 start_addr = 0x0;
                __u8 buf = 0x0;
                int tmp = 0;
                if(i2c_smbus_write_byte(fd, start_addr) < 0) {
                    wxLogMessage(_T("UsbAdapterKernel: error sending reading start address (0x%x)"), start_addr);
                    goto out_error;
                }
                for(int i = 0; i < 256; i++) {
                    tmp = i2c_smbus_read_byte(fd);
                    if(tmp < 0) {
                        wxLogMessage(_T("UsbAdapterKernel: error reading byte at 0x%x"), (int)start_addr + i);
                        goto out_error;
                    }
                    buf = tmp & 0xff;
                    // wxLogMessage(_T("UsbAdapterKernel: read 0x%x"), buf);
                    tmp = xOutput.Write(&buf, 1);
                    if(tmp < 1) {
                        wxLogMessage(_T("UsbAdapterKernel: error writing to output file at 0x%x"), (int)start_addr + i);
                        goto out_error;
                    }
                }
            } else {
                wxLogMessage(_T("UsbAdapterKernel: error opening output file '%s'"), xUsbAdapterKernel->xsReadFile.c_str());
                goto out_error;
            }
        } else {
            wxLogMessage(_T("UsbAdapterKernel: device initialization failed."));
            goto out_error;
        }
    } else {
        wxLogMessage(_T("UsbAdapterKernel: no file selected."));
        goto out_error;
    }
    wxMessageBox(_T("Read 256 bytes."), _T("Success"), wxICON_INFORMATION);
goto out;
out_error:
    wxMessageBox(_T("Error reading EEPROM (consult log window)"), _T("Error"), wxICON_ERROR);
out:
    wxLogMessage(_T("UsbAdapterKernel: idle ..."));
    pButton->Enable();
    pButton->SetLabel(_T("Read"));
    pButton->Update();
}

bool UsbAdapterKernel::Init() {
#ifdef OS_WINDOWS
    return false;
#else
    wxString devfile = wxString(wxT("/dev/") + xslAdapterDevices[iSelectedAdapter]);
    wxLogMessage(_T("UsbAdapterKernel: Opening device '%s'"), devfile.c_str());
    if(xDeviceFile.IsOpened()) {
        xDeviceFile.Close();
    }
    xDeviceFile.Open(devfile);
    if(!xDeviceFile.IsOpened()) {
        wxLogMessage(_T("UsbAdaperKernel: Error opening device '%s'"), devfile.c_str());
        return false;
    }
    int fd = xDeviceFile.fd();
    int addr = 0x50;
    if(ioctl(fd, I2C_SLAVE, addr) < 0) {
        wxLogMessage(_T("UsbAdapterKernel: Error sending i2c slave address 0x%x"), addr);
        return false;
    }
    return true;
#endif
}

BEGIN_EVENT_TABLE(UsbAdapterKernel, wxPanel)
    EVT_CHOICE(ID_USBADAPTERKERNEL_chooser, UsbAdapterKernel::OnDeviceChoice)
END_EVENT_TABLE()
