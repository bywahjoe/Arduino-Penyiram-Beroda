#include <Servo.h>
#include "ardupin.h"

//SERVO;
Servo myservoL;
Servo myservoR;
//SERVO CONFIG
int setSudutServo = 70;
int setDefaultSpeedMotor = 100;
int setMaxServo = 160;
int setMinServo = setSudutServo;
int setIntervalServo = 1;

int waktuTanam = 20;
//VAR BACA SENSOR
int sensorL;
int sensorR;

//JALAN
int jalan = 1;
int maxJalan = 3;

void tanam(int delayTanam = waktuTanam);
void setDefaultServo(int sudut = setSudutServo);
void setDefaultServoL(int sudut = setSudutServo);
void setDefaultServoR(int sudut = setSudutServo);

void motorKanan(int myspeed = setDefaultSpeedMotor);
void motorKiri(int myspeed = setDefaultSpeedMotor);
void maju(int myspeed = setDefaultSpeedMotor);
void mundur(int myspeed = setDefaultSpeedMotor);
char recv;

void setup() {
  Serial.begin(38400);
  pinMode(kananIR, INPUT);
  pinMode(kiriIR, INPUT);
  pinMode(depanIR, INPUT);
  //MOTOR
  pinMode(enA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //RELAY
  pinMode(relayKanan, OUTPUT);
  pinMode(relayKiri, OUTPUT);
  pinMode(relayTanam, OUTPUT);
  digitalWrite(relayKanan, HIGH);
  digitalWrite(relayKiri, HIGH);
  digitalWrite(relayTanam, HIGH);

  //SERVO
  myservoL.attach(servoKiri);
  myservoR.attach(servoKanan);
  setDefaultServo();

}

void loop() {
  if (Serial.available()) {
    recv = Serial.read();
    if (recv == 'r') {
      //Serial.print("RAWAT");
      Serial.write("MODE RAWAT \n");
      while (digitalRead(depanIR) != LOW) {

        if (digitalRead(kananIR) == LOW || digitalRead(kiriIR) == LOW) {
          delay(150);
          //stops();
          rem();
          delay(500);
          if (digitalRead(kananIR) == LOW && digitalRead(kiriIR) == LOW) {
            //stops();
            turunServo();
            startSensorRelay();
            naikServo();
            maju();
            while (digitalRead(kananIR) != HIGH && digitalRead(kiriIR) != HIGH) {
            }
            delay(500);
          } else if (digitalRead(kananIR) == LOW) {
            //stops();
            turunServoR();
            startSensorRelayR();
            naikServoR();
            maju();
            while (digitalRead(kananIR) != HIGH) {}
          } else if (digitalRead(kiriIR) == LOW) {
            //stops();
            turunServoL();
            startSensorRelayL();
            naikServoL();
            maju();
            while (digitalRead(kiriIR) != HIGH) {}
          }

        } else {
          maju();
        }

      }
      stops();
    }
    else if (recv == 't') {
      //Serial.print("TANAM");
      Serial.write("MODE TANAM \n");
      while (digitalRead(depanIR) != LOW) {

        if (digitalRead(kananIR) == LOW || digitalRead(kiriIR) == LOW) {
          delay(150);
          //stops();
          rem();
          delay(500);
          if (digitalRead(kananIR) == LOW && digitalRead(kiriIR) == LOW) {
            //stops();
            turunServo();
            tanam();
            startSensorRelay();
            naikServo();
            maju();
            while (digitalRead(kananIR) != HIGH && digitalRead(kiriIR) != HIGH) {
            }
            delay(500);
          } else if (digitalRead(kananIR) == LOW) {
            //stops();
            turunServoR();
            tanam();
            startSensorRelayR();
            naikServoR();
            maju();
            while (digitalRead(kananIR) != HIGH) {}
          } else if (digitalRead(kiriIR) == LOW) {
            //stops();
            turunServoL();
            tanam();
            startSensorRelayL();
            naikServoL();
            maju();
            while (digitalRead(kiriIR) != HIGH) {}
          }

        } else {
          maju();
        }

      }
      stops();
    } else if (recv == 's') {
      //Serial.print("TANAM");
      Serial.write("MODE SPECIAL \n");
      while (1) {
        if (jalan <= maxJalan) maju();
        else if (jalan <= maxJalan * 2) mundur();
        jalan++;

        delay(1500);
        stops();
        turunServo();

        if (jalan <= maxJalan) {
          delay(2000);
          tanam();
        }

        startSensorRelay();
        naikServo();

        if (jalan > maxJalan * 2) {
          jalan = 1; stops(); break;
        }
      }


    }
    else {}

  }
  //
  //  if (jalan <= maxJalan) maju();
  //  else if (jalan <= maxJalan * 2) mundur();
  //  jalan++;
  //
  //  delay(2000);
  //  stops();
  //  turunServo();
  //  tanam();
  //  startSensorRelay();
  //  naikServo();
  //
  //  if (jalan > maxJalan * 2) {
  //    jalan = 1;
  //  }

}
void tanam(int delayTanam) {
  digitalWrite(relayTanam, LOW);
  delay(20);
  digitalWrite(relayTanam, HIGH);
}
void motorKanan(int myspeed) {
  if (myspeed == 0) {
    analogWrite(enA, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  } else if (myspeed < 0) {
    myspeed = abs(myspeed);
    analogWrite(enA, myspeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    analogWrite(enA, myspeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }

}
void motorKiri(int myspeed) {
  if (myspeed == 0) {
    analogWrite(enB, 0);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
  else if (myspeed < 0) {
    myspeed = abs(myspeed);
    analogWrite(enB, myspeed);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    analogWrite(enB, myspeed);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }

}
void setMotor(int motorKiriSpeed, int motorKananSpeed) {
  motorKiri(motorKiriSpeed);
  motorKanan(motorKananSpeed);
}
void maju(int myspeed) {
  setMotor(myspeed, myspeed);
}
void mundur(int myspeed) {
  setMotor(-myspeed, -myspeed);
}
void stops() {
  setMotor(0, 0);
}
void rem() {
  analogWrite(enA, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(enB, 255);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}
void setDefaultServo(int sudut) {
  int sudutSpecial = constrain(sudut + 10, 0, 180);
  myservoL.write(sudutSpecial);
  myservoR.write(sudut);
}
void setDefaultServoL(int sudut) {
  int sudutSpecial = constrain(sudut + 10, 0, 180);
  myservoL.write(sudutSpecial);
}
void setDefaultServoR(int sudut) {
  myservoR.write(sudut);
}
void turunServo() {
  for (int i = setMinServo; i <= setMaxServo; i += setIntervalServo) {
    setDefaultServo(i);
    delay(50);
  }
}
void turunServoL() {
  for (int i = setMinServo; i <= setMaxServo; i += setIntervalServo) {
    setDefaultServoL(i);
    delay(50);
  }
}
void turunServoR() {
  for (int i = setMinServo; i <= setMaxServo; i += setIntervalServo) {
    setDefaultServoR(i);
    delay(50);
  }
}
void naikServo() {
  for (int i = setMaxServo; i >= setMinServo; i -= setIntervalServo) {
    setDefaultServo(i);
    delay(50);
  }
}
void naikServoL() {
  for (int i = setMaxServo; i >= setMinServo; i -= setIntervalServo) {
    setDefaultServoL(i);
    delay(50);
  }
}
void naikServoR() {
  for (int i = setMaxServo; i >= setMinServo; i -= setIntervalServo) {
    setDefaultServoR(i);
    delay(50);
  }
}
void startSensorRelay() {

  sensorL = digitalRead(sensorKiri);
  sensorR = digitalRead(sensorKanan);
  Serial.print("Kiri:"); Serial.println(sensorL);
  Serial.print("\tKanan:"); Serial.println(sensorR);
  if (sensorL == HIGH) {
    digitalWrite(relayKiri, LOW);
  }
  if (sensorR == HIGH) {
    digitalWrite(relayKanan, LOW);
  }
  delay(5000);
  digitalWrite(relayKanan, HIGH);
  digitalWrite(relayKiri, HIGH);
}
void startSensorRelayL() {
  sensorL = digitalRead(sensorKiri);

  Serial.print("Kiri:"); Serial.println(sensorL);
  if (sensorL == HIGH) {
    digitalWrite(relayKiri, LOW);
  }
  delay(5000);
  digitalWrite(relayKiri, HIGH);
}

void startSensorRelayR() {
  sensorR = digitalRead(sensorKanan);
  Serial.print("\tKanan:"); Serial.println(sensorR);
  
  if (sensorR == HIGH) {
    digitalWrite(relayKanan, LOW);
  }
  delay(5000);
  digitalWrite(relayKanan, HIGH);
}
