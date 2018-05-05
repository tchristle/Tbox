//ESP8266 
////////////////////////////////////////////////////////////////////////////////////////////////
void ATCMD()
{
  bool timeout = true;
  while(Serial1.available()>0) Serial1.read();
  //Serial1.println(rcvstr.substring(7,rcvstr.length()));
  Serial1.println(rcvstr.substring(2));
  delay(100);
  elapsedMillis waiting;     // "waiting" starts at zero
  while (waiting < 2500) {
    if (Serial1.available()) {
      timeout = false;
      MySerial.print(char(Serial1.read()));
      waiting = 2000;     // advance waiting
    }
  }
  if(timeout) MySerial.println("timed out");
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void espcrnl()
{
  MySerial.println();
  Serial1.println();
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void gps2wifi()
{
  float flat, flon;
  unsigned long age;
  
  MySerial.read();
  while(MySerial.available()<1)  //break loop on keystroke
  {
    latlon();
    gps.f_get_position(&flat, &flon, &age);
    // it would make more sense to create the string fisrt then send number of known bytes
    Serial1.println("AT+CIPSEND=0,23");
    delay(500);
    Serial1.print(flat, 6); Serial1.print(", "); Serial1.println(flon, 6);
    delay(500);
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void telem2wifi()
{
  float flat, flon;
  unsigned long age, date, time;
  String telemetry;
  int rssi;
  bool newdata = false;
  unsigned long start = millis();
  
  MySerial.read();
  while(MySerial.available()<1)  //break loop on keystroke
  {
    while (millis() - start < 2000) {
      if (Uart.available()) {
        char c = Uart.read();
        if (gps.encode(c)) {
          newdata = true;
          rssi = getRSSI();
          // break;  // uncomment to print new data immediately!
        }
      }
    }    
    if(newdata)
    {
      gps.f_get_position(&flat, &flon, &age);
      gps.get_datetime(&date, &time, &age);
      imu.readGyro(); imu.readAccel(); imu.readMag();
      telemetry = String(time,6) + " " + String(flat,6) + " " + String(flon,6) + " alt " + 
      String(imu.ax) + " " + String(imu.ay) + " " + String(imu.az) + " " + 
      String(imu.gx-gxbias) + " " + String(imu.gy-gybias) + " " + String(imu.gz-gzbias) + " " + 
      String(int((imu.mx - mxoffset)*mxscale)) + " " + String(int((imu.my - myoffset)*mxscale)) + " " + String(imu.mz) + " " + 
      String("temp") + String(" ") + String(rssi) + "\r\n";
      Serial1.print("AT+CIPSEND=0,"); Serial1.println(telemetry.length());
      delay(10);
      Serial1.print(telemetry);
    }
    newdata = false;
    start = millis();
  }
    
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cipserver()
{
  Serial1.println("AT+CIPMUX=1");
  delay(100);
  Serial1.println("AT+CIPSERVER=1,23");
  delay(100);
  MySerial.println();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void webserver()
{
  Serial1.println("AT+CIPMUX=1");
  delay(100);
  Serial1.println("AT+CIPSERVER=1,80");
  delay(100);
  MySerial.println();
  webserveron = true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int getRSSI()
{
  String msg = "";
  char ibyte;
  int RSSI;

  //clear ESP serial buffer
  while(Serial1.available()>0) Serial1.read();
  
  Serial1.println("AT+CWJAP?");
  elapsedMillis waiting;     // "waiting" starts at zero
  while (waiting < 10) {
    if (Serial1.available()) {
      ibyte = Serial1.read();
      msg += ibyte;
    }
  }
  
  RSSI = msg.substring(msg.length()-11,msg.length()-6).toInt();
  if(msg.substring(0,5) == "No AP") RSSI = -200;
    
  return RSSI;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void cipsend(String message)
{
  int lng = message.length();
  //delay(100);
  Serial1.print("AT+CIPSEND=0,"); Serial1.println(lng);
  delay(100);
  Serial1.print(message);
  delay(150);
  //Serial1.println();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void srvhtml()
{
  cipsend("HTTP/1.0 200 OK\r\n");
  cipsend("\r\n");
  cipsend("<HTML><BODY>");
  cipsend("<h1>Hello World!</h1>");
  cipsend("This is ESP8266. Lick my balls.");
  cipsend("<form id=\"contact-form\" action=\"script.php\" method=\"post\">");
  cipsend("<input type=\"text\" name=\"input-data\" id=\"input-data\" value=\"\" />");
  
  cipsend("</HTML></BODY>");
  Serial1.print("AT+CIPCLOSE="); Serial1.println(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void text2wifi()
{
  String textmsg;

  textmsg = rcvstr.substring(10);
  MySerial.println();
  //MySerial.println(rcvstr.length());
  //MySerial.println(textmsg);
  
  Serial1.print("AT+CIPSEND=0,"); Serial1.println(textmsg.length());
  delay(50);
  Serial1.print(textmsg);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void txt2wi(int con_num, String textmsg)
{
  Serial1.print("AT+CIPSEND="); Serial1.print(con_num); Serial1.print(","); Serial1.println(textmsg.length());
  delay(50);
  Serial1.println(textmsg);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void en_esp(){
  digitalWrite(4, HIGH);  //set RST high
  digitalWrite(2, HIGH);  //set CE high
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void fesp(){
  
  digitalWrite(3, LOW);  //set GPIO0
  delay(500);
  digitalWrite(4, LOW);  //set RST 
  delay(500);
  digitalWrite(4, HIGH);  //reset RST
  delay(500);
  digitalWrite(3, HIGH);  //boot serial
  delay(500);
  
  while(1){
    if (Serial.available() > 0) {
      Serial1.write(Serial.read());
    }
    if (Serial1.available() > 0) {
      //Serial3.print(Serial1.read());
      Serial.write(Serial1.read());
    }
  }
  
}

