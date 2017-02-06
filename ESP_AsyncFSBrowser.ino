#include "POSTData.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>


#include "js.h"
#include "html.h"
#include "HtmlFileClass.h"
//#include "ESP8266Webduino.h"
#include <string.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include "user_interface.h"
//#include "wol.h"
#include "Logger.h"
#include "HttpHelper.h"
#include "JSON.h"
#include "Shield.h"
#include "Command.h"
#include "Program.h"
#include "Actuator.h"
#include "DS18S20Sensor.h"
#include <Time.h>
#include "TimeLib.h"
#include "css.h"
#include "FS.h"

#define Versione 0.92
int EPROM_Table_Schema_Version = 6;
const char SWVERSION[] = "1.01";

String bodyContent = "";
const char* ssid = "TP-LINK_3BD796";
const char* password = "giacomocasa";
const char * hostName = "esp-async";
const char* http_username = "admin";
const char* http_password = "admin";




Logger logger;
String sensorNames = "";
const char* ssidx = "Telecom-29545833";
//const char* password = "6oGzjkdMJU2q9XoQLfWiV3nj";
String tag = "Webduino";
const char *ssidAP = "ES8266";
const char *passwordAP = "thereisnospoon";
//const char* ssid = "xxBUBBLES";
/*   da eliminare*/
byte mac[] = { 0x00, 0xA0, 0xB0, 0xC0, 0xD0, 0x90 };

extern const char* statusStr[] = { "unused", "idle", "program", "manual", "disabled", "restarted", "manualoff" };
Shield shield;

void initEPROM();
void readEPROM();
extern void writeEPROM();

String showMain(AsyncWebServerRequest *request);
String showChangeSettings(AsyncWebServerRequest *request);
String receiveCommand(String jsonString);

String showSettings(AsyncWebServerRequest *request);
//void showPage(String data);
//void getPostdata(char *data, int maxposdata);
//String getPostdata();

//int parsePostdata(const char* data, const char* param, char* value);
String getJsonStatus();
String getJsonSensorsStatus(AsyncWebServerRequest *request);
String getJsonActuatorsStatus(AsyncWebServerRequest *request);
String getJsonHeaterStatus(AsyncWebServerRequest *request);
String getJsonSettings(AsyncWebServerRequest *request);
String showIODevices(AsyncWebServerRequest *request);
String showHeater(AsyncWebServerRequest *request);
String showChangeIODevices(AsyncWebServerRequest *request);
//String showPower(AsyncWebServerRequest *request);
String softwareReset(AsyncWebServerRequest *request);
//String showIndex();
String setRemoteTemperature(AsyncWebServerRequest *request);
//String getFile(String filename);
//void sendFile(String header, const char * file);

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
WiFiClient client;

const int maxposdataChangeSetting = 150;
char databuff[maxposdataChangeSetting];

#define P(name) static const prog_uchar name[] PROGMEM // declare a static string
const byte EEPROM_ID = 0x99; // used to identify if valid data in EEPROM
const int ID_ADDR = 0; // the EEPROM address used to store the ID
const int TIMERTIME_ADDR = 1; // the EEPROM address used to store the pin
const int maxposdata = 101; // massimo numero di caratteri della variabile posdata
const int maxvaluesize = maxposdata - 1/*10*/; // massimo numero di digit del valore di una variabile nel POS data

bool statusChangeSent = true;
const int flash_interval = 30000;
unsigned long lastFlash = 0;//-flash_interval;
const int lastStatusChange_interval = 60000; // timeout retry invio status change
unsigned long lastStatusChange = 0;//-lastStatusChange_interval;// 0;
unsigned long lastNotification = 0;
const int Notification_interval = 20000;
unsigned long lastSendLog = 0;
const int SendLog_interval = 10000;// 10 secondi
unsigned long lastTimeSync = 0;
const int timeSync_interval = 60000;// 60 secondi
int offlineCounter = 0;
// variables created by the build process when compiling the sketch
extern int __bss_end;
extern void *__brkval;
int sendRestartNotification = 0;
bool shieldRegistered = false; // la shield si registra all'inizio e tutte le volte che va offline





