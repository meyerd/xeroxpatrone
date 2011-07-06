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

#include "xeroxpatrone.h"

typedef unsigned char __u8;

UsbAdapter* xUsbAdapter = NULL;

UsbAdapter::UsbAdapter(wxPanel* pPanel, const wxPoint& pos) : ProgrammingAdapter(pPanel, pos),
    xsReadFile(wxT("")), xsWriteFile(wxT("")), handle(NULL) {

    xUsbAdapter = this;

    SetAdapterName(_T("USB Adapter (libusb)"));
    SetAdapterType(USB);

    wxBoxSizer* xTopSizer = new wxBoxSizer(wxVERTICAL);

    /*wxChoice* */xDeviceChooser = new wxChoice(this, ID_USBADAPTER_chooser, wxDefaultPosition, wxDefaultSize);
    xTopSizer->Add(xDeviceChooser, 0, wxEXPAND | wxTOP | wxBOTTOM, 2);

    wxChoicebook* xModeChoiceBook = new wxChoicebook(this, ID_USBADAPTER_choicebook);

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
    xReadFilePicker->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(UsbAdapter::OnReadFilePickerChanged), NULL, NULL);
    xReadBoxSizer->Add(xReadFilePicker, 0, wxEXPAND | wxALL, 2);
    wxButton* xReadStartButton = new wxButton(xReadPage, wxID_ANY, _T("Read ..."));
    xReadStartButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UsbAdapter::OnReadClick), NULL, NULL);
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
    xWriteFilePicker->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(UsbAdapter::OnWriteFilePickerChanged), NULL, NULL);
    xWriteBoxSizer->Add(xWriteFilePicker, 0, wxEXPAND | wxALL, 2);
    wxButton* xWriteStartButton = new wxButton(xWritePage, wxID_ANY, _T("Write ..."));
    xWriteStartButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UsbAdapter::OnWriteClick), NULL, NULL);
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

    wxLogMessage(_T("UsbAdapter: idle ..."));
}

UsbAdapter::~UsbAdapter() {
    if(handle) {
#ifndef OS_WINDOWS
        if(usb_release_interface(handle, 0)) {
            wxLogMessage(_T("UsbAdapter: failed to release usb interface."));
        }
#endif
        usb_close(handle);
        handle = NULL;
    }
}

void UsbAdapter::OnDeviceChoice(wxCommandEvent& event) {
    iSelectedAdapter = event.GetSelection();
}

void UsbAdapter::OnReadFilePickerChanged(wxFileDirPickerEvent& event) {
    xUsbAdapter->xsReadFile = event.GetPath();
}

void UsbAdapter::OnWriteFilePickerChanged(wxFileDirPickerEvent& event) {
    xUsbAdapter->xsWriteFile = event.GetPath();
}

bool UsbAdapter::OnShow() {
    usb_init();
    usb_find_busses();
    usb_find_devices();

    struct usb_bus* bus;
    struct usb_device* dev;

    for(bus = usb_get_busses(); bus; bus = bus->next) {
        for(dev = bus->devices; dev; dev = dev->next) {
            if((dev->descriptor.idVendor == I2C_TINY_USB_VID) &&
                (dev->descriptor.idProduct == I2C_TINY_USB_PID)) {
                    xslAdapterDevices.Add(wxString(wxT("i2c-tiny-usb @ "))
#ifndef OS_WINDOWS
						+ wxString(wxT("bus "))
#endif
						+ wxString::FromAscii(bus->dirname) +
                                          wxString(wxT(" device ")) + wxString::FromAscii(dev->filename));
                    vDevs.push_back(dev);
                    iSelectedAdapter = xslAdapterDevices.GetCount() - 1;
            }
        }
    }

    xDeviceChooser->Clear();
    xDeviceChooser->Append(xslAdapterDevices);
    xDeviceChooser->SetSelection(iSelectedAdapter);

    if(xslAdapterDevices.GetCount() <= 0) {
        wxLogMessage(_T("UsbAdapter: didn't find a i2c-tiny-usb device"));
		xDeviceChooser->Append(wxString(wxT("No i2c-tiny-usb device found")));
		xDeviceChooser->SetSelection(0);
        return false;
    }

    return true;
}

