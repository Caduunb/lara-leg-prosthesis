/*
Measuring Current Using ACS712 (DC)
*/
const int analogIn = A1;
int mVperAmp = 185; // use 185 for 5A, 100 for 20A Module and 66 for 30A Module
int RawValue= 0;
float meanValue = 0;
int ACSoffset = 2500; //ajustar offset
double mVolts = 0;
double mAmps = 0;

void setup(){ 
 Serial.begin(9600);
}

void loop(){
 
 for(i=0;i<50;i++){ //faz 50 leituras
   RawValue += analogRead(analogIn);
 }
 
  meanValue = RawValue / 50.0;
  mVolts = (RawValue / 1023.0) * 5000; // Gets you mV
  Amps = ((mVolts - ACSoffset) / mVperAmp); //A
 
 
 Serial.print("Raw Value = " ); // shows pre-scaled value 
 Serial.print(RawValue); 
 Serial.print("\t mV = "); // shows the voltage measured 
 Serial.print(mVolts,3); // the '3' after voltage allows you to display 3 digits after decimal point
 Serial.print("\t Amps = "); // shows the voltage measured 
 Serial.println(Amps,3); // the '3' after voltage allows you to display 3 digits after decimal point
 delay(2500); 
 
}