void writeEPROM() {

	logger.println(tag, ">>write EPROM");

	int addr = TIMERTIME_ADDR;
	EEPROM.write(ID_ADDR, EEPROM_ID); // write the ID to indicate valid data
	byte hiByte;
	byte loByte;

	// dummy
	byte dummy;
	dummy = 3;
	EEPROM.write(addr++, dummy);
	logger.print(tag, "\n\t dummy = " + String(dummy));

	// build version
	hiByte = highByte(EPROM_Table_Schema_Version);
	loByte = lowByte(EPROM_Table_Schema_Version);
	EEPROM.write(addr++, hiByte);
	EEPROM.write(addr++, loByte);

	// heater pin
	uint8_t pin = Shield::getHeaterPin();
	EEPROM.write(addr++, pin);
	logger.print(tag, "\n\t pin = " + String(pin));

	// heater enabled
	bool heaterEnabled = Shield::getHeaterEnabled();
	EEPROM.write(addr++, heaterEnabled);
	if (heaterEnabled)
		logger.print(tag, "\n\t heaterenabled = true");
	else
		logger.print(tag, "\n\t heaterenabled = false");

	// io devices
	for (int i = 0; i < 10; i++) {
		hiByte = highByte(Shield::getIODevice(i));
		loByte = lowByte(Shield::getIODevice(i));
		EEPROM.write(addr++, hiByte);
		EEPROM.write(addr++, loByte);
		logger.print(tag, "\n\t iodevice = " + String(Shield::getIODevice(i)));
	}
	// local port
	int port = Shield::getLocalPort();
	hiByte = highByte(port);
	loByte = lowByte(port);
	EEPROM.write(addr++, hiByte);
	EEPROM.write(addr++, loByte);
	logger.print(tag, "\n\t port = " + String(port));
	// ssid
	char networkSSID[Shield::networkSSIDLen];
	Shield::getNetworkSSID().toCharArray(networkSSID, sizeof(networkSSID));
	int res = EEPROM_writeAnything(addr, networkSSID);
	addr += res;
	logger.print(tag, "\n\t networkSSID = " + String(networkSSID));
	// password
	char networkPasswordBuffer[Shield::networkPasswordLen];
	Shield::getNetworkPassword().toCharArray(networkPasswordBuffer, sizeof(networkPasswordBuffer));
	res = EEPROM_writeAnything(addr, networkPasswordBuffer);
	addr += res;
	logger.print(tag, "\n\t networkPasswordBuffer = " + String(networkPasswordBuffer));
	// server name
	char serverNameBuffer[Shield::serverNameLen];
	Shield::getServerName().toCharArray(serverNameBuffer, sizeof(serverNameBuffer));
	res = EEPROM_writeAnything(addr, serverNameBuffer);
	addr += res;
	logger.print(tag, "\n\t serverNameBuffer = " + String(serverNameBuffer));
	// server port
	port = Shield::getServerPort();
	hiByte = highByte(port);
	loByte = lowByte(port);
	EEPROM.write(addr++, hiByte);
	EEPROM.write(addr++, loByte);
	logger.print(tag, "\n\t port = " + String(port));
	// board name
	char shieldNameBuffer[Shield::shieldNameLen];
	Shield::getServerName().toCharArray(shieldNameBuffer, sizeof(shieldNameBuffer));
	res = EEPROM_writeAnything(addr, shieldNameBuffer);
	addr += res;
	logger.print(tag, "\n\t shieldNameBuffer = " + String(shieldNameBuffer));
	// sensor names
	String str = "";
	for (int i = 0; i < shield.sensorList.count; i++) {
		DS18S20Sensor* sensor = (DS18S20Sensor*)shield.sensorList.get(i);
		str += String(sensor->sensorname);
		str += ";";
	}
	logger.print(tag, "\n\t str=" + str);
	char buffer[100];
	str.toCharArray(buffer, sizeof(buffer));
	res = EEPROM_writeAnything(addr, buffer);
	addr += res;
	EEPROM.commit();
	logger.println(tag, "<<write EPROM");
}
void readEPROM() {

	logger.println(tag, "read EPROM");

	byte hiByte;
	byte lowByte;
	int addr = TIMERTIME_ADDR;
	// dummy
	byte dummy = EEPROM.read(addr++);

	logger.print(tag, "\n\t dummy=" + String(dummy));

	logger.print(tag, "\EPROM_Table_Schema_Version=");
	logger.print(tag, String(EPROM_Table_Schema_Version));

	int epromversion = 0;
	// epromversion
	hiByte = EEPROM.read(addr++);
	lowByte = EEPROM.read(addr++);
	epromversion = word(hiByte, lowByte);
	logger.print(tag, "\n\t epromversion=" + String(epromversion));

	if (epromversion >= 6/*EPROM_Table_Schema_Version*/) {
		uint8_t pin = EEPROM.read(addr++);
		Shield::setHeaterPin(pin);

		bool heaterEnabled = EEPROM.read(addr++);
		Shield::setHeaterEnabled(heaterEnabled);

		if (heaterEnabled)
			logger.println(tag, "\n\t heaterenabled = true");
		else
			logger.println(tag, "\n\t heaterenabled = false");

	}

	if (epromversion >= 5/*EPROM_Table_Schema_Version*/) {
		for (int i = 0; i < Shield::getMaxIoDevices(); i++) {
			hiByte = EEPROM.read(addr++);
			lowByte = EEPROM.read(addr++);
			Shield::setIODevice(i, word(hiByte, lowByte));

			logger.print(tag, "\ni=" + String(i));
			logger.print(tag, ", dev=" + String(Shield::getIODevice(i)));
		}
	}

	// local port
	hiByte = EEPROM.read(addr++);
	lowByte = EEPROM.read(addr++);
	int port = word(hiByte, lowByte);
	logger.print(tag, "\n\t port = ");
	logger.print(tag, String(port));
	Shield::setLocalPort(port);
	// ssid
	char networkSSIDBuffer[Shield::networkSSIDLen];
	int res = EEPROM_readAnything(addr, networkSSIDBuffer);
	//int res = EEPROM_readAnything(addr, buffer);
	logger.print(tag, "\n\t networkSSIDBuffer=" + String(networkSSIDBuffer));
	Shield::setNetworkSSID(String(networkSSIDBuffer)/*"TP-LINK_3BD796"*/);
	addr += res;
	// password
	char networkPasswordBuffer[Shield::networkPasswordLen];
	res = EEPROM_readAnything(addr, networkPasswordBuffer);
	//res = EEPROM_readAnything(addr, buffer);
	logger.print(tag, "\n\t networkPasswordBuffer = " + String(networkPasswordBuffer));
	Shield::setNetworkPassword(String(networkPasswordBuffer)/*"giacomocasa"*/);
	//Shield::setNetworkPassword("giacomocasa");
	addr += res;
	//server name
	char servernnameBuffer[Shield::serverNameLen];
	res = EEPROM_readAnything(addr, servernnameBuffer);
	logger.print(tag, "\n\tservernnameBuffer=" + String(servernnameBuffer));
	Shield::setServerName(String(servernnameBuffer));
	addr += res;
	// server port
	hiByte = EEPROM.read(addr++);
	lowByte = EEPROM.read(addr++);
	port = word(hiByte, lowByte);
	logger.print(tag, "\n\t port=" + String(port));
	Shield::setServerPort(port);
	// board name
	char shieldNameBuffer[Shield::shieldNameLen];
	res = EEPROM_readAnything(addr, shieldNameBuffer);
	logger.print(tag, "\n\t shieldNameBuffer =" + String(shieldNameBuffer));
	Shield::setShieldName(String(shieldNameBuffer));
	addr += res;
	// sensor names
	char buffer[100];
	res = EEPROM_readAnything(addr, buffer);
	addr += res;
	sensorNames = String(buffer);
	logger.print(tag, "\n\tsensorNames=" + sensorNames);
	/*char buffer[100];
	for (int i = 0; i < shield.sensorList.count; i++) {
	DS18S20Sensor* sensor = (DS18S20Sensor*)shield.sensorList.get(i);
	res = EEPROM_writeAnything(addr, buffer);
	sensor->sensorname = String(buffer);
	addr += res;
	}*/
}
void initEPROM()
{
	logger.println(tag, "initEPROM");

	EEPROM.begin(512);

	byte id = EEPROM.read(ID_ADDR); // read the first byte from the EEPROM
	if (id == EEPROM_ID)
	{
		readEPROM();
	}
	else
	{
		writeEPROM();
	}
}
String getJsonStatus()
{
	logger.print(tag, F(">>\n\t getJsonStatus"));

	String data;
	data += "";
	data += shield.hearterActuator.getJSON();

	logger.print(tag, "\n\t<< getJsonStatus data=" + data);
	return data;
}
String getJsonSensorsStatus(AsyncWebServerRequest *request)
{
	logger.println(tag, F(">>\n\t getJsonSensorsStatus"));

	String data;
	data += "";
	data += shield.getSensorsStatusJson();

	logger.println(tag, "\n\t<< getJsonSensorsStatus data=" + data);
	return data;
}
String getJsonActuatorsStatus(AsyncWebServerRequest *request)
{
	logger.println(tag, F("\n\t>> getJsonActuatorsStatus"));

	String data;
	data += "";
	String json = shield.getActuatorsStatusJson();
	data += json;

	logger.println(tag, "\n\t<< getJsonActuatorsStatus data=" + data);
	return data;
}
String getJsonHeaterStatus(AsyncWebServerRequest *request)
{
	logger.println(tag, F("\n\t>> getJsonHeaterStatus"));

	String data;
	data += "";

	data += shield.getHeaterStatusJson();
	logger.println(tag, "\n\t<< getJsonHeaterStatus data=" + data);
	return data;
}
String getJsonSettings(AsyncWebServerRequest *request) {
	logger.println(tag, F("\n\t>> getJsonSettings"));

	String data;
	data += "";

	data += shield.getSettingsJson();
	logger.println(tag, "\n\t<< getJsonSettings data=" + data);
	return data;
}
/*String registerShield() {
	logger.println(tag, F("register shield "));

	String data;
	data += "";
	data += F("HTTP/1.1 200 OK\r\nContent-Type: text/html");
	data += F("\n\n<html><head><meta HTTP-EQUIV='REFRESH' content='0; url=/main?msg=2'><title>rele</title></head><body>");
	data += F("REGISTER SHIELD - Attendere prego");
	data += F("</body></html>");

	Command command;
	command.registerShield(shield);

	return data;
}*/
String receiveCommand(String jsonString) {
	logger.println(tag, "\n\t >>receiveCommand "+jsonString);
	String jsonResult = shield.sendCommand(jsonString);
	logger.println(tag, "<<receiveCommand "+jsonResult);
	return jsonResult;
}
String showRele(AsyncWebServerRequest *request) {

	logger.println(tag, F("\n\t >>called showRele "));

	logger.print(tag, "\n\t status=" + request->arg("status"));
	if (!request->arg("status").equals("")) {
		String str = request->arg("temperature");
		shield.hearterActuator.setRemoteTemperature(str.toFloat());
		logger.print(tag, "\n\t networkSSID=" + String(shield.hearterActuator.getRemoteTemperature()));
	}
	else {
		logger.print(tag, "\n\t invalid temperatuire");
	}
	// status
	int status;
	logger.print(tag, "\n\t status=" + request->arg("status"));
	if (!request->arg("status").equals("")) {
		String str = request->arg("status");
		status = str.toInt();
		logger.print(tag, "\n\t status=" + status);
	}
	String command = "";
	switch (status) {
	case 0:
		command = "programoff";
		break;
	case 1:
		command = "programon";
		break;
	case 2:
		command = "disabled";
		break;
	case 3:
		command = "enabled";
		break;
	case 4:
		command = "manualoff";
		break;
	case 5:
		command = "manual";
		break;
	case 6:
		command = "manualend";
		break;
	}
	logger.print(tag, "\n\t command=" + command);
	// duration
	long duration = 0;
	if (!request->arg("duration").equals("")) {
		duration = request->arg("duration").toInt();
	}
	logger.print(tag, "\n\t duration=" + String(duration) + " minuti");
	duration = duration * 60 * 1000;
	logger.print(tag, "\n\tduration =" + String(duration) + " millisecondi");
	// sensorid
	int sensorId = 0;
	if (!request->arg("sensorid").equals("")) {
		sensorId = request->arg("sensorid").toInt();
	}
	logger.print(tag, "\n\tsensorId=" + sensorId);
	// target
	float target = 0;
	if (!request->arg("target").equals("")) {
		target = request->arg("target").toFloat();
	}
	logger.print(tag, "\n\ttarget =" + String(target));
	shield.hearterActuator.setTargetTemperature(target);
	// remote temperature
	float remoteTemperature = -1;
	if (!request->arg("temperature").equals("")) {
		remoteTemperature = request->arg("temperature").toFloat();
		logger.print(tag, "\n\tremoteTemperature =" + String(remoteTemperature));
		shield.hearterActuator.setRemoteTemperature(remoteTemperature);
	}
	else {
		logger.print(tag, F("\n\t remoteTemperature MISSING"));
	}
	// program
	int program = 0;
	if (!request->arg("program").equals("")) {
		program = request->arg("program").toInt();
	}
	logger.print(tag, "\nt \program=" + program);
	// timerange
	int timerange = 0;
	if (!request->arg("timerange").equals("")) {
		timerange = request->arg("timerange").toInt();
	}
	logger.print(tag, "\n\t timerange=" + timerange);
	// local sensor
	int localsensor = 0;
	if (!request->arg("localsensor").equals("")) {
		localsensor = request->arg("localsensor").toInt();
	}
	logger.print(tag, "\n\t localsensor=" + localsensor);
	
	// jsonRequest
	// DA CAMBIARE. Controllarer in base all'header
	/*int json = parsePostdata(databuff, "json", posdata);
	logger.print(tag, F("\n\tjson="));
	logger.print(tag, json);*/

	shield.hearterActuator.changeProgram(command, duration,
		!localsensor,
		remoteTemperature,
		sensorId,
		target, program, timerange);



	//lastFlash = millis() - flash_interval;

	//////////////////
	/*if (json == 1) {
		String data = getJsonStatus();
		client.println(data);
	}
	else {*/
		String data;
		data += "";
		data += F("<html><head><meta HTTP-EQUIV='REFRESH' content='0; url=/main?msg=2'><title>rele</title></head><body>");

		char   buffer[50];
		sprintf(buffer, "relestatus=%d,status=%s", shield.hearterActuator.getReleStatus(), statusStr[shield.hearterActuator.getStatus()]);
		data += String(buffer);

		data += F("</body></html>");
		client.println(data);
	//}

		
	logger.print(tag, F("\n\t<<end show rele\n"));
	return data;
}
void showPage(String data) {

	logger.println(tag, "showPage ");

	client.println(data);
	delay(1000);
	//client.stop();
}
String showIODevices(AsyncWebServerRequest *request)
{
	logger.println(tag, F("showIODevices "));

	String data;
	data += "";
	data += F("<!DOCTYPE html><html><head><title>Webduino</title><body>\r\n");
	data += F("<font color='#53669E' face='Verdana' size='2'><b>IO devices </b></font>\r\n<form action='/chiodevices' method='POST'><table width='80%' border='1'><colgroup bgcolor='#B6C4E9' width='20%' align='left'></colgroup><colgroup bgcolor='#FFFFFF' width='30%' align='left'></colgroup>");

	for (int i = 0; i < Shield::getMaxIoDevices(); i++) {
		// local port
		data += F("<tr><td>device ");
		data += String(i);
		data += F("</td><td>");

		data += F("<select name='iodevice");
		data += String(i);
		data += F("' >");

		for (int k = 0; k < Shield::getMaxIoDeviceTypes(); k++) {

			data += F("<option value='");
			data += String(k);
			if (Shield::getIODevice(i) == k)
				data += F(" selected ");
			data += F("' >");

			data += Shield::getIoDevicesTypeName(k);
			data += F("</option>");

		}
		data += F("</select>");
		data += F("</td></tr>");
	}
	data += F("</table><input type='submit' value='save'/></form>");
	data += F("</body></html>");
	return data;
}
String showSettings(AsyncWebServerRequest *request)
{
	logger.println(tag, F("showSettings "));

	String data;
	data += "";
	data += F("<!DOCTYPE html><html><head><title>Webduino</title><body>\r\n");

	data += F("<font color='#53669E' face='Verdana' size='2'><b>Impostazioni </b></font>\r\n<form action='/chstt' method='POST'><table width='80%' border='1'><colgroup bgcolor='#B6C4E9' width='20%' align='left'></colgroup><colgroup bgcolor='#FFFFFF' width='30%' align='left'></colgroup>");
	// local port
	data += F("<tr><td>Local port</td><td><input type='num' name='localport");
	data += F("' value='");
	data += Shield::getLocalPort();
	data += F("' size='4' maxlength='4'> </td></tr>");
	// board name
	data += F("<tr><td>Board name</td><td><input type='num' name='shieldname' value='");
	data += String(Shield::getShieldName());
	data += F("' size='");
	data += String(shield.shieldNameLen - 1);
	data += F("' maxlength='");
	data += String(shield.shieldNameLen - 1);
	data += F("'> </td></tr>");
	// ssid
	data += F("<tr><td>SSID</td><td><input type='num' name='ssid");
	data += F("' value='");
	data += String(Shield::getNetworkSSID());
	data += F("' size='32' maxlength='32'> </td></tr>");
	// password
	data += F("<tr><td>password</td><td><input type='num' name='password");
	data += F("' value='");
	data += String(Shield::getNetworkPassword());
	data += F("' size='96' maxlength='96'> </td></tr>");
	// server name
	data += F("<tr><td>Server name</td><td><input type='num' name='servername' value='");
	data += String(Shield::getServerName());
	data += F("' size='");
	data += (Shield::serverNameLen - 1);
	data += F("' maxlength='");
	data += (Shield::serverNameLen - 1);
	data += F("'> </td></tr>");
	// server port
	data += F("<tr><td>Server port</td><td><input type='num' name='serverport");
	data += F("' value='");
	data += String(Shield::getServerPort());
	data += F("' size='4' maxlength='4'> </td></tr>");

	data += F("</table><input type='submit' value='save'/></form>");


	data += F("</body></html>");

	return data;

}
String showMain(AsyncWebServerRequest *request)
{
	logger.println(tag, F("showMain "));

	char buffer[200];

	String data;
	data += "";
	//data += F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title>Webduino</title><body>\r\n");
	data += F("<!DOCTYPE html><html><head><title>Webduino</title><body>\r\n");
	// comandi
	data += F("\n<font color='#53669E' face='Verdana' size='2'><b>Webduino - ");
	data += logger.getStrDate();
	data += F("</b></font>");
	data += F("\r\n<table width='80%' border='1'><colgroup bgcolor='#B6C4E9' width='20%' align='left'></colgroup><colgroup bgcolor='#FFFFFF' width='30%' align='left'></colgroup>");
	// power
	data += F("<tr><td>Power ");
	if (shield.hearterActuator.getStatus() == Program::STATUS_DISABLED) {
		data += F("OFF</td><td><form action='/power' method='POST'>");
		data += F("<input type='hidden' name='status' value='1' >");
		data += F("<input type='submit' value='on'></form>");
	}
	else {
		data += F("ON</td><td><form action='/power' method='POST'>");
		data += F("<input type='hidden' name='status' value='0' >");
		data += F("<input type='submit' value='off'></form>");
	}
	//client.print(F("<input type='submit' value='set'></form>"));
	data += F("</td></tr>");
	// status & rele	
	data += "<tr><td>Actuator:</td><td>";
	data += statusStr[shield.hearterActuator.getStatus()];
	data += " - Rele: ";
	if (shield.hearterActuator.getReleStatus()) {
		data += F("on - ");
	}
	else {
		data += F("off - ");
	}

	if (shield.hearterActuator.getStatus() == Program::STATUS_PROGRAMACTIVE ||
		shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_AUTO ||
		shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_OFF) {

		time_t onStatusDuration = (millis() - shield.hearterActuator.programStartTime) / 1000;
		time_t duration = (shield.hearterActuator.programDuration) / 1000;
		time_t remainingTime = (duration - onStatusDuration);
		time_t hr = remainingTime / 3600L;
		time_t mn = (remainingTime - hr * 3600L) / 60L;
		time_t sec = (remainingTime - hr * 3600L) % 60L;

		data += F(" onStatusDuration:");
		data += String(onStatusDuration);
		data += F(" ");
		data += String(onStatusDuration / 60);
		data += F(" duration:");
		data += String(duration);
		data += F(" ");
		data += String(duration / 60L);
		data += F(" remainingTime:");
		data += String(remainingTime);
		data += F(" ");
		data += String(remainingTime / 60L);
				
	}
	data += F("</td></tr>");
	// program
	data += F("<tr><td>program </td><td>");
	if (shield.hearterActuator.getStatus() == Program::STATUS_PROGRAMACTIVE) {
		data += F("program ");
		data += shield.hearterActuator.getActiveProgram();
		data += F("timerange ");
		data += shield.hearterActuator.getActiveTimeRange();

		data += " Target: " + String(shield.hearterActuator.getTargetTemperature()) + "°C";
		data += F(" Sensor: ");
		if (shield.hearterActuator.sensorIsRemote()) {
			data += " Remote (" + String(shield.hearterActuator.getRemoteSensorId()) + ")";
			data += String(shield.hearterActuator.getRemoteTemperature()) + "°C";
		}
		else {
			data += " Local (" + String(shield.hearterActuator.getLocalSensorId()) + ")";
			data += String(shield.hearterActuator.getLocalTemperature()) + "°C";
		}
	}
	else if (shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_AUTO) {

		data += F("program manual auto");

	}
	else if (shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_OFF) {

		data += F("program manual  off");
	}
	data += F("</td></tr>");
	// Manual
	data += F("<tr><td>Manual-- ");
	if (shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_AUTO || shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_OFF) {

		data += F("<tr><td>Manual ON</td><td>");

		if (shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_AUTO) {
			sprintf(buffer, "Target: %d.%02d<BR>", (int)shield.hearterActuator.getTargetTemperature(), (int)(shield.hearterActuator.getTargetTemperature() * 100.0) % 100);
			data += String(buffer);

			if (!shield.hearterActuator.sensorIsRemote()) {
				data += F("Sensor: Local");
			}
			else {
				data += "Sensor: Remote (" + String(shield.hearterActuator.getRemoteSensorId()) + ")";
			}
		}
		data += F("<form action='/rele' method='POST'>");
		//data += F("<input type='hidden' name='manual' value='3'>"); 
		data += F("<input type='hidden' name='status' value='6'>"); // 6 = manual end  
		data += F("<input type='submit' value='stop manual'></form>");
	}
	else if (shield.hearterActuator.getStatus() == Program::STATUS_PROGRAMACTIVE || shield.hearterActuator.getStatus() == Program::STATUS_IDLE) {
		data += F("OFF</td><td><form action='/rele' method='POST'>");
		data += F("Minutes:<input type='num' name='duration' value='");
		data += 30;
		data += F("' size='5' maxlength='5'><BR>");
		data += F("<input type='hidden' name='status' value='5'>"); // 5 = manual auto
		data += F("Target:<input type='number' name='target' value='22.0' step='0.10' ><BR>");
		data += F("Sensor:<input type='radio' name='sensor' value='0' checked>Local<input type='radio' name='sensor' value='1'>Remote<BR>");
		data += F("<input type='submit' value='start manual'></form>");

		// manual off
		data += F("<form action='/rele' method='POST'>");
		data += F("Minutes:<input type='num' name='duration' value='");
		data += 30;
		data += F("' size='5' maxlength='5'><BR>");
		data += F("<input type='hidden' name='status' value='4'>"); // 4 = manual off 
		data += F("<input type='submit' value='start manual off'></form>");
	}
	/*else {
	data += F("</td><td>--");
	}*/
	data += F("</td></tr>");
	// temperature sensor
	int count = 0;
	DS18S20Sensor* pSensor = (DS18S20Sensor*)shield.sensorList.getFirst();
	while (pSensor != NULL) {

		logger.print(tag, "\n\tpSensor->sensorname=" + pSensor->sensorname);

		count++;
		data += "<tr><td>Sensor " + String(count) + " [" + pSensor->getSensorAddress() + "]</td>"
			+ "<td>" + String(pSensor->getTemperature()) + "°C (Av."
			+ String(pSensor->getAvTemperature()) + "°C)"
			+ "<br><form action='/chstt' method='POST'>"
			+ "<input type = 'num' name = 'sensor" + String(count) + "' value='" + pSensor->sensorname + "' "
			+ "size='32' maxlength='32'>"
			+ "<input type='submit' value='save'/>"
			+ "</form></td></tr>";
		pSensor = (DS18S20Sensor*)shield.sensorList.getNext();
	}
	// targetTemperature
	/*data += "<tr><td>Target Temperature: </td><td>" + String(targetTemperature)  + "°C</td></tr>";
	// remote Sensor
	data += "<tr><td>Remote Sensor: </td><td>" + String(temperatureSensor) + "(" +
	String(remoteTemperature)+ "°C)</td></tr>";
	// remote temperature
	//sprintf(buffer, "<tr><td>Remote Temperature: </td><td>%d.%02d</td></tr>", (int)remoteTemperature, (int)(remoteTemperature * 100.0) % 100);
	//data += String(buffer);
	// program update
	sprintf(buffer, "<tr><td>Last program or temperature update: </td><td>%d msec</td></tr>", (millis() - last_RemoteSensor));
	data += String(buffer);*/
	// wol
	//data += F("<tr><td>WOL</td><td><form action='/wol' method='POST'><input type='submit' value='send'></form></td></tr>");

	// sofware reset
	data += F("<tr><td>Software reset</td><td><form action='/reset' method='POST'><input type='submit' value='reset'></form></td></tr>");
	// MACAddress
	data += "<tr><td>MACAddress: </td><td>";
	data += String(shield.MAC_char);
	data += "</td></tr>";
	// localip
	String ip = WiFi.localIP().toString();
	char ip1[20], ip2[20];
	ip.toCharArray(ip1, sizeof(ip1));
	shield.localIP.toCharArray(ip2, sizeof(ip2));
	data += "<tr><td>Local IP: </td><td>" + WiFi.localIP().toString() +
		"(" + shield.localIP + ")</td></tr>";
	// shield id
	//data += "<tr><td>ID</td><td>" + String(shield.id) + "</td></tr>";
	data += "<tr><td>Shield Id</td><td>" + String(shield.id) + "<form action='/register' method='POST'><input type='submit' value='register'></form></td></tr>";
	data += F("</table>");
	data += String(Versione);
	data += "\nEPROM_Table_Schema_Version=" + String(EPROM_Table_Schema_Version);
	data += F("</body></html>");
	return data;
}
String showHeater(AsyncWebServerRequest *request)
{
	logger.println(tag, F("showHeater "));

	char buffer[200];

	String data;
	data += "";
	data += F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title>Webduino</title><body>\r\n");
	data += F("\n<font color='#53669E' face='Verdana' size='2'><b>Webduino - ");
	data += logger.getStrDate();
	data += F("</b></font>");
	data += F("\r\n<table width='80%' border='1'><colgroup bgcolor='#B6C4E9' width='20%' align='left'></colgroup><colgroup bgcolor='#FFFFFF' width='30%' align='left'></colgroup>");
	// status & rele	
	data += "<tr><td>Actuator:</td><td>";
	data += statusStr[shield.hearterActuator.getStatus()];
	data += " - Rele: ";
	if (shield.hearterActuator.getReleStatus()) {
		data += F("on - ");
	}
	else {
		data += F("off - ");
	}

	if (shield.hearterActuator.getStatus() == Program::STATUS_PROGRAMACTIVE ||
		shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_AUTO ||
		shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_OFF) {

		time_t onStatusDuration = (millis() - shield.hearterActuator.programStartTime) / 1000;
		time_t duration = (shield.hearterActuator.programDuration) / 1000;
		time_t remainingTime = (duration - onStatusDuration);

		time_t hr = remainingTime / 3600L;
		time_t mn = (remainingTime - hr * 3600L) / 60L;
		time_t sec = (remainingTime - hr * 3600L) % 60L;

		data += F(" onStatusDuration:");
		data += String(onStatusDuration);
		data += F(" ");
		data += String(onStatusDuration / 60);
		data += F(" duration:");
		data += String(duration);
		data += F(" ");
		data += String(duration / 60L);
		data += F(" remainingTime:");
		data += String(remainingTime);
		data += F(" ");
		data += String(remainingTime / 60L);
	}
	data += F("</td></tr>");

	// program
	data += F("<tr><td>program </td><td>");
	if (shield.hearterActuator.getStatus() == Program::STATUS_PROGRAMACTIVE) {
		data += F("program ");
		data += shield.hearterActuator.getActiveProgram();
		data += F("timerange ");
		data += shield.hearterActuator.getActiveTimeRange();

		data += " Target: " + String(shield.hearterActuator.getTargetTemperature()) + "°C";
		data += F(" Sensor: ");
		if (shield.hearterActuator.sensorIsRemote()) {
			data += " Remote (" + String(shield.hearterActuator.getRemoteSensorId()) + ")";
			data += String(shield.hearterActuator.getRemoteTemperature()) + "°C";
		}
		else {
			data += " Local (" + String(shield.hearterActuator.getLocalSensorId()) + ")";
			data += String(shield.hearterActuator.getLocalTemperature()) + "°C";
		}
	}
	else if (shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_AUTO) {

		data += F("program manual auto");
	}
	else if (shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_OFF) {

		data += F("program manual  off");
	}
	data += F("</td></tr>");
	// Manual
	data += F("<tr><td>Manual-- ");
	if (shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_AUTO || shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_OFF) {

		data += F("<tr><td>Manual ON</td><td>");

		if (shield.hearterActuator.getStatus() == Program::STATUS_MANUAL_AUTO) {
			sprintf(buffer, "Target: %d.%02d<BR>", (int)shield.hearterActuator.getTargetTemperature(), (int)(shield.hearterActuator.getTargetTemperature() * 100.0) % 100);
			data += String(buffer);

			if (!shield.hearterActuator.sensorIsRemote()) {
				data += F("Sensor: Local");
			}
			else {
				data += "Sensor: Remote (" + String(shield.hearterActuator.getRemoteSensorId()) + ")";
			}
		}
		data += F("<form action='/rele' method='POST' id='manualForm'>");
		//data += F("<input type='hidden' name='manual' value='3'>"); 
		data += F("<input type='hidden' name='status' value='6'>"); // 6 = manual end  
		data += F("<input type='submit' value='stop manual'></form>");
	}
	else if (shield.hearterActuator.getStatus() == Program::STATUS_PROGRAMACTIVE || shield.hearterActuator.getStatus() == Program::STATUS_IDLE) {
		data += F("OFF</td><td><form action='/rele' method='POST'  id='manualForm'>");
		data += F("Minutes:<input type='num' name='duration' value='");
		data += 30;
		data += F("' size='5' maxlength='5'><BR>");
		data += F("<input type='hidden' name='status' value='5'>"); // 5 = manual auto
		data += F("Target:<input type='number' name='target' value='22.0' step='0.10' ><BR>");
		data += F("Sensor:<input type='radio' name='sensor' value='0' checked>Local<input type='radio' name='sensor' value='1'>Remote<BR>");
		data += F("<input type='submit' value='start manual'></form>");

		// manual off
		data += F("<form action='/rele' method='POST' id='manualForm'>");
		data += F("Minutes:<input type='num' name='duration' value='");
		data += 30;
		data += F("' size='5' maxlength='5'><BR>");
		data += F("<input type='hidden' name='status' value='4'>"); // 4 = manual off 
		data += F("<input type='submit' value='start manual off'></form>");
	}
	data += F("</td></tr>");

	data += F("</table>");
	data += String(Versione);
	data += "\nEPROM_Table_Schema_Version=" + String(EPROM_Table_Schema_Version);

	data += "<script>"
		"var form = document.getElementById('manualForm');"
		"form.onsubmit = function(e) {"
		"e.preventDefault();"
		"var data = {};"
		"for (var i = 0, ii = form.length; i < ii; ++i) {"
		"var input = form[i];"
		"if (input.name) {"
		"data[input.name] = input.value;"
		"}"
		"}"

		"alert('The form was submitted'+JSON.stringify(data));"

		"var xhr = new XMLHttpRequest();"
		"xhr.open(form.method, form.action, true);"
		"xhr.setRequestHeader('Content-Type', 'application/json; charset=UTF-8');"
		"xhr.send(JSON.stringify(data));"
		"xhr.onloadend = function() {"
		"};"
		"};"
		""

		"function sendCommand() {"
		"alert('The form was submitted');"
		"}"
		"</script>";

	data += F("</body></html>");

	return data;
}
String showChangeIODevices(AsyncWebServerRequest *request) {
	
	logger.print(tag, F("\n\t >>showChangeIODevices "));

	for (int i = 0; i < Shield::getMaxIoDevices(); i++) {

		char buffer[20];
		String str = "iodevice" + String(i + 1);
		
		logger.print(tag, "\n\t iodevice=" + str);
		if (request->arg(str).equals("") == false) {
			logger.print(tag, "\n\t iodevice=" + request->arg(str));
			Shield::setIODevice(i, request->arg(str).toInt());
		}
	}
	String data;
	data += "";
	data += F("<html><head><meta HTTP-EQUIV='REFRESH' content='0; url=/main'><title>Timer</title></head><body></body></html>");
	data += F("</body></html>");
	
	writeEPROM();
	logger.print(tag, F("\n\t <<showChangeIODevices "));
	return data;
}
String showChangeSettings(AsyncWebServerRequest *request) {

	logger.println(tag, "showChangeSettings:");

	String data;
	data += "";

	// localport
	logger.print(tag, "\n\t localPort=" + request->arg("localport"));
	if (request->arg("localport").equals("") == false) {
		Shield::setLocalPort(request->arg("localport").toInt());
		logger.print(tag, "\n\t localPort=" + request->arg("localport"));
	}
	// ssid
	logger.print(tag, "\n\t ssid=" + request->arg("ssid"));
	if (!request->arg("ssid").equals("")) {
		Shield::setNetworkSSID(request->arg("ssid"));
		logger.print(tag, "\n\t networkSSID=" + Shield::getNetworkSSID());
	}
	// password
	if (!request->arg("password").equals("")) {
		Shield::setNetworkPassword(request->arg("password"));
		logger.print(tag, "\n\t networkPassword=" + Shield::getNetworkPassword());
	}
	// server name
	if (!request->arg("servername").equals("")) {
		Shield::setServerName(request->arg("servername"));
		logger.print(tag, "\n\t servername=" + Shield::getServerName());
	}
	// server port
	logger.print(tag, "\n\t serverPort=" + request->arg("serverport"));
	if (!request->arg("serverport").equals("")) {
		Shield::setServerPort(request->arg("serverport").toInt());
		logger.print(tag, "\n\t serverport=" + Shield::getServerPort());
	}
	// board name
	if (!request->arg("shieldname").equals("")) {
		Shield::setShieldName(request->arg("shieldname"));
		logger.print(tag, "\n\t shieldname=" + Shield::getShieldName());
	}
	// sensor names
	for (int i = 0; i < shield.sensorList.count; i++) {
		DS18S20Sensor*  sensor = (DS18S20Sensor*)shield.sensorList.get(i);

		String str = "sensor" + String(i + 1);
		str = request->arg(str);
		if (!str.equals("")) {
			sensor->sensorname = str;
			logger.print(tag, "\n\t " + str + "=" + sensor->sensorname);
		}
	}

	data += F("<html><head><meta HTTP-EQUIV='REFRESH' content='0; url=/main'><title>Timer</title></head><body></body></html>");
	data += F("</body></html>");

	writeEPROM();
	return data;
}
String setRemoteTemperature(AsyncWebServerRequest *request) {

	logger.print(tag, F("\n\t >> setRemoteTemperature: "));

	logger.print(tag, "\n\t temperature=" + request->arg("temperature"));
	if (!request->arg("temperature").equals("")) {
		String str = request->arg("temperature");
		shield.hearterActuator.setRemoteTemperature(str.toFloat());
		logger.print(tag, "\n\t networkSSID=" + String(shield.hearterActuator.getRemoteTemperature()));
	}
	else {
		logger.print(tag, "\n\t invalid temperatuire");
	}
	String data;
	data += getJsonStatus();

	logger.print(tag, F("\n\t << setRemoteTemperature: "));
	return data;
}


