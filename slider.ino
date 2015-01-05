#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

YunServer server;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *sliderMotor = AFMS.getStepper(200, 2);

#define SHUTTER_PIN 7
#define SHUTTER_SHUDDER_DELAY 150
#define SHUTTER_DELAY 50
#define STEPS_PER_MM 6.25

boolean RUN = false;
boolean RUNNING = false;

int DISTANCE = 630;
int DELAY = 800;
int STEPS_PER_RUN = 1;

int TOTAL_RUNS = 0;
int RUN_COUNT = 0;

void setup() {

	Serial.begin(9600);

	// Setup shutter pin
	pinMode(SHUTTER_PIN, OUTPUT);

	// Setup stepper motor
	AFMS.begin();
	sliderMotor->setSpeed(500);
	sliderMotor->release();

	// Bridge startup
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);
	Bridge.begin();
	digitalWrite(13, HIGH);

	// Start server
	server.listenOnLocalhost();
	server.begin();

}

void loop() {

	// Get clients coming from server
	YunClient client = server.accept();
	if (client) {

		// read the command
		String command = client.readStringUntil('/');

		// Run
		if (command == "run") {
			RUN = (bool) client.parseInt();
		}

		// Reset
		if (command == "reset") {
			sliderMotor->step((RUN_COUNT * STEPS_PER_RUN), BACKWARD, DOUBLE);
			sliderMotor->release();
			RUN_COUNT = 0;
		}

		// Distance
		if (command == "distance") {
			DISTANCE = client.parseInt();
		}

		// Delay
		if (command == "delay") {
			DELAY = client.parseInt();
		}

		// Steps per run
		if (command == "steps") {
			STEPS_PER_RUN = client.parseInt();
		}

		// Send status
		TOTAL_RUNS = (int) floor((DISTANCE * STEPS_PER_MM) / STEPS_PER_RUN);
		status(client);

		client.stop();

	}

	// Start running
	if (RUN && ! RUNNING) {
		RUNNING = true;
	}

	// Stop running
	if ( ! RUN && RUNNING) {
		RUN = false;
		RUNNING = false;
		sliderMotor->release();
	}

	// Run
	if (RUN && RUNNING) {

		if ((RUN_COUNT + 1) >= TOTAL_RUNS) {
			RUN = false;
			RUNNING = false;
			sliderMotor->release();
		} else {

			RUN_COUNT++;

			// Trigger shutter
			delay(SHUTTER_SHUDDER_DELAY);
			digitalWrite(SHUTTER_PIN, HIGH);
			delay(SHUTTER_DELAY);
			digitalWrite(SHUTTER_PIN, LOW);
			delay(DELAY);

			// Move slider
			sliderMotor->step(STEPS_PER_RUN, FORWARD, DOUBLE);

		}

	}

	delay(10);

}

void status(YunClient client) {

	client.print("Shutter shudder delay: ");
	client.print(SHUTTER_SHUDDER_DELAY);
	client.print("\n");

	client.print("Shutter delay: ");
	client.print(SHUTTER_DELAY);
	client.print("\n");

	client.print("Steps per mm: ");
	client.print(STEPS_PER_MM);
	client.print("\n");

	client.print("Run: ");
	client.print(RUN);
	client.print("\n");

	client.print("Running: ");
	client.print(RUNNING);
	client.print("\n");

	client.print("Distance: ");
	client.print(DISTANCE);
	client.print("\n");

	client.print("Delay: ");
	client.print(DELAY);
	client.print("\n");

	client.print("Steps per run: ");
	client.print(STEPS_PER_RUN);
	client.print("\n");

	client.print("Total runs: ");
	client.print(TOTAL_RUNS);
	client.print("\n");

	client.print("Runs completed: ");
	client.print(RUN_COUNT);
	client.print("\n");

	client.print("Slide duration: ");
	client.print((int) (TOTAL_RUNS * (float) ((float) (DELAY + SHUTTER_SHUDDER_DELAY + SHUTTER_DELAY) / 1000)));
	client.print("\n");

}
