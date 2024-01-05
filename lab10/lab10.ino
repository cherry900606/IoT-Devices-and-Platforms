
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-mesh-esp32-esp8266-painlessmesh/
  
  This is a simple example that uses the painlessMesh library: https://github.com/gmag11/painlessMesh/blob/master/examples/basic/basic.ino
*/
// Author: 312552024 林琛琛
// 題目： 兩個以上的 ESP32 實現 mesh 自建網路傳遞超音波測距資料，並顯示兩塊板子測量的訊息

#include "painlessMesh.h"

#define   MESH_PREFIX     "312552024" // Name of the MESH
#define   MESH_PASSWORD   "somethingSneaky"  // Password the mesh password
#define   MESH_PORT       5555 // the TCP port

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;  // Create a painlessMesh object called mesh to handle the mesh network.

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1, TASK_FOREVER, &sendMessage );

// 超音波測距參數
int const trigPin = 14; 
int const echoPin = 13; 
int Duration; 
int Distance; 

// The sendMessage() function sends a message to all nodes in the message network (broadcast).
void sendMessage() {
  // The message contains the “Hi from node: "NodeID" 1” text followed by the board chip ID. You can change it to whatever you like.
  String msg = "Hi there, from node 1:"; 
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg ); // To broadcast a message
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("msg=%s\n", msg.c_str()); // <------------ 將 mesh 網路中其他板子傳送的訊息印出來，也就是距離資料


}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin,OUTPUT); 
  pinMode(echoPin,INPUT);  

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();

  // 自己板子的超音波測距，並印出
  digitalWrite(trigPin,LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin,HIGH); //發射超音波 
  delayMicroseconds(10); 
  digitalWrite(trigPin,LOW); 
  Duration = pulseIn(echoPin,HIGH); //超音波發射到接收的時間 
  Distance = Duration*0.034/2; //計算距離(cm)
  delay(500); //delay 500ms
  Serial.println(Distance); 
}