int testWifi(void) {
	int c = 0;
	logger.println(tag, "Waiting for Wifi to connect");
	while (c < 40) {
		if (WiFi.status() == WL_CONNECTED) {
			logger.println(tag, "WiFi connected");
			return(20);
		}
		delay(500);
		logger.println(tag, WiFi.status());
		c++;
	}
	logger.println(tag, "Connect timed out, opening AP");
	return(10);
}
void setupAP(void) {

	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);
	int n = WiFi.scanNetworks();
	logger.println(tag, "scan done");
	if (n == 0)
		logger.println(tag, "no networks found");
	else
	{
		logger.println(tag, n);
		logger.println(tag, " networks found");
		for (int i = 0; i < n; ++i)
		{
			// Print SSID and RSSI for each network found
			logger.print(tag, i + 1);
			logger.print(tag, ": ");
			logger.print(tag, WiFi.SSID(i));
			logger.print(tag, " (");
			logger.print(tag, WiFi.RSSI(i));
			logger.print(tag, ")");
			logger.println(tag, (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
			delay(10);
		}
	}
	logger.print(tag, "\n");
	String st = "<ul>";
	for (int i = 0; i < n; ++i)
	{
		// Print SSID and RSSI for each network found
		st += "<li>";
		st += i + 1;
		st += ": ";
		st += WiFi.SSID(i);
		st += " (";
		st += WiFi.RSSI(i);
		st += ")";
		st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
		st += "</li>";
	}
	st += "</ul>";
	delay(100);
	String ssidName = String(ssidAP);
	for (int i = 0; i < sizeof(shield.MAC_array); ++i) {
		if (i > 0)
			ssidName += ":";
		ssidName += shield.MAC_array[i];
	}
	char buffer[100];
	ssidName.toCharArray(buffer, 100);
	logger.print(tag, "\nssidAP buffer= ");
	logger.println(tag, buffer);
	WiFi.softAP(buffer, passwordAP);
}
void checkOTA()
{
	// Port defaults to 8266
	// ArduinoOTA.setPort(8266);

	// Hostname defaults to esp8266-[ChipID]
	// ArduinoOTA.setHostname("myesp8266");

	// No authentication by default
	// ArduinoOTA.setPassword((const char *)"123");

	ArduinoOTA.onStart([]() {
		Serial.println("Start");
	});
	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});
	ArduinoOTA.begin();
	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	//ArduinoOTA.handle();  uesta chiamata deve essere messa in loop()
}
void showwol(String param) {

	logger.println(tag, F("showwol "));

	/*wol* w = new wol();
	w->init();
	w->wakeup();
	delete w;*/

	String data;
	data += "";
	data += F("HTTP/1.1 200 OK\r\nContent-Type: text/html");
	data += F("\n\n<html><head><meta HTTP-EQUIV='REFRESH' content='0; url=/main?msg=2'><title>WOL</title></head><body>");
	data += F("</body></html>");
	client.println(data);
	client.stop();
}
String softwareReset(AsyncWebServerRequest *request) {

	logger.println(tag, F(">>software reset "));

	String data;
	data += "";
	data += F("<html><head><meta HTTP-EQUIV='REFRESH' content='0; url=/main?msg=2'><title>rele</title></head><body>");
	data += F("SOFTWARE RESET - Attendere prego");
	data += F("</body></html>");

	request->send(200, "text/html", data);

	ESP.restart();
	return "";
}

