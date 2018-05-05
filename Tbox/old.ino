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
