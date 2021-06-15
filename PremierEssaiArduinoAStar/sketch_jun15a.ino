
#include <ESP8266WiFi.h>
#include <SimpleTimer.h>

#include <PubSubClient.h>

#define moteurGaucheVitesse 5
#define moteurGaucheDirection 0
#define capteurMoteurGauche 15
#define moteurDroitVitesse 4
#define moteurDroitDirection 2
#define capteurMoteurDroit 13

SimpleTimer timer;


const char* ssid = "iPhone";
const char* password = "benneben";
const char* mqttServer = "35.180.248.89";
const int mqttPort = 1883;
const char* mqttUser = "0x17";
const char* mqttPassword = "WP06qS";


void callback(char*topic, byte*payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(espClient);

volatile unsigned int compteurTicksMoteurGaucheAsservissement = 0;
volatile unsigned int compteurTicksMoteurGauche = 0;
volatile double toursParSecondeMoteurGauche = 0;
volatile double toursParSecondeMoteurGaucheAsservissement = 0;
volatile  int angleParcouruMoteurGauche = 0;
volatile int distanceParcouruMoteurGauche = 0;

volatile unsigned int compteurTicksMoteurDroitAsservissement = 0;
volatile double toursParSecondeMoteurDroitAsservissement = 0;
volatile unsigned int compteurTicksMoteurDroit = 0;
volatile double toursParSecondeMoteurDroit = 0;
volatile  int angleParcouruMoteurDroit = 0;
volatile int distanceParcouruMoteurDroit = 0;

volatile int degreRotation = 0;

volatile int distanceDemandeeGauche = 0, distanceDemandeeDroit = 0, distanceDemandee = 0;
volatile int degreRotationConverti = 0;
volatile bool changementSensMoteur = false;
volatile int tournerMoteur = 0;

void ICACHE_RAM_ATTR ajoutTicksMG ();
void ICACHE_RAM_ATTR ajoutTicksMD ();

float kp = 250; //coefficient proportionnel
float ki = 2.5; //coefficient intégrateur
float kd = 20; //coefficient dérivateur

volatile long vitesseMoteurs = 1000;
int periodeEchantillonage = 100;
double rapportReducteurMoteur = 48;

int correctionGauche = 0;
double sommeErreurGauche = 0;
double erreurPrecedenteGauche = 0;

int correctionDroit = 0;
double sommeErreurDroit = 0;
double erreurPrecedenteDroit = 0;


int vitesseMoteurGauche = 0, vitesseMoteurDroit = 0;

char x[50] =  "\0";
char y[50] =  "\0";

void setup()
{
  Serial.begin(115200);

  pinMode(moteurGaucheVitesse, OUTPUT);
  pinMode(moteurGaucheDirection, OUTPUT);
  pinMode(capteurMoteurGauche, INPUT);
  pinMode(moteurDroitVitesse, OUTPUT);
  pinMode(moteurDroitDirection, OUTPUT);
  pinMode(capteurMoteurDroit, INPUT);

  timer.setInterval(periodeEchantillonage, asservissement);
  timer.setInterval(periodeEchantillonage, pilotageMoteurs);

  digitalWrite(moteurGaucheVitesse, LOW);
  digitalWrite(moteurGaucheDirection, LOW);
  digitalWrite(moteurDroitVitesse, LOW);
  digitalWrite(moteurDroitDirection, LOW);

  attachInterrupt(digitalPinToInterrupt(capteurMoteurGauche), ajoutTicksMG, RISING);
  attachInterrupt(digitalPinToInterrupt(capteurMoteurDroit), ajoutTicksMD, RISING);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  /*Connexion au serveur MQTT*/
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  /*Veillez a bien verifier les identifiants de connexion*/
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("connected");
    }
    else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  client.subscribe("0x17/robotX");
    client.subscribe("0x17/robotY");

 
}


void loop()
{
  client.loop();

  
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
  

  timer.run();
  delay(1);
}


void asservissement() {

  double ticksGauche = compteurTicksMoteurGaucheAsservissement;
  compteurTicksMoteurGaucheAsservissement = 0;

  /* toursParSecondeMoteurGauche = (ticksGauche / 22);
    angleParcouruMoteurGauche += ticksGauche * 16;
    distanceParcouruMoteurGauche += ticksGauche;*/

  double frequenceRoueCodeuseGauche = periodeEchantillonage * ticksGauche;
  toursParSecondeMoteurGaucheAsservissement = frequenceRoueCodeuseGauche / 22 / rapportReducteurMoteur;
  double erreurGauche = vitesseMoteurs - toursParSecondeMoteurGaucheAsservissement;

  sommeErreurGauche += erreurGauche;
  double differenceErreurGauche = erreurGauche - erreurPrecedenteGauche;
  erreurPrecedenteGauche = erreurGauche;


  correctionGauche = kp * erreurGauche + ki * sommeErreurGauche + kd * differenceErreurGauche;
  //correctionGauche = kp*erreurGauche;


  double ticksDroit = compteurTicksMoteurDroitAsservissement;
  compteurTicksMoteurDroitAsservissement = 0;

  /*toursParSecondeMoteurDroit = (ticksDroit / 22);
    angleParcouruMoteurDroit += ticksDroit * 16;
    distanceParcouruMoteurDroit += ticksDroit ;*/

  double frequenceRoueCodeuseDroit = periodeEchantillonage * ticksDroit;
  toursParSecondeMoteurDroitAsservissement = frequenceRoueCodeuseDroit / 22 / rapportReducteurMoteur;
  double erreurDroit = vitesseMoteurs - toursParSecondeMoteurDroitAsservissement;

  sommeErreurDroit += erreurDroit;
  double differenceErreurDroit = erreurDroit - erreurPrecedenteDroit;
  erreurPrecedenteDroit = erreurDroit;


  correctionDroit = kp * erreurDroit + ki * sommeErreurDroit + kd * differenceErreurDroit;
  //correctionDroit = kp*erreurDroit;
}






