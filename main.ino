#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(2, 3);
int led = 13;
int led2 = 12;
#define FLOWSENSORPIN 5
int solenoidPin = 11;  

bool lockState = false;
uint32_t state;
int activationCounter = 0;
int lastActivation = 0;
int activationTime = 12 * 1000;


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//start flow sensor code

// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!

SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);
  
  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return; // nothing changed!
  }
  
  if (x == HIGH) {
    //low to high transition!
    pulses++;
  }
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;  // in hertz
  lastflowratetimer = 0;
}



void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

//end flow sensor code


void setup()
{
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(solenoidPin, OUTPUT);    
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
//  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
//  if (finger.verifyPassword()) {
//    Serial.println("Found fingerprint sensor!");
//  } else {
//    Serial.println("Did not find fingerprint sensor :(");
//    while (1) { delay(1); }
//  }

  pinMode(FLOWSENSORPIN, INPUT);
   digitalWrite(FLOWSENSORPIN, HIGH);
   lastflowpinstate = digitalRead(FLOWSENSORPIN);
   useInterrupt(true);

   digitalWrite(led2, HIGH);

}

void loop()                     // run over and over again
{
  int id = getFingerprintIDez();
  float liters = pulses;
  liters /= (7.5*60);  
  liters*=5;  
    
  int storedId;
  if (id != -1 && lockState == false){
    lockState = true;
    state = 1;
    lastActivation = millis();   
    digitalWrite(led, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(solenoidPin, HIGH); 
    storedId = id;
    lastflowratetimer = 0;
    pulses=0;
  }
  
  if ((activationCounter - lastActivation > activationTime) && lockState == true) {
    Serial.print(storedId); Serial.print("\t"); Serial.println(liters);
    digitalWrite(led2, HIGH);
    digitalWrite(led, LOW);
    digitalWrite(solenoidPin, LOW);  
    lastflowratetimer = 0;
    pulses=0;
    lockState = false;
    state = 0;
  }

    

  activationCounter = millis();
  delay(50);             //don't ned to run this at full speed.
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
//  Serial.print("Found ID #"); Serial.print(finger.fingerID);
//  Serial.print(" with confidence of "); Serial.println(finger.confidence);
    
  return finger.fingerID;
}
