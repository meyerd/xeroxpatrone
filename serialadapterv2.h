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

#ifndef _SERIALADAPTERV2_H_
#define _SERIALADAPTERV2_H_

#include "global.h"

#include <wx/wx.h>
#include <wx/filepicker.h>
#include <wx/choicebk.h>
#include <wx/file.h>

#include <vector>

#include "programmingadapter.h"

#define EEPROM_DEVADDR_SERIAL_READ 0xA1
#define EEPROM_DEVADDR_SERIAL_WRITE 0xA0

class SerialAdapterV2 : public ProgrammingAdapter {
  public:
	SerialAdapterV2(wxPanel* pPanel, const wxPoint& pos = wxPoint(0,0));
	~SerialAdapterV2();

	void OnWriteClick(wxCommandEvent& event);
	void OnReadClick(wxCommandEvent& event);

	void OnReadFilePickerChanged(wxFileDirPickerEvent& event);
	void OnWriteFilePickerChanged(wxFileDirPickerEvent& event);
	
	void OnDeviceChoice(wxCommandEvent& event);

	bool Init();
  bool OnShow();

	wxString xsWriteFile;
  wxString xsReadFile;

	wxChoice* xDeviceChooser;

  private:
#ifdef OS_WINDOWS
	HANDLE handle;
#endif
  wxArrayString xslAdapterDevices;
  int iSelectedAdapter;
  protected:
    DECLARE_EVENT_TABLE();
};

class SerialAdapterV2Helper {
	typedef unsigned char __u8;
#ifdef OS_WINDOWS
public:
	static void set_sda(int out, HANDLE handle);
	static void set_scl(int out, HANDLE handle);
	static int get_sda(HANDLE handle);
	static int get_scl(HANDLE handle);
	static void delay();
	static void pulse();
	
	static void start(HANDLE handle);
	static void stop(HANDLE handle);
	static int checkack(HANDLE handle);
	static void ack(HANDLE handle);
	static void nack(HANDLE handle);

	static void sendbyte(__u8 out, HANDLE handle);
	static void recvbyte(__u8* in, HANDLE handle);
#endif
};

#endif /* _SERIALADAPTERV2_H_ */