// Pilotage avancement robot


void pilotageMoteurs() {


  if (distanceDemandee != 0) {

    if (distanceDemandee < 0) {
      distanceDemandee = -distanceDemandee;
      changementSensMoteur = true;
    }

      if (changementSensMoteur == true) {
      digitalWrite(moteurGaucheDirection, HIGH);
      digitalWrite(moteurDroitDirection, HIGH);
      }
      else {
        digitalWrite(moteurGaucheDirection, LOW);
      digitalWrite(moteurDroitDirection, LOW);
      }


    
    

    if (compteurTicksMoteurGauche < distanceDemandee) {
        
      analogWrite(moteurGaucheVitesse, correctionGauche);
    }
    else {
      changementSensMoteur = false;
      digitalWrite(moteurGaucheDirection, LOW);
      digitalWrite(moteurDroitDirection, LOW);
      analogWrite(moteurGaucheVitesse, 0);
      analogWrite(moteurDroitVitesse, 0);
      compteurTicksMoteurDroit = 0;
      compteurTicksMoteurGauche = 0;
      distanceDemandee = 0;
    }
    if (compteurTicksMoteurDroit < distanceDemandee) {

    
      analogWrite(moteurDroitVitesse, correctionDroit);
    }
    else {
      changementSensMoteur = false;
      digitalWrite(moteurGaucheDirection, LOW);
      digitalWrite(moteurDroitDirection, LOW);
      analogWrite(moteurGaucheVitesse, 0);
      analogWrite(moteurDroitVitesse, 0);
      compteurTicksMoteurDroit = 0;
      compteurTicksMoteurGauche = 0;
      distanceDemandee = 0;
    }
  
  }
  
  
  if (degreRotation != 0) {
    if (degreRotation < 0) {
      degreRotation = -degreRotation;
      tournerMoteur = 1;
    }
    else {
      tournerMoteur = 2;
    }
    if (tournerMoteur == 1) {
      digitalWrite(moteurGaucheDirection, LOW);
      digitalWrite(moteurDroitDirection, HIGH);
    }
    else if (tournerMoteur == 2) {
      digitalWrite(moteurGaucheDirection, HIGH);
      digitalWrite(moteurDroitDirection, LOW);
    }


    
    if (compteurTicksMoteurGauche < degreRotation) {
      analogWrite(moteurGaucheVitesse, correctionGauche);
    }
      else {
      tournerMoteur = 0;
      digitalWrite(moteurGaucheDirection, LOW);
      digitalWrite(moteurDroitDirection, LOW);
      analogWrite(moteurGaucheVitesse, 0);
      analogWrite(moteurDroitVitesse, 0);
      compteurTicksMoteurDroit = 0;
      compteurTicksMoteurGauche = 0;
      degreRotation = 0;
    }

    if (compteurTicksMoteurDroit < degreRotation) {
      analogWrite(moteurDroitVitesse, correctionDroit);
    }
      else {
      tournerMoteur = 0;
      digitalWrite(moteurGaucheDirection, LOW);
      digitalWrite(moteurDroitDirection, LOW);
      analogWrite(moteurGaucheVitesse, 0);
      analogWrite(moteurDroitVitesse, 0);
      compteurTicksMoteurDroit = 0;
      compteurTicksMoteurGauche = 0;
      degreRotation = 0;
    }
  }
  }
  



void callback(char*topic, byte*payload, unsigned int length) {


  if(String(topic) == "0x17/robotX"){
    for (int i = 0; i < length; i++) {
    x[i] = (char)payload[i];
    }
  }

    else if (String(topic) == "0x17/robotY"){
    for (int i = 0; i < length; i++) {
    y[i] = (char)payload[i];
    }

}

   
   

  

  
  delay(1);
}




void ajoutTicksMG() {
  compteurTicksMoteurGauche += 1;
  compteurTicksMoteurGaucheAsservissement += 1;;
}

void ajoutTicksMD() {
  compteurTicksMoteurDroit += 1;
  compteurTicksMoteurDroitAsservissement += 1;
}
