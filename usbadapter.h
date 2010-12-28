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

#ifndef _USBADAPTER_H_
#define _USBADAPTER_H_

#include "global.h"

#include <wx/wx.h>
#include <wx/filepicker.h>
#include <wx/choicebk.h>
#include <wx/file.h>

#include <usb.h>
#include <vector>

#include "programmingadapter.h"

using namespace std;

/* most of the i2c_tiny_usb code is stolen from the
 * test project of i2c-tiny-usb ...
 * http://www.harbaum.org/till/i2c_tiny_usb/index.shtml */
#define USB_CTRL_IN    (USB_TYPE_CLASS | USB_ENDPOINT_IN)
#define USB_CTRL_OUT   (USB_TYPE_CLASS)

#define I2C_TINY_USB_VID  0x0403
#define I2C_TINY_USB_PID  0xc631

#define I2C_M_RD        0x01

/* commands via USB, must e.g. match command ids firmware */
#define CMD_ECHO       0
#define CMD_GET_FUNC   1
#define CMD_SET_DELAY  2
#define CMD_GET_STATUS 3
#define CMD_I2C_IO     4
#define CMD_I2C_BEGIN  1  // flag to I2C_IO
#define CMD_I2C_END    2  // flag to I2C_IO

#define STATUS_IDLE          0
#define STATUS_ADDRESS_ACK   1
#define STATUS_ADDRESS_NAK   2

#define EEPROM_DEVADDR 0x50

class UsbAdapter : public ProgrammingAdapter {
  public:
	UsbAdapter(wxPanel* pPanel, const wxPoint& pos = wxPoint(0,0));
	~UsbAdapter();

	void OnWriteClick(wxCommandEvent& event);
	void OnReadClick(wxCommandEvent& event);

	void OnReadFilePickerChanged(wxFileDirPickerEvent& event);
	void OnWriteFilePickerChanged(wxFileDirPickerEvent& event);

	bool Init();
	bool OnShow();

    wxString xsWriteFile;
    wxString xsReadFile;

    wxChoice* xDeviceChooser;

private:
    usb_dev_handle* handle;
    vector<struct usb_device*> vDevs;
    wxArrayString xslAdapterDevices;
    int iSelectedAdapter;

    bool i2c_tiny_usb_set(usb_dev_handle* handle, unsigned char cmd, int value);
    bool i2c_tiny_usb_write(usb_dev_handle* handle, int request, int value, int index);
    int i2c_tiny_usb_read(usb_dev_handle* handle, unsigned char cmd, char* data, int len);
    int i2c_tiny_usb_get_status(usb_dev_handle* handle);

    bool i2c_eeprom_set_read_start(usb_dev_handle* handle, unsigned char addr, unsigned char start);
    bool i2c_eeprom_read_one_byte(usb_dev_handle* handle, unsigned char addr, unsigned char start, unsigned char* data);
    bool i2c_eeprom_continue_read(usb_dev_handle* handle, unsigned char addr, unsigned char** data, size_t* len);
    bool i2c_write_cmd_and_byte(usb_dev_handle* handle, unsigned char addr, unsigned char cmd, unsigned char data);


    /* wxFilePickerCtrl* xReadFilePicker; */
    /* wxTextCtrl* xReadStatusText; */
    /* wxFilePickerCtrl* xWriteFilePicker; */
    /* wxTextCtrl* xWriteStatusText; */
};

#endif /* _USBADAPTER_H_ */
