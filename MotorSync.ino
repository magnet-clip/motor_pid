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

#define LED1_PIN 12
#define LED2_PIN 13

#define WHEEL_STOP_THRESHOLD 1000
#define ENCODER_LINES 20
#define ENCODER_AVG_PERIOD 20
#define ENCODER_ALPHA ((2.0/(ENCODER_AVG_PERIOD+1)))

#define SPEED_AVG_PERIOD 10
#define SPEED_ALPHA ((2.0/(SPEED_AVG_PERIOD+1)))

volatile int angle;
volatile double smoothedRpm1, smoothedRpm2;
volatile double realRpm1, realRpm2;

volatile double error1, error2;
volatile int stepNum = 0;

volatile double desiredPotSpeed;
volatile int speedPwmDutyCycle1 = 0, speedPwmDutyCycle2 = 0;

#define MAX_RPM 5000.0

#define RESET_PERIOD 30000
#define REPORT_PERIOD 20000

// does not make sense - encoders get updated only several times per second, while pot get updated continuously!
#define READ_PERIOD 100
#define CHECK_ENCODERS 10
#define UPDATE_WHEELS 10000

#define KP 0.075
#define KI 0.0

#define MAX_ERROR_INTEGRAL 20

#define DEBUG_PID

//double pid1(double desiredRpm, double actualRpm) {
	//static double errorIntegral = 0.0;
	//
	//error1 = desiredRpm - actualRpm;
	//errorIntegral += error1;
//
	//if (errorIntegral > MAX_ERROR_INTEGRAL) errorIntegral = MAX_ERROR_INTEGRAL;
	//if (errorIntegral < -MAX_ERROR_INTEGRAL) errorIntegral = -MAX_ERROR_INTEGRAL;
//
	//return KP*error1 + KI*errorIntegral;
//}
//
//double pid2(double desiredRpm, double actualRpm) {
	//static double errorIntegral = 0.0;
	//
	//error2 = desiredRpm - actualRpm;
	//errorIntegral += error2;
	//
	//if (errorIntegral > MAX_ERROR_INTEGRAL) errorIntegral = MAX_ERROR_INTEGRAL;
	//if (errorIntegral < -MAX_ERROR_INTEGRAL) errorIntegral = -MAX_ERROR_INTEGRAL;
//
	//return KP*error2 + KI*errorIntegral;
//}
//
//void readSpeedAndAngle() {
	//static double lastPotSpeed;
//
	//lastPotSpeed = desiredPotSpeed;
	//double potSpeed = (double)analogRead(SPEED_PIN);
	//desiredPotSpeed = SPEED_ALPHA*potSpeed + (1-SPEED_ALPHA)*lastPotSpeed;
//}
//
//void updateWheels() {
	//double desiredRpm = desiredPotSpeed / (1024.0 / MAX_RPM); 
	//desiredRpm = desiredRpm > MAX_RPM ? MAX_RPM: desiredRpm; // for any occasion
		//
	//double p1 = pid1(desiredRpm, smoothedRpm1); // delta in rpms
	//double p2 = pid2(desiredRpm, smoothedRpm2);
		//
	//double p1Abs = p1 < 0 ? -p1 : p1;
	//double p2Abs = p2 < 0 ? -p2 : p2;
		//
	//double p1AbsConstrained = constrain(p1Abs, 0, MAX_RPM);
	//double p2AbsConstrained = constrain(p2Abs, 0, MAX_RPM);
		//
	//double thePid1 = map(p1AbsConstrained, 0, MAX_RPM, 0, 255);
	//double thePid2 = map(p2AbsConstrained, 0, MAX_RPM, 0, 255);
//
	//int direction1 = p1 < 0 ? -1 : 1;
	//int direction2 = p2 < 0 ? -1 : 1;
//
	//if (direction1 > 0) {
		//speedPwmDutyCycle1 += thePid1;
		//} else {
		//speedPwmDutyCycle1 -= thePid1;
	//}
