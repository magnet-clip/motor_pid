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

#define WHEEL_STOP_THRESHOLD 1000
#define ENCODER_LINES 8
#define ENCODER_AVG_PERIOD 20
#define ENCODER_ALPHA ((2.0/(ENCODER_AVG_PERIOD+1)))

#define SPEED_AVG_PERIOD 10
#define SPEED_ALPHA ((2.0/(SPEED_AVG_PERIOD+1)))

int angle;
double smoothedRpm1, smoothedRpm2;
double realRpm1, realRpm2;

double error1, error2;

#define KP 0.1
#define KI 0

double pid1(double desiredRpm, double actualRpm) {
	static double errorIntegral = 0.0;
	
	error1 = desiredRpm - actualRpm;
	errorIntegral += error1;

	return KP*error1 + KI*errorIntegral;
}

double pid2(double desiredRpm, double actualRpm) {
	static double errorIntegral = 0.0;
	
	error2 = desiredRpm - actualRpm;
	errorIntegral += error2;

	return KP*error2 + KI*errorIntegral;
}

void readSpeedAndAngle() {
	static double desiredPotSpeed, lastPotSpeed;
	static int speedPwmDutyCycle1 = 0, speedPwmDutyCycle2 = 0;

	lastPotSpeed = desiredPotSpeed;
	double potSpeed = (double)analogRead(SPEED_PIN);
	desiredPotSpeed = SPEED_ALPHA*potSpeed + (1-SPEED_ALPHA)*lastPotSpeed;
	
	double desiredRpm = desiredPotSpeed / 10.24; // desiredPotSpeed is between 0 and 1024, and max rpm is 100
	desiredRpm = desiredRpm > 100 ? 100: desiredRpm; // for any occasion
	
	Serial.print("Not smoothed actual 1/2 RPM: ");
	Serial.print(realRpm1);
	Serial.print("/");
	Serial.println(realRpm2);
	
	
	Serial.print("Smoothed actual1/2 & desired RPM: ");
	Serial.print(smoothedRpm1);
	Serial.print("/");
	Serial.print(smoothedRpm2);
	Serial.print(" & ");
	Serial.println(desiredRpm);
	
	double p1 = pid1(desiredRpm, smoothedRpm1); // delta in rpms
	double p2 = pid2(desiredRpm, smoothedRpm2);
	
	Serial.print("PIDs: ");
	Serial.print(p1);
	Serial.print(" ");
	Serial.println(p2);
	
	double p1Abs = p1 < 0 ? -p1 : p1;
	double p2Abs = p2 < 0 ? -p2 : p2;
	
	double p1AbsConstrained = constrain(p1Abs, 0, 100);
	double p2AbsConstrained = constrain(p2Abs, 0, 100);
	
	double thePid1 = map(p1AbsConstrained, 0, 100, 0, 255);
	double thePid2 = map(p2AbsConstrained, 0, 100, 0, 255);
	
	Serial.print("PIDs adjusted: ");
	Serial.print(thePid1);
	Serial.print(" ");
	Serial.println(thePid2);

	int direction1 = p1 < 0 ? -1 : 1;
	int direction2 = p2 < 0 ? -1 : 1;
	

	if (direction1 > 0) {
		speedPwmDutyCycle1 += thePid1;
	} else {
		speedPwmDutyCycle1 -= thePid1; 
	}

	if (direction2 > 0) {
		speedPwmDutyCycle2 += thePid2;
	} else {
		speedPwmDutyCycle2 -= thePid2;
	}
	
	Serial.print("PWMs: ");
	Serial.print(speedPwmDutyCycle1);
	Serial.print(" ");
	Serial.println(speedPwmDutyCycle2);

	analogWrite(M1_PWM_PIN, constrain(speedPwmDutyCycle1, 0, 255));
	analogWrite(M2_PWM_PIN, constrain(speedPwmDutyCycle2, 0, 255));
	
	Serial.println("--------------");


	angle = constrain(map(analogRead(ANGLE_PIN),  0, 1023, 0, 255), 0, 255);
}


void readSpeedAndAngleOld() {
	static int speedPwmDutyCycle;
	static double desiredSpeed, lastSpeed;

	lastSpeed = desiredSpeed;
	double potSpeed = (double)analogRead(SPEED_PIN);
	desiredSpeed = SPEED_ALPHA*potSpeed + (1-SPEED_ALPHA)*lastSpeed;
	
	angle = constrain(map(analogRead(ANGLE_PIN),  0, 1023, 0, 255), 0, 255);

	speedPwmDutyCycle = map((int)desiredSpeed, 0, 1023, 0, 255);
	analogWrite(M1_PWM_PIN, speedPwmDutyCycle);
	analogWrite(M2_PWM_PIN, speedPwmDutyCycle);
}

void checkEncoders() {
	static unsigned long lastChangeTime1 = 0, lastChangeTime2 = 0;
	static int enc1, enc2;	
	//static double rpm1, rpm2;
	static double lastRealRpm1 = 0, lastRealRpm2 = 0;

	unsigned long t = millis();
	
	unsigned long dt1 = t - lastChangeTime1;
	unsigned long dt2 = t - lastChangeTime2;
	
	lastRealRpm1 = smoothedRpm1;
	lastRealRpm2 = smoothedRpm2;
	
	int new_enc1 = digitalRead(ENC_1_PIN);
	if (new_enc1 != enc1) {
		if (lastChangeTime1 > 0) {
			realRpm1 = 30000.0/(dt1*ENCODER_LINES);
		}
		lastChangeTime1 = t;
		digitalWrite(LED1_PIN, new_enc1);
		smoothedRpm1 = realRpm1*ENCODER_ALPHA + lastRealRpm1*(1-ENCODER_ALPHA);
	} else if (dt1 > WHEEL_STOP_THRESHOLD) {
		realRpm1 = 0;
		lastRealRpm1 = 0;
		smoothedRpm1 = 0;
		lastChangeTime1 = 0;
	}	
	enc1 = new_enc1;
	
	int new_enc2 = digitalRead(ENC_2_PIN);
	if (new_enc2 != enc2) {
		if (lastChangeTime2 > 0) {
			realRpm2 = 30000.0/(dt2*ENCODER_LINES);
		}
		lastChangeTime2 = t;
		digitalWrite(LED2_PIN, new_enc2);
		smoothedRpm2 = realRpm2*ENCODER_ALPHA + lastRealRpm2*(1-ENCODER_ALPHA);
	} else if (dt2 > WHEEL_STOP_THRESHOLD) {
		realRpm2 = 0;
		lastRealRpm2 = 0;
		smoothedRpm2 = 0;
		lastChangeTime2 = 0;
	}
	enc2 = new_enc2;
}

void setup() {
	pinMode(SPEED_PIN, INPUT);
	pinMode(ANGLE_PIN, INPUT);
	pinMode(ENC_1_PIN, INPUT);
	pinMode(ENC_2_PIN, INPUT);

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
	
	
	Serial.begin(57600);
	Serial.print("===============");
}


#define RESET_PERIOD 30000

#define REPORT_PERIOD 20000
#define READ_PERIOD 1000
#define CHECK_ENCODERS 10

int stepNum = 0;

void loop() {
	stepNum++;
	
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
	
	if (stepNum % READ_PERIOD == 0) {
		readSpeedAndAngle();
	}

	if (stepNum % CHECK_ENCODERS == 0) {
		checkEncoders();
	}

	if (stepNum % RESET_PERIOD == 0) {
		stepNum = 0;
	}
}