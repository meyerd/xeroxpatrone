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

#include "serialadapterv2.h"
#include "xeroxpatrone.h"

typedef unsigned char __u8;

SerialAdapterV2* xSerialAdapterV2 = NULL;

SerialAdapterV2::SerialAdapterV2(wxPanel* pPanel, const wxPoint& pos) : ProgrammingAdapter(pPanel, pos)
#ifdef OS_WINDOwS
	,xsReadFile(wxT("")), xsWriteFile(wxT("")), handle(NULL) 
#endif
{
	SetAdapterName(_T("Serial Adapter (v2, \"Easy Adapter\")"));
	SetAdapterType(COMv1);

	xSerialAdapterV2 = this;
#ifdef OS_WINDOWS

	wxBoxSizer* xTopSizer = new wxBoxSizer(wxVERTICAL);

	/*wxChoice* */xDeviceChooser = new wxChoice(this, ID_SERIALADAPTERV2_chooser, wxDefaultPosition, wxDefaultSize);
	xTopSizer->Add(xDeviceChooser, 0, wxEXPAND | wxTOP | wxBOTTOM, 2);

	wxChoicebook* xModeChoiceBook = new wxChoicebook(this, wxID_ANY, wxPoint(0, 5), wxSize(390, 450));

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
	xReadFilePicker->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(SerialAdapterV2::OnReadFilePickerChanged), NULL, NULL);
	xReadBoxSizer->Add(xReadFilePicker, 0, wxEXPAND | wxALL, 2);
	wxButton* xReadStartButton = new wxButton(xReadPage, wxID_ANY, _T("Read ..."));
	xReadStartButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SerialAdapterV2::OnReadClick), NULL, NULL);
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
	xWriteFilePicker->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(SerialAdapterV2::OnWriteFilePickerChanged), NULL, NULL);
	xWriteBoxSizer->Add(xWriteFilePicker, 0, wxEXPAND | wxALL, 2);
	wxButton* xWriteStartButton = new wxButton(xWritePage, wxID_ANY, _T("Write ..."));
	xWriteStartButton->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SerialAdapterV2::OnWriteClick), NULL, NULL);
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

	wxLogMessage(_T("SerialAdapterV2: idle ..."));
#endif
}

SerialAdapterV2::~SerialAdapterV2() {
#ifdef OS_WINDOWS
	if(handle) {
		CloseHandle(handle);
		handle = NULL;
	}
#endif
}

void SerialAdapterV2::OnReadFilePickerChanged(wxFileDirPickerEvent& event) {
#ifdef OS_WINDOWS
	xSerialAdapterV2->xsReadFile = event.GetPath();
#endif
}

void SerialAdapterV2::OnWriteFilePickerChanged(wxFileDirPickerEvent& event) {
#ifdef OS_WINDOWS
	xSerialAdapterV2->xsWriteFile = event.GetPath();
#endif
}

bool SerialAdapterV2::Init() {
#ifdef OS_WINDOWS
	if(handle) {
		if(!CloseHandle(handle))
			return false;
		handle = NULL;
	}
	TCHAR pcCommPort[5];
		_snwprintf_s(pcCommPort, 4, TEXT("COM%i"), iSelectedAdapter);
		handle = CreateFile( pcCommPort,
				GENERIC_READ | GENERIC_WRITE,
				0,    // must be opened with exclusive-access
				NULL, // default security attributes
				OPEN_EXISTING, // must use OPEN_EXISTING
				0,    // not overlapped I/O
				NULL  // hTemplate must be NULL for comm devices
				);
		
		if (handle == INVALID_HANDLE_VALUE) {
			wxLogMessage(_T("SerialAdapterV2: %s port opening failed with error %d.\n"), xslAdapterDevices[iSelectedAdapter].c_str(),
					GetLastError());
				return false;
		}

	DCB dcb;
	SecureZeroMemory(&dcb, sizeof(DCB));
		dcb.DCBlength = sizeof(DCB);
		BOOL fSuccess = GetCommState(handle, &dcb);
		if (!fSuccess) {
			wxLogMessage(_T("SerialAdapterV2: GetCommState failed with error %d.\n"), GetLastError());
				return false;
		}
	
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fRtsControl = RTS_CONTROL_ENABLE;
		/* dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
			 dcb.fRtsControl = RTS_CONTROL_HANDSHAKE; */
		
		fSuccess = SetCommState(handle, &dcb);
		if (!fSuccess) {
			wxLogMessage(_T("SerialAdapterV2: SetCommState failed with error %d.\n"), GetLastError());
				return false;
		}
	
		COMSTAT stat;
		DWORD errors;
		ClearCommError(handle, &errors, &stat);
		ClearCommBreak(handle);

	SerialAdapterV2Helper::set_scl(1, handle);
		SerialAdapterV2Helper::set_sda(1, handle);
#endif
	return true;
}

