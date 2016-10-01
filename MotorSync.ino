#include "m_ewma.h"
#include "m_smoother.h"
#include "m_click_counter.h"
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

MPotReader speedReader(SPEED_PIN, READ_PERIOD);
MPotReader angleReader(ANGLE_PIN, READ_PERIOD);
MPotReader m1CurrentReader(M1_CURR_PIN, READ_PERIOD);
MPotReader m2CurrentReader(M2_CURR_PIN, READ_PERIOD);

MPid wheel1Pid(KP, KI, KD, 255, 100);
MPid wheel2Pid(KP, KI, KD,255, 100);

MDcMotor motor1(M1_PIN1, M1_PIN2, M1_PWM_PIN);
MDcMotor motor2(M2_PIN1, M2_PIN2, M2_PWM_PIN);

MClickCounter counter1;
MClickCounter counter2;

MMotorController<18000> controller1(UPDATE_PID, &motor1, &counter1, &wheel1Pid, &speedReader);
MMotorController<18000> controller2(UPDATE_PID, &motor2, &counter2, &wheel2Pid, &speedReader);

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
	
	controller1.init();
	controller2.init();

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
	
	controller1.execute();
	controller2.execute();
}

void onEncoder1() {
	counter1.increment();
}

void onEncoder2() {
	counter2.increment();
}