//
	//if (direction2 > 0) {
		//speedPwmDutyCycle2 += thePid2;
		//} else {
		//speedPwmDutyCycle2 -= thePid2;
	//}
		//
	//int m1Signal = constrain(speedPwmDutyCycle1, 0, 255);
	//int m2Signal = constrain(speedPwmDutyCycle2, 0, 255);
//
	//analogWrite(M1_PWM_PIN, m1Signal);
	//analogWrite(M2_PWM_PIN, m2Signal);
		//
		//
	//#ifdef DEBUG_PID
	//Serial.print("Not smoothed actual 1/2 RPM: ");
	//Serial.print(realRpm1);
	//Serial.print("/");
	//Serial.println(realRpm2);
		//
		//
	//Serial.print("Smoothed actual 1/2 & desired RPM: ");
	//Serial.print(smoothedRpm1);
	//Serial.print("/");
	//Serial.print(smoothedRpm2);
	//Serial.print(" & ");
	//Serial.println(desiredRpm);
		//
	//Serial.print("PIDs: ");
	//Serial.print(p1);
	//Serial.print(" ");
	//Serial.println(p2);
		//
	//Serial.print("PIDs adjusted: ");
	//Serial.print(thePid1);
	//Serial.print(" ");
	//Serial.println(thePid2);
		//
	//Serial.print("PWMs: ");
	//Serial.print(speedPwmDutyCycle1);
	//Serial.print(" ");
	//Serial.println(speedPwmDutyCycle2);
		//
		//
	//Serial.print("Signals: ");
	//Serial.print(m1Signal);
	//Serial.print(" ");
	//Serial.println(m2Signal);
//
	//Serial.println("--------------");
	//#endif
//
	//speedPwmDutyCycle1 = m1Signal;
	//speedPwmDutyCycle2 = m2Signal;
//
	//angle = constrain(map(analogRead(ANGLE_PIN),  0, 1023, 0, 255), 0, 255);
//}
//
//void readSpeedAndAngleOld() {
	//static int speedPwmDutyCycle;
	//static double desiredSpeed, lastSpeed;
//
	//lastSpeed = desiredSpeed;
	//double potSpeed = (double)analogRead(SPEED_PIN);
	//desiredSpeed = SPEED_ALPHA*potSpeed + (1-SPEED_ALPHA)*lastSpeed;
	//
	//angle = constrain(map(analogRead(ANGLE_PIN),  0, 1023, 0, 255), 0, 255);
//
	//speedPwmDutyCycle = map((int)desiredSpeed, 0, 1023, 0, 255);
	//analogWrite(M1_PWM_PIN, speedPwmDutyCycle);
	//analogWrite(M2_PWM_PIN, speedPwmDutyCycle);
//}
	
//volatile unsigned long lastChangeTime1 = 0;
//volatile unsigned long lastChangeTime2 = 0;
volatile byte v = LOW;


void onEncoder1() {
	//static double lastRealRpm = 0;
	//
	//unsigned long t = millis();
	//unsigned long dt = t - lastChangeTime1;
	//
	//if (dt == 0) return; // safety check
	//
	//if (lastChangeTime1 > 0) {
		//realRpm1 = (60*1000.0)/(dt*ENCODER_LINES);
	//}
	//lastChangeTime1 = t;

	v = !v;

	//lastRealRpm = smoothedRpm1;
	//smoothedRpm1 = realRpm1*ENCODER_ALPHA + lastRealRpm*(1-ENCODER_ALPHA);
}
//
//void onEncoder2() {
	////static int v = HIGH;
	////static double lastRealRpm = 0;
	////
	////unsigned long t = millis();
	////unsigned long dt = t - lastChangeTime2;
	////
	////if (dt == 0) return; // safety check
	////
	////if (lastChangeTime2 > 0) {
		////realRpm2 = (60*1000.0)/(dt*ENCODER_LINES);
	////}
	////lastChangeTime2 = t;
////
	////v = (v == HIGH) ? LOW : HIGH;
	////digitalWrite(LED2_PIN, v);
////
	////lastRealRpm = smoothedRpm2;
	////smoothedRpm2 = realRpm1*ENCODER_ALPHA + lastRealRpm*(1-ENCODER_ALPHA);
//}

