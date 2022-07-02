


/* required includes */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H


// Try to ensure endianess
#pragma pack(1)

// Server Specifics
#define PORT 6543
#define SERVER "192.168.0.14"
//#define SERVER "localhost"


// Data we expect to send and receive.
// Should be the same on the robot side.
typedef struct comm_packet {
	int status;
	float x;
	float y;
	float theta;

} comm_packet_t;


/* 
 * This structure is used to store and pass around
 * all of the details for the wifi connection, as
 * well as maintain the last sent/received data
 * for inspection.
 */
typedef struct wifi_details {
	int sock;
	struct sockaddr_in server;
	struct sockaddr_in from;
	struct hostent *hp;
	int length;
	int n_bytes;
	
	// See the packet structure above.
	comm_packet_t out_packet;
	comm_packet_t in_packet;
} wifi_struct_t;

int openWifi( wifi_struct_t * wifi_ptr );
void closeWifi( wifi_struct_t * wifi_ptr );
int sendWifiPacket( wifi_struct_t * wifi_ptr );


#endif