bool UsbAdapter::i2c_tiny_usb_set(usb_dev_handle* handle, unsigned char cmd, int value) {
    if(usb_control_msg(handle, USB_TYPE_VENDOR, cmd, value, 0, NULL, 0, 1000) < 0) {
        wxLogMessage(_T("UsbAdapter: i2c_tiny_usb_set failed."));
        return false;
    }
    return true;
}

bool UsbAdapter::i2c_tiny_usb_write(usb_dev_handle* handle, int request, int value, int index) {
    if(usb_control_msg(handle, USB_CTRL_OUT, request, value, index, NULL, 0, 1000) < 0) {
        wxLogMessage(_T("UsbAdapter: i2c_tiny_usb_write failed."));
        return false;
    }
    return true;
}

int UsbAdapter::i2c_tiny_usb_read(usb_dev_handle* handle, unsigned char cmd, char* data, int len) {
    int nBytes = 0;
    if((nBytes = usb_control_msg(handle, USB_CTRL_IN, cmd, 0, 0, data, len, 1000)) < 0) {
        wxLogMessage(_T("UsbAdapter: i2c_tiny_usb_read failed."));
        return nBytes;
    }
    return 0;
}

int UsbAdapter::i2c_tiny_usb_get_status(usb_dev_handle* handle) {
    int i = 0;
    char status = 0;
    if((i = i2c_tiny_usb_read(handle, CMD_GET_STATUS, &status, sizeof(status))) < 0) {
        wxLogMessage(_T("UsbAdapter: i2c_tiny_usb_get_status error reading status."));
        return i;
    }
    return status;
}

bool UsbAdapter::i2c_eeprom_set_read_start(usb_dev_handle* handle, unsigned char addr, unsigned char start) {
    if(usb_control_msg(handle, USB_CTRL_OUT, CMD_I2C_IO + CMD_I2C_BEGIN, 0, addr, (char*)&start, 1, 1000) < 1) {
        wxLogMessage(_T("UsbAdapter: i2c_eeprom_set_read_start failed."));
        return false;
    }

    if(i2c_tiny_usb_get_status(handle) != STATUS_ADDRESS_ACK) {
        wxLogMessage(_T("UsbAdapter: i2c_eeprom_set_read_start got NACK"));
        return false;
    }
    return true;
}

bool UsbAdapter::i2c_eeprom_continue_read(usb_dev_handle* handle, unsigned char addr, unsigned char** data, size_t* len) {
    *data = new unsigned char[256];
    memset(*data, 0, sizeof(unsigned char) * 256);
    *len = 256;
    if(usb_control_msg(handle, USB_CTRL_OUT, CMD_I2C_IO + CMD_I2C_BEGIN + CMD_I2C_END, I2C_M_RD, addr, (char*)(*data), *len, 1000) < 256) {
        wxLogMessage(_T("UsbAdapter: i2c_eeprom_continue_read failed."));
        return false;
    }

    if(i2c_tiny_usb_get_status(handle) != STATUS_ADDRESS_ACK) {
        wxLogMessage(_T("UsbAdapter: i2c_eeprom_continue_read got NACK"));
        return false;
    }
    return true;
}

bool UsbAdapter::i2c_write_cmd_and_byte(usb_dev_handle* handle, unsigned char addr, unsigned char cmd, unsigned char data) {
    char msg[2];

    msg[0] = cmd;
    msg[1] = data;

    if(usb_control_msg(handle, USB_CTRL_OUT, CMD_I2C_IO + CMD_I2C_BEGIN + CMD_I2C_END,
                       0, addr, msg, 2, 1000) < 1) {
        wxLogMessage(_T("UsbAdapter: i2c_write_cmd_and_byte failed."));
        return false;
    }
    if(i2c_tiny_usb_get_status(handle) != STATUS_ADDRESS_ACK) {
        wxLogMessage(_T("UsbAdapter: i2c_write_cmd_and_byte got NACK"));
        return false;
    }
    return true;
}

