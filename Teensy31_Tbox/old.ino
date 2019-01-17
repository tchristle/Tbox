/*
class Serial_sel
{
  // 1 -> USB
  // 2 -> BT
  // 3 -> wifi
  public:
    int sel_pos;
    int cipnum;
    void set_pos(int num)
    {
      sel_pos = num;
    }
    void setcipnum(int num)
    {
      cipnum = num;
    }
    void print(String msg = "", int prec = 2)
    {
      if(sel_pos == 1) Serial.print(msg);
      if(sel_pos == 2) Serial3.print(msg);
      if(sel_pos == 3) txt2wi(0,msg);
    }
    void println(String msg = "", int prec = 2)
    {     
      if(sel_pos == 1) Serial.println(msg);
      if(sel_pos == 2) Serial3.println(msg);
      if(sel_pos == 3) txt2wi(0,msg);
    }
    byte read(void)
    {
      byte ret = 0;      
      if(sel_pos == 1) ret = Serial.read();
      if(sel_pos == 2) ret = Serial3.read();
      if(sel_pos == 3) ret = Serial1.read();
      return ret;
    }
    int available(void)
    {
      int num = 0;
      if(sel_pos == 1) num = Serial.available();
      if(sel_pos == 2) num = Serial3.available();
      if(sel_pos == 3) num = Serial1.available();
      return num;
    }
};

Serial_sel MySerial;
*/

  //MySerial.set_pos(1);
  //MySerial.println("myser sel test1");
  //MySerial.set_pos(2);
  //MySerial.println("myser sel test2");

  //MySerial.set_pos(2);
  //MySerial.println();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define Serial Serial3

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
#include <PulsePosition.h>
PulsePositionInput myInput; 
//DSMX Radio -----------------------------------------------
myInput.begin(23); 

void ppm()
{
  //this function reads cPPM data from OrangeRX R614XN
  // CH# 1-THR, 2-YAW, 3-PITCH, 4-ROLL, 5-Mode, 6-Gyro
  int ch_num;
  
  while(MySerial.available()<1)
  {
    ch_num = myInput.available(); 
    MySerial.print(ch_num); MySerial.print(" "); 
    MySerial.print(myInput.read(1)); MySerial.print(" "); 
    MySerial.print(myInput.read(2)); MySerial.print(" "); 
    MySerial.print(myInput.read(3)); MySerial.print(" "); 
    MySerial.print(myInput.read(4)); MySerial.print(" "); 
    MySerial.print(myInput.read(5)); MySerial.print(" "); 
    MySerial.print(myInput.read(6)); MySerial.print(" "); 
    MySerial.println(); 
    delay_gps(100);
  }  
}

else if (rcvstr == "ppm") { ppm(); }     
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*void telem2wifi()
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
          //rssi = getRSSI();
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
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
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
 */
