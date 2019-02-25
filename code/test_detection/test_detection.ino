
/*
   Un simple sketch permettant de lire la valeur du capteur IR sur
   le terminal série ou le traceur série de l'IDE Arduino.

   Renvoie "1" lorsque qu'une contraction est détectée.
   Renvoie "0" sinon.

   Un premier lissage par moyenne glissante est effectué sur
   le signal brut.

   Le signal est ensuite dérivé et lissé à nouveau.

   Enfin, le signal est de nouveau dérivé.

   Finalement, lorsque cette dérivée seconde du signal brut dépasse
   un seuil défini, une information est envoyée sur la liaison série.
*/

// seuil de détection
#define seuil 750

// Broche sur laquelle est connectée la led du bracelet
#define led 13

// Le capteur IR est branché sur le port analogique
#define analogPin A0

// Nombre de valeurs utilisées pour le lissage du signal brut
#define signal_longueur 8 // max 32 !!!
int signal_brut[signal_longueur];
int signal_lisse_precedent = 0;
int signal_lisse_actuel = 0;

// Nombre de valeurs utilisées pour le lissage de la dérivée
#define derivee_longueur 8 // max 32 !!!
float derivee[derivee_longueur];
float derivee_lisse_actuelle = 0;
float derivee_lisse_precedente = 0;

void setup() {
  // initialisation de la led
  pinMode(led, OUTPUT);

  // activation de la liaison série
  Serial.begin(115200);
  Serial1.begin(115200);

  // on initialise les tableaux
  for (int i = 0; i < signal_longueur; i++) {
    signal_brut[i] = 0;
  }

  for (int i = 0; i < derivee_longueur; i++) {
    derivee[i] = 0;
  }
}

void loop() {
  // on décale les valeurs précédentes
  for (int i = 0; i < (signal_longueur - 1); i++) {
    signal_brut[i] = signal_brut[i + 1];
  }

  signal_brut[signal_longueur - 1] = analogRead(analogPin);

  signal_lisse_precedent = signal_lisse_actuel;

  signal_lisse_actuel = 0;
  for (int i = 0; i < signal_longueur; i++) {
    signal_lisse_actuel += signal_brut[i];
  }
  signal_lisse_actuel = signal_lisse_actuel / signal_longueur;

  // on décale les valeurs précédentes
  for (int i = 0; i < (derivee_longueur - 1); i++) {
    derivee[i] = derivee[i + 1];
  }

  derivee[derivee_longueur - 1] = signal_lisse_actuel - signal_lisse_precedent;

  derivee_lisse_precedente = derivee_lisse_actuelle;

  // on calcule la moyenne
  derivee_lisse_actuelle = 0;
  for (int i = 0; i < derivee_longueur; i++) {
    derivee_lisse_actuelle += derivee[i];
  }
  derivee_lisse_actuelle = derivee_lisse_actuelle / derivee_longueur;

  int sortie = max(0, int(200 * (derivee_lisse_actuelle - derivee_lisse_precedente)));

  // contraction détectée ?
  if (sortie >= seuil) {
    digitalWrite(led, HIGH);
    Serial.println("1");
  }
  else {
    digitalWrite(led, LOW);
    Serial.println("0");
  }

  // on temporise un peu
  delay(20);
}
