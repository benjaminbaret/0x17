#include <SimpleTimer.h>



#define moteurDroitVitesse 5
#define moteurDroitDirection 0
#define capteurMoteurDroit 15

SimpleTimer timer;



volatile unsigned int compteurTicksMoteurDroit;

void ICACHE_RAM_ATTR ajoutTicksMD ();

volatile long distanceDemandee = 0;
int tickCm = 1;

int degreParTick = 16;
int objectifDistance = 0;
int objectifTicks = 0;






void setup()
{
  Serial.begin(115200);

  pinMode(moteurDroitVitesse, OUTPUT);
  pinMode(moteurDroitDirection, OUTPUT);


  pinMode(capteurMoteurDroit, INPUT);





  digitalWrite(moteurDroitVitesse, LOW);
  digitalWrite(moteurDroitDirection, HIGH);

  attachInterrupt(digitalPinToInterrupt(capteurMoteurDroit), ajoutTicksMD, RISING);


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



  digitalWrite(moteurDroitDirection, LOW);
  if (distanceDemandee > 0) {
    while (compteurTicksMoteurDroit != distanceDemandee) {
      analogWrite(moteurDroitVitesse, 1000);
      Serial.print("Ticks Roue : ");
      Serial.println(compteurTicksMoteurDroit); 
    }
  }
  compteurTicksMoteurDroit = 0;
  analogWrite(moteurDroitVitesse, 0);



  delay(1);



}