bool UsbAdapter::i2c_eeprom_read_one_byte(usb_dev_handle* handle, unsigned char addr, unsigned char start, unsigned char* data) {
    *data = 0x0;

    if(usb_control_msg(handle, USB_CTRL_OUT, CMD_I2C_IO + CMD_I2C_BEGIN, 0, addr, (char*)&start, 1, 1000) < 1) {
        wxLogMessage(_T("UsbAdapter: i2c_eeprom_read_one_byte address setup failed."));
        return false;
    }
    if(i2c_tiny_usb_get_status(handle) != STATUS_ADDRESS_ACK) {
        wxLogMessage(_T("UsbAdapter: i2c_eeprom_read_one_byte address setup got NAK"));
        return false;
    }

    if(usb_control_msg(handle, USB_CTRL_IN, CMD_I2C_IO + CMD_I2C_END, I2C_M_RD, addr, (char*)data, 1, 1000) < 1) {
        wxLogMessage(_T("UsbAdapter: i2c_eeprom_read_one_byte byte intput failed."));
        return false;
    }
    if(i2c_tiny_usb_get_status(handle) != STATUS_ADDRESS_ACK) {
        wxLogMessage(_T("UsbAdapter: i2c_eeprom_read_one_byte byte input got NAK"));
        return false;
    }

    // wxLogMessage(_T("UsbAdapter: result 0x%x 0x%x"), tmp[0], tmp[1]);

    return true;
}

void UsbAdapter::OnWriteClick(wxCommandEvent& event) {
    wxButton* pButton = (wxButton*)event.GetEventObject();
    pButton->Disable();
    pButton->SetLabel(_T("Writing ..."));
    pButton->Update();
    wxLogMessage(_T("UsbAdapter: Starting write ..."));
    if(xUsbAdapter->xsWriteFile != wxT("")) {
        if(xUsbAdapter->Init()) {
            wxFile xInput;
            xInput.Open(xUsbAdapter->xsWriteFile, wxFile::read);
            if(xInput.IsOpened()) {
                size_t fsize = 0;
                xInput.SeekEnd(0);
                fsize = xInput.Tell();
                xInput.Seek(0);
                if(fsize < 256) {
                    wxLogMessage(_T("UsbAdapter: input file too short (%i < 256 bytes)"), fsize);
                    xInput.Close();
                    goto out_error;
                }
                wxLogMessage(_T("UsbAdapter: input file size %i bytes"), fsize);

                int write_addr_counter = 0;
                __u8 write_addr = 0x0;
                __u8 buf = 0x0;
                int tmp = 0;
                for(write_addr_counter = 0; write_addr_counter < 256; write_addr_counter += 1) {
                    write_addr = (__u8)write_addr_counter;
                    tmp = xInput.Read(&buf, 1);
                    if(tmp < 1) {
                        wxLogMessage(_T("UsbAdapter: error reading from input file at byte %i"), write_addr_counter);
                        goto out_error;
                    }
                    if(!xUsbAdapter->i2c_write_cmd_and_byte(xUsbAdapter->handle, EEPROM_DEVADDR, write_addr, buf)) {
                        wxLogMessage(_T("UsbAdapter: error writing byte to EEPROM at byte %i"), write_addr_counter);
                        goto out_error;
                    }
                }
                xInput.Close();
            } else {
                wxLogMessage(_T("UsbAdapter: error opening input file '%s'"), xUsbAdapter->xsReadFile.c_str());
                goto out_error;
            }
        } else {
            wxLogMessage(_T("UsbAdapter: device initialization failed."));
            goto out_error;
        }
    } else {
        wxLogMessage(_T("UsbAdapter: no file selected."));
        goto out_error;
    }
    wxMessageBox(_T("Wrote 256 bytes."), _T("Success"), wxICON_INFORMATION);
goto out;
out_error:
    wxMessageBox(_T("Error writing EEPROM (consult log window)"), _T("Error"), wxICON_ERROR);
out:
    wxLogMessage(_T("UsbAdapter: idle ..."));
    pButton->Enable();
    pButton->SetLabel(_T("Write"));
    pButton->Update();
}

