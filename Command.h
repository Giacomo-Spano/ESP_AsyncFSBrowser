#pragma once

#include <Arduino.h>


#include "Logger.h"
#include "Shield.h"
#include "HttpHelper.h"
#include "JSON.h"
#include "Program.h"

class Command
{
private:
	String tag;
public:
	
	static const int maxLogSize = 1000;
	static String serverName;
	static int serverport;

	Command();
	~Command();

	int registerShield(Shield shield);
	void sendRestartNotification();
	boolean sendSensorsStatus(Shield shield);
	bool sendLog(String log/*, int shieldid/*, String serverName, int port*/);
	int timeSync(/*String serverName, int port*/);
	boolean sendActuatorStatus(HeaterActuator actuator);
	boolean download(String filename, Shield shield);

	//time_t getNtpTime();
	static Command *getTimeObject;

	static time_t globalGetNTPTime() {
		Serial.println("AAA");
		return Command::getTimeObject->getNtpTime();
	}
private:
	static time_t getNtpTime();
};

