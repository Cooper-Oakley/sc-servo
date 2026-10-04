// https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
#include <esp_now.h>
#include <WiFi.h>
#include <WebServer.h>
#include "WEBPAGE.h"


// Create AsyncWebServer object on port 80
WebServer server(80);


// select the ID of active servo.
void activeID(int cmdInput){
  activeNumInList += cmdInput;
  if(activeNumInList >= searchNum){
    activeNumInList = 0;
  }
  else if(activeNumInList < 0){
    activeNumInList = searchNum;
  }
}


void activeSpeed(int cmdInput){
  activeServoSpeed += cmdInput;
  if (activeServoSpeed > ServoMaxSpeed){
    activeServoSpeed = ServoMaxSpeed;
  }
  else if(activeServoSpeed < 0){
    activeServoSpeed = 0;
  }
}


int rangeCtrl(int rawInput, int minInput, int maxInput){
  if(rawInput > maxInput){
    return maxInput;
  }
  else if(rawInput < minInput){
    return minInput;
  }
  else{
    return rawInput;
  }
}

int servoPositionMin(byte servoID){
  if(servoID == 2){return 60;}
  if(servoID == 3){return 35;}
  if(servoID == 4){return 90;}
  if(servoID == 6){return 725;}
  return 0;
}

int servoPositionMax(byte servoID){
  if(servoID == 2 || servoID == 3){return 800;}
  if(servoID == 4){return 950;}
  if(servoID == 6){return 800;}
  return (int)ServoDigitalRange - 1;
}

int clampServoPosition(byte servoID, int position){
  return rangeCtrl(position, servoPositionMin(servoID), servoPositionMax(servoID));
}


void activeCtrl(int cmdInput){
  switch(cmdInput){
    case 1:{
      byte servoID = listID[activeNumInList];
      st.WritePosEx(servoID, clampServoPosition(servoID, ServoDigitalMiddle), activeServoSpeed, ServoInitACC);
      break;
    }
    case 2:
      if(modeRead[listID[activeNumInList]] == 0) {
        servoStop(listID[activeNumInList]);
      }
      else if(modeRead[listID[activeNumInList]] == 3){
        st.WritePos(listID[activeNumInList], 0, 0, 0);
      }
      break;
    case 3:servoTorque(listID[activeNumInList],0);Torque_List[activeNumInList] = false;break;
    case 4:servoTorque(listID[activeNumInList],1);Torque_List[activeNumInList] = true;break;
    case 5:
      if(modeRead[listID[activeNumInList]] == 0){
        if(SERVO_TYPE_SELECT == 1){
          byte servoID = listID[activeNumInList];
          st.WritePosEx(servoID, clampServoPosition(servoID, ServoDigitalRange - 1), activeServoSpeed, ServoInitACC);
        }
        else if(SERVO_TYPE_SELECT == 2){
          byte servoID = listID[activeNumInList];
          int position = ServoDigitalRange - MAX_MIN_OFFSET;
          st.WritePosEx(servoID, clampServoPosition(servoID, position), activeServoSpeed, ServoInitACC);
        }
      }


      else if(modeRead[listID[activeNumInList]] == 3){
        if(SERVO_TYPE_SELECT == 1){
          st.WritePosEx(listID[activeNumInList], 10000, activeServoSpeed, ServoInitACC);
        }
        else if(SERVO_TYPE_SELECT == 2){
          st.WritePos(listID[activeNumInList], 0, rangeCtrl(activeServoSpeed,200,999), 0);
        }
      }
      break;
    case 6:
      if(modeRead[listID[activeNumInList]] == 0){
        if(SERVO_TYPE_SELECT == 1){
          byte servoID = listID[activeNumInList];
          st.WritePosEx(servoID, clampServoPosition(servoID, 0), activeServoSpeed, ServoInitACC);
        }
        else if(SERVO_TYPE_SELECT == 2){
          byte servoID = listID[activeNumInList];
          st.WritePosEx(servoID, clampServoPosition(servoID, MAX_MIN_OFFSET), activeServoSpeed, ServoInitACC);
        }
      }


      else if(modeRead[listID[activeNumInList]] == 3){
        if(SERVO_TYPE_SELECT == 1){
          st.WritePosEx(listID[activeNumInList], -10000, activeServoSpeed, ServoInitACC);
        }
        else if(SERVO_TYPE_SELECT == 2){
          st.WritePos(listID[activeNumInList], 0, rangeCtrl(activeServoSpeed,200,999)+1024, 0);
        }
      }
      break;
    case 7:activeSpeed(100);break;
    case 8:activeSpeed(-100);break;
    case 9:servotoSet += 1;if(servotoSet > 250){servotoSet = 0;}break;
    case 10:servotoSet -= 1;if(servotoSet < 0){servotoSet = 0;}break;
    case 11:setMiddle(listID[activeNumInList]);break;
    case 12:
      setMode(listID[activeNumInList], 0);
      modeRead[listID[activeNumInList]] = 0;
      break;
    case 13:
      setMode(listID[activeNumInList], 3);
      modeRead[listID[activeNumInList]] = 3;
      break;
    case 14:SERIAL_FORWARDING = true;break;
    case 15:SERIAL_FORWARDING = false;break;
    case 16:setID(listID[activeNumInList], servotoSet);break;

    case 17:DEV_ROLE = 0;break;
    case 18:DEV_ROLE = 1;break;
    case 19:DEV_ROLE = 2;break;

    case 20:RAINBOW_STATUS = 1;break;
    case 21:RAINBOW_STATUS = 0;break;
  }
}


