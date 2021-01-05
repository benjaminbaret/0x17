
//Initialisation entrée et sortie numérique utlisisées

int pinTrig = 2;
int pinEcho = 3;

//Initialisation variable temps et distance

long temps;               //temps de parcours
float distance;           //distance de parcours



void setup() {
 pinMode (pinTrig, OUTPUT);  //OUTPUT=SORTIE : il envoie un signal au capteur
 pinMode(pinEcho, INPUT);    //INPUT=ENTREE : il lit le retour du capteur

 digitalWrite(pinTrig, LOW);

 Serial.begin(9600);
}

void loop() {
  
 digitalWrite(pinTrig, HIGH);
 delayMicroseconds(10);  //activation capteur --> declenchement d'une mesure
 digitalWrite(pinTrig, LOW);

 temps = pulseIn (pinEcho, HIGH);

 if (temps > 25000) {
  Serial.println("Echec de la mesure");
 }
 else{
  temps= temps/2;                       //car temps correspond à l'aller et au retour ie 2 fois la distance qu'on cherche
  distance = (temps * 340)/10000.0);    // formule d = (temps * vitesse son ds air) / 1000.0 car on cherche la distance en cm dcp "/1000" est la coversion 
  Serial.print("Distance:");
  Serial.print(distance);
  Serial.println("cm");
 }

 delay(2000);
}
