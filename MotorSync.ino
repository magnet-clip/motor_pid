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
double realRpm1, realRpm2;

void readSpeedAndAngle() {
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
	static double rpm1, rpm2;
	static double lastRealRpm1 = 0, lastRealRpm2 = 0;

	unsigned long t = millis();
	
	unsigned long dt1 = t - lastChangeTime1;
	unsigned long dt2 = t - lastChangeTime2;
	
	lastRealRpm1 = realRpm1;
	lastRealRpm2 = realRpm2;
	
	int new_enc1 = digitalRead(ENC_1_PIN);
	if (new_enc1 != enc1) {
		if (lastChangeTime1 > 0) {
			rpm1 = 30000.0/(dt1*ENCODER_LINES);
		}
		lastChangeTime1 = t;
		digitalWrite(LED1_PIN, new_enc1);
		realRpm1 = rpm1*ENCODER_ALPHA + lastRealRpm1*(1-ENCODER_ALPHA);
	} else if (dt1 > WHEEL_STOP_THRESHOLD) {
		rpm1 = 0;
		lastRealRpm1 = 0;
		realRpm1 = 0;
		lastChangeTime1 = 0;
	}	
	enc1 = new_enc1;
	
	int new_enc2 = digitalRead(ENC_2_PIN);
	if (new_enc2 != enc2) {
		if (lastChangeTime2 > 0) {
			rpm2 = 30000.0/(dt2*ENCODER_LINES);
		}
		lastChangeTime2 = t;
		digitalWrite(LED2_PIN, new_enc2);
		realRpm2 = rpm2*ENCODER_ALPHA + lastRealRpm2*(1-ENCODER_ALPHA);
	} else if (dt2 > WHEEL_STOP_THRESHOLD) {
		rpm2 = 0;
		lastRealRpm2 = 0;
		realRpm2 = 0;
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
}


#define RESET_PERIOD 30000

#define REPORT_PERIOD 20000
#define READ_PERIOD 100
#define CHECK_ENCODERS 10

int stepNum = 0;

void loop() {
	stepNum++;
	
	if (stepNum % REPORT_PERIOD == 0) {
		Serial.print("RPM: ");
		Serial.print(realRpm1);
		Serial.print(" ");
		Serial.println(realRpm2);
		Serial.print("Current current: ");
		Serial.print(analogRead(M1_CURR_PIN));
		Serial.print(" ");
		Serial.println(analogRead(M2_CURR_PIN));
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