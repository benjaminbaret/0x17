#define moteurDroitVitesse 4
#define moteurDroitDirection 2
#define capteurMoteurDroit 13


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


  


  digitalWrite(moteurDroitVitesse, LOW);
  digitalWrite(moteurDroitDirection, HIGH);

  attachInterrupt(digitalPinToInterrupt(capteurMoteurDroit), ajoutTicksMD, RISING);
  

}


void ajoutTicksMD(){
  compteurTicksMoteurDroit++;
}



void loop()
{


   if(Serial.available()){
     vitesseMoteurs = Serial.parseInt();
   }

 
  digitalWrite(moteurDroitDirection, LOW);
  analogWrite(moteurDroitVitesse, vitesseMoteurs);
  Serial.print("Nombre ticks moteur droit : ");
  Serial.println(compteurTicksMoteurDroit);

  delay(1);

}
