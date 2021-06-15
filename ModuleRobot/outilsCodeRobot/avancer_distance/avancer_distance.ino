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

  timer.run();
  if (Serial.available()) {
    distanceDemandee = Serial.parseInt();
  }



  


  if (distanceDemandee > 0) {
    digitalWrite(moteurGaucheDirection, LOW);
    digitalWrite(moteurDroitDirection, LOW);
    while (compteurTicksMoteurGauche != distanceDemandee && compteurTicksMoteurDroit != distanceDemandee) {
      analogWrite(moteurGaucheVitesse, 1000);
      analogWrite(moteurDroitVitesse, 1000);
      Serial.print("Ticks RoueG : ");
      Serial.println(compteurTicksMoteurGauche); 
      Serial.print("Ticks Roue : ");
      Serial.println(compteurTicksMoteurDroit); 
    }
  }

  else if (distanceDemandee < 0) {
    digitalWrite(moteurGaucheDirection, HIGH);
    digitalWrite(moteurDroitDirection, HIGH);
    while (-compteurTicksMoteurGauche != distanceDemandee && -compteurTicksMoteurDroit != distanceDemandee) {
      analogWrite(moteurGaucheVitesse, 1000);
      analogWrite(moteurDroitVitesse, 1000);
      Serial.print("Ticks Roue : ");
      Serial.println(compteurTicksMoteurGauche); 
      Serial.print("Ticks Roue : ");
      Serial.println(compteurTicksMoteurDroit); 
    }
  }
  
  
  compteurTicksMoteurGauche = 0;
  analogWrite(moteurGaucheVitesse, 0);
  compteurTicksMoteurDroit = 0;
  analogWrite(moteurDroitVitesse, 0);



  delay(1);



}
