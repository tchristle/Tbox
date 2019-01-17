#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

#define ssid_STA 0x00
#define password_STA 0x40
#define ssid_AP 0x80
#define password_AP 0xC0
#define node_id 0x100
#define net_mode 0x120
#define starts 0x140

#define sw_ver 0.65

const char* help = "? this menu \n\r"
  "test \n\r"
  "// WIFI ////////////////////////////////////////////////////////// \n\r"
  "wiscan  wifi scan \n\r"
  "lc list clients \n\r"
  "c2t connect telnet \n\r"
  "ip get ip \n\r"
  "id get id \n\r"
  "sw stop watch \n\r"
  "cs check_server \n\r"
  "rssi get rssi \n\r"
  "stanum get_STAnum \n\r"
  "// TCP ////////////////////////////////////////////////////////// \n\r"
  "sp - server print \n\r"
  "ping ping() \n\r"
  "pong pong() \n\r"
  "con connect \n\r"
  "cp - client print \n\r"
  "// ESP ////////////////////////////////////////////////////////// \n\r"
  "rst - reset \n\r"
  "// EEPROM /////////////////////////////////////////////////////// \n\r" 
  "STAid - set station ssid \n\r"
  "STApw - set station password \n\r"
  "APid - set AP ssid \n\r"
  "APpw - set AP password \n\r"
  "NODid - set node id \n\r"
  "setNODE - set as node \n\r"
  "setHOST - set as host \n\r"
  "setSTRT - startup function \n\r"
  "ver - version \n\r";

const char* ssid = "NETGEAR11";
const char* password = "niftypotato037";
char* host = "192.168.4.1";
String rIP = "192.168.1.1";
String node_id_c;
String net_mode_c;
String ssid_STA_c;
String password_STA_c;
String ssid_AP_c; 
String password_AP_c;
String strtscr;
char incomingByte;
String rcvstr, last_rcvstr;
char espchar = ' ';
String espmsg = "";
String cmd_msg = "";
char srvchar = ' ';
String srvmsg = "";
const int ESP_BUILTIN_LED = 2;
#define MAX_SRV_CLIENTS 10
Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, 4, NEO_RGB + NEO_KHZ800);
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, 2, NEO_RGB + NEO_KHZ800);
WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];
WiFiClient client;
WiFiUDP Udp;
unsigned int localUdpPort = 5006;  // local port to listen on
char incomingPacket[1025];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back


//=========================================================
void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  
  EEPROM_setup();
  wifi_setup();
  OTA_setup();

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(12, OUTPUT);  //sonoff SW
  digitalWrite(13, HIGH);
  pinMode(13, OUTPUT);  //sonoff sLED
  digitalWrite(13, LOW);
  
  Serial.print("SketchMD5: "); Serial.println(ESP.getSketchMD5());
  Serial.print("IP address: "); Serial.println(WiFi.localIP());

  telnet_setup();
  Serial.print("ESP:>");
  command_lookup(strtscr);
}

//=========================================================
void loop() {
  ArduinoOTA.handle();
  check_clients();
  get_client_data();
  get_server_data();
  //get_udp();
  check_uart();
}