bool SerialAdapterV2::OnShow() {
#ifdef OS_WINDOWS
	for(int i = 1; i < 5; ++i) {
		xslAdapterDevices.Add(wxString(wxT("COM")) + wxString::Format(wxT("%02i"), i));
	}
	iSelectedAdapter = 1;

	xDeviceChooser->Clear();
	xDeviceChooser->Append(xslAdapterDevices);
	xDeviceChooser->SetSelection(iSelectedAdapter - 1);
#endif
	return true;
}

void SerialAdapterV2::OnDeviceChoice(wxCommandEvent& event) {
#ifdef OS_WINDOWS
	iSelectedAdapter = event.GetSelection() - 1;
#endif
}

#ifdef OS_WiNDOWS
#define CHECKACK_EXIT(x) {if(SerialAdapterV2Helper::checkack(xSerialAdapterV2->handle) != 1) \
	{(x); \
		goto out_error; \
	}};
#endif

void SerialAdapterV2::OnWriteClick(wxCommandEvent& event) {
#ifdef OS_WINDOWS
	wxButton* pButton = (wxButton*)event.GetEventObject();
	pButton->Disable();
	pButton->SetLabel(_T("Writing ..."));
	pButton->Update();
	wxLogMessage(_T("SerialAdapterV2: Starting write ..."));
	if(xSerialAdapterV2->xsWriteFile != wxT("")) {
		if(xSerialAdapterV2->Init()) {
			wxFile xInput;
			xInput.Open(xSerialAdapterV2->xsWriteFile, wxFile::read);
			if(xInput.IsOpened()) {
				size_t fsize = 0;
				xInput.SeekEnd(0);
				fsize = xInput.Tell();
				xInput.Seek(0);
				if(fsize < 256) {
					wxLogMessage(_T("SerialAdapterV2: input file too short (%i < 256 bytes)"), fsize);
					xInput.Close();
					goto out_error;
				}
				wxLogMessage(_T("SerialAdapterV2: input file size %i bytes"), fsize);

				int write_addr_counter = 0;
				__u8 write_addr = 0x0;
				__u8 buf = 0x0;
				int tmp = 0;

				for(write_addr_counter = 0; write_addr_counter < 256; write_addr_counter += 16) {
					write_addr = (__u8)(write_addr_counter);        
						
						SerialAdapterV2Helper::stop(xSerialAdapterV2->handle);
						SerialAdapterV2Helper::start(xSerialAdapterV2->handle);
						
						SerialAdapterV2Helper::sendbyte(EEPROM_DEVADDR_SERIAL_WRITE, xSerialAdapterV2->handle);
						
						SerialAdapterV2Helper::delay();
						CHECKACK_EXIT(wxLogMessage(wxT("SerialAdapterV2: Error EEPROM address.")));
						
						SerialAdapterV2Helper::sendbyte(write_addr, xSerialAdapterV2->handle);
						
						SerialAdapterV2Helper::delay();
						CHECKACK_EXIT(wxLogMessage(wxT("SerialAdapterV2: Error sending EEPROM start address.")));
						
						for(int sub_write_addr_counter = 0; sub_write_addr_counter < 16; ++sub_write_addr_counter) {
							
								tmp = xInput.Read(&buf, 1);
							if(tmp < 1) {
								wxLogMessage(wxT("SerialAdapterV2: Error reading from input file at byte %i"), write_addr_counter + sub_write_addr_counter);
								goto out_error;
							}
							
								SerialAdapterV2Helper::sendbyte(tmp, xSerialAdapterV2->handle);
								
								for(int k = 0; k < 20; ++k) {
									SerialAdapterV2Helper::delay();
								}
							CHECKACK_EXIT(wxLogMessage(wxT("SerialAdapterV2: Error writing byte to EEPROM at byte %i"), write_addr_counter + sub_write_addr_counter));
								SerialAdapterV2Helper::set_scl(0, xSerialAdapterV2->handle);
								SerialAdapterV2Helper::delay();
						}
					
						SerialAdapterV2Helper::delay();
						
				}
				SerialAdapterV2Helper::stop(xSerialAdapterV2->handle);
					for(int i = 0; i < 20; ++i) {
						SerialAdapterV2Helper::delay();
					}

				xInput.Close();
			} else {
				wxLogMessage(_T("SerialAdapterV2: error opening input file '%s'"), xSerialAdapterV2->xsReadFile.c_str());
				goto out_error;
			}
		} else {
			wxLogMessage(_T("SerialAdapterV2: device initialization failed."));
			goto out_error;
		}
	} else {
		wxLogMessage(_T("SerialAdapterV2: no file selected."));
		goto out_error;
	}
	wxMessageBox(_T("Wrote 256 bytes."), _T("Success"), wxICON_INFORMATION);
	goto out;
out_error:
	wxMessageBox(_T("Error writing EEPROM (consult log window)"), _T("Error"), wxICON_ERROR);
out:
	wxLogMessage(_T("SerialAdapterV2: idle ..."));
	pButton->Enable();
	pButton->SetLabel(_T("Write"));
	pButton->Update();
#endif
}

