#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


/*Variables de connexion a configurer*/


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


volatile unsigned int compteurTicksMoteurGaucheAsservissement;
volatile unsigned int compteurTicksMoteurGauche;
volatile unsigned int compteurTicksMoteurDroit;

volatile double toursParSecondeMoteurGaucheAsservissement;
volatile double toursParSecondeMoteurDroit;
volatile double toursParSecondeMoteurGauche;

volatile double vitesseDemandeeTS = 0;


volatile long vitesseMoteurs = 1000;
int periodeEchantillonage = 100;
double rapportReducteurMoteur = 48;

float kp=250; //coefficient proportionnel
float ki=2.5; //coefficient intégrateur
float kd=20; //coefficient dérivateur


volatile int correction = 0;
double sommeErreur = 0;
double erreurPrecedente = 0;

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

   timer.setInterval(periodeEchantillonage, asservissement);
   timer.setInterval(1000, compteur);


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



  client.publish("0x17", "hello");
  client.subscribe("0x17/distanceDemandee");



}


void compteur(){
  const double tick1 = compteurTicksMoteurDroit;
  compteurTicksMoteurDroit = 0; 
  toursParSecondeMoteurDroit = (tick1/ 22);

  const double tick2 = compteurTicksMoteurGauche;
  compteurTicksMoteurGauche = 0; 
  toursParSecondeMoteurGauche = (tick2/ 22);

  
}


void ajoutTicksMD(){
  compteurTicksMoteurGaucheAsservissement;
}

void ajoutTicksMG(){
  compteurTicksMoteurGauche++;
}



void loop()
{

  client.loop();
  


}










void callback(char*topic, byte*payload, unsigned int length) {



Serial.print("Message arrived in topic: ");
Serial.println(topic);Serial.print("Message:");
for (int i = 0; i < length; i++) 
{Serial.print((char)payload[i]);}
Serial.println();Serial.println("-----------------------");


}



  /*if (topic = "0x17/distanceDemandee") {

    distanceDemandee = atoi((char *)payload);


    if (distanceDemandee > 0) {
     
      digitalWrite(moteurDroitDirection, LOW);
      while (compteurTicksMoteurGauche != distanceDemandee && compteurTicksMoteurDroit != distanceDemandee) {
          
       analogWrite(moteurGaucheVitesse, correction);
  
       
        analogWrite(moteurDroitVitesse, 1000);

   

      }
    }

    else if (distanceDemandee < 0) {
      
      digitalWrite(moteurDroitDirection, HIGH);
      while (-compteurTicksMoteurGauche != distanceDemandee && -compteurTicksMoteurDroit != distanceDemandee) {
        analogWrite(moteurGaucheVitesse, correction);
  
       
        analogWrite(moteurDroitVitesse, 1000);

      }
    }


    compteurTicksMoteurGauche = 0;
    //analogWrite(moteurGaucheVitesse, 0);
    compteurTicksMoteurDroit = 0;
    //analogWrite(moteurDroitVitesse, 0);



    delay(1);



    Serial.println(); Serial.println("-----------------------");

  }
  }*/


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
 
  
  
}