void handleGET(AsyncWebServerRequest *request) {
	logger.print(tag, "\n\t >>handleGET " + request->url());
	
	/*if (request->url().equals("/main")) {
		request->send(200, "text/html", showMain(request));
	} else if (request->url().equals("/heater")) {
		request->send(200, "text/html", showHeater(request));
	}*/
	request->send(404);
}

void handlePOST(AsyncWebServerRequest *request) {
	
	logger.print(tag, "\n\t >>handlePOST "+ request->url());

	logger.print(tag, "\n\t >>content Type: " + request->contentType());
	
	int args = request->args();
	for (int i = 0; i < args; i++) {
		logger.print(tag, "\n\t ARG[" + request->argName(i) + "]=" + request->arg(i));
	}

	/*if (request->url().equals("/chstt")) {
		request->send(200, "text/html", showChangeSettings(request));
	} else */if (request->url().equals("/command")) {

		String jsonString = bodyContent;
		request->send(200, "application/json", receiveCommand(jsonString));
	}

	request->send(404);
}


// SKETCH BEGIN
AsyncWebServer asyncWebServer(80);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");

void onWsEvent(AsyncWebSocket * asyncWebServer, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
	if (type == WS_EVT_CONNECT) {
		Serial.printf("ws[%s][%u] connect\n", asyncWebServer->url(), client->id());
		client->printf("Hello Client %u :)", client->id());
		client->ping();
	}
	else if (type == WS_EVT_DISCONNECT) {
		Serial.printf("ws[%s][%u] disconnect: %u\n", asyncWebServer->url(), client->id());
	}
	else if (type == WS_EVT_ERROR) {
		Serial.printf("ws[%s][%u] error(%u): %s\n", asyncWebServer->url(), client->id(), *((uint16_t*)arg), (char*)data);
	}
	else if (type == WS_EVT_PONG) {
		Serial.printf("ws[%s][%u] pong[%u]: %s\n", asyncWebServer->url(), client->id(), len, (len) ? (char*)data : "");
	}
	else if (type == WS_EVT_DATA) {
		AwsFrameInfo * info = (AwsFrameInfo*)arg;
		String msg = "";
		if (info->final && info->index == 0 && info->len == len) {
			//the whole message is in a single frame and we got all of it's data
			Serial.printf("ws[%s][%u] %s-message[%llu]: ", asyncWebServer->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);

			if (info->opcode == WS_TEXT) {
				for (size_t i = 0; i < info->len; i++) {
					msg += (char)data[i];
				}
			}
			else {
				char buff[3];
				for (size_t i = 0; i < info->len; i++) {
					sprintf(buff, "%02x ", (uint8_t)data[i]);
					msg += buff;
				}
			}
			Serial.printf("%s\n", msg.c_str());

			if (info->opcode == WS_TEXT)
				client->text("I got your text message");
			else
				client->binary("I got your binary message");
		}
		else {
			//message is comprised of multiple frames or the frame is split into multiple packets
			if (info->index == 0) {
				if (info->num == 0)
					Serial.printf("ws[%s][%u] %s-message start\n", asyncWebServer->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
				Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", asyncWebServer->url(), client->id(), info->num, info->len);
			}

			Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", asyncWebServer->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);

			if (info->opcode == WS_TEXT) {
				for (size_t i = 0; i < info->len; i++) {
					msg += (char)data[i];
				}
			}
			else {
				char buff[3];
				for (size_t i = 0; i < info->len; i++) {
					sprintf(buff, "%02x ", (uint8_t)data[i]);
					msg += buff;
				}
			}
			Serial.printf("%s\n", msg.c_str());

			if ((info->index + len) == info->len) {
				Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", asyncWebServer->url(), client->id(), info->num, info->len);
				if (info->final) {
					Serial.printf("ws[%s][%u] %s-message end\n", asyncWebServer->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
					if (info->message_opcode == WS_TEXT)
						client->text("I got your text message");
					else
						client->binary("I got your binary message");
				}
			}
		}
	}
}


void setup() {
	Serial.begin(115200);
	Serial.setDebugOutput(true);

	logger.print(tag, "\n\n\"*******************RESTARTING************************--");
	String str = "\n\nstarting.... Versione ";
	str += String(Versione);
	logger.print(tag, str);
	// get MAC Address
	logger.print(tag, "\n\tMAC Address=");
	WiFi.macAddress(shield.MAC_array);
	for (int i = 0; i < sizeof(shield.MAC_array); ++i) {
		if (i > 0) sprintf(shield.MAC_char, "%s:", shield.MAC_char);
		sprintf(shield.MAC_char, "%s%02x", shield.MAC_char, shield.MAC_array[i]);

	}
	logger.print(tag, shield.MAC_char);
	initEPROM();
	logger.print(tag, "\n\tSensorNames=" + sensorNames);
	// Connect to WiFi network
	logger.print(tag, "\n\nConnecting to " + Shield::getNetworkSSID() + " " + Shield::getNetworkPassword());


	////////////////////////////
	WiFi.hostname(hostName);
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAP(hostName);
	WiFi.begin(ssid, password);
	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.printf("STA: Failed!\n");
		WiFi.disconnect(false);
		delay(1000);
		WiFi.begin(ssid, password);
	}

	//Send OTA events to the browser
	ArduinoOTA.onStart([]() { events.send("Update Start", "ota"); });
	ArduinoOTA.onEnd([]() { events.send("Update End", "ota"); });
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		char p[32];
		sprintf(p, "Progress: %u%%\n", (progress / (total / 100)));
		events.send(p, "ota");
	});
	ArduinoOTA.onError([](ota_error_t error) {
		if (error == OTA_AUTH_ERROR) events.send("Auth Failed", "ota");
		else if (error == OTA_BEGIN_ERROR) events.send("Begin Failed", "ota");
		else if (error == OTA_CONNECT_ERROR) events.send("Connect Failed", "ota");
		else if (error == OTA_RECEIVE_ERROR) events.send("Recieve Failed", "ota");
		else if (error == OTA_END_ERROR) events.send("End Failed", "ota");
	});
	ArduinoOTA.setHostname(hostName);
	ArduinoOTA.begin();

	MDNS.addService("http", "tcp", 80);

	SPIFFS.begin();

	ws.onEvent(onWsEvent);
	asyncWebServer.addHandler(&ws);

	events.onConnect([](AsyncEventSourceClient *client) {
		client->send("hello!", NULL, millis(), 1000);
	});
	asyncWebServer.addHandler(&events);

	asyncWebServer.addHandler(new SPIFFSEditor(http_username, http_password));

	asyncWebServer.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", String(ESP.getFreeHeap()));
	});

	asyncWebServer.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(SPIFFS, "/favicon.ico");
	});
	
		
	asyncWebServer.on("/main", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/html", showMain(request));
	});

	asyncWebServer.on("/setting", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/html", showSettings(request));
	});

	asyncWebServer.on("/heater", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/html", showHeater(request));
	});

	/*asyncWebServer.on("/command", HTTP_POST, [](AsyncWebServerRequest *request) {	
		String jsonString = bodyContent;
		logger.print(tag, "\n\t bodyContent=" + bodyContent);
		request->send(200, "application/json", receiveCommand(jsonString));
	});*/

	asyncWebServer.on("/chstt", HTTP_POST, [](AsyncWebServerRequest *request) {
		request->send(200, "text/html", showChangeSettings(request));
	});

	asyncWebServer.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "application/json", getJsonSettings(request));
	});
	asyncWebServer.on("/heaterstatus", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "application/json", getJsonHeaterStatus(request));
	}); 
	
	asyncWebServer.on("/sensorstatus", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "application/json", getJsonSensorsStatus(request));
	}); 
	
	asyncWebServer.on("/actuatorsstatus", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "application/json", getJsonActuatorsStatus(request));
	});

	asyncWebServer.on("/iodevices", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/html", showIODevices(request));
	});

	asyncWebServer.on("/chiodevices", HTTP_POST, [](AsyncWebServerRequest *request) {
		request->send(200, "text/html", showChangeIODevices(request));
	});

	
	
	

