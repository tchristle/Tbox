//ESP8266 
////////////////////////////////////////////////////////////////////////////////////////////////
void wi()
{
  bool timeout = true;
  MySerial.println();
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void srvhtml()
{
  //cipsend("HTTP/1.0 200 OK\r\n");
  //cipsend("\r\n");
  //cipsend("<HTML><BODY>");
  //cipsend("<h1>Hello World!</h1>");
  //cipsend("This is ESP8266. Lick my balls.");
  //cipsend("<form id=\"contact-form\" action=\"script.php\" method=\"post\">");
  //cipsend("<input type=\"text\" name=\"input-data\" id=\"input-data\" value=\"\" />");
  
  //cipsend("</HTML></BODY>");
  //Serial1.print("AT+CIPCLOSE="); Serial1.println(0);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void en_esp(){
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(4, HIGH);  //set RST high
  digitalWrite(2, HIGH);  //set CE high
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void rst_esp(){
  digitalWrite(4, LOW);  //set RST
  delay(100);
  digitalWrite(4, HIGH); 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void fesp(){

  en_esp();
  pinMode(2, INPUT_PULLUP);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("ESP8266 FLASH MODE");
  display.display();  
  
  digitalWrite(3, LOW);  //set GPIO0
  delay(500);
  digitalWrite(4, LOW);  //set RST 
  delay(500);
  digitalWrite(4, HIGH);  //reset RST
  delay(500);
  digitalWrite(3, HIGH);  //boot serial
  delay(500);
  
  while(digitalRead(2==HIGH)){
    if (Serial.available() > 0) {
      Serial1.write(Serial.read());
    }
    if (Serial1.available() > 0) {
      Serial.write(Serial1.read());
    }
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.display();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void b2e(){

  en_esp();
  pinMode(3, INPUT_PULLUP);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("ESP8266<->HC-05");
  display.display();  
      
  while(1){//digitalRead(3==HIGH)){
    if (Serial3.available() > 0) {
      Serial1.write(Serial3.read());
    }
    if (Serial1.available() > 0) {
      Serial3.write(Serial1.read());
    }
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.display();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void u2e(){
  
  en_esp();
  pinMode(2, INPUT_PULLUP);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("ESP8266<->USB_ComPort");
  display.display();  
      
  while(digitalRead(2==HIGH)){
    if (Serial.available() > 0) {
      Serial1.write(Serial.read());
    }
    if (Serial1.available() > 0) {
      Serial.write(Serial1.read());
    }
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.display();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void showESPio()
{
  pinMode(16, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  int GPIO0 = 0, GPIO2 = 0;
  
  while(MySerial.available()==0){
    while (Serial1.available() > 0) MySerial.write(Serial1.read());
    GPIO0 = digitalRead(16);
    GPIO2 = digitalRead(3);
    display.clearDisplay(); 
    display.setCursor(0,0);
    display.print("GPIO0 = "); display.print(GPIO0); display.print(", GPIO2 = "); display.print(GPIO2);
    display.display();       
  }
}

