void telnet_setup(){
  if(net_mode_c == "host"){
    server.begin();
    server.setNoDelay(true);
    //Serial.print("Use 'telnet ");
    //Serial.print(WiFi.localIP());
    //Serial.println(" 23' to connect");
  }
  else{
    if (!client.connect(host, 23)) {
    Serial.println("connection failed");
    return;
    }
  }
}
////////////////////////////////////////////////////////////
void check_clients(){
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        //Serial.print("New client: "); Serial.print(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
}
////////////////////////////////////////////////////////////
void check_server(){
  //if (!client.connected()) client.connect(host, 23);
  Serial.println(client.connected());
}
////////////////////////////////////////////////////////////
void get_client_data(){
  uint8_t i;
  //check clients for data
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){
        //get data from the telnet client and push it to the UART
        espmsg = "";
        while(serverClients[i].available()) {
          //Serial.write(serverClients[i].read());
          espchar = (serverClients[i].read());
          espmsg += char(espchar);
        }
        cmd_msg = espmsg;
        Serial.print(i);Serial.print(":");Serial.print(cmd_msg);
        cmd_msg.trim();
        command_lookup(cmd_msg);
        Serial.print("Ready:>");
      }
    }
  }
}
////////////////////////////////////////////////////////////
void get_server_data(){
  if(net_mode_c == "node"){
    //if(!client.connected()) client.connect(host, 23);  //this chokes the main loop
    if(client.available()>0){
      srvchar = client.read();
      srvmsg += char(srvchar);
      if (srvchar == 13 || srvchar == 10)
      {
        srvmsg.trim();
        Serial.println(srvmsg);
        command_lookup(srvmsg); 
        Serial.print("Ready:>");
        srvmsg = "";
      }
    }
  }
}
////////////////////////////////////////////////////////////
void connect_telnet(){
  if (!client.connect(host, 23)) {
    Serial.println("connection failed");
    return;
  }
}
/////////////////////////////////////////////////////////////
void con(){
  Serial.println(client.connect(host, 23));
}
/////////////////////////////////////////////////////////////
void cp(String msg){
  client.print(msg);
}
/////////////////////////////////////////////////////////////
void list_clients(){
  int i = 0;
  Serial.println();
  while(i<MAX_SRV_CLIENTS){
    if(serverClients[i].connected()){
      Serial.print(i);Serial.print(" : ");Serial.println(serverClients[i].remoteIP());
    }
    i++;
  }
}
/////////////////////////////////////////////////////////////
void sp(String msg){
  int i;
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      serverClients[i].println(msg);
      delay(1);
    }
  }  
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ping(){
  int i=0;
  
  if(net_mode_c == "node") {
    client.println("ping");
    while(client.available()<1){
      delay(1);
      i++;
    }
  }
  
  if(net_mode_c == "host") {
    serverClients[0].println("ping");
    while(serverClients[0].available()<1){
      delay(1);
      i++;
    }
  }
  
  Serial.print("ping time = "); Serial.println(i);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void pong(){
  if(net_mode_c == "node") {delay(1); client.println("pong");}
  if(net_mode_c == "host") {delay(1); serverClients[0].println("pong");}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
