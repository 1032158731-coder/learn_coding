#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "user.h"
#include "public.h"

class TcpServer
{
public:
	Int32 test();

private:

	bool set_nonblocking(int sock) ;
	void handle_client(int client_socket);
	//void worker_thread() ;
	//void signal_handler(int signal);
	void print_server_status() ;


};

#endif

