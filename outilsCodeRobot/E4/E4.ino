#include <SimpleTimer.h>



#define moteurDroitVitesse 4
#define moteurDroitDirection 2
#define capteurMoteurDroit 12

#define moteurGaucheVitesse 5
#define moteurGaucheDirection 0
#define capteurMoteurGauche 15

SimpleTimer timer;



volatile unsigned int compteurTicksMoteurDroit;
volatile unsigned int compteurTicksMoteurGauche;


void ICACHE_RAM_ATTR ajoutTicksMD ();
void ICACHE_RAM_ATTR ajoutTicksMG ();

volatile double toursParSecondeMoteurDroit;
volatile double toursParSecondeMoteurGauche;


volatile long vitesseMoteurs = 0;
int periodeEchantillonage = 10;





void setup()
{
  Serial.begin(115200);

  pinMode(moteurDroitVitesse, OUTPUT);
  pinMode(moteurDroitDirection, OUTPUT);
  pinMode(moteurGaucheVitesse, OUTPUT);
  pinMode(moteurGaucheDirection, OUTPUT);


  pinMode(capteurMoteurDroit, INPUT);
    pinMode(capteurMoteurGauche, INPUT);



  

  timer.setInterval(periodeEchantillonage, asservissement);
  timer.setInterval(1000, compteur);

  digitalWrite(moteurDroitVitesse, LOW);
  digitalWrite(moteurDroitDirection, HIGH);
  digitalWrite(moteurGaucheVitesse, LOW);
  digitalWrite(moteurGaucheDirection, HIGH);

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
   if(Serial.available()){
     vitesseMoteurs = Serial.parseInt();
   }

 
  digitalWrite(moteurDroitDirection, LOW);
  analogWrite(moteurDroitVitesse, vitesseMoteurs);

  Serial.print("Vitesse moteur Droit : ");
  Serial.print(toursParSecondeMoteurDroit);
  Serial.print("   /   Vitesse moteur Gauche : ");
  Serial.println(toursParSecondeMoteurGauche);


  delay(1);



}

void compteur(){
  const double tick1 = compteurTicksMoteurDroit;
  compteurTicksMoteurDroit = 0; 
  toursParSecondeMoteurDroit = (tick1/ 22);

  const double tick2 = compteurTicksMoteurGauche;
  compteurTicksMoteurGauche = 0; 
  toursParSecondeMoteurGauche = (tick2/ 22);
}



void asservissement() {

  digitalWrite(moteurGaucheDirection, LOW);
  analogWrite(moteurGaucheVitesse, vitesseMoteurs);
  
}
