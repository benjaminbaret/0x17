#include <SimpleTimer.h>



#define moteurDroitVitesse 4
#define moteurDroitDirection 2
#define capteurMoteurDroit 13

#define moteurGaucheVitesse 5
#define moteurGaucheDirection 0
#define capteurMoteurGauche 15

SimpleTimer timer;



volatile unsigned int compteurTicksMoteurGaucheAsservissement;
volatile unsigned int compteurTicksMoteurGauche;
volatile unsigned int compteurTicksMoteurDroit;


void ICACHE_RAM_ATTR ajoutTicksMD ();
void ICACHE_RAM_ATTR ajoutTicksMG ();

volatile double toursParSecondeMoteurDroit;
volatile double toursParSecondeMoteurGauche;
volatile double toursParSecondeMoteurGaucheAsservissement;

volatile double vitesseDemandeeTS = 0;


volatile long nombreTours = 0;
int periodeEchantillonage = 100;
double rapportReducteurMoteur = 48;

float kp=250; //coefficient proportionnel
float ki=2.5; //coefficient intégrateur
float kd=20; //coefficient dérivateur


int correction = 0;
double sommeErreur = 0;
double erreurPrecedente = 0;

volatile int nbTicks = 0;
volatile int toursEffectues = 0;

volatile int var = 0;





void setup()
{
  Serial.begin(115200);

  pinMode(moteurDroitVitesse, OUTPUT);
  pinMode(moteurDroitDirection, OUTPUT);
  pinMode(moteurGaucheVitesse, OUTPUT);
  pinMode(moteurGaucheDirection, OUTPUT);


  pinMode(capteurMoteurDroit, INPUT);
    pinMode(capteurMoteurGauche, INPUT);



  


  digitalWrite(moteurDroitVitesse, LOW);
  digitalWrite(moteurDroitDirection, LOW);
  digitalWrite(moteurGaucheVitesse, LOW);
  digitalWrite(moteurGaucheDirection, LOW);

 
  attachInterrupt(digitalPinToInterrupt(capteurMoteurDroit), ajoutTicksMD, RISING);
  attachInterrupt(digitalPinToInterrupt(capteurMoteurGauche), ajoutTicksMG, RISING);

  
  

}


void ajoutTicksMD(){
  compteurTicksMoteurDroit++;
}

void ajoutTicksMG(){
  compteurTicksMoteurGauche++;
}




void loop()
{

  timer.run();

  if (nbTicks == compteurTicksMoteurDroit || nbTicks == compteurTicksMoteurGauche){
    nbTicks = 0;
    compteurTicksMoteurDroit = 0;
    compteurTicksMoteurGauche = 0;
  }
  
   if(Serial.available()){
     nombreTours = Serial.parseInt();
     nbTicks = 30*nombreTours;
   }

   Serial.print("nb ticks = ");
   Serial.println(nbTicks);

   Serial.print("compteurTicksMoteurDroit = ");
   Serial.println(compteurTicksMoteurDroit);

   Serial.print("compteurTicksMoteurGauche = ");
   Serial.println(compteurTicksMoteurGauche);



   if (nbTicks > 0){
    if((compteurTicksMoteurDroit <= nbTicks) && (compteurTicksMoteurGauche <= nbTicks)){
      var = 1023;
    }
   }
   else {
    var = 0;
   }

    digitalWrite(moteurDroitDirection, HIGH);
  analogWrite(moteurDroitVitesse, var);

 // digitalWrite(moteurGaucheDirection, HIGH);
 // analogWrite(moteurGaucheVitesse, var);

   
 
 
  


  delay(1);



}