void handleRoot() {
 server.send(200, "text/html", index_html); //Send web page
}


void handleID() {
  if(!searchedStatus && searchFinished){
    String IDmessage = "ID:";
    for(int i = 0; i< searchNum; i++){
      IDmessage += String(listID[i]) + " ";
    }
    server.send(200, "text/plane", IDmessage);
  }
  else if(searchedStatus){
    String IDmessage = "Searching...";
    server.send(200, "text/plane", IDmessage);
  }
}


void handleSTS() {
  String stsValue = "Active ID:" + String(listID[activeNumInList]);
  if(voltageRead[listID[activeNumInList]] != -1){
    stsValue += "  Position:" + String(posRead[listID[activeNumInList]]);
    if(DEV_ROLE == 0){
      stsValue += "<p>Device Mode: Normal";
    }
    else if(DEV_ROLE == 1){
      stsValue += "<p>Device Mode: Leader";
    }
    else if(DEV_ROLE == 2){
      stsValue += "<p>Device Mode: Follower";
    }
    stsValue += "<p>Voltage:" + String(float(voltageRead[listID[activeNumInList]])/10);
    stsValue += "  Load:" + String(loadRead[listID[activeNumInList]]);
    stsValue += "<p>Speed:" + String(speedRead[listID[activeNumInList]]);

    stsValue += "  Temper:" + String(temperRead[listID[activeNumInList]]);
    stsValue += "<p>Speed Set:" + String(activeServoSpeed);
    stsValue += "<p>ID to Set:" + String(servotoSet);
    stsValue += "<p>Mode:";
    if(modeRead[listID[activeNumInList]] == 0){
      stsValue += "Servo Mode";
    }
    else if(modeRead[listID[activeNumInList]] == 3){
      stsValue += "Motor Mode";
    }

    if(Torque_List[activeNumInList]){
      stsValue += "<p>Torque On";
    }
    else{
      stsValue += "<p>Torque Off";
    }
  }
  else{
    stsValue += " FeedBack err";
  }
  server.send(200, "text/plane", stsValue); //Send ADC value only to client ajax request
}

bool parseUnsignedArgument(const String &argument, long maxValue, long &value) {
  if (argument.length() == 0) {
    return false;
  }

  value = 0;
  for (unsigned int i = 0; i < argument.length(); i++) {
    char digit = argument.charAt(i);
    if (digit < '0' || digit > '9') {
      return false;
    }
    int numericDigit = digit - '0';
    if (value > maxValue / 10 ||
        (value == maxValue / 10 && numericDigit > maxValue % 10)) {
      return false;
    }
    value = value * 10 + numericDigit;
  }
  return value <= maxValue;
}

void handleReadSpeed() {
  String response = "{\"speed\":";
  response += String(activeServoSpeed);
  response += ",\"maxSpeed\":";
  response += String(ServoMaxSpeed);
  response += "}";
  server.send(200, "application/json", response);
}

void handleSetSpeed() {
  long speed;
  if (!server.hasArg("value") ||
      !parseUnsignedArgument(server.arg("value"), ServoMaxSpeed, speed)) {
    server.send(400, "text/plain", "Speed must be between 0 and ServoMaxSpeed");
    return;
  }

  activeServoSpeed = (s16)speed;
  server.send(200, "text/plain", String(activeServoSpeed));
}

