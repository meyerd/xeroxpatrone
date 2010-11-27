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

#include "OptionParser.h"

using namespace std;
using namespace optparse;

// SetCommMask CTS DSR
// EscapeCommm RTS DTR

// #define ADAPTER_TYPE 1 // default adapter with active power source and inverters
#define ADAPTER_TYPE 2 // adapter type with usb power and z-diodes

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
	bool helpmode = false;
	string filename = "";
	int comport = 1;
	int devadr = 0xA0;

	const string usage = "Usage: xeroxpatrone -c <Com port number> -a <Device address> [-r <output file> | -w <input file>]\n"
						 "	where the com port number is between 1 and 9\n"
						 "	the device address must be given with the r/w bit set to 0\n"
						 "		e.g.: 1010 000 0 = 0xA0 = 160\n"
						 "	either the -r or the -w command can be given for \n"
						 "	either reading the whole 265 * 8 Bits of EEPROM content\n"
						 "	265 * 8 Bits of EEPROM content to file or writing \n"
						 "	the whole EEPROM from file\n\n"
						 "	example: xeroxpatrone -c 1 -a 160 -r content.bin\n"
						 "		reads the eeprom to file content.bin\n";

	OptionParser parser = OptionParser()
    .usage(usage);

	parser.set_defaults("readmode", "0");
	parser.set_defaults("writemode", "0");
	parser.set_defaults("analyzemode", "0");
	//parser.set_defaults("helpmode", "0");
	/* parser.set_defaults("comport", "1");
	parser.set_defaults("devadr", "0xA0"); */

	parser.add_option("-r", "--read") .action("store_true") .dest("readmode") .help("read from the EEPROM 265*8 bits to <file>");
	parser.add_option("-w", "--write") .action("store_true") .dest("writemode") .help("write the EEPROM from <file>");
	parser.add_option("--analyze") .action("store_true") .dest("analyzemode") .help("analyze a dumped EEPROM file");
	parser.add_option("-c", "--comport") .action("store") .type("int") .dest("comport") .set_default(1) .help("com port number between 1 and 9\n\tdefault: %default");
	parser.add_option("-a", "--address") .action("store") .type("int") .dest("devadr") .set_default(160) .help("com port number between 1 and 9\n\tdefault: %default (0xA0)");
	// parser.add_option("-h", "--help") .action("store_true") .dest("helpmode") .help("this help message");

	Values& options = parser.parse_args(argc, argv);
	vector<string> args = parser.args();
	
	helpmode = options.get("helpmode");
	readmode = options.get("readmode");
	writemode = options.get("writemode");
	analyzemode = options.get("analyzemode");

	comport = (int)options.get("comport");
	devadr = (int)options.get("devadr");

	/* if (helpmode) {
		
		cerr << "\nexample: xeroxpatrone -c 1 -a 160 -r content.bin\n\nThe Xerox Phase 6130 cardridges have exacly this eeprom address (0xA0)\nand a size of 8*256 bytes\n";
		return 1;
	} */

	if((readmode && writemode) || (!readmode && !writemode && !analyzemode)) {
		fprintf(stderr, "either read, write or analyze mode has to be selected\n\nuse: %s --help for help\n", argv[0]);
		exit(1);
	}

	if(args.size() <= 0) {
		fprintf(stderr, "no file specified\n");
		return 1;
	}

	filename = args.front();

	cout << "filename: " << filename << endl;
	cout << "comport: " << comport << endl;
	cout << "devadr: " << devadr << endl;

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

	int buf = 0;
	int devadrr;
	devadrr = (devadr + 1);
	int devadrw;
	devadrw = devadr;

	/* Sleep(1000);

	int i = 0;
	while (true) {
		i = (i + 1) % 2;
		set_sda(i);
		printf("%i\n", i);
		Sleep(2000);
	} */

	//int j = 0;
	//for(int i = 0; i < 20; i++) {
	//	j = (j + 1) % 2;
	//	set_sda(j);
	//	set_scl(j);
	//	delay();
	//}

	set_scl(1);
	set_sda(1);

	if(readmode) {
		// read
		// int devadrr = 0xA1; // 1010 000 1
		// int devadrw = 0xA0; // 1010 000 0
		debugout("init...\n");
		// stop();

		debugout("start...\n");
		start();

		debugout("send adr: 0x%x (%i)\n", devadrw, devadrw);
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
	delay();
	set_sda(1);
	set_scl(0);
	delay();
	set_scl(1);
	delay();
	int ret = get_sda();
	if(ret == 0)
		ack = 1;
	delay();
	set_scl(0);
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
		debugout("set_sda(1)\n");
		if(!EscapeCommFunction(hCom, SETDTR)) {
			printf("Set DTR 1 failed\n");
		}
	} else {
		debugout("set_sda(0)\n");
		if(!EscapeCommFunction(hCom, CLRDTR)) {
			printf("Set DTR 0 failed\n");
		}
	}
}

void set_scl(int out) {
	// RTS
	if(out == 1) {
		debugout("set_scl(1)\n");
		if(!EscapeCommFunction(hCom, SETRTS)) {
			printf("Set RTS 1 failed\n");
		}
	} else {
		debugout("set_scl(0)\n");
		if(!EscapeCommFunction(hCom, CLRRTS)) {
			printf("Set RTS 0 failed\n");
		}
	}
}

int get_sda() {
#if ADAPTER_TYPE == 1
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
#else if ADAPTER_TYPE == 2
	DWORD status = 0;
	if(GetCommModemStatus(hCom, &status)) {
		if(status & MS_CTS_ON) {
			return 1;
		} else {
			return 0;
		}
	} else {
		printf("error get_sda()\n");
		return 0;
	}
#endif
}
int get_scl() {
#if ADAPTER_TYPE == 1
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
#else if ADAPTER_TYPE == 2
		return 1;
#endif
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