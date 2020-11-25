//RF Setup section
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <FastLED.h>

RF24 radio(7, 8); // CE, CSN on Nano
const byte address[6] = "00001";
int secondHand;
int deadMan;

//FastLED Setup section
#define LED_PIN     2
#define NUM_LEDS    100
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
CRGB leds[NUM_LEDS];


#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedGoldGreenPalette_p PROGMEM;


void setup() {
    //Serial.begin(9600);
    //Serial.begin(115200);
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setChannel(120);
    radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate(RF24_250KBPS);           // Lower the data rate to increase transmission distance
    radio.startListening();
    //Serial.println("RF Start complete");
    
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


void loop() {
    //secondHand = 0;
    //EVERY_N_MILLISECONDS(100)
    if (radio.available()) {
      radio.read(&secondHand, sizeof(secondHand));
      deadMan = 0;}
      else { deadMan++;}
    if (deadMan > 100) {secondHand = 0;}
    //Serial.print("secondHand = "); 
    //Serial.println(secondHand);    
    //Serial.print("deadMan = "); 
    //Serial.println(deadMan);

    
    ChangePalettePeriodically();
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < ((NUM_LEDS/2)-7); i++) { //7 nodes not used
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        leds[i + (NUM_LEDS/2)] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);        
        // ColorFromPalette ( palette set 16-32-256, index, brightness, blending)
        colorIndex += 3;
    }
}

void ChangePalettePeriodically()
{   
        if( secondHand ==  0)  { SetupAllBlackPalette();                    currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { SetupTotallyRandomPalette();               currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { currentPalette = RainbowColors_p;          currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupRedAndGreenPalette();                 currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { SetupBlackAndGreenStripedPalette();        currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { SetupBlackAndRedStripedPalette();          currentBlending = LINEARBLEND; }
        if( secondHand == 60)  { SetupBlackAndGoldStripedPalette();         currentBlending = LINEARBLEND; }
        if( secondHand == 70)  { currentPalette = myRedGoldGreenPalette_p;  currentBlending = LINEARBLEND; }
        if( secondHand == 80)  { SetupFillSolidGold();                      currentBlending = LINEARBLEND; }
        if( secondHand == 90)  { SetupFillSolidRed();                       currentBlending = LINEARBLEND; }
        if( secondHand == 100) { SetupFillSolidGreen();                     currentBlending = LINEARBLEND; }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

void SetupAllBlackPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
}

void SetupBlackAndGreenStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::DarkGreen;
    currentPalette[4] = CRGB::DarkGreen;
    currentPalette[8] = CRGB::DarkGreen;
    currentPalette[12] = CRGB::DarkGreen;
    
}

void SetupBlackAndRedStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Red;
    currentPalette[4] = CRGB::Red;
    currentPalette[8] = CRGB::Red;
    currentPalette[12] = CRGB::Red;
    
}

void SetupBlackAndGoldStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Goldenrod;
    currentPalette[4] = CRGB::Goldenrod;
    currentPalette[8] = CRGB::Goldenrod;
    currentPalette[12] = CRGB::Goldenrod;
    
}

void SetupFillSolidGreen()
{
    // fill solid all palette entries
    fill_solid( currentPalette, 16, CRGB::DarkGreen);
}

void SetupFillSolidRed()
{
    fill_solid( currentPalette, 16, CRGB::DarkRed);
}

void SetupFillSolidGold()
{
    fill_solid( currentPalette, 16, CRGB::Goldenrod);
}

// This function sets up a palette of purple and green stripes.
void SetupRedAndGreenPalette()
{
    CRGB red = CHSV( HUE_RED, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   red, red, black,  black,
                                   green,  green,  black,  black,
                                   red, red, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedGoldGreenPalette_p PROGMEM =
{
    CRGB::DarkRed,
    CRGB::Goldenrod,
    CRGB::DarkGreen,
    CRGB::Black,
    
    CRGB::DarkRed,
    CRGB::Goldenrod,
    CRGB::DarkGreen,
    CRGB::Black,
    
    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::Goldenrod,
    CRGB::Goldenrod,
    CRGB::DarkGreen,
    CRGB::DarkGreen,
    CRGB::Black,
    CRGB::Black
};
