#include "SIM900.h"
#include <SoftwareSerial.h>
//If not used, is better to exclude the HTTP library,
//for RAM saving.
//If your sketch reboots itself proprably you have finished,
//your memory available.
//#include "inetGSM.h"

//If you want to use the Arduino functions to manage SMS, uncomment the lines below.
#include "sms.h"
SMSGSM sms;
//sending details
char* no = "254704168140";
bool Recv;
//Receiving details
int numdata;
boolean started=false;
char msg[160];
char n[20];

#include<DHT.h>
DHT DHT;
static char Tdhtbuffer[10];
static char Hdhtbuffer[10];

//for LM35 temp sensor.
int lm35 = 0;
static char Tlmbuffer[10];
String data =""; // 4 holding data 4rm lm35 & Dht sensors.

int Delay = 3600000;


void setup()
{
     //Serial connection.
     Serial.begin(9600);
     DHT.setup(7);
     Serial.println("GSM Shield testing.");
     //Start configuration of shield with baudrate.
     //For http uses is raccomanded to use 4800 or slower.
     if (gsm.begin(2400)) {
          Serial.println("\nstatus=READY");
          started=true;
     } else Serial.println("\nstatus=IDLE");

};

void loop()
{
  //taking readings from th dht sensor.
  float Tdht = DHT.getTemperature();
  float Hdht = DHT.getHumidity();
  //taking readings from LM35.
  lm35 = analogRead(A0);
  float volts = (lm35/1024.0)*5.0;
  float Tlm =  volts* 100.0; // converting voltage to actuall
                             //reading of temp celcius
    //Converting all float readings to String
    dtostrf(Tdht, 5, 2, Tdhtbuffer);
    dtostrf(Hdht, 5, 2, Hdhtbuffer);

    dtostrf(Tlm, 5, 2, Tlmbuffer); //for lm35.

    //Putting all the data into one String.
    data = "DHT Temp: ";
    data += Tdht;
    data +=" \"C.""\n";

    data +=" DHT Humid: ";
    data +=Hdht;
    data +=" %%""\n";

    data +="LM35 temp: ";
    data += Tlm;
    data += " \"C.";

    //Converting String to Char for Sending.
    //Length (with one extra char for Null terminator).
    int str_len= data.length() + 1;
    //prepare the char array(the buffer)
    char sending_data[str_len];
    //copy it over.
    data.toCharArray(sending_data, str_len);
    // end of convertin str to char.
     Serial.println(data);

     if(started) {
          //Read if there are messages on SIM card and print them.
          gsm.readSMS(msg, 160, n, 20); 
          Recv = gsm.readSMS(msg, 160, n, 20);
          if(Recv = true){
           if(n == no){
            Serial.println(n);
           }
           if(msg == "A"){
                  sms.SendSMS(no, sending_data);
                  Serial.println(msg);
                  sms.DeleteSMS(1);
            }
          
           
    /* if(started) {
          //Enable this two lines if you want to send an SMS.
          if (sms.SendSMS(no, sending_data))
          Serial.println("\nSMS sent OK");
          sms.DeleteSMS(1);
     }*/

    
          delay(5000);
     }
}
}
