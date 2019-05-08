#include <SoftwareSerial.h>

//Script para configurar modulo BT com arduino uno através do terminal de comando (serial monitor)

SoftwareSerial BTSerial(10, 11); // CONNECT BT RX PIN TO ARDUINO 11 PIN | CONNECT BT TX PIN TO ARDUINO 10 PIN

void setup() 
{
  /*pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode --> nao funciona
  //digitalWrite(9, HIGH);*/
  
  //colocar em AT mode com procedimento de apertar o botao
  
  Serial.begin(38400);
  Serial.println("Enter AT commands:");
  BTSerial.begin(38400);  // HC-05 default speed in AT command more
}

void loop()
{
  //para ativar certos comandos, como 'AT+NAME?', botao deve estar pressionado durante o envio do comando (FULL AT MODE) -->referir à documentacao de configuracao

  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
    BTSerial.write(Serial.read());
}