#ifdef dopo
	
	
	else if (page.equalsIgnoreCase("rele")) {
		showRele(param);
	}
	else if (page.equalsIgnoreCase("power")) {
		showPower(param);
	}
	else if (page.equalsIgnoreCase("temp")) {
		setRemoteTemperature(param);
	}
	else if (page.equalsIgnoreCase("status")) {
		data = getJsonStatus();
		showPage(data);
	}


	else if (page.equalsIgnoreCase("wol")) {
		showwol(param);
	}
	
	

	else {

	}
#endif
	

	asyncWebServer.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");

	asyncWebServer.onNotFound([](AsyncWebServerRequest *request) {
		logger.print(tag,"\n\t NOT_FOUND: ");

		logger.print(tag,"\n\t http://" + request->host() + request->url());
		if (request->contentLength()) {
			logger.print(tag, "\n\t _CONTENT_TYPE: "+request->contentType());
			logger.print(tag, "\n\t _CONTENT_LENGTH: "+String(request->contentLength()));
		}

		int headers = request->headers();
		int i;
		for (i = 0; i < headers; i++) {
			AsyncWebHeader* h = request->getHeader(i);
			logger.print(tag, "\n\t _HEADER["+ h->name() +"]: %s"+h->value());
		}

		int params = request->params();
		for (i = 0; i < params; i++) {
			AsyncWebParameter* p = request->getParam(i);
			if (p->isFile()) {
				logger.print(tag, "\n\t _FILE["+ p->name() +"]: "+ p->value() +", size: " + p->size());
			}
			else if (p->isPost()) {
				logger.print(tag, "\n\t _POST["+ p->name() +"]: "+ p->value());
			}
			else {
				logger.print(tag, "\n\t _GET["+ p->name() +"]: "+ p->value());
			}
		}

		if (request->method() == HTTP_GET)
			logger.print(tag, "\n\t GET");
		else if (request->method() == HTTP_POST) {
						
			logger.print(tag, "\n\t POST");
			handlePOST(request);
		}
		else if (request->method() == HTTP_DELETE) {
			logger.print(tag, "\n\t DELETE");
			request->send(404);
		}
		else if (request->method() == HTTP_PUT) {
			logger.print(tag, "\n\t PUT");
			request->send(404);
		}
		else if (request->method() == HTTP_PATCH) {
			logger.print(tag, "\n\t PATCH");
			request->send(404);
		}
		else if (request->method() == HTTP_HEAD) {
			logger.print(tag, "\n\t HEAD");
			request->send(404);
		}
		else if (request->method() == HTTP_OPTIONS) {
			logger.print(tag, "\n\t OPTIONS");
			request->send(404);
		}
		else {
			logger.print(tag, "\n\t UNKNOWN");
			request->send(404);
		}		
		//request->send(404);
	});
	asyncWebServer.onFileUpload([](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
		if (!index)
			Serial.printf("UploadStart: %s\n", filename.c_str());
		Serial.printf("%s", (const char*)data);
		if (final)
			Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
	});
	asyncWebServer.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		
		logger.print(tag, "\n\t onRequestBody");
		
		if (!index)
			Serial.printf("BodyStart: %u\n", total);
		Serial.printf("%s", (const char*)data);
		if (index + len == total)
			Serial.printf("BodyEnd: %u\n", total);

		String str((char*)data); //String(data);
		bodyContent = str;
		logger.print(tag, "\n\t bodyContent=" + bodyContent);

	});
	asyncWebServer.begin();


	wdt_disable();
	wdt_enable(WDTO_8S);

	shield.hearterActuator.init(String(shield.MAC_char));
	shield.addOneWireSensors(sensorNames);
	shield.addActuators();
	Command command;
	shield.id = command.registerShield(shield);
	if (shield.id != -1) {
		shieldRegistered = true;
		logger.print(tag, "\n\n\tSHIELD REGISTERED " + String(shield.id) + "\n");
	}
	else {
		shieldRegistered = false;
		logger.print(tag, "\n\n\tSHIELD NOT REGISTERED\n");
	}

}

void loop() {
	ArduinoOTA.handle();

	wdt_enable(WDTO_8S);

	if (shield.id >= 0 && !WiFi.localIP().toString().equals(shield.localIP)) {

		Command command;
		logger.println(tag, "IP ADDRESS ERROR - re-register shield");
		command.registerShield(shield);
		shield.localIP = WiFi.localIP().toString();
		return;
	}

	shield.checkActuatorsStatus();
	shield.checkSensorsStatus();

	unsigned long currMillis = millis();
	unsigned long timeDiff = currMillis - lastFlash;
	if (timeDiff > flash_interval) {

		lastFlash = currMillis;
		if (shield.id <= 0) {
			Command command;
			logger.println(tag, F("ID NON VALIDO"));
			shield.id = command.registerShield(shield);
		}

		return;
	}

	if (currMillis - lastTimeSync > timeSync_interval) {
		Command command;
		lastTimeSync = currMillis;
		command.timeSync();
		return;
	}

	if (currMillis - lastSendLog > SendLog_interval) {
		lastSendLog = currMillis;
		//logger.send();
		return;
	}
}
