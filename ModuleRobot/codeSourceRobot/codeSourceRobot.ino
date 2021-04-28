#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define moteurGaucheVitesse 5
#define moteurGaucheDirection 0
#define capteurMoteurGauche 15
#define moteurDroitVitesse 4
#define moteurDroitDirection 2
#define capteurMoteurDroit 13


SimpleTimer timer;

const char* ssid = "SFR_B320";
const char* password = "ibfabjaphyeccamtios7";
const char* mqttServer = "35.180.248.89";
const int mqttPort = 1883;
const char* mqttUser = "0x17";
const char* mqttPassword = "WP06qS";

void callback(char*topic, byte*payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(espClient);


volatile unsigned int compteurTicksMoteurGaucheAsservissement;
volatile unsigned int compteurTicksMoteurGauche;
volatile double toursParSecondeMoteurGauche;
volatile double toursParSecondeMoteurGaucheAsservissement;
volatile  int angleParcouruMoteurGauche = 0;


volatile unsigned int compteurTicksMoteurDroitAsservissement;
volatile double toursParSecondeMoteurDroitAsservissement;
volatile unsigned int compteurTicksMoteurDroit;
volatile double toursParSecondeMoteurDroit;
volatile  int angleParcouruMoteurDroit = 0;

volatile int distanceDemandeeGauche = 0, distanceDemandeeDroit = 0, distanceDemandee = 0;


void ICACHE_RAM_ATTR ajoutTicksMG ();
void ICACHE_RAM_ATTR ajoutTicksMD ();




float kp = 250; //coefficient proportionnel
float ki = 2.5; //coefficient intégrateur
float kd = 20; //coefficient dérivateur



volatile long vitesseMoteurs = 0;
int periodeEchantillonage = 100;
double rapportReducteurMoteur = 48;

int correctionGauche = 0;
double sommeErreurGauche = 0;
double erreurPrecedenteGauche = 0;

int correctionDroit = 0;
double sommeErreurDroit = 0;
double erreurPrecedenteDroit = 0;

int vitesseMoteurGauche = 0, vitesseMoteurDroit = 0;




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
  timer.setInterval(1000, compteur);

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

  client.subscribe("0x17/distanceDemandee");

}


void loop()
{

  client.loop();

  timer.run();




  delay(1);



}
void compteur() {
  const double tick2 = compteurTicksMoteurGauche;
  compteurTicksMoteurGauche = 0;
  toursParSecondeMoteurGauche = (tick2 / 22);
  angleParcouruMoteurGauche = compteurTicksMoteurGauche * 16;


  const double tick1 = compteurTicksMoteurDroit;
  compteurTicksMoteurDroit = 0;
  toursParSecondeMoteurDroit = (tick1 / 22);
  angleParcouruMoteurDroit = compteurTicksMoteurDroit * 16;



}





void asservissement() {

//Code en commentaire à ne pas prendre en compte (sera utilisé par la suite)
  /*double ticksGauche = compteurTicksMoteurGaucheAsservissement;
  compteurTicksMoteurGaucheAsservissement = 0;

   double frequenceRoueCodeuseGauche = periodeEchantillonage*ticksGauche;
    toursParSecondeMoteurGaucheAsservissement = frequenceRoueCodeuseGauche / 22 / rapportReducteurMoteur;
    double erreurGauche = vitesseMoteurs - toursParSecondeMoteurGaucheAsservissement;

    sommeErreurGauche += erreurGauche;
    double differenceErreurGauche = erreurGauche - erreurPrecedenteGauche;
    erreurPrecedenteGauche = erreurGauche;


    //correctionGauche = kp*erreur + ki*sommeErreurGauche + kd*differenceErreurGauche;
    correctionGauche = kp*erreurGauche;


    double ticksDroit = compteurTicksMoteurDroitAsservissement;
  compteurTicksMoteurDroitAsservissement = 0;

   double frequenceRoueCodeuseDroit = periodeEchantillonage*ticksDroit;
    toursParSecondeMoteurDroitAsservissement = frequenceRoueCodeuseDroit / 22 / rapportReducteurMoteur;
    double erreurDroit = vitesseMoteurs - toursParSecondeMoteurDroitAsservissement;

    sommeErreurDroit += erreurDroit;
    double differenceErreurDroit = erreurDroit - erreurPrecedenteDroit;
    erreurPrecedenteDroit = erreurDroit;


    //correctionGauche = kp*erreur + kiDroit*sommeErreurDroit + kDroit*differenceErreurDroit;
    correctionDroit = kp*erreurDroit;
*/

  if (distanceDemandeeGauche == compteurTicksMoteurGauche) {
    distanceDemandeeGauche = 0;
    compteurTicksMoteurGauche = 0;
  }

  if (distanceDemandeeDroit == compteurTicksMoteurDroit) {
    distanceDemandeeDroit = 0;
    compteurTicksMoteurDroit = 0;
  }






  if (distanceDemandeeDroit > 0) {
    if ((compteurTicksMoteurDroit <= distanceDemandeeDroit) && (distanceDemandeeDroit != 0)) {

      vitesseMoteurDroit = 1000;
    }

  }
  else {
    vitesseMoteurDroit = 0;

  }

  if (distanceDemandeeGauche > 0) {
    if ((compteurTicksMoteurGauche <= distanceDemandeeGauche) && (distanceDemandeeGauche != 0)) {

      vitesseMoteurGauche = 1000;
    }

  }
  else {
    vitesseMoteurGauche = 0;

  }



  analogWrite(moteurGaucheVitesse, vitesseMoteurGauche);
  analogWrite(moteurDroitVitesse, vitesseMoteurDroit);

}



void callback(char*topic, byte*payload, unsigned int length) {


  if (topic = "0x17/distanceDemandee") {

    distanceDemandee = atoi((char *)payload);
    distanceDemandeeGauche = distanceDemandee;
    distanceDemandeeDroit = distanceDemandee;

    distanceDemandee = 0;


  }








  delay(1);





}




void ajoutTicksMG() {
  compteurTicksMoteurGauche++;
}

void ajoutTicksMD() {
  compteurTicksMoteurGauche++;
}