void SerialAdapterV2::OnReadClick(wxCommandEvent& event) {
#ifdef OS_WINDOWS
	wxButton* pButton = (wxButton*)event.GetEventObject();
	pButton->Disable();
	pButton->SetLabel(_T("Reading ..."));
	pButton->Update();
	wxLogMessage(_T("SerialAdapterV2: Starting read ..."));
	if(xSerialAdapterV2->xsReadFile != wxT("")) {
		if(xSerialAdapterV2->Init()) {
			wxFile xOutput;
			xOutput.Open(xSerialAdapterV2->xsReadFile, wxFile::write);
			if(xOutput.IsOpened()) {
				// start reading from address 0x0

				int start_addr_counter = 0;
				__u8 start_addr = 0x0;
				__u8 buf = 0x0;
				int tmp = 0;

				SerialAdapterV2Helper::start(xSerialAdapterV2->handle);
					
					SerialAdapterV2Helper::sendbyte(EEPROM_DEVADDR_SERIAL_WRITE, xSerialAdapterV2->handle);
					
					SerialAdapterV2Helper::delay();
					CHECKACK_EXIT(wxLogMessage(wxT("SerialAdapterV2: Error sending EEPROM address")));
					
					SerialAdapterV2Helper::sendbyte(0x00, xSerialAdapterV2->handle);
					
					SerialAdapterV2Helper::delay();
					CHECKACK_EXIT(wxLogMessage(wxT("SerialAdapterV2: Error sending EEPROM read start address")));
					
					SerialAdapterV2Helper::set_scl(0, xSerialAdapterV2->handle);
					SerialAdapterV2Helper::start(xSerialAdapterV2->handle);
					
					SerialAdapterV2Helper::sendbyte(EEPROM_DEVADDR_SERIAL_READ, xSerialAdapterV2->handle);
					
					SerialAdapterV2Helper::delay();
					CHECKACK_EXIT(wxLogMessage(wxT("SerialAdapterV2: Error sending EEPROM address")));
					
					SerialAdapterV2Helper::recvbyte(&buf, xSerialAdapterV2->handle);
					wxLogMessage(_T("SerialAdapterV2: read 0x%x"), buf);
					tmp = xOutput.Write(&buf, 1);
				if(tmp < 1) {
					wxLogMessage(_T("SerialAdapterV2: error writing to output file at address 0x%x"), start_addr);
					goto out_error;
				}
				SerialAdapterV2Helper::delay();
					SerialAdapterV2Helper::stop(xSerialAdapterV2->handle);
					SerialAdapterV2Helper::set_scl(0, xSerialAdapterV2->handle);
					
					
					SerialAdapterV2Helper::delay();
					SerialAdapterV2Helper::start(xSerialAdapterV2->handle);
					
					SerialAdapterV2Helper::sendbyte(EEPROM_DEVADDR_SERIAL_READ, xSerialAdapterV2->handle); 
					
					SerialAdapterV2Helper::delay();
					CHECKACK_EXIT(wxLogMessage(wxT("SerialAdapterV2: Error sending EEPROM address")));
					
					for(start_addr_counter = 1; start_addr_counter < 255; start_addr_counter += 1) {
						buf = 0;
							SerialAdapterV2Helper::recvbyte(&buf, xSerialAdapterV2->handle);
							SerialAdapterV2Helper::ack(xSerialAdapterV2->handle);
							wxLogMessage(_T("SerialAdapterV2: read 0x%x"), buf);
							tmp = xOutput.Write(&buf, 1);
						if(tmp < 1) {
							wxLogMessage(_T("SerialAdapterV2: error writing to output file at address 0x%x"), start_addr);
							goto out_error;
						}
					}
				
					buf = 0;
					SerialAdapterV2Helper::recvbyte(&buf, xSerialAdapterV2->handle);
					wxLogMessage(_T("SerialAdapterV2: read 0x%x"), buf);
					tmp = xOutput.Write(&buf, 1);
				if(tmp < 1) {
					wxLogMessage(_T("SerialAdapterV2: error writing to output file at address 0x%x"), start_addr);
					goto out_error;
				}
				SerialAdapterV2Helper::nack(xSerialAdapterV2->handle);
					
					SerialAdapterV2Helper::stop(xSerialAdapterV2->handle);

				xOutput.Close();
			} else {
				wxLogMessage(_T("SerialAdapterV2: error opening output file '%s'"), xSerialAdapterV2->xsReadFile.c_str());
				goto out_error;
			}
		} else {
			wxLogMessage(_T("SerialAdapterV2: device initialization failed."));
			goto out_error;
		}
	} else {
		wxLogMessage(_T("SerialAdapterV2: no file selected."));
		goto out_error;
	}
	wxMessageBox(_T("Read 256 bytes."), _T("Success"), wxICON_INFORMATION);
	goto out;
out_error:
	wxMessageBox(_T("Error reading EEPROM (consult log window)"), _T("Error"), wxICON_ERROR);
out:
	wxLogMessage(_T("SerialAdapterV2: idle ..."));
	pButton->Enable();
	pButton->SetLabel(_T("Read"));
	pButton->Update();
#endif
}