//void checkEncoders() {
	////unsigned long t = millis();
	////
	////unsigned long dt1 = t - lastChangeTime1;
	////unsigned long dt2 = t - lastChangeTime2;
	////
	////if (dt1 > WHEEL_STOP_THRESHOLD) {
		////realRpm1 = 0;
		////smoothedRpm1 = 0;
		////lastChangeTime1 = 0;
	////}	
	////
	////if (dt2 > WHEEL_STOP_THRESHOLD) {
		////realRpm2 = 0;
		////smoothedRpm2 = 0;
		////lastChangeTime2 = 0;
	////}
//}

void setup() {
	pinMode(SPEED_PIN, INPUT);
	pinMode(ANGLE_PIN, INPUT);

	pinMode(ENC_1_PIN, INPUT);
	pinMode(ENC_2_PIN, INPUT);

	pinMode(M1_CURR_PIN, INPUT);
	pinMode(M2_CURR_PIN, INPUT);

	pinMode(M1_PIN1, OUTPUT);
	pinMode(M1_PIN2, OUTPUT);
	pinMode(M2_PIN1, OUTPUT);
	pinMode(M2_PIN2, OUTPUT);

	pinMode(LED1_PIN, OUTPUT);
	pinMode(LED2_PIN, OUTPUT);

	digitalWrite(M1_PIN1, HIGH);
	digitalWrite(M1_PIN2, LOW);
	digitalWrite(M2_PIN1, HIGH);
	digitalWrite(M2_PIN2, LOW);
	
	attachInterrupt(digitalPinToInterrupt(ENC_1_PIN), onEncoder1, CHANGE);
//	attachInterrupt(digitalPinToInterrupt(ENC_2_PIN), onEncoder2, CHANGE);
	
	
	Serial.begin(57600);
	#ifdef DEBUG_PID
	Serial.println("===============");
	#endif
}

void loop() {
	
	digitalWrite(LED1_PIN, v);
	
	stepNum++;
		
	#ifdef DEBUG_PID
	if (stepNum % REPORT_PERIOD == 0) {
		Serial.print("RPM: ");
		Serial.print(smoothedRpm1);
		Serial.print(" ");
		Serial.println(smoothedRpm2);
		Serial.print("Current current: ");
		Serial.print(analogRead(M1_CURR_PIN));
		Serial.print(" ");
		Serial.println(analogRead(M2_CURR_PIN));
		Serial.print("Errors: ");
		Serial.print(error1);
		Serial.print(" ");
		Serial.println(error2);
	}
	#endif
		
	//if (stepNum % READ_PERIOD == 0) {
		//readSpeedAndAngle();
	//}

	//if (stepNum % CHECK_ENCODERS == 0) {
		//checkEncoders();
	//}
		
	// this seems to be wrong too
	// the main idea is that there are two approaches:
	//  1) calculate speed on pin change interrupts (and at that time update PIDs) => wheel stop undetectable
	//  2) calculate speed by polling wheels periodically (done here) => risk of missing pulses and pid update not tied to actual speed change
	// Here I use second approach, but first one is potentially better
	// And the third one:
	//  3)  - Register speed via PCINT
	//		- Have additional timer which resets speed to zero at overflow
	//		- At PCINT reset that timer
	//		- In case required speed is given by ADC, read ADC whenever you need (and should it be faster than time between PCINTs)
	//		- Or if speed is given by ADC by wheels rotate faster then ADC...
	// I must do calculations. Here they are:		
	// Let wheel be 3000 RPM => 50 RPS => 10000 lines per second => 10 kHz. Should handling one event require 1000 cycles (maths!), controller must operate at 10 MHz
	// We must be independent from both ADC speed and wheel speed. Wheel and ADC interrupt handler must be really fast: 
	// they must return only current values (or maybe some smoothed values). Hence there must be standalone time to handle PID updates. 
	// Just like I did here
	// => Everything's more or less correct
		
	//if (stepNum % UPDATE_WHEELS == 0) {
		//updateWheels();
	//}

	if (stepNum % RESET_PERIOD == 0) {
		stepNum = 0;
	}
}