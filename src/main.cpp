#include <Arduino.h>

/* DEFINES */
#define HELP_BUTTON 23

/* DECLARATIONS */
void GPIO_Open(void);

/* GLOBALS */
bool helpPressed = false;


int main(void)
{
    Serial.begin(115200);
    GPIO_Open();


    while(1)
    {
        printf("Test: \n");
        if(helpPressed)
        {
            printf("Help Button Pressed \n");
            helpPressed = false; 
        }
        delay(1000);
    }

    return 0;
}

void IRAM_ATTR HELP_ISR()
{
    helpPressed = true;
}

void GPIO_Open()
{
    pinMode(HELP_BUTTON, INPUT_PULLUP);
    attachInterrupt(HELP_BUTTON, HELP_ISR, FALLING);
}

void setup() 
{
    main();
}

void loop() 
{
    delay(1000);
}

// struct Button {
//   const uint8_t PIN;
//   uint32_t numberKeyPresses;
//   bool pressed;
// };

// Button button1 = {23, 0, false};

// void IRAM_ATTR HELP_ISR() {
//   button1.numberKeyPresses += 1;
//   button1.pressed = true;
// }

// void setup() {
//   Serial.begin(115200);
//   pinMode(button1.PIN, INPUT_PULLUP);
//   attachInterrupt(button1.PIN, HELP_ISR, FALLING);
// }

// void loop() {
//   if (button1.pressed) {
//       Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
//       button1.pressed = false;
//   }
//   printf("In loop \n");

//   //Detach Interrupt after 1 Minute
//   static uint32_t lastMillis = 0;
//   if (millis() - lastMillis > 60000) {
//     lastMillis = millis();
//     detachInterrupt(button1.PIN);
// 	Serial.println("Interrupt Detached!");
//   }
//   delay(1000);
// }