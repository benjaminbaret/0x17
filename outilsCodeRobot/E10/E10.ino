#include <SimpleTimer.h>



#define moteurDroitVitesse 5
#define moteurDroitDirection 0
#define capteurMoteurDroit 15

SimpleTimer timer;



volatile unsigned int compteurTicksMoteurDroit;
volatile unsigned int compteurTicksMoteurDroitVitesse = 0;



void ICACHE_RAM_ATTR ajoutTicksMD ();

volatile long distanceDemandee = 0;
int tickCm = 1;

int degreParTick = 16;
int objectifDistance = 0;
int objectifTicks = 0;

volatile  int angleParcouru = 0;
volatile double toursParSeconde =0;






void setup()
{


  Serial.begin(115200);

  pinMode(moteurDroitVitesse, OUTPUT);
  pinMode(moteurDroitDirection, OUTPUT);


  pinMode(capteurMoteurDroit, INPUT);





  digitalWrite(moteurDroitVitesse, LOW);
  digitalWrite(moteurDroitDirection, HIGH);

    timer.setInterval(1000, compteur);

  attachInterrupt(digitalPinToInterrupt(capteurMoteurDroit), ajoutTicksMD, RISING);


}


void ajoutTicksMD() {
  compteurTicksMoteurDroit++;
  compteurTicksMoteurDroitVitesse++;
}



void loop()
{
  timer.run();
  int var = 0;
  if(distanceDemandee == compteurTicksMoteurDroit){
      distanceDemandee = 0;
      compteurTicksMoteurDroit = 0;
  }

  if (Serial.available()) {
    distanceDemandee = Serial.parseInt();
  }



  if (distanceDemandee > 0) {
    if ((compteurTicksMoteurDroit <= distanceDemandee) && (distanceDemandee != 0)) {

       var = 1000;
       

      Serial.print("Vitesse moteur : ");
  Serial.print(toursParSeconde);

  Serial.print("    / Angle parcouru (en deg): ");
  Serial.println(angleParcouru);
      
    }
    
  }
  else {
            var = 0;

    }

     digitalWrite(moteurDroitDirection, LOW);
      analogWrite(moteurDroitVitesse, var);

  



  delay(1);



}



void compteur(){
  const double tick1 = compteurTicksMoteurDroitVitesse;
  compteurTicksMoteurDroitVitesse = 0; 
  toursParSeconde = tick1/ 22;

  angleParcouru = compteurTicksMoteurDroit*16;
  
  
  
}
