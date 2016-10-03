#include "m_reporter.h"
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

#define ENC_1_PIN 2
#define ENC_2_PIN 3

#define M1_PWM_PIN 5
#define M2_PWM_PIN 6

#define M1_PIN1 7
#define M1_PIN2 8
#define M2_PIN1 4
#define M2_PIN2 9

#define M1_CURR_PIN A2
#define M2_CURR_PIN A3

#define MAX_RPM 100
#define ENCODER_LINES 100

// Task periodicity
#define READ_PERIOD 10
#define UPDATE_PID 100
#define REPORT_PERIOD 2000

// PID params
#define KP 5
#define KI 0.0
#define KD 0.0

#define DEBUG_PID

MPotReader speedReader(SPEED_PIN, READ_PERIOD);
MPotReader angleReader(ANGLE_PIN, READ_PERIOD);
MPotReader m1CurrentReader(M1_CURR_PIN, READ_PERIOD);
MPotReader m2CurrentReader(M2_CURR_PIN, READ_PERIOD);

MPid wheel1Pid(KP, KI, KD, 1024, 1024);
MPid wheel2Pid(KP, KI, KD, 1024, 1024);

MDcMotor motor1(M1_PIN1, M1_PIN2, M1_PWM_PIN);
MDcMotor motor2(M2_PIN1, M2_PIN2, M2_PWM_PIN);

MClickCounter counter1;
MClickCounter counter2;

MMotorController controller1(UPDATE_PID, MAX_RPM, ENCODER_LINES, &motor1, &counter1, &wheel1Pid, &speedReader);
MMotorController controller2(UPDATE_PID, MAX_RPM, ENCODER_LINES, &motor2, &counter2, &wheel2Pid, &speedReader);

MReporter reporter(REPORT_PERIOD, &controller1, &controller2);

void setup() {
	speedReader.init();
	angleReader.init();
	m1CurrentReader.init();
	m2CurrentReader.init();	
	
	motor1.init();
	motor1.forward();

	motor2.init();
	motor2.forward();
		
	
	controller1.init();
	controller2.init();

	reporter.init();
	
	#ifndef DEBUG_PID
	reporter.disable();
	#endif

	pinMode(ENC_1_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(ENC_1_PIN), onEncoder1, CHANGE);

	pinMode(ENC_2_PIN, INPUT);
	attachInterrupt(digitalPinToInterrupt(ENC_2_PIN), onEncoder2, CHANGE);
}

unsigned long c = 0;

volatile unsigned long count1 = 0;
volatile unsigned long count2 = 0;

void loop() {
	speedReader.execute();
	angleReader.execute();
	m1CurrentReader.execute();
	m2CurrentReader.execute();
	
	controller1.execute();
	controller2.execute();
	
	reporter.execute();
	//
	//if (c++ == 100000) {
		//Serial.print("--------------------");
		//Serial.print(count1);
		//Serial.print(" ");
		//Serial.print(count2);
		//Serial.println("--------------------");
		//c = 0;	
	//}
}

void onEncoder1() {
//	count1++;
	counter1.increment();
}

void onEncoder2() {
//	count2++;
	counter2.increment();
}