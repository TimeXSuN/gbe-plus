// GB Enhanced Copyright Daniel Baxter 2016
// Licensed under the GPLv2
// See LICENSE.txt for full license text

// File : sio.h
// Date : April 30, 2016
// Description : Game Boy Serial Input-Output emulation
//
// Sets up SDL networking
// Emulates Gameboy-to-Gameboy data transfers
// Emulates GBC IR port (not exactly SIO, but the RP register is related to pin 4 of the link port)
// Emulates the GB Printer

#ifndef GB_SIO
#define GB_SIO

#ifdef GBE_NETPLAY
#include <SDL2/SDL_net.h>
#endif

#include "mmu.h"
#include "sio_data.h"

class DMG_SIO
{
	public:

	//Link to memory map
	DMG_MMU* mem;

	dmg_sio_data sio_stat;

	bool network_init;

	#ifdef GBE_NETPLAY

	//Receiving server
	struct tcp_server
	{
		TCPsocket host_socket, remote_socket;
		IPaddress host_ip;
		bool connected;
		u16 port;
	} server;

	//Sending client
	struct tcp_sender
	{
		TCPsocket host_socket;
		IPaddress host_ip;
		bool connected;
		u16 port;
	} sender;

	SDLNet_SocketSet tcp_sockets;

	#endif

	//GB Printer
	struct gb_printer
	{
		std::vector <u32> scanline_buffer;
		std::vector <u8> packet_buffer;
		u32 packet_size;
		printer_state current_state;

		u8 command;
		u8 compression_flag;
		u8 strip_count;
		u16 data_length;
		u16 checksum;
		u8 status;
		u8 pal[4];
	} printer;

	//GB Mobile Adapter
	struct gb_mobile_adapter
	{
		std::vector <u8> data;
		std::vector <u8> packet_buffer;
		u32 packet_size;	
		mobile_state current_state;
		std::string http_data;

		u8 command;
		u16 checksum;
		u8 data_length;

		u16 port;
		u32 ip_addr;
		bool pop_session_started;
		bool http_session_started;
		bool line_busy;
	} mobile_adapter;

	//Bardigun barcode scanner
	struct gb_bardigun_barcode_scanner
	{
		std::vector <u8> data;
		bardigun_state current_state;
		u16 inactive_counter;
		u32 barcode_pointer;
	} bardigun_scanner;

	//Barcode Boy
	struct gb_barcode_boy_scanner
	{
		std::vector <u8> data;
		u16 counter;
		barcode_boy_state current_state;
		u8 timeout_data[12];
		u8 byte;
		bool send_data;
	} barcode_boy;

	DMG_SIO();
	~DMG_SIO();

	bool init();
	void reset();

	bool send_byte();
	bool send_ir_signal();
	bool receive_byte();
	bool request_sync();
	void process_network_communication();

	void printer_process();
	void printer_execute_command();
	void printer_data_process();
	void print_image();

	void mobile_adapter_process();
	void mobile_adapter_process_pop();
	void mobile_adapter_process_http();

	void bardigun_process();
	bool bardigun_load_barcode(std::string filename);

	void barcode_boy_process();
	bool barcode_boy_load_barcode(std::string filename);
};

#endif // GB_SIO
