#include <Wire.h>
#include <MIDIUSB.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Encoder.h>

// OLED display TWI address
#define OLED_ADDR   0x3C
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


const int N_BUTTONS = 11; //*  total numbers of buttons
const int BUTTON_ARDUINO_PIN[N_BUTTONS] = {4, 5, 6, 7, 8, 9, 10, 16, 14, 15, 20}; //* pins of each button connected straight to the Arduino

int buttonCState[N_BUTTONS] = {};        // stores the button current value
int buttonPState[N_BUTTONS] = {};        // stores the button previous value

//#define pin13 1 //* uncomment if you are using pin 13 (pin with led), or comment the line if not using
//byte pin13index = 12; //* put the index of the pin 13 of the buttonPin[] array if you are using, if not, comment

// debounce
unsigned long lastDebounceTime[N_BUTTONS] = {0};  // the last time the output pin was toggled
unsigned long debounceDelay = 5;    //* the debounce time; increase if the output flickers

/////////////////////////////////////////////
// POTENTIOMETERS
const int N_POTS = 1; //* total numbers of pots (slide & rotary)
const int POT_ARDUINO_PIN[N_POTS] = {A0}; //* pins of each pot connected straight to the Arduino

int potCState[N_POTS] = {0}; // Current state of the pot
int potPState[N_POTS] = {0}; // Previous state of the pot
int potVar = 0; // Difference between the current and previous state of the pot
int lastCcValue[N_POTS] = {0};

int midiCState[N_POTS] = {0}; // Current state of the midi value
int midiPState[N_POTS] = {0}; // Previous state of the midi value

const int TIMEOUT = 300; //* Amount of time the potentiometer will be read after it exceeds the varThreshold
const int varThreshold = 10; //* Threshold for the potentiometer signal variation
boolean potMoving = true; // If the potentiometer is moving
unsigned long PTime[N_POTS] = {0}; // Previously stored time
unsigned long timer[N_POTS] = {0}; // Stores the time that has elapsed since the timer was reset

// ENCODERS

// You can add as many encoders you want separated in many encoderChannels you want

const int N_ENCODERS = 1; //* number of encoders
const int N_ENCODER_CHANNELS = 1; //* number of encoderChannels
const int N_ENCODER_PINS = N_ENCODERS * 2; //number of pins used by the encoders

Encoder encoder = {19, 21}; // the two pins of each encoder (backwards) -  Use pins with Interrupts!

int encoderMinVal = 0; //* encoder minimum value
int encoderMaxVal = 127; //* encoder maximum value

int preset[N_ENCODER_CHANNELS][N_ENCODERS] = { //stores presets to start your encoders
  {64},
};

int lastEncoderValue[N_ENCODER_CHANNELS][N_ENCODERS] = {127};
int encoderValue[N_ENCODER_CHANNELS][N_ENCODERS] = {127};

/////////////////////////////////////////////
// MIDI
byte midiCh = 1; //* MIDI channel to be used
byte note = 36; //* Lowest note to be used
byte cc = 1; //* Lowest MIDI CC to be used
// Set a button to any pin

void setup() {

 // Buttons
  // Initialize buttons with pull up resistors
  for (int i = 0; i < N_BUTTONS; i++) {
    pinMode(BUTTON_ARDUINO_PIN[i], INPUT_PULLUP);
  }
  /*/ ENCODER
  for (int i = 0; i < N_ENCODERS; i++) { // if you want to start with a certain value use presets
    encoder.write(preset[0][i]);
  }

  for (int z = 0; z < N_ENCODER_CHANNELS; z++) {
    for (int i = 0; i < N_ENCODERS; i++) {
      lastEncoderValue[z][i] = preset[z][i];
      encoderValue[z][i] = preset[z][i];
    }
  }
  */
  
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  // display a pixel in each corner of the screen
    display.drawPixel(0, 0, WHITE);
  display.drawPixel(127, 0, WHITE);
  display.drawPixel(0, 31, WHITE);
  display.drawPixel(127, 31, WHITE);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4,6);
  display.print("4 N D R E S O L I S");
  display.setCursor(4,20);
  display.print("             DESIGN");
  display.display();

  
  delay (500);
}

void loop() {
  void buttons();
  void readPots();
}

void buttons() 
  {


 
  for (int i = 0; i < N_BUTTONS; i++) {
     buttonCState[i] = digitalRead(BUTTON_ARDUINO_PIN[i]);
    
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {

      if (buttonPState[i] != buttonCState[i]) {
        lastDebounceTime[i] = millis();
        }
        
        if (buttonCState[i] == LOW) {
  
  noteOn(midiCh, note + i, 127);  // channel, note, velocity
  MidiUSB.flush();  
  display.clearDisplay(); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4,6);
  display.print(" 100 + i ");
   display.display();
   delay (50);
   display.clearDisplay();
   display.display(); 
   
    
    
  }
  
  else {noteOn(midiCh, note + i, 0);  // channel, note, velocity
  MidiUSB.flush();
    
    ;
  }
}
    }
  }
void readPots() {

  for (int i = 0; i < N_POTS; i++) { // le todas entradas analogicas utilizadas
    potCState[i] = analogRead(POT_ARDUINO_PIN);
    //Serial.print(potCState[i]); Serial.print(" ");
    //Serial.println();
    }

  for (int i = 0; i < N_POTS; i++) {

    potVar = abs(potCState[i] - potPState[i]); // calcula a variacao da porta analogica

    if (potVar >= varThreshold) {  //sets a threshold for the variance in the pot state, if it varies more than x it sends the cc message
      PTime[i] = millis(); // armazena o tempo previo
    }
    timer[i] = millis() - PTime[i]; // reseta o timer
    if (timer[i] < TIMEOUT) { // se o timer for menor que o tempo maximo permitido significa que o potenciometro ainda esta se movendo
      potMoving = true;
    }
    else {
      potMoving = false;
    }

    if (potMoving == true) { // se o potenciometro ainda esta se movendo, mande o control change
      int ccValue = (potCState[i]/8);
      if (lastCcValue[i] != ccValue) {
        controlChange(11, cc + i, ccValue); // manda control change (channel, CC, value)
        MidiUSB.flush();
        //MIDI.sendControlChange(cc + i, map(potCState[i], 0, 1023, 0, 127), 11); // envia Control Change (numero do CC, valor do CC, canal midi)
        //Serial.print("CC: "); Serial.print(cc + i); Serial.print(" value:"); Serial.println(map(potCState[i], 22, 1023, 0, 127));
        potPState[i] = potCState[i]; // armazena a leitura atual do potenciometro para comparar com a proxima
        lastCcValue[i] = ccValue;
      }
    }
  }
}


  void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
