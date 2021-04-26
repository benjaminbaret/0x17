#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


/*Variables de connexion a configurer*/

const int LED = 13;
const char* ssid = "SFR_B320";
const char* password = "ibfabjaphyeccamtios7";
const char* mqttServer = "35.180.248.89";
const int mqttPort = 1883;
const char* mqttUser = "0x17";
const char* mqttPassword = "WP06qS";




#define moteurGaucheVitesse 5
#define moteurGaucheDirection 0
#define capteurMoteurGauche 15

#define moteurDroitVitesse 4
#define moteurDroitDirection 2
#define capteurMoteurDroit 13



SimpleTimer timer;


/*Declaration de la fonction de callback*/
void callback(char*topic, byte*payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(espClient);


volatile unsigned int compteurTicksMoteurGauche;
volatile unsigned int compteurTicksMoteurDroit;


void ICACHE_RAM_ATTR ajoutTicksMG ();
void ICACHE_RAM_ATTR ajoutTicksMD ();


volatile long distanceDemandee = 0;
int tickCm = 1;

int degreParTick = 16;
int objectifDistance = 0;
int objectifTicks = 0;






void setup()
{
  Serial.begin(115200);

  pinMode(moteurGaucheVitesse, OUTPUT);
  pinMode(moteurGaucheDirection, OUTPUT);
  pinMode(moteurDroitVitesse, OUTPUT);
  pinMode(moteurDroitDirection, OUTPUT);


  pinMode(capteurMoteurGauche, INPUT);
  pinMode(capteurMoteurDroit, INPUT);

  digitalWrite(moteurGaucheVitesse, LOW);
  digitalWrite(moteurGaucheDirection, HIGH);
  digitalWrite(moteurDroitVitesse, LOW);
  digitalWrite(moteurDroitDirection, HIGH);

  attachInterrupt(digitalPinToInterrupt(capteurMoteurGauche), ajoutTicksMG, RISING);
  attachInterrupt(digitalPinToInterrupt(capteurMoteurDroit), ajoutTicksMD, RISING);


  /*Connexion au WiFi*/

  Serial.print("Connecting to the Newtork");

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

  /*NB: Les topics fournis ici sont des topics de test. Veillez a bien lesremplacer par ceux auxquels votre equipe a acces.*/

  //Serial.println("Publishing");
  //client.publish("0x17", "hello");
  client.subscribe("0x17/vitesseDemandee");



}


void ajoutTicksMG() {
  compteurTicksMoteurGauche++;
}

void ajoutTicksMD() {
  compteurTicksMoteurDroit++;
}



void loop()
{

  client.loop();


}










void callback(char*topic, byte*payload, unsigned int length) {


  if (topic = "0x17/vitesseDemandee") {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic); Serial.print("Message:");

    int distanceDemandee = atoi((char *)payload);

    Serial.print(distanceDemandee);

    if (distanceDemandee > 0) {
      digitalWrite(moteurGaucheDirection, LOW);
      digitalWrite(moteurDroitDirection, LOW);
      while (compteurTicksMoteurGauche != distanceDemandee && compteurTicksMoteurDroit != distanceDemandee) {
        analogWrite(moteurGaucheVitesse, 1000);
        analogWrite(moteurDroitVitesse, 1000);
        Serial.print("Ticks Roue : ");
        Serial.println(compteurTicksMoteurGauche);
        Serial.print("Ticks Roue : ");
        Serial.println(compteurTicksMoteurDroit);
      }
    }

    else if (distanceDemandee < 0) {
      digitalWrite(moteurGaucheDirection, HIGH);
      digitalWrite(moteurDroitDirection, HIGH);
      while (-compteurTicksMoteurGauche != distanceDemandee && -compteurTicksMoteurDroit != distanceDemandee) {
        analogWrite(moteurGaucheVitesse, 1000);
        analogWrite(moteurDroitVitesse, 1000);
        Serial.print("Ticks Roue : ");
        Serial.println(compteurTicksMoteurGauche);
        Serial.print("Ticks Roue : ");
        Serial.println(compteurTicksMoteurDroit);
      }
    }


    compteurTicksMoteurGauche = 0;
    //analogWrite(moteurGaucheVitesse, 0);
    compteurTicksMoteurDroit = 0;
    //analogWrite(moteurDroitVitesse, 0);



    delay(1);



    Serial.println(); Serial.println("-----------------------");

  }




}
