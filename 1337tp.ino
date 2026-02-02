 /*
 * 
 *
 * 
 *
 * Av Ben Karlsson TE24A
 */

#include <Servo.h>
// Skapar servos
Servo myservo;  
Servo catapultPos;
Servo catapultFire;

// Initierar variabler
int pos = 0;
int i = 0;
int aim = 0;
const int trigPin = 6;
const int echoPin = 7;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif    // variable to store the servo position

// Neopixel värden
#define PIN        9
#define NUMPIXELS 24
#define DELAYVAL 500

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

float duration, distance;

void setup() {
  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Fixa servos + starta seriel monitorn
  myservo.attach(12);
  catapultPos.attach(11);
  catapultFire.attach(10);
  Serial.begin(9600);

  // Konstig ultrasonic grej, tror inte den behövs egentligen
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  // Starta Neopixel
  pixels.begin();
}

void loop() {

  // Sätter katapulten i utgångläge
  catapultFire.write(0);
  
  for (pos = 0; pos <= 180; pos += 15) { // Går från 0 grader till 180 grader i steg om 15 grader
  myservo.write(pos);

  // Samlar värden och räknar avståd med ultrasonic sensorn
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  int distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);

  // Räknar ut vilken pixel som motsvarar riktningen på sensorn, lagrar i, i
  i = 180 / 15 - pos / 15;
  if (i < 1){
    i = 0;
  }
  if (distance < 25) { // Om sensorn hittr något som är närmre än 25 cm
    // Sätter pixeln på Neopixel ringen som korresponderar med nuvarande riktning på sensorn till RÖD
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.show();

    // Sätter siktet på aktuella positionen 
    aim = pos;
    catapultPos.write(aim);
    
    // Avfyrar katapulten
    delay(2000);
    catapultFire.write(100);
    delay(1000);
    catapultFire.write(0);
  } else {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Sätter pixeln på Neopixel ringen som korresponderar med nuvarande riktning på sensorn till GRÖN
    pixels.show();
  }
  delay(50);
  }
  
  for (pos = 180; pos >= 0; pos -= 15) { // Går från 180 till 0 grader i steg om 15 grader
  myservo.write(pos);

  // Samlar värden och räknar avståd med ultrasonic sensorn
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  int distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);

  // Räknar ut vilken pixel som motsvarar riktningen på sensorn, lagrar i, i
  i = 180 / 15 - pos / 15;
  if (i < 1){
    i = 0;
  }
  
  if (distance < 25) { // Om sensorn hittr något som är närmre än 25 cm
    // Sätter pixeln på Neopixel ringen som korresponderar med nuvarande riktning på sensorn till RÖD
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.show();

    // Sätter siktet på aktuella positionen 
    aim = pos;
    catapultPos.write(aim);

    // Avfyrar katapulten
    delay(2000);
    catapultFire.write(100);
    
    delay(1000);
    catapultFire.write(0);
  } else {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Sätter pixeln på Neopixel ringen som korresponderar med nuvarande riktning på sensorn till GRÖN
    pixels.show();
  }
  delay(50);
  }
}