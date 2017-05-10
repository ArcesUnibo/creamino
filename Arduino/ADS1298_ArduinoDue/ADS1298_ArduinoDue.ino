// include the libraries:

#include <SPI.h>
#include "ADS1298.h" //find all definition for ADS1298

//Type
ADS1298 ADS1298device;

// global variable
int transmit = 0;
volatile int interrupt_pin2 = 0;
int connect;
byte SampleRate[1];
byte ModeADS[1];
byte ADSNum[1];
byte ChipSelect[1];
byte TriggerWord[1];
byte ADCWord[1];
byte Gain[1];

byte Stimulation[1];
byte Amplitude[2];
byte Frequency[2];
byte DutyCycle[1];
byte ONOFF[1];
byte Synch[1] = {0xF0};

boolean toggle = 0;
String ConnectingWord;

void setup() {

  //prova
  ModeADS[0] = 0x00;
  ADSNum[0] = 0x00;
  
  connect=0;  //Disconnect from a user Interface (Java, C++)  
  attachInterrupt(2, SPIInterrupt, FALLING); //Interrupt on ArduinoDue // 2 menas pin 2
  
  Serial.begin(9600); //USB programming port Init --> for trigger
  Serial.flush();
  
  SerialUSB.begin(256000);  // USB Initialization
  SerialUSB.flush();
  delay(100);

  Serial1.begin(9600);
  Serial1.flush();

  pinMode(13, OUTPUT); // LED pin for debug
  digitalWrite(13, toggle);
  TriggerWord[0] = 0x00;  
  ADCWord[0] = 0x00;
}


//interrupt function IR2
void SPIInterrupt() {
    interrupt_pin2 = 1;
}

void loop() {

  //Arduino Connection and ADC initialization
  while(!connect){
    if(SerialUSB.available()>6){ // Check if data are available on the serial bus
      ConnectingWord = SerialUSB.readString(); //Read data
      if(ConnectingWord.equals("Connect")){ //Check the connecting word
        connect = 1;
        SerialUSB.println("Ready"); // Send the ack word //println-->write
        delay(500);
        
        while(!SerialUSB.available()>3); // Wait the Sample Rate
         
        SerialUSB.readBytes(SampleRate,1);  //Read the sample rate;
        SerialUSB.readBytes(ModeADS,1);  //Read the Mode;
        SerialUSB.readBytes(ADSNum,1);  //Read the number of ADS;
        SerialUSB.readBytes(ChipSelect,1);  //Read the Chip Select;
        SerialUSB.readBytes(Gain,1);  //Read the Chip Select;
        
        delay(100);
  
        ADS1298device.initialize(SampleRate[0],ADSNum[0],ChipSelect[0]); //Initialize the ADS
        delay(100);

        ADS1298device.reINITforMoreADC(SampleRate[0], ModeADS[0]); //Configure the ADS channels
        delay(100); 
 
        ADS1298device.START(); //Start conversion
        delay(100);

         if (Serial.available() > 0) //Flush the Trigger buffer
           Serial.read();

        toggle = !toggle;
        digitalWrite(13, toggle);
      }  
    } 
  }

  //ADC Acquisition
  
  if(interrupt_pin2==1){
    ADS1298device.RDATA();
    ADS1298device.writeADSchannelData(TriggerWord[0], ADCWord[0]);
    interrupt_pin2 = 0;  
    TriggerWord[0] = 0x00;
    ADCWord[0] = 0x00;
  }
  
  //Arduino Connection and ADC initialization
  
  if(connect){
    if(SerialUSB.available()>9){
      ConnectingWord = SerialUSB.readStringUntil('\0');
      
      if(ConnectingWord.equals("Disconnect")){
        toggle = !toggle;
        digitalWrite(13, toggle);
        connect = 0;
        ADS1298device.STOP();
        delay(100);
        SerialUSB.flush();
      }

    //Start Stimulation
    if(ConnectingWord.equals("StartStimu")){
        
      toggle = !toggle;
      digitalWrite(13, toggle);
      
      ONOFF[0] = Synch[0] | 0x01;
      
      while(!(SerialUSB.available()>5)); // Wait the Sample Rate
      
      SerialUSB.readBytes(Stimulation,1);
      SerialUSB.readBytes(Amplitude,2);
      SerialUSB.readBytes(Frequency,2);
      SerialUSB.readBytes(DutyCycle,1);

      Serial1.write(Synch,1);
      Serial1.write(ONOFF,1);  
      Serial1.write((byte*)Stimulation,1);
      Serial1.write((byte*)Amplitude,2);
      Serial1.write((byte*)Frequency,2);
      Serial1.write((byte*)DutyCycle,1);
      
      }

    //Stop Stimulation
    if(ConnectingWord.equals("StopStimul")){

      toggle = !toggle;
      digitalWrite(13, toggle);
      
      ONOFF[0] = Synch[0] | 0x00;
      Serial1.write(Synch,1);
      Serial1.write(ONOFF,1);
      Serial1.write((byte*)Stimulation,1);
      Serial1.write((byte*)Amplitude,2);
      Serial1.write((byte*)Frequency,2);
      Serial1.write((byte*)DutyCycle,1);        
    }

      
    }
    
    if(Serial.available()){
      Serial.readBytes(TriggerWord,1);
      Serial.println(TriggerWord[0]);
    }

    if(Serial1.available()){
      Serial1.readBytes(ADCWord,1);
      //Serial1.println(TriggerWord[0]);
    }

    
  }
  
}



