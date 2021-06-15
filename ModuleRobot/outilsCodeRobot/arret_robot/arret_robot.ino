#include <SimpleTimer.h>



#define moteurGaucheVitesse 5
#define moteurGaucheDirection 0
#define capteurMoteurGauche 15

#define moteurDroitVitesse 4
#define moteurDroitDirection 2
#define capteurMoteurDroit 13



SimpleTimer timer;



volatile unsigned int compteurTicksMoteurGauche;
volatile unsigned int compteurTicksMoteurDroit;


void ICACHE_RAM_ATTR ajoutTicksMG ();
void ICACHE_RAM_ATTR ajoutTicksMD ();


volatile long distanceDemandee = 0;
int tickCm = 1;

int degreParTick = 16;
int objectifDistance = 0;
int objectifTicks = 0;


int trigger  = 12;
int echo = 14;

const unsigned long measureTimeout = 25000UL; 
const float soundSpeed = 340.0 / 1000;






void setup()
{
  Serial.begin(115200);

  pinMode(moteurGaucheVitesse, OUTPUT);
  pinMode(moteurGaucheDirection, OUTPUT);
  pinMode(moteurDroitVitesse, OUTPUT);
  pinMode(moteurDroitDirection, OUTPUT);


  pinMode(capteurMoteurGauche, INPUT);
  pinMode(capteurMoteurDroit, INPUT);





  digitalWrite(moteurGaucheVitesse, LOW);
  digitalWrite(moteurGaucheDirection, HIGH);
  digitalWrite(moteurDroitVitesse, LOW);
  digitalWrite(moteurDroitDirection, HIGH);

  attachInterrupt(digitalPinToInterrupt(capteurMoteurGauche), ajoutTicksMG, RISING);
  attachInterrupt(digitalPinToInterrupt(capteurMoteurDroit), ajoutTicksMD, RISING);


  /* Initialise les broches */
  pinMode(trigger, OUTPUT);
  digitalWrite(trigger, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(echo, INPUT);


}


void ajoutTicksMG() {
  compteurTicksMoteurGauche++;
}

void ajoutTicksMD() {
  compteurTicksMoteurDroit++;
}



void loop()
{
  distanceDemandee = 0;

  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
  long measure = pulseIn(echo, HIGH, measureTimeout);
   
  float distance_mm = measure / 2.0 * soundSpeed;

  timer.run();

 Serial.print(F("Distance: "));
  Serial.print(distance_mm);
  Serial.print(F("mm ("));
  Serial.print(distance_mm / 10.0, 2);
  Serial.print(F("cm, "));
  Serial.print(distance_mm / 1000.0, 2);
  Serial.println(F("m)")); 



  


  if (distance_mm < 200) {
      analogWrite(moteurGaucheVitesse, 0);
      analogWrite(moteurDroitVitesse, 0);
  }

  else {
    digitalWrite(moteurGaucheDirection, LOW);
    digitalWrite(moteurDroitDirection, LOW);
      analogWrite(moteurGaucheVitesse, 1000);
      analogWrite(moteurDroitVitesse, 1000);
      Serial.print("Ticks Roue : ");
      Serial.println(compteurTicksMoteurGauche); 
      Serial.print("Ticks Roue : ");
      Serial.println(compteurTicksMoteurDroit); 
    
  }

 delay(500);


}
