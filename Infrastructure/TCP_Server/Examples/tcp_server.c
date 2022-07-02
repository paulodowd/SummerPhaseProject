/*
 *
 */ 


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>

#include "tcp_server.h"

int main( int argc, const char ** argv ) {
	// Network specifics
	int sock, length, fromlen, n;
   	struct sockaddr_in server;
   	struct sockaddr_in from;


	// To temporarily store/configure
	// data to send/receive
	comm_packet_t incoming_packet;
  	comm_packet_t outgoing_packet;
   	
	// General
	int i,ip;
	time_t start_time, current_time;
   	float diff;
	char buf[200];

   	// Record the launch time of this program so that we
	// can add time stamps to all our recorded data
   	start_time = time(NULL);

	// Sort out our server status.	
   	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		printf(" Error: Opening socket\n");
		printf(" Abort\n");
		return 0;
	}

   	length = sizeof(server);
   	bzero(&server,length);
   	server.sin_family=AF_INET;
   	server.sin_addr.s_addr=INADDR_ANY;
   	server.sin_port=htons(PORT);

   	if( bind( sock,(struct sockaddr *)&server, length) < 0 ) {
		printf(" Error: binding to socket/server");
		printf(" Abort\n");
		return 0;
	}

   	fromlen = sizeof(struct sockaddr_in);

   	printf(" Server %s:%d\n", SERVER, PORT);
   	printf(" Expecting packet size: %lu\n", sizeof(incoming_packet));

	// Sit in this loop to transact messages
	while (1) {
	
		// Blocking, waits for incoming message
		n = recvfrom(sock,&incoming_packet,sizeof(incoming_packet),0,(struct sockaddr *)&from,&fromlen);
		
		if( n < 0 ) {
			printf("Error receiving\n");
		} else {
		
   
			// Record time message arrived
			current_time = time(NULL);
      			diff = difftime( current_time, start_time);

			// Update UI
			printf("STATUS: %d: %.2f, %.2f, %.2f\n", incoming_packet.status, incoming_packet.x, incoming_packet.y, incoming_packet.theta);
	

			// Send something back
			outgoing_packet.status = 10;
			n = sendto(sock, &outgoing_packet, sizeof(outgoing_packet),0,(struct sockaddr *)&from, fromlen);
       			
			if (n  < 0) {
				printf("Error sending back\n");
			}
	
		}
	   } // infinite while

 }// program close

