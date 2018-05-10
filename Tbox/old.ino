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
