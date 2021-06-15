//Librairie
#include <Servo.h>

//Déclaration des constantes
#define UPDATE_TIME 15
#define MAX_POS 180
#define MIN_POS 0

//Déclaration des paramètres
int servoPin = 9;
int pulse = 1500;
char s="1.5";
//Déclaration des variables
Servo myServo;  // création d'un objet Servo
// Sur la plupart des cartes, on peut créer jusqu'à douze objets

int pos=0;    // variable contenant la position du servomoteur

void setup() {
  myServo.attach(servoPin); 
}

void loop() {
  for (pos = MIN_POS; pos <= MAX_POS; pos += 1) { 
    myServo.write(pos);              
    delay(UPDATE_TIME);                       
  }
  for (pos = MAX_POS; pos >= MIN_POS; pos -= 1) { 
    myServo.write(pos);             
    delay(UPDATE_TIME);                      
  }
}