void handleReadPositionControls() {
  static const byte servoIDs[] = {1, 2, 3, 4, 5, 6};
  String response = "{\"maxPosition\":";
  response += String((int)ServoDigitalRange - 1);
  response += ",\"servos\":[";
  for (byte i = 0; i < sizeof(servoIDs) / sizeof(servoIDs[0]); i++) {
    byte servoID = servoIDs[i];
    getFeedBack(servoID);
    if (i > 0) {
      response += ",";
    }
    response += "{\"id\":";
    response += String(servoID);
    response += ",\"position\":";
    response += String(posRead[servoID]);
    response += ",\"minPosition\":";
    response += String(servoPositionMin(servoID));
    response += ",\"maxPosition\":";
    response += String(servoPositionMax(servoID));
    response += ",\"mode\":";
    response += String(modeRead[servoID]);
    response += ",\"ready\":";
    response += feedbackValid[servoID] ? "true" : "false";
    response += "}";
  }
  response += "],\"servo2Position\":";
  response += String(posRead[2]);
  response += ",\"servo2Ready\":";
  response += feedbackValid[2] ? "true" : "false";
  response += "}";
  server.send(200, "application/json", response);
}

void handleSetServoPosition() {
  long servoID;
  long position;
  if (!server.hasArg("id") ||
      !parseUnsignedArgument(server.arg("id"), 6, servoID) ||
      (servoID != 1 && servoID != 2 && servoID != 3 && servoID != 4 && servoID != 5 && servoID != 6) ||
      !server.hasArg("position") ||
      !parseUnsignedArgument(server.arg("position"), (int)ServoDigitalRange - 1, position)) {
    server.send(400, "text/plain", "Invalid servo ID or position");
    return;
  }
  if (position < servoPositionMin((byte)servoID) || position > servoPositionMax((byte)servoID)) {
    server.send(400, "text/plain", "Servo position is outside the allowed range");
    return;
  }

  byte targetID = (byte)servoID;
  getFeedBack(targetID);
  if (!feedbackValid[targetID]) {
    server.send(409, "text/plain", "Servo feedback is unavailable");
    return;
  }
  if (modeRead[targetID] != 0) {
    server.send(409, "text/plain", "Servo is not in servo mode");
    return;
  }
  if (targetID == 1) {
    getFeedBack(2);
    if (!feedbackValid[2]) {
      server.send(409, "text/plain", "Servo 2 feedback is unavailable");
      return;
    }
    if (posRead[2] <= 70) {
      server.send(409, "text/plain", "Servo 1 movement requires servo 2 position greater than 70");
      return;
    }
  }

  st.WritePosEx(targetID, (s16)position, activeServoSpeed, ServoInitACC);
  server.send(200, "text/plain", String(position));
}

void handleSetServos2And3Position() {
  long position;
  if (!server.hasArg("position") ||
      !parseUnsignedArgument(server.arg("position"), servoPositionMax(2), position) ||
      position < servoPositionMin(2) ||
      position < servoPositionMin(3) ||
      position > servoPositionMax(3)) {
    server.send(400, "text/plain", "Shared servo 2/3 position must be between 60 and 800");
    return;
  }

  getFeedBack(2);
  getFeedBack(3);
  if (!feedbackValid[2] || !feedbackValid[3]) {
    server.send(409, "text/plain", "Servo 2 or servo 3 feedback is unavailable");
    return;
  }
  if (modeRead[2] != 0 || modeRead[3] != 0) {
    server.send(409, "text/plain", "Servo 2 and servo 3 must both be in servo mode");
    return;
  }

  st.WritePosEx(2, (s16)position, activeServoSpeed, ServoInitACC);
  st.WritePosEx(3, (s16)position, activeServoSpeed, ServoInitACC);
  server.send(200, "text/plain", String(position));
}

