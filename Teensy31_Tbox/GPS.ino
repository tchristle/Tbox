// GPS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void GPS_update()
{
    MySerial.println();
    MySerial.println("Acquired Data");
    MySerial.println("-------------");
    gpsdump(gps);
    MySerial.println("-------------");
}

void gpsdump(TinyGPS &gps)
{
  long lat, lon;
  float flat, flon;
  unsigned long age, date, time, chars;
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned short sentences, failed;

  gps.get_position(&lat, &lon, &age);
  MySerial.print("Lat/Long(10^-5 deg): "); MySerial.print(lat); MySerial.print(", "); MySerial.print(lon); 
  MySerial.print(" Fix age: "); MySerial.print(age); MySerial.println("ms.");
  
  // On Arduino, GPS characters may be lost during lengthy MySerial.print()
  // On Teensy, Serial prints to USB, which has large output buffering and
  //   runs very fast, so it's not necessary to worry about missing 4800
  //   baud GPS characters.

  gps.f_get_position(&flat, &flon, &age);
  MySerial.print("Lat/Long(float): "); printFloat(flat, 5); MySerial.print(", "); printFloat(flon, 5);
  MySerial.print(" Fix age: "); MySerial.print(age); MySerial.println("ms.");

  gps.get_datetime(&date, &time, &age);
  MySerial.print("Date(ddmmyy): "); MySerial.print(date); MySerial.print(" Time(hhmmsscc): ");
    MySerial.print(time);
  MySerial.print(" Fix age: "); MySerial.print(age); MySerial.println("ms.");

  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  MySerial.print("Date: "); MySerial.print(static_cast<int>(month)); MySerial.print("/"); 
    MySerial.print(static_cast<int>(day)); MySerial.print("/"); MySerial.print(year);
  MySerial.print("  Time: "); MySerial.print(static_cast<int>(hour)); MySerial.print(":"); 
    MySerial.print(static_cast<int>(minute)); MySerial.print(":"); MySerial.print(static_cast<int>(second));
    MySerial.print("."); MySerial.print(static_cast<int>(hundredths));
  MySerial.print("  Fix age: ");  MySerial.print(age); MySerial.println("ms.");

  MySerial.print("Alt(cm): "); MySerial.print(gps.altitude()); MySerial.print(" Course(10^-2 deg): ");
    MySerial.print(gps.course()); MySerial.print(" Speed(10^-2 knots): "); MySerial.println(gps.speed());
  MySerial.print("Alt(float): "); printFloat(gps.f_altitude()); MySerial.print(" Course(float): ");
    printFloat(gps.f_course()); MySerial.println();
  MySerial.print("Speed(knots): "); printFloat(gps.f_speed_knots()); MySerial.print(" (mph): ");
    printFloat(gps.f_speed_mph());
  MySerial.print(" (mps): "); printFloat(gps.f_speed_mps()); MySerial.print(" (kmph): ");
    printFloat(gps.f_speed_kmph()); MySerial.println();

  gps.stats(&chars, &sentences, &failed);
  MySerial.print("Stats: characters: "); MySerial.print(chars); MySerial.print(" sentences: ");
    MySerial.print(sentences); MySerial.print(" failed checksum: "); MySerial.println(failed);
}

void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0) {
     MySerial.print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  MySerial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    MySerial.print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    int toPrint = int(remainder);
    MySerial.print(toPrint);
    remainder -= toPrint;
  }
}
////////////////////////////////////////////////////////////////////////////////////
void latlon()
{
  float flat, flon;
  unsigned long age;
  
  gps.f_get_position(&flat, &flon, &age);
  printFloat(flat, 6); MySerial.print(", "); printFloat(flon, 6);
  MySerial.println();
}
////////////////////////////////////////////////////////////////////////////////
float getalt()
{
  MySerial.println();
  MySerial.println(gps.altitude());
  return gps.altitude();
}
/////////////////////////////////////////////////////////////////////////////
float findheading(float lat1, float lon1, float lat2, float lon2)
{
  //caculate heading angle between two gps coordinates
  //flat earth approximation used
  //257972 ft/(deg lat)
  //368110 ft/(deg lon)
  float angle, delta_lat, delta_lon;
  delta_lat = (lat2-lat1)*257972;
  delta_lon = (lon2-lon1)*368110;
  
  if(delta_lat >= 0 && delta_lon > 0)
  {
    angle = 90-atan(delta_lat/delta_lon)*180/3.14159;
  }
  else if(delta_lat >= 0 && delta_lon < 0)
  {
    angle = 270-atan(delta_lat/delta_lon)*180/3.14159;    
  }
  else if(delta_lat <= 0 && delta_lon < 0)
  {
    angle = 180+atan(delta_lat/delta_lon)*180/3.14159;    
  }
  else if(delta_lat <= 0 && delta_lon > 0)
  {
    angle = 180+atan(delta_lat/delta_lon)*180/3.14159;    
  }
  else if (delta_lon == 0 && delta_lat > 0) angle = 0.0;
  else if (delta_lon == 0 && delta_lat < 0) angle = 180.0;
  else angle = 0.0;
  
  return angle;
}
////////////////////////////////////////////////////////////////////////////
float getdist(float lat1, float lon1, float lat2, float lon2)
{
  //caculate heading angle between two gps coordinates
  //flat earth approximation used
  //257972 ft/(deg lat)
  //368110 ft/(deg lon)

  float dist, delta_lat, delta_lon;
  delta_lat = (lat2-lat1)*257972;
  delta_lon = (lon2-lon1)*368110;
  
  dist = powf((powf(delta_lat,2) + powf(delta_lon,2)),0.5);
  
  return dist;
}
///////////////////////////////////////////////////////////////////////////////
void delay_gps(unsigned int delms)
{
  elapsedMillis waiting;
  while(waiting < delms)
  {
    if(Uart.available())
      {
        digitalWrite(LED, HIGH);
        char c = Uart.read();
        gps.encode(c);
        digitalWrite(LED, LOW);
      }
  }
}
/////////////////////////////////////////////////////////////////////////////////



