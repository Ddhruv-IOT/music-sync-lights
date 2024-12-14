// const int soundPin = A0;  // Pin connected to sound sensor
// int soundValue;

// void setup() {
//   Serial.begin(9600);  // Initialize serial communication
//   pinMode(soundPin, INPUT);
// }

// void loop() {
//   soundValue = analogRead(soundPin);  // Read sound intensity
//   if (soundValue <= 100) {
//     Serial.println(0);  
//   }
//   else{
//     Serial.println(soundValue);
//     }         // Send data to serial monitor
//   delay(10);  // Adjust delay for smoother readings
// }


// const int soundPin = A0;
// void setup() {
//   Serial.begin(115200);  // Use higher baud rate for faster data transfer
//   pinMode(soundPin, INPUT);
// }

// void loop() {
//   int value = analogRead(soundPin);  // Read audio signal
//   Serial.println(value);  // Send raw data
//   delayMicroseconds(100);  // Fine-tune based on sample rate needed
// }

#include <FastLED.h>
#define LED_PIN     3
#define NUM_LEDS    61
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
// #define COLOR_ORDER GRB
 
CRGB leds[NUM_LEDS];
 
#define AUDIO_PIN   A0  // Pin where the audio signal is connected
 
void setup() {
  // Initialize FastLED
  FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
 
  // Initialize serial communication
  Serial.begin(9600);
}
 
void loop() {
  // Read audio signal from analog pin
  int audioValue = analogRead(AUDIO_PIN);
  if (audioValue <= 80){
    Serial.println(0);
  }
  else{
  Serial.println(audioValue);
  }
  
 
  // Smooth out the audio value to reduce flickering
  static int smoothedValue = 0;
  smoothedValue = (smoothedValue * 0.3) + (audioValue * 1);
 
  // Map the audio value to the number of LEDs
  int numLedsToLight = map(smoothedValue, 0, 1023, 0, NUM_LEDS);
  numLedsToLight  = (numLedsToLight <= 5) ? 0 : numLedsToLight; // full off if no sound
  Serial.println(numLedsToLight);
 
  // Create a moving rainbow effect
  static uint8_t hue = 0;
  hue += 1;  // Adjust this value to change the speed of the rainbow effect
 
  // Set the LEDs based on the audio signal and apply the rainbow effect
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < numLedsToLight) {
      leds[i] = CHSV((hue + (i * 10)) % 255, 255, 255); // Rainbow effect
    } else {
      leds[i] = CRGB::Black; // Turn off the remaining LEDs
    }
  }
 
  // Update the LEDs
  FastLED.show();
  
  // Add a small delay to smooth the visualizer effect
  delay(10);
  
}