void handlePresetPositions(const s16 positions[], byte positionCount) {
  static const byte servoIDs[] = {1, 2, 3, 4, 5, 6};
  String movedIDs;
  String skippedIDs;
  byte movedCount = 0;
  byte skippedCount = 0;

  for (byte i = 0; i < positionCount; i++) {
    byte servoID = servoIDs[i];
    if (positions[i] < servoPositionMin(servoID) || positions[i] > servoPositionMax(servoID)) {
      if (skippedCount > 0) {
        skippedIDs += ",";
      }
      skippedIDs += String(servoID) + ":out of range";
      skippedCount++;
      continue;
    }

    getFeedBack(servoID);
    if (!feedbackValid[servoID]) {
      if (skippedCount > 0) {
        skippedIDs += ",";
      }
      skippedIDs += String(servoID) + ":no feedback";
      skippedCount++;
      continue;
    }
    if (modeRead[servoID] != 0) {
      if (skippedCount > 0) {
        skippedIDs += ",";
      }
      skippedIDs += String(servoID) + ":not in servo mode";
      skippedCount++;
      continue;
    }

    st.WritePosEx(servoID, positions[i], activeServoSpeed, ServoInitACC);
    if (movedCount > 0) {
      movedIDs += ",";
    }
    movedIDs += String(servoID);
    movedCount++;
  }

  String response = "{\"moved\":[";
  response += movedIDs;
  response += "],\"skipped\":\"";
  response += skippedIDs;
  response += "\"}";
  server.send(movedCount > 0 ? 200 : 503, "application/json", response);
}

void handleWakeUp() {
  static const s16 positions[] = {500, 350, 250, 560, 500, 800};
  handlePresetPositions(positions, sizeof(positions) / sizeof(positions[0]));
}

void handleSleep() {
  static const s16 positions[] = {500, 60, 35, 400, 500, 730};
  handlePresetPositions(positions, sizeof(positions) / sizeof(positions[0]));
}


void webCtrlServer(){
    server.on("/", handleRoot);
    server.on("/readID", handleID);
    server.on("/readSTS", handleSTS);
    server.on("/readSpeed", handleReadSpeed);
    server.on("/setSpeed", HTTP_POST, handleSetSpeed);
    server.on("/readPositionControls", handleReadPositionControls);
    server.on("/setServoPosition", HTTP_POST, handleSetServoPosition);
    server.on("/setServos2And3Position", HTTP_POST, handleSetServos2And3Position);
    server.on("/wakeUp", HTTP_POST, handleWakeUp);
    server.on("/sleep", HTTP_POST, handleSleep);

    server.on("/cmd", [](){
    int cmdT = server.arg(0).toInt();
    int cmdI = server.arg(1).toInt();
    int cmdA = server.arg(2).toInt();
    int cmdB = server.arg(3).toInt();

    switch(cmdT){
      case 0:activeID(cmdI);break;
      case 1:activeCtrl(cmdI);break;
      case 9:searchCmd = true;break;
    }
    server.send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
  Serial.println("Server Starts.");
}


void webServerSetup(){
  webCtrlServer();
}


void getMAC(){
  WiFi.mode(WIFI_AP_STA);
  MAC_ADDRESS = WiFi.macAddress();
  Serial.print("MAC:");
  Serial.println(WiFi.macAddress());
}


void getIP(){
  IP_ADDRESS = WiFi.localIP();
}


void setAP(){
  WiFi.softAP(AP_SSID, AP_PWD);
  IPAddress myIP = WiFi.softAPIP();
  IP_ADDRESS = myIP;
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  WIFI_MODE = 1;
}


void setSTA(){
  WIFI_MODE = 3;
  WiFi.begin(STA_SSID, STA_PWD);
}


void getWifiStatus(){
  if(WiFi.status() == WL_CONNECTED){
    WIFI_MODE = 2;
    getIP();
    WIFI_RSSI = WiFi.RSSI();
  }
  else if(WiFi.status() == WL_CONNECTION_LOST && DEFAULT_WIFI_MODE == 2){
    WIFI_MODE = 3;
    // WiFi.disconnect();
    WiFi.reconnect();
  }
}


void wifiInit(){
  DEV_ROLE  = DEFAULT_ROLE;
  WIFI_MODE = DEFAULT_WIFI_MODE;
  if(WIFI_MODE == 1){setAP();}
  else if(WIFI_MODE == 2){setSTA();}
}


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  if(DEV_ROLE == 2){
    memcpy(&myData, incomingData, sizeof(myData));
    myData.Spd_send = abs(myData.Spd_send);
    if(myData.Spd_send < 50){
      myData.Spd_send = 200;
    }
    st.WritePosEx(myData.ID_send, myData.POS_send, abs(myData.Spd_send), 0);

    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("POS: ");
    Serial.println(myData.POS_send);
    Serial.print("SPEED: ");
    Serial.println(abs(myData.Spd_send));
  }
}


void espNowInit(){
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  esp_now_peer_info_t peerInfo={};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  MAC_ADDRESS = WiFi.macAddress();
  Serial.print("MAC:");
  Serial.println(WiFi.macAddress());
}