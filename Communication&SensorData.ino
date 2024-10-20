#include <Servo.h> 

#define Trig 7
#define Echo 6
#define Ster_Serwo 3
 
Servo Serwo; 
int Pozycja = 0;
int Zmiana = 5;

void Ramka()
{
    Serial.print("#");
    Serial.print(",");
    if(Pozycja == 0 || Pozycja == 5)
    {
      Serial.print(0);
      Serial.print(Pozycja);
    }
    else
    {
      Serial.print(Pozycja);
    }
    Serial.print(",");
    Serial.print(Zmierz_Dystans_cm());
    Serial.print(",");
    Serial.println("!B");
}


int Zmierz_Dystans_cm() {
  long Czas, Odleglosc;
 
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
 
  Czas = pulseIn(Echo, HIGH);
  Odleglosc = Czas / 58;
 
  return Odleglosc;
}
 
void setup() 
{ 
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT); 
  Serwo.attach(Ster_Serwo);
} 
 
void loop() 
{  
  if (Pozycja < 90) { 
    Serwo.write(Pozycja); 
  } else {
    Pozycja = 0;
  }    
  
  
  delay(500);
  Pozycja = Pozycja + Zmiana;
  Ramka(); 
  delay(200);                       
}
