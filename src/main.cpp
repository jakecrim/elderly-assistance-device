#include <Arduino.h>

/* DEFINES */
#define HELP_BUTTON 23
//ultrasonic sensor 1
#define trigPin1 4
#define echoPin1 5

/* DECLARATIONS */
void GPIO_Open();
void getDistance();

/* GLOBALS */
bool helpPressed = false;

long duration1; // for the duration of the wave travel
int distance1; // for the distance measurement

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
        getDistance();
        Serial.print("distance 1: ");
        Serial.print(distance1);
        Serial.print("\n");
        delay(1000);
    }

    return 0;
}

void IRAM_ATTR HELP_ISR()
{
    helpPressed = true;
}

void setup() 
{
    main();
}

void loop() 
{
    delay(1000);
}

void GPIO_Open()
{
    pinMode(HELP_BUTTON, INPUT_PULLUP);
    attachInterrupt(HELP_BUTTON, HELP_ISR, FALLING);

    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
}

void getDistance()
{
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    // set the trigPin1 high for 10 ms
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(100);
    digitalWrite(trigPin1, LOW);
    // Reads the echoPin, returns the sound wave travel time in ms
    duration1 = pulseIn(echoPin1, HIGH);
    // calculating the distance
    distance1 = duration1 * 0.034 / 2; //speed of sound wave divided by 2
}
