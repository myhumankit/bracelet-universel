
/*
   Un simple sketch permettant de lire la valeur du capteur IR sur
   le terminal série ou le traceur série de l'IDE Arduino.
*/

// Le capteur IR est branché sur le port analogique
#define analogPin A0

void setup() {
  // activation de la liaison série
  Serial.begin(115200);
}

void loop() {
  // on envoie directement sur la liaison série la valeur brut du capteur
  Serial.println(analogRead(analogPin));
  
  // on temporise un peu
  delay(10);
}
