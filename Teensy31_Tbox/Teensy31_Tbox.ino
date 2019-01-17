
//Teensy Pinout
//0) TX1 'ESP8266'
//1) RX1 'ESP8266'
#define ESP_Ser Serial1
//2) CE 'ESP8266'
#define ESP_CE 2
//3) GPIO0 'ESP8266'
#define ESP_GPIO0 3
//4) RST 'ESP8266'
#define ESP_RST 4
//5) PWM 'servo'
#define PWM5 5
#define BTNA 5
//6) CMD 'HC-05'
#define BT_CMD 6
//7) RX3 'HC-05'
//8) TX3 'HC-05'
#define BT_Ser Serial3
//9) TX2 'GPS'
#define GPS_Ser Serial2
//10) CS
#define SPI_CS 10
//11) MOSI 
//12) MISO
//13) LED
#define LED 13
//14) SCLK
//15) 
//16) GPIO2 'ESP8266'
#define ESP_GPIO2 16
#define ENC_A 16
//17)
#define ENC_B 17 
//18) SDA0 I2C 'IMU'
//19) SCL0 I2C 'IMU'
//20)
#define BTNB 20 
//21) 
//22) JoyStick_X
#define JX A9
//23) JoyStick_y
#define JY A8

#define MySerial Serial3
#define ARM_MATH_CM4
#include <arm_math.h>


#include <EEPROM.h>
int gxbias = 0, gybias = 0, gzbias =0;
int mxoffset = 0, myoffset = 0, mzoffset =0;
float mxscale = 0, myscale = 0, mzscale = 0;
float heading_offset = 250.0;

//const int led_pin = 13;

/////////////////////////////////////////////////////////

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


/////////////////////////////////////////////////////////

#include <Servo.h>
Servo myservo;
int pos = 0;

#include <Encoder.h>
Encoder myEnc(ENC_A, ENC_B);
long oldPosition  = -999;
int ENC_POS=0;
//long newPosition = myEnc.read();

////////////////////////////////////////////////////////
//IMU
// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Wire.h>
#include <SparkFunLSM9DS1.h>

// LSM9DS1 Library Init //
// Use the LSM9DS1 class to create an object.
LSM9DS1 imu;

// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 250 // 250 ms between prints

// Earth's magnetic field varies by location.
#define DECLINATION 0

#include <MadgwickAHRS.h>
Madgwick AHRSfilter;

//-------------------------------------------------------------
#include <TinyGPS.h>

/* This sample code demonstrates the normal use of a TinyGPS object. */
TinyGPS gps;

/* On Teensy, the UART (real serial port) is always best to use. */
/* Unlike Arduino, there's no need to use NewSoftSerial because */
/* the "Serial" object uses the USB port, leaving the UART free. */
//HardwareSerial Uart = HardwareSerial();
#define Uart Serial2
void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);

//-------------------------------------------------------------

//ESP8266

String espmsg;
char espchar;

//--------------------------------------------------------------

void setup() 
{

  pinMode(LED, OUTPUT);  
  
// USB serial
  Serial.begin(115200);
  delay(100);
  Serial.println("usb serial started");
  delay(100);
  Serial3.begin(115200);
  delay(100);
  Serial3.println("BT serial started");
  delay(100);
  MySerial.println("serial started...");

//SSD1306 --------------------------------------------------------

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(100);

  // Clear the buffer.
  display.setRotation(2);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Ready:>");
  display.display();

//Servo ----------------------------------------------------------

  //myservo.attach(PWM5);
  //myservo.write(93);
    
//IMU ---------------------------------------------------------  

  // Before initializing the IMU, there are a few settings
  // we may need to adjust. Use the settings struct to set
  // the device's communication mode and addresses:
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  // The above lines will only take effect AFTER calling
  // imu.begin(), which verifies communication with the IMU
  // and turns it on.
  if (!imu.begin())
  {
    MySerial.println("Failed to communicate with LSM9DS1.");
    MySerial.println("Double-check wiring.");
    MySerial.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1)
      ;
  }
  Wire.setClock(400000);
  MySerial.println("imu started...");
  calgyro();
  MySerial.println("recall_magcal");
  recall_magcal();
  
  imu.settings.temp.enabled = true;
  imu.readTemp();
  MySerial.print("T: ");
  MySerial.println(imu.temperature);
  
//GPS -------------------------------------------------------
  #define Uart Serial2
  Serial2.begin(9600);
  delay(100);
  //MySerial.print("Testing TinyGPS library v. "); MySerial.println(TinyGPS::library_version());
  //MySerial.println("by Mikal Hart");
  //MySerial.println();
  MySerial.print("Sizeof(gpsobject) = "); MySerial.println(sizeof(TinyGPS));
  MySerial.println("gps started...");

//ESP ------------------------------------------------------
  pinMode(ESP_GPIO0, OUTPUT);
  digitalWrite(ESP_GPIO0, HIGH);  //set GPIO0 high
  pinMode(ESP_RST, OUTPUT);
  digitalWrite(ESP_RST, HIGH);  //set RST low
  pinMode(ESP_CE, OUTPUT);
  digitalWrite(ESP_CE, HIGH);  //set CE low
  //delay(100);
  //pinMode(ESP_GPIO2, INPUT);
  Serial1.begin(115200);
  //en_esp();
  MySerial.println();

//Joystick ---------------------------------------------------
  //pinMode(JX, INPUT);
  //pinMode(JY, INPUT);
  pinMode(BTNA, INPUT_PULLUP);
  pinMode(BTNB, INPUT_PULLUP);
  
  //Ready ----------------------------------------------------
  MySerial.print("Ready:>");

} //end setup
//#####################################################################

String rcvstr, last_rcvstr;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void loop()
{

  // On a Teensy, large groups of bytes tend to arrive all at once.
  // This bytecount prevents taking too much time processing them.
  unsigned char bytecount = 0;
  char incomingByte;
//////////////////////////////////////////////////////////////////////////////  
//Catch serial data from USB or HC-05  
    if (MySerial.available())
    {
      bytecount++;
      incomingByte = MySerial.read();
      rcvstr += incomingByte;
      if (incomingByte==127 && rcvstr.length()>1) rcvstr = rcvstr.substring(0,rcvstr.length()-2);
      if (incomingByte != 13 || incomingByte != 10) MySerial.print(incomingByte);
      if (incomingByte == 27) {rcvstr = last_rcvstr; MySerial.print(rcvstr); }
      if (incomingByte == 13 || incomingByte == 10)
        {
          rcvstr.trim(); 
          //MySerial.println(rcvstr);
          // Interperate Input Command
          // maybe use switch(rcvstr) {}

          command_lookup();
           
          MySerial.println();
          //MySerial.println(int(rcvstr[0]));
          MySerial.print("Ready:>");
          last_rcvstr = rcvstr;
          rcvstr = "";
        }
    }   
//////////////////////////////////////////////////////////////////////////////
// Catch Serial Data from ESP8266
    if(Serial1.available()) {MySerial.write(Serial1.read());}
     
//////////////////////////////////////////////////////////////////////////////      
// Catch Serial Data from GPS
    if(Uart.available())
      {
        digitalWrite(LED, HIGH);
        char c = Uart.read();
        gps.encode(c);
        digitalWrite(LED, LOW);
      }
///////////////////////////////////////////////////////////////////////////////
//read encoder
    if (digitalRead(BTNA)==LOW && digitalRead(BTNB)==LOW){
      ENC_POS = encB();
      command_lookup();
      ENC_POS = 0;
    }
    
          
} // end loop