////////////////////////////////////////////////////////////////////////////////
/*
void hdtrack()
{
  float lat_ant, lon_ant, lat_client, lon_client, dist, heading;
  String espmsg = "", client_data, num;
  char espchar;
  int j;

  //get local gps data
  gps.f_get_position(&lat_ant, &lon_ant);
  //scan for connected wifi for track
  //setup for listen on port 23

  MySerial.read();
  while(MySerial.available()<1)
  {
    //wait for valid data 
    if(Serial1.available()>0) 
    {
      espchar = char(Serial1.read());
      espmsg += espchar;
    }
    if(espchar == 10)
    {
      //store recvd gps data to lat_client, lon_client
      client_data = espmsg.trim();
      j=0;
      for( int i = 0; espmsg.length(); i++)
      {
        if(espmsg[i] == 32 && j == 0) {lat_client = num.toFloat(); num=""; j++;}
        if(espmsg[i] == 32 && j == 1) {lon_client = num.toFloat(); num=""; j++;}
        num += espmsg[i];
      }
    }
    
    dist = getdist(lat_ant, lon_ant, lat_client, lon_client);
    heading = findheading(lat_ant, lon_ant, lat_client, lon_client);
    if(dist>30) hd2(heading, 0);
  }
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////
/*
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
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  
  else if (rcvstr.substring(0,2) == "az") { MySerial.println(); MySerial.println("moving"); Az(); }
  else if (rcvstr == "elscan") { MySerial.println(); MySerial.println("moving"); ElSCAN(); }
  else if (rcvstr.substring(0,3) == "el ") { MySerial.println(); MySerial.println("moving"); El(); }
  else if (rcvstr.substring(0,3) == "hd2") { hd2(rcvstr.substring(4,7).toInt(), rcvstr.substring(8,11).toInt()); }
  else if (rcvstr == "") { MySerial.println(); }         
  
 
void Az()
{
  int pwid, dur;
  pwid = rcvstr.substring(3,4).toInt();
  dur = rcvstr.substring(5,6).toInt();
  if(rcvstr.substring(5,6)=='a') {dur = 'a';} //  MySerial.println("caught the a");}

  //az TX
  digitalWrite(LED, HIGH); // Flash a light to show transmitting
  Serial1.print("rot "); Serial1.println(pwid); 
  delay_gps(dur*10); 
  Serial1.println("rot 0");
  digitalWrite(LED, LOW);  

  // average mag data to filter noise
  float amy=0, amx=0, amz=0;
  int avgs = 20;
  for(int i=0;i<avgs;i++)
  {
    imu.readMag();
    imu.readAccel(); 
    amy += imu.my;
    amx += imu.mx;
    amz +=imu.mz;
    delay_gps(10);
  }  
  amy = amy/avgs;
  amx = amx/avgs;
  amz = amz/avgs;
  printAttitude(imu.ax, imu.ay, imu.az, -(amy - myoffset)*myscale, -(amx - mxoffset)*mxscale, amz);
    
  //clear ESP serial buffer
  while(Serial1.available()>0) Serial1.read();
  
  
  if(rcvstr.substring(5,6).trim()=='a')
  {
    MySerial.read();
    elapsedMillis waiting;
    while(MySerial.available()<1)
    {
      imu.readGyro();
      imu.readMag();
      MySerial.print(calcHeading(-(imu.my - myoffset)*myscale, -(imu.mx - mxoffset)*mxscale)); MySerial.print(" ");  
      MySerial.print(imu.calcGyro(imu.gz-gzbias), 2); MySerial.println(" ");  
      delay(100);
    }
    
    digitalWrite(LED, HIGH); // Flash a light to show transmitting
    Serial1.println("rot 0"); 
    digitalWrite(LED, LOW); 
    MySerial.println();

    //clear ESP serial buffer
    while(Serial1.available()>0) Serial1.read();
  }

}
/////////////////////////////////////////////////////////////////////////////////////
void Az_move(int pwid, int dur)
{
  if(pwid < 0) pwid = 0;
  if(pwid > 9) pwid = 9;
  if(dur < 0) dur = 0;
  if(dur > 9) dur = 9;
  digitalWrite(LED, HIGH); // Flash a light to show transmitting
  Serial1.println("AT+CIPSTART=4,\"TCP\",\"192.168.4.2\",3333");
  delay(200);
  Serial1.println("AT+CIPSEND=4,6");
  delay(200);
  Serial1.print("pw"); Serial1.print(pwid); Serial1.print("du"); 
  if(dur>=0 && dur<=9) Serial1.println(dur);
  else Serial1.println(char(dur));
  delay(60);
  Serial1.println("AT+CIPCLOSE=4");
  delay(20);
  digitalWrite(LED, LOW);  
}
/////////////////////////////////////////////////////////////////////////////////////
void El()
{
  int el_min = 20;
  int el_max = 170;
  if(rcvstr.substring(3,6).toInt()>el_max) myservo.write(el_max);
  else if(rcvstr.substring(3,6).toInt()<el_min) myservo.write(el_min);
  else myservo.write(rcvstr.substring(3,6).toInt());
}

void ElSCAN()
{
  MySerial.read();
  while(MySerial.available()<1)
  {
    for(pos = 77; pos <= 103; pos += 1) 
      {                                  
        myservo.write(pos);             
        delay(100);                   
      } 
    for(pos = 103; pos >= 77; pos -= 1) 
      {                                  
        myservo.write(pos);             
        delay(100);                   
      }
  }         
}

void hd2(float heading, float elev)
{
  //Ready:>hd2 HHH EEE
  float cur_heading, cur_elev;
  float err_heading, err_elev;
  int next_servo_pos = 92;
  float cal_mx, cal_my;
  
  MySerial.println();
  MySerial.println(heading);
  MySerial.println(elev);
  
  //set heading and elevation limits
  if(elev < -12) elev = 12;
  if(elev > 12) elev = 12;
  if(heading < 0) heading = 0;
  if(heading > 359) heading = 359;

  MySerial.println();
  MySerial.println(heading);
  MySerial.println(elev);

  //set elevation first
  //get current elevation
  imu.readAccel();
  cur_elev = atan2(imu.ay, imu.az)*180.0/PI;
  MySerial.println(cur_elev);
  err_elev = elev - cur_elev;
  MySerial.println(err_elev);
  next_servo_pos = int(myservo.read());
  //start PID loop
  while(abs(err_elev) > 2.0)
  {
    if(err_elev > 0) 
    {
      next_servo_pos ++;
      if(int(myservo.read()) < 170) {myservo.write(next_servo_pos);}
      if(int(myservo.read()) >= 170) {myservo.write(170); MySerial.println("high limit"); break;}
    }
    if(err_elev < 0) 
    {
      next_servo_pos --;
      if(int(myservo.read()) > 20) {myservo.write(next_servo_pos);}
      if(int(myservo.read()) <= 20) {myservo.write(170); MySerial.println("low limit"); break; }
    }
    
    delay(50);
    imu.readAccel();
    cur_elev = atan2(imu.ay, imu.az)*180.0/PI;
    err_elev = elev - cur_elev;
    MySerial.println(err_elev); 
  }

  //now move to heading
  //get current heading
  imu.readMag();
  cal_mx = -(imu.mx - mxoffset)*mxscale;
  cal_my = -(imu.my - mxoffset)*myscale;
  if (cal_my == 0)
    cur_heading = (cal_mx < 0) ? 180.0 : 0;
  else
    cur_heading = atan2(cal_mx, cal_my)*180.0/PI;
  //if(cur_heading < 0) cur_heading = 360 + cur_heading;
  cur_heading = -1*cur_heading+180;
  MySerial.print("starting heading "); MySerial.println(cur_heading);
  //subtract target heading from current heading
  err_heading = heading - cur_heading;
  //find shorter move
  if(abs(err_heading) > 180) err_heading = -1.0*(360.0 - err_heading);
  
  //start PID loop
  while(abs(err_heading) > 2.0)
  {
    if(err_heading > 0 && err_heading <= 45) Az_move(9,1);    //little move clockwise
    if(err_heading < 0 && err_heading >= -45) Az_move(4,1);   //little move counter clockwise
    //if(err_heading > 25 && err_heading <= 45) Az_move(7,4);  //medium move clockwise
    //if(err_heading < -25 && err_heading >= -45) Az_move(2,4); //medium move counter clockwise
    if(err_heading > 45) Az_move(9,4);     //big move clockwise
    if(err_heading < -45) Az_move(4,4);    //bige move counter clockwise
    
    //wait until move is done
    imu.readGyro();
    while(abs(imu.gz) > 500) 
    {
      imu.readGyro();
      MySerial.print("gyro "); MySerial.println(imu.gz);
      delay(50);
    }
    
    //get updated heading
    imu.readMag();
    cal_mx = -(imu.mx - mxoffset)*mxscale;
    cal_my = -(imu.my - mxoffset)*myscale;
    if (cal_my == 0)
      cur_heading = (cal_mx < 0) ? 180.0 : 0;
    else
      cur_heading = atan2(cal_mx, cal_my)*180.0/PI;
    //if(cur_heading < 0) cur_heading = 360 + cur_heading;
    cur_heading = -1*cur_heading+180;;
    MySerial.println(cur_heading);  
    //subtract target heading from current heading
    err_heading = heading - cur_heading;
  }
  
  //move done
  //clear ESP8266 Serial1 buffer
  while(Serial1.available()>0) Serial1.read();
}
///////////////////////////////////////////////////////////////////////////////////////
*/
