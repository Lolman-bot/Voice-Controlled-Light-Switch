#include "DFRobot_DF2301Q.h"
#include <Servo.h>

Servo servolight;
Servo servofan;

const int Servolightpin = 3;
const int Servofanpin = 2;
const int mosphetpin = 12;

//I2C communication
DFRobot_DF2301Q_I2C asr;

void setup() {
  pinMode(mosphetpin, OUTPUT);    //Init LED pin to output mode
  digitalWrite(mosphetpin, HIGH);  //Set LED pin to low 

  servolight.attach(Servolightpin);
  servofan.attach(Servofanpin);

  // Init the sensor
  while (!(asr.begin())) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   * @brief Set voice volume
   * @param voc - Volume value(1~7)
   */
  asr.setVolume(1);

  /**
     @brief Set mute mode
     @param mode - Mute mode; set value 1: mute, 0: unmute
  */
  asr.setMuteMode(1);

  /**
     @brief Set wake-up duration
     @param wakeTime - Wake-up duration (0-255)
  */
  asr.setWakeTime(20);

  /**
     @brief Get wake-up duration
     @return The currently-set wake-up period
  */
  uint8_t wakeTime = 0;
  wakeTime = asr.getWakeTime();
  Serial.print("wakeTime = ");
  Serial.println(wakeTime);

  // asr.playByCMDID(1);   // Wake-up command

  /**
     @brief Play the corresponding reply audio according to the ID
     @param CMDID - command word ID
  */
  //asr.playByCMDID(23);  // Command word ID
}

void loop() {
  /**
     @brief Get the ID corresponding to the command word 
     @return Return the obtained command word ID, returning 0 means no valid ID is obtained
  */
  uint8_t CMDID = asr.getCMDID();
  switch (CMDID) {
    case 75: {                                               // If the command is " Turn on the Fan"
      digitalWrite(mosphetpin, LOW);
      servofan.write(160);
      delay(500);
      digitalWrite(mosphetpin, HIGH);
      break;
    }
    case 76: {
      digitalWrite(mosphetpin, LOW);
      servofan.write(0);
      delay(500);
      digitalWrite(mosphetpin, HIGH);
      break;
    }
    case 103: {                                               // If the command is " Turn on the light"
      digitalWrite(mosphetpin, LOW);
      servolight.writeMicroseconds(1700);
      delay(500);
      servolight.writeMicroseconds(1500);
      delay(1000);
      digitalWrite(mosphetpin, HIGH);
      break;
    }
    case 104: {                                             // If the command is " Turn off the light"
      digitalWrite(mosphetpin, LOW);
      servolight.writeMicroseconds(1300);
      delay(500);
      servolight.writeMicroseconds(1500);
      delay(1000);
      digitalWrite(mosphetpin, HIGH);
    }
    default:
      if (CMDID != 0) {
        Serial.print("CMDID = ");  //Printing command ID
        Serial.println(CMDID);
      }
  }
  delay(300);
}