/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/mligh/OneDrive/Particle/DecentralizedLV-ULPDRV/src/DecentralizeULPDRV.ino"
/*
 * Project DecentralizeULPDRV
 * Description: Code for the Decentralized Low-Voltge System Low-Power Driver Boards
 * Date: February 1, 2025
 */
#include "DecentralizedLV-Boards/DecentralizedLV-Boards.h"

/////////////////////////////////////////////////////////
// BOARD DEFINITION - UNCOMMENT BOARD TO BE PROGRAMMED //
/////////////////////////////////////////////////////////

//#define BDFL      //Front-Left Board
//#define BDFR      //Front-Right Board
void setup();
void loop();
void animationHandler();
void animationHandler2();
#line 14 "c:/Users/mligh/OneDrive/Particle/DecentralizedLV-ULPDRV/src/DecentralizeULPDRV.ino"
#define BDRL      //Rear-Left Board
//#define BDRR        //Rear-Right Board

/////////////////////////////////////////////////////////
// BOARD PIN CONFIG - Toggle SPI/UART on each board    //
/////////////////////////////////////////////////////////

//For USE_SPI: Decalare FALSE for using the Sense pins normally or TRUE if you're bypassing the resistors and using SPI on the sense pins

//For USE_UART: Declare FALSE for using the Sense pins normally or for SPI. TRUE if you're bypassing the resistors and using Serial on the sense pins

#ifdef BDFL
    #define USE_SPI     FALSE
    #define USE_UART    FALSE
#endif

#ifdef BDFR
    #define USE_SPI     TRUE        //Example for hypothetical TFT LCD for displaying AC charge status
    #define USE_UART    FALSE
#endif

#ifdef BDRL
    #define USE_SPI     FALSE
    #define USE_UART    FALSE
#endif

#ifdef BDRR
    #define USE_SPI     FALSE
    #define USE_UART    TRUE        //Example for hypothetical Serial peripheral
#endif


/////////////////////////////////////////////////////////
/////////////      PIN MAPPINGS      ////////////////////
/////////////////////////////////////////////////////////

#define LP5     D1      //Low Power Output 5 (PWM)
#define LP4     A2      //Low Power Output 4 (PWM)
#define LP3     A5      //Low Power Output 3 (PWM)
#define LP2     S4      //Low Power Output 2
#define LP1     S5      //Low Power Output 1
#define LP0     S6      //Low Power Output 0

#define IP7     A0      //Sense Input 7 (ADC Capable)
#define IP6     A1      //Sense Input 6 (ADC Capable)
#define IP5     D10     //Sense Input 5 
#define IP0     D7      //Sense Input 0

#if USE_SPI == TRUE

// D2 (IP4) is for SPI MOSI when using the Sense Pins for SPI output
// D3 (IP3) is for SPI MISO when using the Sense Pins for SPI output
// D4 (IP2) is for SPI SCK when using the Sense Pins for SPI output
// D5 (IP1) is for SPI Chip-Select when using the Sense Pins for SPI output

#elif USE_UART == TRUE

#define IP4     D2      //Sense Input 4
#define IP3     D3      //Sense Input 3
// D4 (IP2) is for Serial2 TX when using the Sense Pins for UART
// D5 (IP1) is for Serial2 RX when using the Sense Pins for UART

#else

#define IP4     D2      //Sense Input 4
#define IP3     D3      //Sense Input 3
#define IP2     D4      //Sense Input 2
#define IP1     D5      //Sense Input 1

#endif


////////////////////////////
// CAN Message Formatting //
////////////////////////////

CAN_Controller canController;

LPDRV_RearLeft_CAN rearLeftDriver(REAR_LEFT_DRIVER);

DashController_CAN dashController(DASH_CONTROL_ADDR);

PowerController_CAN powerController(POWER_CONTROL_ADDR);

LV_CANMessage inputMessage;    //Input message for receiving from multiple addresses and filtering

Timer pTimer(10, animationHandler);     //Create a timer that happens ever 10 milliseconds for updating the animations
Timer pTimer2(250, animationHandler2);  //A timer for the LV board that handles flashing the BPS fault light

//Misc control-flow variables
uint32_t loop_time = 0;

void boardConfig();     //Function prototype
void transmitLPDRVBoards();
void updateOutputPins();
void updateInputPins();
void updateAnimations();
void performSpecialFeatures();

SYSTEM_MODE(SEMI_AUTOMATIC);    //Disable Wi-Fi for this system

