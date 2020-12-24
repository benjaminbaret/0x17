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


volatile long vitesseMoteurs = 0;
int periodeEchantillonage = 100;
double rapportReducteurMoteur = 48;

float kp=250; //coefficient proportionnel
float ki=2.5; //coefficient intégrateur
float kd=20; //coefficient dérivateur


int correction = 0;
double sommeErreur = 0;
double erreurPrecedente = 0;





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
  digitalWrite(moteurDroitDirection, LOW);
  digitalWrite(moteurGaucheVitesse, LOW);
  digitalWrite(moteurGaucheDirection, LOW);

 
  attachInterrupt(digitalPinToInterrupt(capteurMoteurDroit), ajoutTicksMD, RISING);
  attachInterrupt(digitalPinToInterrupt(capteurMoteurGauche), ajoutTicksMG, RISING);

  
  

}


void ajoutTicksMD(){
  compteurTicksMoteurGaucheAsservissement;
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



  double ticks = compteurTicksMoteurGaucheAsservissement;
  compteurTicksMoteurGaucheAsservissement = 0;

   double frequenceRoueCodeuse = periodeEchantillonage*ticks;
    toursParSecondeMoteurGaucheAsservissement = frequenceRoueCodeuse / 22 / rapportReducteurMoteur;
    double erreur = vitesseMoteurs - toursParSecondeMoteurGaucheAsservissement;

    sommeErreur += erreur;
    double differenceErreur = erreur - erreurPrecedente;
    erreurPrecedente = erreur;



    //correction = kp*erreur + ki*sommeErreur + kd*differenceErreur;
    correction = kp*erreur;


    


  
 

  digitalWrite(moteurGaucheDirection, HIGH);
  analogWrite(moteurGaucheVitesse, correction);
  
     
  
  
}
