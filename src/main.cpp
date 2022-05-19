#include <Arduino.h>
#define ledPin 9
#define adc1 A0
#define adc2 A1
#define adc3 A2
#define trans1 13
#define trans2 5
#define trans3 10

bool isActive = false;
bool isDataSend = false;

double voltage1;
double k = 5.08/1024.0; //охуевший коэффициент

unsigned short voltageBuffer[3] = {0, 0, 0};
float convertVoltage[3] = {0.0, 0.0, 0.0};
uint8_t analogPins[3] = {A0, A1, A2};

void setup() 
{
  Serial.begin(115200);
  
  //led отображения процесса
  pinMode(ledPin, OUTPUT); //show start process

  //инициализация АЦП
  pinMode(adc1, INPUT);
  pinMode(adc2, INPUT);
  pinMode(adc3, INPUT);

  //пины управления транзисторами
  pinMode(trans1, OUTPUT);
  pinMode(trans2, OUTPUT);
  pinMode(trans3, OUTPUT);

}

void loop() 
{
  unsigned long startTime = millis();
  unsigned long ledDelayer = 0;
  while((millis() - startTime) < 30000) //3600000
  {
    if(!isActive)
    {
      analogWrite(trans1, 55);
      analogWrite(trans2, 55);
      analogWrite(trans3, 55);
      isActive = true;
    }
    ledDelayer = millis();
    while((millis() - ledDelayer) < 2000)
    {
      if((millis() - ledDelayer) < 1000) 
      {
        digitalWrite(ledPin, HIGH);
        for(uint8_t i = 0; i < 3; i++)
        {
          voltageBuffer[i] = analogRead(analogPins[i]);
          convertVoltage[i] = voltageBuffer[i] * k;
        }
        if (!isDataSend)
        {
          for(uint8_t i = 0; i < 3; i++)
          {
            Serial.print(convertVoltage[i], 3);
            Serial.print('\t');

          }
          isDataSend = true;
        }
      }
      else digitalWrite(ledPin, LOW);
      
    }
    isDataSend = false;
    Serial.println();
  }
  analogWrite(trans1, 255);
  analogWrite(trans2, 255);
  analogWrite(trans3, 255);
  isActive = false;
  Serial.println();
  //открываем транзистор -> мерим напругу->выводим напругу
  
  delay(10000);
  
  // put your main code here, to run repeatedly:
}