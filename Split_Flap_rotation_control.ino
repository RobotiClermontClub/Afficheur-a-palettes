#include "Stepper.h"
  
int phase1 = 3;
int phase2 = 4;
int phase3 = 5;
int phase4 = 6;

int hallSensor = 2;
int interruptOn;

int stepsPerRevolution = 2048;
// 64 pas = 2048 micro-pas réels
// 2048 / 11 emplacements = 186.1818 micro-pas par emplacement = steps a effectuer
// 372.36 / 64 = 2.905 tours stepper par empl.
int gearbox = 64;
int motorSpeed = 8; // en RPM
int stepCount = 0;

// Liste des caractères à display
char caracteres[] = {
    // Lettres majuscules 0-25
    '\n','\r',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',

    // Chiffres 26-35
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

    // Caractères spéciaux usuels
    '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', 
    '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', 
    '_', '`', '{', '|', '}', '~'
};

int stepsPerLetters[] = {0,0,186,186*2,186*3,186*4,186*5,186*6,186*7,186*8,186*9};

int size = sizeof(caracteres) / sizeof(caracteres[0]);


// taille de la liste caracteres


// Fonction pour contruire la chaine de position de chaque lettre
// Steps_Per_Revolution / size => donne nombre de steps entre chaque lettre

// Création de l'objet "moteur" de type Stepper
Stepper moteur = Stepper(stepsPerRevolution, phase1, phase2, phase3, phase4);

char ordre[64];

void setup() {
  // put your setup code here, to run once:
  pinMode(phase1, OUTPUT);
  pinMode(phase2, OUTPUT);
  pinMode(phase3, OUTPUT);
  pinMode(phase4, OUTPUT);
  
  // définition de la vitesse de rotation du moteur pas à pas 
  moteur.setSpeed(motorSpeed); //Vitesse max testé 1000 => 7.5s par tour

  // Sensor
  pinMode(hallSensor, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(hallSensor), interruptHandler, FALLING);

  //Communication
  Serial.begin(9600);

  homeSequence();

}

void loop() {
  
  // ContinuousRotation();

  //moteur.step(2048);
  //delay(1000);

  // Rearmemant interruption

  /*
  Communication IHM
  */
  char c;
  if (Serial.available() > 0) {
    // Lire le caractère envoyé par l'utilisateur
    c = Serial.read();
    if (c != '\n' || c != '\n') {
      // Afficher le caractère reçu dans la console
      Serial.print("Vous avez entré : ");
      Serial.println(c);

      ToLetter(toSteps(c));
      //moteur.step(1);
    }
  }

  /*
  Interruption
  */


}

void ContinuousRotation() {
  // Rotation continue jusqu'a reset de la carte Arduino
  while (true) {
  }
}

int toSteps(char lettre) {
  for (int i = 0; i < size; i++) {
    if (caracteres[i] == lettre){
      return stepsPerLetters[i];
      }
  }
  return -1;
}

void ToLetter(int steps) {

  int diff = steps - stepCount;

  if (diff == 0) {
    Serial.println(diff);
    //moteur.step(stepsPerRevolution);
    }
  if (diff > 0) {
    stepCount += diff;
    Serial.println(stepCount);
    Serial.println(diff);
    moteur.step(diff);
    }
  if (diff < 0)  {
    Serial.println(stepCount);
    Serial.println(diff);
    stepCount += diff;
    moteur.step(stepsPerRevolution + diff);
    }
}

void homeSequence() {
  //int etat = digitalRead(hallSensor);
  if (digitalRead(hallSensor) == LOW) {
    moteur.step(500);
  }
  while (digitalRead(hallSensor) != LOW) {
    moteur.step(1);
    Serial.print(stepCount);
  }
  stepCount = 0;
}

/*
INUTILISé


// interrupt on hall effect front
void interruptHandler() {
  Serial.print("AIMANT !!  '\n'");
  detachInterrupt(digitalPinToInterrupt(hallSensor));
}
// set step count to 0 -> position initial


*/