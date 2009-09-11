// xeroxpatrone.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <AtlBase.h>
#include <AtlConv.h>
#include <iostream>
#include <string>

#include "boost/program_options.hpp"

using namespace std;
namespace po = boost::program_options;

// SetCommMask CTS DSR
// EscapeCommm RTS DTR

DCB dcb;
HANDLE hCom;
BOOL fSuccess;
DWORD dwEvtMask;

#define CLKWAIT 1
#define DELAY 1

#define CHECKACK_EXIT {if(checkack() == 1) {debugout("ack\n");} else {debugout("nack\n");fprintf(stderr, "got NACK from eeprom\n");exit(1);}}
#define CHECKACK_EXIT_NOPRINT {if(checkack() == 1) {;} else {debugout("nack\n");fprintf(stderr, "got NACK from eeprom\n");exit(1);}}

void set_sda(int out);
void set_scl(int out);
int get_sda();
int get_scl();
void delay();
void pulse();

void start();
void stop();
int checkack();
void ack();
void nack();
void selectdevread();
void selectdevwrite();
void sendbyte(int out);
void recvbyte(int* in);

int analyze_file(string filename);

void debugout(char *fmt, ...) {
#ifdef DEBUG
	va_list argp;
	va_start(argp, fmt);
	printf(fmt, argp);
	va_end(argp);
#endif
}


void usage(char* filename) {
	fprintf(stderr, "Usage: %s -c <Com port number> -a <Device address> [-r <output file> | -w <input file>]\n", filename);
	fprintf(stderr, "	where the com port number is between 1 and 9\n");
	fprintf(stderr, "	the device address must be given with the r/w bit set to 0\n");
	fprintf(stderr, "		e.g.: 1010 000 0 = 0xA0 = 160\n");
	fprintf(stderr, "	either the -r or the -w command can be given for \n");
	fprintf(stderr, "	either reading the whole 265 * 8 Bits of EEPROM content\n");
	fprintf(stderr, "	265 * 8 Bits of EEPROM content to file or writing \n");
	fprintf(stderr, "	the whole EEPROM from file\n\n");
	fprintf(stderr, "	example: %s -c 1 -a 160 -r content.bin\n", filename);
	fprintf(stderr, "		reads the eeprom to file content.bin\n");
	exit(1);
}

