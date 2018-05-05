// IMU @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/*****************************************************************
LSM9DS1_Basic_I2C.ino
SFE_LSM9DS1 Library Simple Example Code - I2C Interface
Jim Lindblom @ SparkFun Electronics
Original Creation Date: April 30, 2015
https://github.com/sparkfun/LSM9DS1_Breakout

The LSM9DS1 is a versatile 9DOF sensor. It has a built-in
accelerometer, gyroscope, and magnetometer. Very cool! Plus it
functions over either SPI or I2C.

This Arduino sketch is a demo of the simple side of the
SFE_LSM9DS1 library. It'll demo the following:
* How to create a LSM9DS1 object, using a constructor (global
  variables section).
* How to use the begin() function of the LSM9DS1 class.
* How to read the gyroscope, accelerometer, and magnetometer
  using the readGryo(), readAccel(), readMag() functions and 
  the gx, gy, gz, ax, ay, az, mx, my, and mz variables.
* How to calculate actual acceleration, rotation speed, 
  magnetic field strength using the calcAccel(), calcGyro() 
  and calcMag() functions.
* How to use the data from the LSM9DS1 to calculate 
  orientation and heading.

Hardware setup: This library supports communicating with the
LSM9DS1 over either I2C or SPI. This example demonstrates how
to use I2C. The pin-out is as follows:
  LSM9DS1 --------- Arduino
   SCL ---------- SCL (A5 on older 'Duinos')
   SDA ---------- SDA (A4 on older 'Duinos')
   VDD ------------- 3.3V
   GND ------------- GND
(CSG, CSXM, SDOG, and SDOXM should all be pulled high. 
Jumpers on the breakout board will do this for you.)

The LSM9DS1 has a maximum voltage of 3.6V. Make sure you power it
off the 3.3V rail! I2C pins are open-drain, so you'll be 
(mostly) safe connecting the LSM9DS1's SCL and SDA pins 
directly to the Arduino.

Development environment specifics:
  IDE: Arduino 1.6.3
  Hardware Platform: SparkFun Redboard
  LSM9DS1 Breakout Version: 1.0

This code is beerware. If you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, 
please buy us a round!