typedef unsigned char __u8;
// #define SERIALADAPTERV2_TYPE 1 // default adapter with active power source and inverters
#define SERIALADAPTERV2_TYPE 2 // adapter type with usb power and z-diodes

#define SERIALADAPTERV2_CLKWAIT 1
#define SERIALADAPTERV2_DELAY 1

#ifdef OS_WINDOWS
void SerialAdapterV2Helper::start(HANDLE handle) {
	set_sda(1, handle);
		set_scl(1, handle);
		delay();
		set_sda(0, handle);
		delay();
}

void SerialAdapterV2Helper::stop(HANDLE handle) {
	set_sda(0, handle);
		set_scl(1, handle);
		delay();
		set_sda(1, handle);
		delay();
}

int SerialAdapterV2Helper::checkack(HANDLE handle) {
	int ack = 0;
		delay();
		set_sda(1, handle);
		set_scl(0, handle);
		delay();
		set_scl(1, handle);
		delay();
		int ret = get_sda(handle);
		if(ret == 0)
			ack = 1;
				delay();
				set_scl(0, handle);
				return ack;
}

void SerialAdapterV2Helper::ack(HANDLE handle) {
	set_scl(0, handle);
		delay();
		set_sda(0, handle);
		delay();
		set_scl(1, handle);
		delay();
}

void SerialAdapterV2Helper::nack(HANDLE handle) {
	set_scl(0, handle);
		delay();
		set_sda(1, handle);
		delay();
		set_scl(1, handle);
		delay();
}

void SerialAdapterV2Helper::sendbyte(__u8 out, HANDLE handle) {
	//debugout("sending: ");
	int bitmask = 128;
		for(int i = 0; i < 8; i++) {
			set_scl(0, handle);
				if((out & bitmask) > 0) {
					//debugout("1");
					set_sda(1, handle);
				} else {
					//debugout("0");
					set_sda(0, handle);
				}
			delay();
				set_scl(1, handle);
				delay();
				bitmask /= 2;
		}
	//debugout("\n");
	set_scl(0, handle);
}

