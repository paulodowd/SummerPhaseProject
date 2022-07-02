#include <M5Core2.h>

#include <Wire.h>           // i2c to connect to IR communication board.
#define ROBOT_I2C_ADDR  8

// Data to send(tx) and receive(rx)
// on the i2c bus.
// Needs to match the master device

#pragma pack(1)
typedef struct i2c_status {
  float x;                  // 4 bytes
  float y;                  // 4 bytes
  float theta;              // 4 bytes
  uint8_t status;           // 1 byte
} i2c_status_t;
#pragma pack()

i2c_status_t i2c_status_tx;
i2c_status_t i2c_status_rx;

void setup() {
  // put your setup code here, to run once:
  
  //M5.begin(true, false, true, true);
  M5.begin();
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Wire.begin();  
  M5.lcd.println("\nSetting up I2C");

}

void loop() {

  // Setup data to send to robot


  // Send an update down to the robot
  
  i2c_status_tx.x = 987;
  i2c_status_tx.y = 654;
  i2c_status_tx.theta = 6.28;
  i2c_status_tx.status++;
  
  Wire.beginTransmission(ROBOT_I2C_ADDR);
  Wire.write( (uint8_t*)&i2c_status_tx, sizeof( i2c_status_tx ));
  Wire.endTransmission();


  // Read values back from the robot.
  Serial.println("Read: ");
  Wire.requestFrom( ROBOT_I2C_ADDR, sizeof( i2c_status_rx ));
  Wire.readBytes( (uint8_t*)&i2c_status_rx, sizeof( i2c_status_rx ));
  printRXStatus();
  

  delay(500);
}

void printRXStatus() {
  Serial.println( i2c_status_rx.x ); 
  Serial.println( i2c_status_rx.y );
  Serial.println( i2c_status_rx.theta );
  Serial.println( i2c_status_rx.status );
}