void UsbAdapter::OnReadClick(wxCommandEvent& event) {
	wxButton* pButton = (wxButton*)event.GetEventObject();
	pButton->Disable();
	pButton->SetLabel(_T("Reading ..."));
	pButton->Update();
	wxLogMessage(_T("UsbAdapter: Starting read ..."));
	if(xUsbAdapter->xsReadFile != wxT("")) {
	  if(xUsbAdapter->Init()) {
		wxFile xOutput;
		xOutput.Open(xUsbAdapter->xsReadFile, wxFile::write);
		if(xOutput.IsOpened()) {
		  // start reading from address 0x0

          int start_addr_counter = 0;
		  __u8 start_addr = 0x0;
		  __u8 buf = 0x0;
		  int tmp = 0;

          for(start_addr_counter = 0; start_addr_counter < 256; start_addr_counter += 1) {
            start_addr = (__u8)start_addr_counter;
            if(!xUsbAdapter->i2c_eeprom_read_one_byte(xUsbAdapter->handle, EEPROM_DEVADDR, start_addr, &buf)) {
                wxLogMessage(_T("UsbAdapter: error reading one byte at address 0x%x"), start_addr);
                goto out_error;
            }
            // wxLogMessage(_T("UsbAdapter: read 0x%x"), buf);
            tmp = xOutput.Write(&buf, 1);
            if(tmp < 1) {
                wxLogMessage(_T("UsbAdapter: error writing to output file at address 0x%x"), start_addr);
                goto out_error;
            }
          }
		  xOutput.Close();
		} else {
		  wxLogMessage(_T("UsbAdapter: error opening output file '%s'"), xUsbAdapter->xsReadFile.c_str());
		  goto out_error;
		}
	  } else {
		wxLogMessage(_T("UsbAdapter: device initialization failed."));
		goto out_error;
	  }
	} else {
	  wxLogMessage(_T("UsbAdapter: no file selected."));
	  goto out_error;
	}
	wxMessageBox(_T("Read 256 bytes."), _T("Success"), wxICON_INFORMATION);
	goto out;
out_error:
	wxMessageBox(_T("Error reading EEPROM (consult log window)"), _T("Error"), wxICON_ERROR);
out:
	wxLogMessage(_T("UsbAdapter: idle ..."));
	pButton->Enable();
	pButton->SetLabel(_T("Read"));
	pButton->Update();
}

bool UsbAdapter::Init() {
    if(handle) {
#ifndef OS_WINDOWS
        if(usb_release_interface(handle, 0)) {
            wxLogMessage(_T("UsbAdapter: failed to release usb interface."));
        }
#endif
        usb_close(handle);
        handle = NULL;
    }

    if(!(handle = usb_open(vDevs[iSelectedAdapter]))) {
        wxLogMessage(_T("UsbAdapter: failed to open device '%s'"), xslAdapterDevices[iSelectedAdapter].c_str());
        handle = NULL;
        return false;
    }
#ifndef OS_WINDOWS
    if(usb_claim_interface(handle, 0) != 0) {
        wxLogMessage(_T("UsbAdapter: failed to claim usb interface."));
        usb_close(handle);
        handle = NULL;
        return false;
    }
#endif

    wxLogMessage(_T("UsbAdapter: probing for EEPROM device at addr 0x%x"), EEPROM_DEVADDR);
    if(usb_control_msg(handle, USB_CTRL_IN, CMD_I2C_IO + CMD_I2C_BEGIN + CMD_I2C_END,
                       0, EEPROM_DEVADDR, NULL, 0, 1000) < 0) {
        wxLogMessage(_T("UsbAdapter: error probing: USB error."));
        return false;
    }
    if(i2c_tiny_usb_get_status(handle) != STATUS_ADDRESS_ACK) {
        wxLogMessage(_T("UsbAdapter: error probing: EEPROM not found at addr 0x%x"), EEPROM_DEVADDR);
        return false;
    }
    return true;
}

BEGIN_EVENT_TABLE(UsbAdapter, wxPanel)
    EVT_CHOICE(ID_USBADAPTER_chooser, UsbAdapter::OnDeviceChoice)
END_EVENT_TABLE()
