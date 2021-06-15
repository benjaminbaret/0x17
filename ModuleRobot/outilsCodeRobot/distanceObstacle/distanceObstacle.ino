int trigger  = 12;
int echo = 14;

const unsigned long measureTimeout = 25000UL; 
const float soundSpeed = 340.0 / 1000;



void setup() {
  Serial.begin(115200);
   
  /* Initialise les broches */
  pinMode(trigger, OUTPUT);
  digitalWrite(trigger, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(echo, INPUT);
}

void loop() {
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  
  long measure = pulseIn(echo, HIGH, measureTimeout);
   
  float distance_mm = measure / 2.0 * soundSpeed;
   
  Serial.print(F("Distance: "));
  Serial.print(distance_mm);
  Serial.print(F("mm ("));
  Serial.print(distance_mm / 10.0, 2);
  Serial.print(F("cm, "));
  Serial.print(distance_mm / 1000.0, 2);
  Serial.println(F("m)"));
   
  delay(500);

}