int main(int argc, char* argv[])
{
	bool readmode = false;
	bool writemode = false;
	bool analyzemode = false;
	string filename = "";
	int comport = 1;
	int devadr = 0xA0;
	
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "this help message")
		("comport,c", po::value<int>(), "com port number between 1 and 9\n\tdefault: 1")
		("address,a", po::value<int>(), "the EEPROMs device address with the r/w bit set to 0\n\n\t4 bit device type|3 bit device address|1 bit r/w bit\n\te.g.: 1010 000 0 = 0xA0 = 160\n\tdefault: 0xA0")
		("read,r", "read from the EEPROM 265*8 bits to <file>")
		("write,w", "write the EEPROM from <file>")
		("analyze", "analyze a dumped EEPROM file")
		("file", po::value<string>(), "the file to read from or write to\n\t(warning: existing files will be overwritten without prompt)")
	;

	po::positional_options_description p;
	p.add("file", -1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).
          options(desc).positional(p).run(), vm);
	po::notify(vm);    

	if (vm.count("help")) {
		cerr << desc << "\n";
		cerr << "\nexample: xeroxpatrone -c 1 -a 160 -r content.bin\n\nThe Xerox Phase 6130 cardridges have exacly this eeprom address (0xA0)\nand a size of 8*256 bytes\n";
		return 1;
	}

	if(vm.count("analyze")) {
		analyzemode = true;
	}

	if(vm.count("read")) {
		readmode = true;
	}
	if(vm.count("write")) {
		writemode = true;
	}

	if((readmode && writemode) || (!readmode && !writemode && !analyzemode)) {
		fprintf(stderr, "either read, write or analyze mode has to be selected\n\nuse: %s --help for help\n", argv[0]);
		exit(1);
	}

	if(vm.count("file")) {
		filename = vm["file"].as<string>();
	} else {
		fprintf(stderr, "no file specified\n");
	}

	if(vm.count("comport")) {
		comport = vm["comport"].as<int>();
	}

	if(vm.count("address")) {
		devadr = vm["address"].as<int>();
	}

	if(analyzemode) {
		return analyze_file(filename);
	}

	TCHAR pcCommPort[5];
	_snwprintf_s(pcCommPort, 4, TEXT("COM%i"), comport);
    hCom = CreateFile( pcCommPort,
                    GENERIC_READ | GENERIC_WRITE,
                    0,    // must be opened with exclusive-access
                    NULL, // default security attributes
                    OPEN_EXISTING, // must use OPEN_EXISTING
                    0,    // not overlapped I/O
                    NULL  // hTemplate must be NULL for comm devices
                    );

	if (hCom == INVALID_HANDLE_VALUE) {
       // Handle the error.
       fprintf(stderr, "com port opening failed with error %d.\n", GetLastError());
       return (1);
    }

	/* COMMCONFIG portconfig;
	SecureZeroMemory(&portconfig, sizeof(COMMCONFIG));
	portconfig.dwSize = sizeof(COMMCONFIG);

	GetDefaultCommConfig(pcCommPort, &portconfig, &portconfig.dwSize);
	SetCommConfig(pcCommPort, &portconfig, sizeof(COMMCONFIG)); */

	// Build on the current configuration, and skip setting the size
    // of the input and output buffers with SetupComm.

    SecureZeroMemory(&dcb, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);
    fSuccess = GetCommState(hCom, &dcb);
    if (!fSuccess) {
      // Handle the error.
      fprintf (stderr, "GetCommState failed with error %d.\n", GetLastError());
      return (2);
    }
	
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
	/* dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
    dcb.fRtsControl = RTS_CONTROL_HANDSHAKE; */
    
    fSuccess = SetCommState(hCom, &dcb);
    if (!fSuccess) {
      // Handle the error.
      fprintf (stderr, "SetCommState failed with error %d.\n", GetLastError());
      return (3);
    }
	
	COMSTAT stat;
	DWORD errors;
	ClearCommError(hCom, &errors, &stat);
	ClearCommBreak(hCom);

	set_sda(1);
	set_scl(1);

	int buf = 0;
	int devadrr = (int)(devadr + 1);
	int devadrw = (int)devadr;

	if(readmode) {
		// read
		// int devadrr = 0xA1; // 1010 000 1
		// int devadrw = 0xA0; // 1010 000 0
		debugout("init...\n");
		stop();

		debugout("start...\n");
		start();

		debugout("send adr: 0x%x\n", devadrw);
		sendbyte(devadrw);

		delay();
		CHECKACK_EXIT;

		// begin reading at the first address
		debugout("send memory adress 0x0\n");
		sendbyte(0x00);

		delay();
		CHECKACK_EXIT;

		debugout("restart...\n");
		set_scl(0);
		start();

		debugout("send adr: 0x%x\n", devadrr);
		sendbyte(devadrr);

		delay();
		CHECKACK_EXIT;

		FILE* fp;
		printf("writing to %s\n", filename.c_str());
		printf("receive 256 bytes: \n");
		int ret = fopen_s(&fp, filename.c_str(), "wb");
		if(ret != 0) {
			fprintf(stderr, "could not open %s\n", filename.c_str());
			exit(1);
		}
		// printf("receive first byte: \n");
		buf = 0;
		recvbyte(&buf);
		printf("%02x ", buf);
		fprintf(fp, "%c", buf);
		delay();
		stop();
		set_scl(0);

		// printf("\n");

		delay();
		// printf("restart...\n");
		start();

		// printf("send adr: 0x%x\n", devadrr);
		sendbyte(devadrr); 

		delay();
		CHECKACK_EXIT_NOPRINT;
		int j = 2;
		// printf("receive 254 remaining bytes: \n");
		for(int i = 1; i < 255; i++) {
			buf = 0;
			recvbyte(&buf);
			ack();
			printf("%02x ", (unsigned int)buf);
			fprintf(fp, "%c", buf);
			if(j % 16 == 0)
				printf("\n");
			j++;
		}

		// printf("\nreceive last byte: \n");
		buf = 0;
		recvbyte(&buf);
		// nack();
		printf("%02x ", buf);
		fprintf(fp, "%c", buf);
		nack();
		printf("\n");
		debugout("stop...\n");
		fclose(fp);
		stop();

	/*	set_sda(1);
		set_scl(1); */
	}
	if(writemode) {
		// write
		int address = 0x00;
		FILE* fp;
		printf("reading from %s\n", filename.c_str());
		int ret = fopen_s(&fp, filename.c_str(), "rb");
		if(ret != 0) {
			fprintf(stderr, "could not open %s\n", filename.c_str());
			exit(1);
		}
		printf("writing 256 bytes: \n");

		for(int j = 0; j < 256; j+=16) {
			address = j;
			debugout("init...\n");
			stop();

			debugout("start...\n");
			start();

			debugout("send adr: 0x%x\n", devadrw);
			sendbyte(devadrw);

			delay();
			CHECKACK_EXIT;

			debugout("send memory adress 0x%x\n", address);
			sendbyte(address);

			delay();
			CHECKACK_EXIT;

			int sendbuf = 0;

			for(int i = 0; i < 16; i++) {
				sendbuf = 0;
				fread_s(&sendbuf, 1, 1, 1, fp);
				printf("%02x ", sendbuf);
				
				sendbyte(sendbuf);

				for(int k = 0; k < 20; k++) {
					delay();
				}
				CHECKACK_EXIT_NOPRINT;
				set_scl(0);
				delay();
			}
			printf("\n");
			delay();
			
			debugout("stop...\n");
			stop();
			for(int i = 0; i < 20; i++) {
				delay();
			}
			delay();

			delay();
			delay();
		}	
	}
	return 0;
}

