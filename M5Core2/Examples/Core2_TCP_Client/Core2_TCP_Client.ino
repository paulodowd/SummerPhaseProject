/*
 * Intended for an M5Stack Core2. 
 * You'll need to install the M5Stack boards.
 *  - I had an issue installing version 2.0.3, 2.0.2 works
 * You'll need to install the M5Stack libraries, let it also
 *  install the dependencies.
 *  
 * You'll want to also compile and run the server.
 * - we probably want to set a static IP for the server.
 *  
 *  This webpage was useful to configure this:
 *  https://esp32.com/viewtopic.php?t=18853
 *
 *
*/
#include <M5Core2.h>
#include <WiFi.h>
#include <WiFiMulti.h>

// For lower level tcp/ip calls.
#include "lwip/sockets.h"

#define MAX_TCP_RX_RETRY 5

#define SERVER "192.168.0.14"
#define PORT 6543

const char *ssid = "ssid";
const char *password = "password";
WiFiMulti WiFiMulti;

// Data we will send and receive
// Needs to match the server code
// running remotely.
typedef struct packet {
  int status;
  float x;
  float y;
  float theta;
} packet_t;

void setup() {


  M5.begin();

  WiFiMulti.addAP(ssid, password);

  M5.lcd.print("\nWaiting connect to WiFi...");

  int attempt = 0;
  while (WiFiMulti.run() != WL_CONNECTED) {
    M5.lcd.print(".");
    delay(1000);
    attempt += 1;
    if (attempt == 8 ) M5.lcd.print("Connect failed!");
  }

  M5.lcd.println("\nWiFi connected");
  M5.lcd.print("IP address: ");
  M5.lcd.println(WiFi.localIP());
  delay(500);

}

void loop() {

  // Add text below start up text.
  M5.lcd.setCursor(0, 40);

  M5.lcd.println("Configure Socket");
  int sockfd = socket( AF_INET, SOCK_DGRAM, IPPROTO_IP);
  fcntl( sockfd, F_SETFL, O_NONBLOCK | FASYNC);

  struct sockaddr_in dest_addr;
  dest_addr.sin_addr.s_addr = inet_addr( SERVER );
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons( PORT );

  M5.lcd.print("Attempt Connection: ");
  int err = connect( sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in6));
  if ( err != 0 ) {
    M5.lcd.println(" Failed to connect");
  } else {

    M5.lcd.println(" Success!");

    // To transmit
    packet_t packet_tx;
    packet_tx.status = 1;
    packet_tx.x = -100;
    packet_tx.y = 100;
    packet_tx.theta = 3.14;

    // To receive
    packet_t packet_rx;
    packet_rx.status = -1;
    

    int n = send( sockfd,
                  &packet_tx,
                  sizeof(packet_tx),
                  0);
    if ( n < 0 ) {
      M5.lcd.println("Error sending");

    } else {
      M5.lcd.print("Sent ");
      M5.lcd.print( n );
      M5.lcd.print(" bytes.");

      // Sent a message?
      // Should receive a message back here
      int retry;
      for( retry = 0; retry < MAX_TCP_RX_RETRY; retry++ ) {
        
        
        n = recv(   sockfd, 
                    &packet_rx,
                    sizeof( packet_rx ),
                    0 );
        
        M5.lcd.print("Receive attempt ");
        M5.lcd.print( retry );
            
        if( n < 0 ) {
            M5.lcd.print(" , transmission error.");
        } else if( n == 0 ) {
            M5.lcd.print(" , connection closed.");
        } else {
            M5.lcd.print(" , status: ");
            M5.lcd.print( packet_rx.status );
            
            break;
        }
      }
    
    }

    

    // The socket should still be open
    // if so, we close it.
    if ( sockfd != -1 ) {
      close(sockfd);
    } else {
      // We shouldn't get this condition.
    }
  }

  // Display status for 5 seconds
  // Then start the test process again
  delay(5000);

  // Clear screen before we start again
  M5.lcd.fillRect(0, 40, 320, 220, BLACK);

  return;

}
