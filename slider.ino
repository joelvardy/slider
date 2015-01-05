#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

YunServer server;

#define SHUTTER_PIN 7
#define SHUTTER_SHUDDER_DELAY 150
#define SHUTTER_DELAY 50
#define STEPS_PER_MM 6.25

boolean RUN = false;
boolean RUNNING = false;

int DISTANCE = 630;
int DELAY = 800;
int STEPS_PER_RUN = 1;

void setup() {

	Serial.begin(9600);

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

		// Status
		if (command == "status") {
			//
		}

		// Run
		if (command == "run") {
			RUN = (bool) client.parseInt();
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
		status(client);

		client.stop();

	}

	delay(50);

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

}
