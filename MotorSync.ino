#include "m_motor_controller.h"
#include "m_dc_motor.h"
#include "m_pot_reader.h"
#include "m_pid.h"

// Pin definitions
#define SPEED_PIN A4
#define ANGLE_PIN A1

#define ENC_1_PIN 10
#define ENC_2_PIN 11

#define M1_PWM_PIN 5
#define M2_PWM_PIN 6

#define M1_PIN1 7
#define M1_PIN2 8
#define M2_PIN1 4
#define M2_PIN2 9

#define M1_CURR_PIN A2
#define M2_CURR_PIN A3

#define LED1_PIN 12
#define LED2_PIN 13

// Task periodicity
#define READ_PERIOD 100
#define UPDATE_PID 1000
#define REPORT_PERIOD 2000

// PID params
#define KP 0.075
#define KI 0.0
#define KD 0.0

volatile unsigned long clicks1;
volatile unsigned long clicks2;

MPotReader<10> speedReader(SPEED_PIN, READ_PERIOD);
MPotReader<10> angleReader(ANGLE_PIN, READ_PERIOD);

MPotReader<1> m1CurrentReader(M1_CURR_PIN, READ_PERIOD);
MPotReader<1> m2CurrentReader(M2_CURR_PIN, READ_PERIOD);

MPid wheel1Pid(KP, KI, KD, 255, 100);
MPid wheel2Pid(KP, KI, KD,255, 100);

MDcMotor motor1(M1_PIN1, M1_PIN2, M1_PWM_PIN);
MDcMotor motor2(M2_PIN1, M2_PIN2, M2_PWM_PIN);

void setup() {
	speedReader.init();
	angleReader.init();
	m1CurrentReader.init();
	m2CurrentReader.init();	
	
	pinMode(LED1_PIN, OUTPUT);
	pinMode(LED2_PIN, OUTPUT);

	motor1.init();
	motor1.forward();

	motor2.init();
	motor2.forward();
		
	pinMode(ENC_1_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(ENC_1_PIN), onEncoder1, RISING);

	pinMode(ENC_2_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(ENC_2_PIN), onEncoder2, RISING);

	Serial.begin(57600);
	#ifdef DEBUG_PID
	Serial.println("===============");
	#endif
}

void loop() {
	speedReader.execute();
	angleReader.execute();
	m1CurrentReader.execute();
	m2CurrentReader.execute();
}

void onEncoder1() {
	clicks1++;
}

void onEncoder2() {
	clicks2++;
}