void SerialAdapterV2Helper::recvbyte(__u8 *in, HANDLE handle) {
	*in = 0x00;
		set_scl(0, handle);
		set_sda(1, handle);
		delay();
		set_scl(1, handle);
		delay();
		if(get_sda(handle) > 0)
		*in += 128;
		set_scl(0, handle);
		delay();
		set_scl(1, handle);
		delay();
		if(get_sda(handle) > 0)
		*in += 64;
		set_scl(0, handle);
		delay();
		set_scl(1, handle);
		delay();
		if(get_sda(handle) > 0)
		*in += 32;
		set_scl(0, handle);
		delay();
		set_scl(1, handle);
		delay();
		if(get_sda(handle) > 0)
		*in += 16;
		set_scl(0, handle);
		delay();
		set_scl(1, handle);
		delay();
		if(get_sda(handle) > 0)
		*in += 8;
		set_scl(0, handle);
		delay();
		set_scl(1, handle);
		delay();
		if(get_sda(handle) > 0)
		*in += 4;
		set_scl(0, handle);
		delay();
		set_scl(1, handle);
		delay();
		if(get_sda(handle) > 0)
		*in += 2;
		set_scl(0, handle);
		delay();
		set_scl(1, handle);
		delay();
		if(get_sda(handle) > 0)
		*in += 1;
}

void SerialAdapterV2Helper::delay() {
	Sleep(SERIALADAPTERV2_DELAY);
}

void SerialAdapterV2Helper::set_sda(int out, HANDLE handle) {
	// DTR
	if(out == 1) {
		//debugout("set_sda(1)\n");
		if(!EscapeCommFunction(handle, SETDTR)) {
			wxLogMessage(wxT("SerialAdapterV2: Set DTR 1 failed\n"));
		}
	} else {
		//debugout("set_sda(0)\n");
		if(!EscapeCommFunction(handle, CLRDTR)) {
			wxLogMessage(wxT("SerialAdapterV2: Set DTR 0 failed\n"));
		}
	}
}

void SerialAdapterV2Helper::set_scl(int out, HANDLE handle) {
	// RTS
	if(out == 1) {
		//debugout("set_scl(1)\n");
		if(!EscapeCommFunction(handle, SETRTS)) {
			wxLogMessage(wxT("SerialAdapterV2: Set RTS 1 failed\n"));
		}
	} else {
		//debugout("set_scl(0)\n");
		if(!EscapeCommFunction(handle, CLRRTS)) {
			wxLogMessage(wxT("SerialAdapterV2: Set RTS 0 failed\n"));
		}
	}
}

int SerialAdapterV2Helper::get_sda(HANDLE handle) {
#if SERIALADAPTERV2_TYPE == 1
	// CTS eigentlich vertauscht!
	DWORD status = 0;
		if(GetCommModemStatus(handle, &status)) {
			if(status & MS_DSR_ON) {
				return 1;
			} else {
				return 0;
			}
		} else {
			wxLogMessage(wxT("SerialAdapterV2: error get_sda()\n"));
				return 0;
		}
#else if SERIALADAPTERV2_TYPE == 2
	DWORD status = 0;
		if(GetCommModemStatus(handle, &status)) {
			if(status & MS_CTS_ON) {
				return 1;
			} else {
				return 0;
			}
		} else {
			wxLogMessage(wxT("SerialAdapterV2: error get_sda()\n"));
				return 0;
		}
#endif
}

int SerialAdapterV2Helper::get_scl(HANDLE handle) {
#if SERIALADAPTERV2_TYPE == 1
	// DSR eigentlich vertauscht!
	DWORD status = 0;
		if(GetCommModemStatus(handle, &status)) {
			if(status & MS_CTS_ON) {
				return 1;
			} else {
				return 0;
			}
		} else {
			wxLogMessage(wxT("SerialAdapterV2: error get_scl()\n");
			return 0;
		}
#else if SERIALADAPTERV2_TYPE == 2
		return 1;
#endif
}
#endif

					BEGIN_EVENT_TABLE(SerialAdapterV2, wxPanel)
					EVT_CHOICE(ID_SERIALADAPTERV2_chooser, SerialAdapterV2::OnDeviceChoice)
					END_EVENT_TABLE()
