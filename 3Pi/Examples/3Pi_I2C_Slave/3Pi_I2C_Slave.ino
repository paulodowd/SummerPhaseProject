
#include <Wire.h>
#define I2C_ADDR  8

unsigned long update_ts;
#define UPDATE_MS 100

// Data to send(tx) and receive(rx)
// on the i2c bus.
// Needs to match the master device
// Note that the Arduino is limited to
// a buffer of 32 bytes!
#pragma pack(1)
typedef struct i2c_status {
  float x;                  // 4 bytes
  float y;                  // 4 bytes
  float theta;              // 4 bytes
  int8_t status;            // 1 byte
} i2c_status_t;
#pragma pack()

i2c_status_t i2c_status_tx;
volatile i2c_status_t i2c_status_rx;


// put your setup code here, to run once:
void setup() {



  // Serial for debugging.
  Serial.begin(9600);
  Serial.println("***RESTART***");

  // Clear out i2c data structs
  memset( &i2c_status_tx, 0, sizeof( i2c_status_tx ) );
  memset( &i2c_status_rx, 0, sizeof( i2c_status_rx ) );

  // Begin I2C as a slave device.
  Wire.begin( I2C_ADDR );
  Wire.onRequest( i2c_sendStatus );
  Wire.onReceive( i2c_recvStatus );

}

// put your main code here, to run repeatedly:
void loop() {

  // General update, here updating not PID controllers
  if ( millis() - update_ts > UPDATE_MS ) {
    update_ts = millis();

    Serial.println("Rx Status:");
    printRXStatus();
    Serial.println("\n");
  }



  //Serial.println( "loop" );
  delay(2);
}

// When the Core2 calls an i2c request, this function
// is executed.  Sends robot status to Core2.
void i2c_sendStatus() {

  // Populate our current status
  i2c_status_tx.x = 123.456;
  i2c_status_tx.y = 789.1011;
  i2c_status_tx.theta = 12.13;
  i2c_status_tx.status--; // debugging

  // Send up
  Wire.write( (byte*)&i2c_status_tx, sizeof( i2c_status_tx ) );

}

// When the Core2 calls and i2c write, the robot
// will call this function to receive the data down.
void i2c_recvStatus(int len ) {


  Wire.readBytes( (byte*)&i2c_status_rx, sizeof( i2c_status_rx ) );

  /*
    // I've had a lot of trouble with this bit of code.
    // Arduino wire (i2c) has a hard limit of 32 bytes
    // buffer.
    uint8_t buf[ 32 ];
    int i = 0;
    while( Wire.available() && i < 32) {
    buf[i] = Wire.read();
    i++;
    }
    memcpy( &i2c_status_rx, buf, sizeof( i2c_status_rx) );

  */
}


void printRXStatus() {

  Serial.println(  i2c_status_rx.x );
  Serial.println(i2c_status_rx.y );
  Serial.println(i2c_status_rx.theta );
  Serial.println(  i2c_status_rx.status );
}