//Function runs once at power up to configure the device.
void setup() {
    Serial.begin(9600);
    canController.begin(500000, S3);
    canController.addFilter(powerController.boardAddress);   //Allow incoming messages from Power Controller
    canController.addFilter(dashController.boardAddress);    //Allow incoming messages from Dash Controller
    boardConfig();              //Call function to configure the pins depending on which board this is
    updateOutputPins();         //Update the pin state based on the data received from other boards
    updateInputPins();          //Update this board's variables that are set by reading the input pins
    pTimer.start();             //start the timer for the animations
    
    RGB.control(true);
    RGB.color(255, 0, 0);
}

//Loop continuously executes as fast as possible
void loop() {
    //All of the fancy pin logic may be complicated but makes life in the loop real EZ
    while(canController.receive(inputMessage)){      //If a CAN Message was received from another board matching the filter (ID 0x100)
        dashController.receiveCANData(inputMessage);
        powerController.receiveCANData(inputMessage);
    }
    updateOutputPins();         //Update the pin state based on the data received from other boards
    updateInputPins();          //Read the multipurpose pins that need to be transmitted from this board
    updateAnimations();

    if(dashController.boardDetected) RGB.color(0,255,0);
    else RGB.color(255,0,0);

    while(millis()-loop_time < 10) delayMicroseconds(1);    //Fancy delay mechanism which takes into account time to read CAN bus frames
    loop_time = millis();

    transmitLPDRVBoards();      //Transmits data from this board to the CAN Bus.
}

/// @brief Call the sendCANData for the respective LPDRV board you've programmed.
void transmitLPDRVBoards(){
    #ifdef BDFL //Front-Left Driver Board
        //Haven't made a Front Left Driver class in Boards API yet. Will need to make one if you want to transmit data from this board.
    #endif
    #ifdef BDFR //Front-Right Driver Board
        //Haven't made a Front Right Driver class in Boards API yet. Will need to make one if you want to transmit data from this board.
    #endif
    #ifdef BDRL
        rearLeftDriver.sendCANData(canController);
    #endif
    #ifdef BDRR
        //Haven't made a Rear Right Driver class in Boards API yet. Will need to make one if you want to transmit data from this board.
    #endif
    
}

//AnimationHandler updates the states of LEDs and other things that play animations over time - using an interrupt allows loop() to continue to run and get updates
void animationHandler(){
    #ifdef BDFL //Front-Left Driver Board Config

    #endif
    #ifdef BDFR //Front-Right Driver Board Config

    #endif
    #ifdef BDRL

    #endif
    #ifdef BDRR
        
    #endif
}

void animationHandler2(){
    //flashOn = !flashOn; //Simply toggle the variable every time the interupt happens to make the section flash

}

/// @brief Configures microcontroller pins and initializes SPI or UART if declared
void boardConfig(){
    
    //Low Power Outputs
    pinMode(LP5, OUTPUT);
    pinMode(LP4, OUTPUT);
    pinMode(LP3, OUTPUT);
    pinMode(LP2, OUTPUT);
    pinMode(LP1, OUTPUT);
    pinMode(LP0, OUTPUT);

    #ifdef BDFL //Front-Left Driver Board Config
        
        pinMode(IP7, INPUT);
        pinMode(IP6, INPUT);
        pinMode(IP5, INPUT);
        pinMode(IP4, INPUT);
        pinMode(IP3, INPUT);
        pinMode(IP2, INPUT);
        pinMode(IP1, INPUT);
        pinMode(IP0, INPUT);
        
    #endif
    #ifdef BDFR //Front-Right Driver Board Config
       
        pinMode(IP7, INPUT);
        pinMode(IP6, INPUT);
        pinMode(IP5, INPUT);
        pinMode(IP0, INPUT);
        
        //These pins used for LCD control
        //pinMode(IP4, INPUT);
        //pinMode(IP3, INPUT);
        //pinMode(IP2, INPUT);
        //pinMode(IP1, INPUT);

        //Initialize the hypothetical TFT LCD here
        

    #endif
    #ifdef BDRL

        pinMode(IP7, INPUT);
        pinMode(IP6, INPUT);
        pinMode(IP5, INPUT);
        pinMode(IP4, INPUT);
        pinMode(IP3, INPUT);
        pinMode(IP2, INPUT);
        pinMode(IP1, INPUT);
        pinMode(IP0, INPUT);

    #endif
    #ifdef BDRR

        pinMode(IP7, INPUT);
        pinMode(IP6, INPUT);
        pinMode(IP5, INPUT);
        pinMode(IP4, INPUT);
        pinMode(IP3, INPUT);
        //pinMode(IP2, INPUT);      //Not used when using Serial1
        //pinMode(IP1, INPUT);
        pinMode(IP0, INPUT);

        Serial1.begin(115200);      //Begin communication with hypothetical Serial peripheral on the Sense Pins

    #endif
    
}

