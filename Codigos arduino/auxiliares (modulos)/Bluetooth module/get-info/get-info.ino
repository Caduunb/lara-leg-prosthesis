/*asks name and baud rate to establish communication*/

char btname[10];//need length of chars being read +1 for null character
char btuart[15];

void setup() {
  // set baud rate then delay to give user time to open serial monitor
  Serial.begin(38400);
  delay(5000); //delay prior to start
  
  Serial.print("AT+NAME?"); //asks name
  delay(600); //delay to wait for response
  
  int count = 0;
  while(1) { //gets name response
    if(Serial.available()) {
        btname[count] = Serial.read(); //read in char
        count++; 
        if(count == 9) break; //after we get all 9 char break out of loop
    }
    delay(10);
  }
  
 
  Serial.print("AT+UART?"); //asks UART info (baud, stop bit, parity)
  delay(600); 
  count = 0; //intialize count
  while(1) {
    if(Serial.available()) {
        btuart[count] = Serial.read(); 
        count++; 
        if(count == 14) break; 
    }
    delay(10);
  }
  
  //print out each message to make sure it worked
  Serial.println("");
  Serial.println(btname);
  Serial.println(btuart);
}

void loop() {
 //do nothing
  delay(50);
}