void start() {
	set_sda(1);
	set_scl(1);
	delay();
	set_sda(0);
	delay();
}

void stop() {
	set_sda(0);
	set_scl(1);
	delay();
	set_sda(1);
	delay();
}

int checkack() {
	int ack = 0;
	set_sda(1);
	set_scl(0);
	delay();
	set_scl(1);
	int ret = get_sda();
	if(ret == 0)
		ack = 1;
	delay();
	return ack;
}

void ack() {
	set_scl(0);
	delay();
	set_sda(0);
	delay();
	set_scl(1);
	delay();
}

void nack() {
	set_scl(0);
	delay();
	set_sda(1);
	delay();
	set_scl(1);
	delay();
}

void sendbyte(int out) {
	debugout("sending: ");
	int bitmask = 128;
	for(int i = 0; i < 8; i++) {
		set_scl(0);
		if((out & bitmask) > 0) {
			debugout("1");
			set_sda(1);
		} else {
			debugout("0");
			set_sda(0);
		}
		delay();
		set_scl(1);
		delay();
		bitmask /= 2;
	}
	debugout("\n");
	set_scl(0);
}

void recvbyte(int *in) {
	*in = 0x00;
	set_scl(0);
	set_sda(1);
	delay();
	set_scl(1);
	delay();
	if(get_sda() > 0)
		*in += 128;
	set_scl(0);
	delay();
	set_scl(1);
	delay();
	if(get_sda() > 0)
		*in += 64;
	set_scl(0);
	delay();
	set_scl(1);
	delay();
	if(get_sda() > 0)
		*in += 32;
	set_scl(0);
	delay();
	set_scl(1);
	delay();
	if(get_sda() > 0)
		*in += 16;
	set_scl(0);
	delay();
	set_scl(1);
	delay();
	if(get_sda() > 0)
		*in += 8;
	set_scl(0);
	delay();
	set_scl(1);
	delay();
	if(get_sda() > 0)
		*in += 4;
	set_scl(0);
	delay();
	set_scl(1);
	delay();
	if(get_sda() > 0)
		*in += 2;
	set_scl(0);
	delay();
	set_scl(1);
	delay();
	if(get_sda() > 0)
		*in += 1;
}

void delay() {
	Sleep(DELAY);
}

void set_sda(int out) {
	// DTR
	if(out == 1) {
		if(!EscapeCommFunction(hCom, SETDTR)) {
			printf("Set DTR 1 failed\n");
		}
	} else {
		if(!EscapeCommFunction(hCom, CLRDTR)) {
			printf("Set DTR 0 failed\n");
		}
	}
}

void set_scl(int out) {
	// RTS
	if(out == 1) {
		EscapeCommFunction(hCom, SETRTS);
	} else {
		EscapeCommFunction(hCom, CLRRTS);
	}
}

int get_sda() {
	// CTS eigentlich vertauscht!
	DWORD status = 0;
	if(GetCommModemStatus(hCom, &status)) {
		if(status & MS_DSR_ON) {
			return 1;
		} else {
			return 0;
		}
	} else {
		printf("error get_sda()\n");
		return 0;
	}
}

int get_scl() {
	// DSR eigentlich vertauscht!
	DWORD status = 0;
	if(GetCommModemStatus(hCom, &status)) {
		if(status & MS_CTS_ON) {
			return 1;
		} else {
			return 0;
		}
	} else {
		printf("error get_scl()\n");
		return 0;
	}
}

void printch(char* buf, int start, int len) {
	for(int i = 0; i < len; i++) {
		printf("%c", buf[start+i]);
	}
	printf("\n");
}

unsigned long getlong(char* buf, int start, int len) {
	unsigned long v = 0;
	for(int i = 0; i < len; i++) {
		v = v<<1;
		v = v | buf[start+i];
	}
	return v;
}

int analyze_file(string filename) {
	char buf = 0;
	char* fbuf;
	fbuf = (char*)malloc(sizeof(char) * 256);
	FILE* fp;
	printf("reading from %s\n", filename.c_str());
	fopen_s(&fp, filename.c_str(), "rb");
	printf("dump: \n");
	for(int i = 0; i < 256; i++) {
		buf = 0x00;
		fread_s(&buf, 1, 1, 1, fp);
		printf("%02x ", (int)buf);
		fbuf[i] = buf;
	}
	printf("\n\n");
	
	printch(fbuf, 11, 5);
	
	unsigned long v = getlong(fbuf, 0xdd, 4);
	printf("%u\n", v);
	v = getlong(fbuf, 0xe2, 4);
	printf("%u\n", v);
	v = getlong(fbuf, 0xe7, 4);
	printf("%u\n", v);
	v = getlong(fbuf, 0xec, 4);
	printf("%u\n", v);

	free(fbuf);
	return 0;
}