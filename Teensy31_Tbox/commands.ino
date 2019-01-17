void command_lookup()
{
  if      (rcvstr == "gps"      || ENC_POS == 3)  { GPS_update(); }
  else if (rcvstr == "test")                      { test(); }
  else if (rcvstr == "latlon")                    { latlon(); }
  else if (rcvstr == "getalt")                    { getalt(); }      
  else if (rcvstr == "imu"      || ENC_POS == 4)  { IMU(); }
  else if (rcvstr == "mad")                       { madgwick(); }
  else if (rcvstr == "fft")                       { get_FFT(); }
  else if (rcvstr == "cfft")                      { cont_FFT(); }
  else if (rcvstr == "wfft"     || ENC_POS == 5)  { wsplot_FFT(); }
  else if (rcvstr == "lvl")                       { lvl(); }
  else if (rcvstr == "enc"      || ENC_POS == 6)  { enc(); }
  else if (rcvstr == "btn")                       { btn(); }
  else if (rcvstr == "js"       || ENC_POS == 7)  { joystick(); }
  else if (rcvstr == "calgyro")                   { calgyro(); }
  else if (rcvstr == "mag")                       { magdata(); }
  else if (rcvstr == "tmp")                       { imutmp(); }
  else if (rcvstr == "telem2ser")                 { telem2ser();}        
  else if (rcvstr == "calmag")                    { calmag(); }
  else if (rcvstr == "store_magcal")              { store_magcal(); }
  else if (rcvstr == "recall_magcal")             { recall_magcal(); }
  else if (rcvstr.substring(0,2) == "wi")         { wi(); }
  else if (rcvstr == "bt")                        { bt(); }
  else if (rcvstr == "espon")                     { en_esp(); }
  else if (rcvstr == "esprst"   || ENC_POS == 8)  { rst_esp(); }
  else if (rcvstr == "espcrnl")                   { espcrnl(); }
  else if (rcvstr == "fesp"     || ENC_POS == 9)  { fesp(); }
  else if (rcvstr == "b2e")                       { b2e(); }
  else if (rcvstr == "b2el")                      { b2el(); }
  else if (rcvstr == "u2e")                       { u2e(); }
  else if (rcvstr == "bu2e")                      { bu2e(); }
  else if (rcvstr == "showESPio")                 { showESPio(); }
  else if (rcvstr == "clr"      || ENC_POS == 10) { clr(); }
  else if (rcvstr != "")                          { MySerial.println(); MySerial.println(rcvstr); 
                                                    MySerial.println("Invalid"); rcvstr = ""; }

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void test()
{
  Serial.println("\ntest ok");
  //_reboot_Teensyduino_();
  (*(uint32_t *)0xE000ED0C = 0x5FA0004);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clr()
{
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bt()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("BT MASTER MODE");
  display.display(); 
  
  while(1){//digitalRead(2==HIGH)){
    if (Serial.available() > 0) {
      Serial3.write(Serial.read());
    }
    if (Serial3.available() > 0) {
      Serial.write(Serial3.read());
    }
  }

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.clearDisplay();
  display.display();
  digitalWrite(6, LOW);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void enc()
{
  while(MySerial.available()==0){
    long newPosition = myEnc.read();
    if (newPosition != oldPosition) {
      oldPosition = newPosition;
      Serial.println(newPosition);
    }
    
    display.clearDisplay(); 
    display.setCursor(0,0);
    display.print(newPosition/4);
    display.display();
    
    delay(50);
  }
  MySerial.println();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int encB()
{
  long newPosition = 0;
  while(digitalRead(BTNA)==LOW && digitalRead(BTNA)==LOW){
    newPosition = myEnc.read();
    if (newPosition != oldPosition) {
      oldPosition = newPosition;
      //Serial.println(newPosition);
    }
    
    display.clearDisplay(); 
    display.setCursor(0,0);
    display.print(newPosition/4);
    display.display();
    
    delay(10);
  }
  MySerial.println();
  MySerial.println(newPosition/4);
  return newPosition/4;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OLEDln(String txt)
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.print(txt);
  display.display();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void joystick(){
    int xpos=0, ypos=0;
    int px=64, py=32;
    int A=0, B=0;
    long newPosition = myEnc.read();
    while(MySerial.available()==0){
      xpos=analogRead(JX);
      ypos=analogRead(JY);
      A = digitalRead(BTNA);
      B = digitalRead(BTNB);
      imu.readAccel();
      px=imu.ax/128+64;
      py=imu.ay/256+32;
      newPosition = myEnc.read();
      if (newPosition != oldPosition) oldPosition = newPosition;
      display.clearDisplay();
      display.setCursor(0,0);
      //display.drawPixel(xpos/8, (1024-ypos)/16, WHITE);
      display.drawPixel(px, py, WHITE);
      display.drawCircle(px, py, 3, WHITE);
      display.drawCircle(xpos/8, (1024-ypos)/16, 5, WHITE);
      display.println(xpos);
      display.println(ypos);
      display.println();display.println();display.println();
      display.println(newPosition/4);
      display.print(A);display.print(' ');display.println(B);
      display.display();
      delay(10);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void btn(){
  MySerial.println();
  MySerial.println(digitalRead(BTNA));
  MySerial.println(digitalRead(BTNB));
  //MySerial.println(newPosition);
}

