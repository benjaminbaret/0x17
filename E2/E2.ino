#include <SimpleTimer.h>



#define moteurDroitVitesse 4
#define moteurDroitDirection 2
#define capteurMoteurDroit 13

SimpleTimer timer;



volatile unsigned int compteurTicksMoteurDroit;

void ICACHE_RAM_ATTR ajoutTicksMD ();


volatile long vitesseMoteurs = 0;
int periodeEchantillonage = 1000;





void setup()
{
  Serial.begin(115200);

  pinMode(moteurDroitVitesse, OUTPUT);
  pinMode(moteurDroitDirection, OUTPUT);


  pinMode(capteurMoteurDroit, INPUT);


  

  timer.setInterval(periodeEchantillonage, compteur);

  digitalWrite(moteurDroitVitesse, LOW);
  digitalWrite(moteurDroitDirection, HIGH);

  attachInterrupt(digitalPinToInterrupt(capteurMoteurDroit), ajoutTicksMD, RISING);
  

}


void ajoutTicksMD(){
  compteurTicksMoteurDroit++;
}



void loop()
{

  timer.run();
   if(Serial.available()){
     vitesseMoteurs = Serial.parseInt();
   }

 
  digitalWrite(moteurDroitDirection, LOW);
  analogWrite(moteurDroitVitesse, vitesseMoteurs);


  delay(1);



}



void compteur() {
 
  const double tick = compteurTicksMoteurDroit;
  compteurTicksMoteurDroit = 0; 
  const double toursParSeconde = tick/ 22;
  Serial.print("Vitesse moteur : ");
  Serial.println(toursParSeconde);
  
  
}