//Update the state of the pins in this function. Do reads/writes to and from the DecentralizedLV API objects.
void updateOutputPins(){
    #ifdef BDFL //Front-Left Driver Board Config
        
        analogWrite(LP5, dashController.leftTurnPWM);   //Left turn signal PWM. analogWrite creates PWM pulse based on duty cycle (0-255)
        digitalWrite(LP2, dashController.highbeam);     //High-beam output
        digitalWrite(LP1, dashController.headlight);    //Headlight output

    #endif
    #ifdef BDFR //Front-Right Driver Board Config

        analogWrite(LP5, dashController.rightTurnPWM);  //Right turn signal PWM. analogWrite creates PWM pulse based on duty cycle (0-255)
        digitalWrite(LP2, dashController.highbeam);     //High-beam output
        digitalWrite(LP1, dashController.headlight);    //Headlight output

    #endif
    #ifdef BDRL

        if(powerController.BrakeSense) analogWrite(LP4, 255);   //If the brake pedal is pressed, turn on brake lights to full brightness (255)
        else if(dashController.headlight) analogWrite(LP4, 80); //If the brake is not pressed and the headlights are on, then dim the brake lights like a regular car
        else analogWrite(LP4, 0);                               //If brake pedal is not pressed and the headlights are not on, thne the brake lights should be fully off.

        analogWrite(LP3, dashController.leftTurnPWM);           //Left turn signal PWM. analogWrite creates PWM pulse based on duty cycle (0-255)
        
        digitalWrite(LP1, dashController.reversePress);         //If we're in reverse, then turn on the backup lights
        digitalWrite(LP0, dashController.reversePress);         //If we're in reverse, turn on the backup camera power

        rearLeftDriver.bmsFaultInput = digitalRead(IP3);        //Read in the state of the general purpose IO 3. This is the BMS fault switch. Update the flag for this controller
        rearLeftDriver.switchFaultInput = digitalRead(IP1);     //Read in the state of the general purpose IO 1. This is the BMS fault switch. Update the flag for this controller

    #endif
    #ifdef BDRR

        if(powerController.BrakeSense) analogWrite(LP4, 255);   //If the brake pedal is pressed, turn on brake lights to full brightness (255)
        else if(dashController.headlight) analogWrite(LP4, 80); //If the brake is not pressed and the headlights are on, then dim the brake lights like a regular car
        else analogWrite(LP4, 0);                               //If brake pedal is not pressed and the headlights are not on, thne the brake lights should be fully off.

        analogWrite(LP3, dashController.rightTurnPWM);           //Left turn signal PWM. analogWrite creates PWM pulse based on duty cycle (0-255)

        digitalWrite(LP1, dashController.reversePress);         //If we're in reverse, then turn on the backup lights

    #endif
}

/// @brief Read any data this board collects (i.e. switches from the 4X low power inputs) and populate the Boards API variables
void updateInputPins(){
    #ifdef BDFL //Front-Left Driver Board Config
        
        //TO-DO: Read in IP0, IP1, IP2, IP3 here for the functions of the front-right board!
        
    #endif
    #ifdef BDFR //Front-Right Driver Board Config
       
       //TO-DO: Read in IP0, IP1, IP2, IP3 here for the functions of the front-right board!

    #endif
    #ifdef BDRL

        //TO-DO: Read in IP0, IP1, IP2, IP3 here for the functions of the front-right board!
        //An example below of how it was done on SPX
        //rearLeftDriver.bmsFaultInput = digitalRead(IP1);
        //rearLeftDriver.switchFaultInput = digitalRead(IP0);
    
    #endif
    #ifdef BDRR

        //TO-DO: Read in IP0, IP1, IP2, IP3 here for the functions of the front-right board!

    #endif
}

/// @brief Perform any special operations for peripherals (SPI1 or Serial)
void performSpecialFeatures(){
    #ifdef BDFL

    #endif
    #ifdef BDFR
        //Update the LCD here
    #endif
    #ifdef BDRL

    #endif
    #ifdef BDRR
        if(Serial1.available()){    //Check if something was received on the Serial bus
            String rxString = Serial1.readString();
            Serial1.println("Hello from the ULPDRV");    //Send a response to the peripheral
            Serial.println(rxString);       //Echo it out to the Serial console on USB
        }
    #endif
}

void updateAnimations(){
    #ifdef BDFL

    #endif
    #ifdef BDFR

    #endif
    #ifdef BDRL

    #endif
    #ifdef BDRR

    #endif
}