*****************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void calgyro()
{
  MySerial.println();
  MySerial.println("Hold Still!!");
  for(int i=0 ; i<10 ; i++)
  {
    MySerial.print(i);
    imu.readGyro();
    gxbias += imu.gx;
    gybias += imu.gy;
    gzbias += imu.gz;
    delay(100);
  }
  gxbias = gxbias/10;
  gybias = gybias/10;
  gzbias = gzbias/10;
  MySerial.println();
  MySerial.print("gyro x bias = "); MySerial.println(gxbias);
  MySerial.print("gyro y bias = "); MySerial.println(gybias);
  MySerial.print("gyro z bias = "); MySerial.println(gzbias);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void calmag()
{
  //int mx_array[299], my_array[299];
  float mx_max=0, mx_min=0, my_max=0, my_min=0;

  MySerial.println();
  MySerial.println("calibrating magnetometer...");

  //start rotate scan

  MySerial.println();  

  for(int i=0; i<300; i++)
  {
    imu.readMag();
    //mx_array[i] = imu.mx;
    //my_array[i]= imu.my;
    if(imu.mx > mx_max) mx_max = imu.mx;
    if(imu.mx < mx_min) mx_min = imu.mx;
    if(imu.my > my_max) my_max = imu.my;
    if(imu.my < my_min) my_min = imu.my;
    MySerial.print(i); MySerial.print("  "); MySerial.print('\r');
    delay(30);
  }

  //stop rotate
  
  MySerial.println();

  //to cal mag
  //find max value
  //find min value
  MySerial.print("max/min x = "); MySerial.print(imu.calcMag(mx_max)); MySerial.print(" / "); MySerial.println(imu.calcMag(mx_min));
  MySerial.print("max/min y = "); MySerial.print(imu.calcMag(my_max)); MySerial.print(" / "); MySerial.println(imu.calcMag(my_min));
  //offset = (max - min)/2
  mxoffset = (mx_max + mx_min)/2;
  myoffset = (my_max + my_min)/2;
  MySerial.print("mag x offset = "); MySerial.println(imu.calcMag(mxoffset));
  MySerial.print("mag y offset = "); MySerial.println(imu.calcMag(myoffset));
  mxscale = ((my_max - my_min)+(mx_max - mx_min))/(2*(mx_max - mx_min));
  myscale = ((my_max - my_min)+(mx_max - mx_min))/(2*(my_max - my_min));
  MySerial.print("mag x full scale = "); MySerial.println(imu.calcMag(mx_max - mx_min));
  MySerial.print("mag y full scale = "); MySerial.println(imu.calcMag(my_max - my_min));
  //MySerial.print("max/min x = "); MySerial.print((mx_max)); MySerial.print(" / "); MySerial.println((mx_min));
  //MySerial.print("max/min y = "); MySerial.print((my_max)); MySerial.print(" / "); MySerial.println((my_min));
  MySerial.print("mag x scale = "); MySerial.println(mxscale);
  MySerial.print("mag y scale = "); MySerial.println(myscale);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void store_magcal()
{
  int imxscale, imyscale;
  imxscale = mxscale * 1000;
  imyscale = myscale * 1000;
  
  MySerial.println();
  MySerial.println(mxoffset);
  EEPROM.write(0x10, mxoffset);
  EEPROM.write(0x11, mxoffset>>8);
  MySerial.println(myoffset);
  EEPROM.write(0x12, myoffset);
  EEPROM.write(0x13, myoffset>>8);
  MySerial.println(mxscale);
  EEPROM.write(0x14, imxscale);
  EEPROM.write(0x15, imxscale>>8);
  MySerial.println(myscale);
  EEPROM.write(0x16, imyscale);
  EEPROM.write(0x17, imyscale>>8);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void recall_magcal()
{
  MySerial.println();
  mxoffset = EEPROM.read(0x10);
  mxoffset = mxoffset + (EEPROM.read(0x11)<<8);
  MySerial.println(mxoffset);
  myoffset = EEPROM.read(0x12);
  myoffset = myoffset + (EEPROM.read(0x13)<<8);
  MySerial.println(myoffset);
  mxscale = EEPROM.read(0x14);
  mxscale = mxscale + (EEPROM.read(0x15)<<8);
  mxscale = mxscale/1000;
  MySerial.println(mxscale);
  myscale = EEPROM.read(0x16);
  myscale = myscale + (EEPROM.read(0x17)<<8);
  myscale = myscale/1000;
  MySerial.println(myscale);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void IMU()
{
  MySerial.println();
  printGyro();  // Print "G: gx, gy, gz"
  printAccel(); // Print "A: ax, ay, az"
  printMag();   // Print "M: mx, my, mz"
  
  // Print the heading and orientation for fun!
  // Call print attitude. The LSM9DS1's magnetometer x and y
  // axes are opposite to the accelerometer, so my and mx are
  // substituted for each other.
  printAttitude(imu.ax, imu.ay, imu.az, -(imu.my - myoffset)*myscale, -(imu.mx - mxoffset)*mxscale, imu.mz);
  MySerial.println();

  display.println();
  display.print("A: "); display.print(imu.ax); display.print(", "); display.print(imu.ay); display.print(", "); display.println(imu.az);
  display.display();
  
  delay(PRINT_SPEED);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void printGyro()
{
  // To read from the gyroscope, you must first call the
  // readGyro() function. When this exits, it'll update the
  // gx, gy, and gz variables with the most current data.
  imu.readGyro();
  
  // Now we can use the gx, gy, and gz variables as we please.
  // Either print them as raw ADC values, or calculated in DPS.
  MySerial.print("G: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcGyro helper function to convert a raw ADC value to
  // DPS. Give the function the value that you want to convert.
  MySerial.print(imu.calcGyro(imu.gx-gxbias), 2);
  MySerial.print(", ");
  MySerial.print(imu.calcGyro(imu.gy-gybias), 2);
  MySerial.print(", ");
  MySerial.print(imu.calcGyro(imu.gz-gzbias), 2);
  MySerial.println(" deg/s");
#elif defined PRINT_RAW
  MySerial.print(imu.gx-gxbias);
  MySerial.print(", ");
  MySerial.print(imu.gy-gybias);
  MySerial.print(", ");
  MySerial.println(imu.gz-gzbias);
#endif
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void printAccel()
{
  // To read from the accelerometer, you must first call the
  // readAccel() function. When this exits, it'll update the
  // ax, ay, and az variables with the most current data.
  imu.readAccel();
  
  // Now we can use the ax, ay, and az variables as we please.
  // Either print them as raw ADC values, or calculated in g's.
  MySerial.print("A: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcAccel helper function to convert a raw ADC value to
  // g's. Give the function the value that you want to convert.
  MySerial.print(imu.calcAccel(imu.ax), 2);
  MySerial.print(", ");
  MySerial.print(imu.calcAccel(imu.ay), 2);
  MySerial.print(", ");
  MySerial.print(imu.calcAccel(imu.az), 2);
  MySerial.println(" g");
#elif defined PRINT_RAW 
  MySerial.print(imu.ax);
  MySerial.print(", ");
  MySerial.print(imu.ay);
  MySerial.print(", ");
  MySerial.println(imu.az);
#endif

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void printMag()
{
  // To read from the magnetometer, you must first call the
  // readMag() function. When this exits, it'll update the
  // mx, my, and mz variables with the most current data.
  imu.readMag();
  
  // Now we can use the mx, my, and mz variables as we please.
  // Either print them as raw ADC values, or calculated in Gauss.
  MySerial.print("M: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcMag helper function to convert a raw ADC value to
  // Gauss. Give the function the value that you want to convert.
  MySerial.print(imu.calcMag(imu.mx-mxoffset)*mxscale, 2);
  MySerial.print(", ");
  MySerial.print(imu.calcMag(imu.my-myoffset)*myscale, 2);
  MySerial.print(", ");
  MySerial.print(imu.calcMag(imu.mz), 2);
  MySerial.println(" gauss");
#elif defined PRINT_RAW
  MySerial.print(imu.mx-mxoffset);
  MySerial.print(", ");
  MySerial.print(imu.my-myoffset);
  MySerial.print(", ");
  MySerial.println(imu.mz);
#endif
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculate pitch, roll, and heading.
// Pitch/roll calculations take from this app note:
// http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
// Heading calculations taken from this app note:
// http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));
  
  float heading;
  if (my == 0)
    heading = (mx < 0) ? 180.0 : 0;
  else
    heading = atan2(mx, my);
    
  //heading -= DECLINATION * PI / 180;
  //heading -= heading_offset * PI / 180;
  
  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;
  
  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  //heading -= 360.0;
  //heading *= -1.0;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;
  
  MySerial.print("Pitch, Roll: ");
  MySerial.print(pitch, 2);
  MySerial.print(", ");
  MySerial.println(roll, 2);
  MySerial.print("Heading: "); MySerial.println(heading, 2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
float calcHeading(float mx, float my)
{
  float heading;
  if (my == 0)
    heading = (mx < 0) ? 180.0 : 0;
  else
    heading = atan2(mx, my);
    
  //heading -= DECLINATION * PI / 180;
  heading -= heading_offset * PI / 180;
  if(heading<(-1.0*PI)) heading += 2.0*PI;
  
  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;
  
  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  //heading -= 360.0;
  //heading *= -1.0;

  return heading;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void magdata()
{
  MySerial.read();
  while(MySerial.available()<1)
    {
      imu.readMag();
      MySerial.print(imu.mx); MySerial.print(" "); MySerial.print(imu.my); MySerial.print(" "); MySerial.println(imu.mz);
      delay(50);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void telem2ser()
{
  float flat, flon;
  unsigned long age, date, time;
  String telemetry;
  int RSSI;
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
          RSSI = getRSSI();
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
      String("temp") + String(" ") + String(RSSI);
      MySerial.println(telemetry);
    }
    newdata = false;
    start = millis();
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void imutmp()
{
  imu.readTemp();
  MySerial.println();
  MySerial.print("T: ");
  MySerial.println(imu.temperature);
}

