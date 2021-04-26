#include <Arduino.h>
#include <BlynkSimpleEsp32.h>

/* DEFINES */
#define HELP_BUTTON 23
//ultrasonic sensor 1
#define trigPin1 4
#define echoPin1 16

//ultrasonic sensor 2
#define trigPin2 18
#define echoPin2 5

/* DECLARATIONS */
void GPIO_Open(void);
void getDistance(int);
void wirelessOpen(void);

/* GLOBALS */
bool helpPressed = false;

// Wi-Fi
char ssid[] = "Choma 5G";
char pass[] = "Kaiser99";
// auth token for blynk servers
char auth[] = "6aW_cmjPMMIvBDqwQ20cJh3kkwYCj-7y";

long duration1; // for the duration of the wave travel
long duration2;
int distance1; // for the distance measurement
int distance2;

int main(void)
{
    Serial.begin(115200);
    GPIO_Open();
    //wirelessOpen();

    while(1)
    {
        //printf("Test: \n");
        //Blynk.run();

        if(helpPressed)
        {
            printf("Help Button Pressed \n");
            helpPressed = false; 
        }
        getDistance(1);
        getDistance(2);
        Serial.print("distance 1: ");
        Serial.print(distance1);
        Serial.print("\n");
        Serial.print("distance 2: ");
        Serial.print(distance2);
        Serial.print("\n");
        delay(1000);
    }

    return 0;
}


void IRAM_ATTR HELP_ISR()
{
    helpPressed = true;
}

/* IOS APP BUTTON INPUT */
BLYNK_WRITE(V0)
{
    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable


    //  signal to the hardware outputs task to power up coils (semaphore unused currently, for a future option)
    // xSemaphoreGive(sema_candle_state);
    if(pinValue == 1)
    {
        printf("Button ON Request from IOS App \n");
    } 
    else
    {
        printf("Button OFF Request from IOS App \n");
    }
}

void wirelessOpen()
{
    printf("|----------------------------|\n");
    printf("Opening Wi-Fi Control\n");

    // Blynk Configures the Wi-Fi connection and authenticates with their servers for coms over the internet
    Blynk.begin(auth, ssid, pass);
}

void GPIO_Open()
{
    pinMode(HELP_BUTTON, INPUT_PULLUP);
    attachInterrupt(HELP_BUTTON, HELP_ISR, FALLING);

    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
}

void getDistance(int ultrasonic)
{
    if(ultrasonic == 1)
    {
        digitalWrite(trigPin1, LOW);
        delayMicroseconds(2);
        // set the trigPin high for 10 ms
        digitalWrite(trigPin1, HIGH);
        
        delayMicroseconds(100);
        digitalWrite(trigPin1, LOW);
        
        // Reads the echoPins, returns the sound wave travel time in ms
        duration1 = pulseIn(echoPin1, HIGH);
        
        // calculating the distance
        distance1 = duration1 * 0.034 / 2; //speed of sound wave divided by 2
        
    }
    if(ultrasonic == 2)
    {
        digitalWrite(trigPin2, LOW);
        delayMicroseconds(2);
        // set the trigPin high for 10 ms
        digitalWrite(trigPin2, HIGH);

        delayMicroseconds(100);
        digitalWrite(trigPin2, LOW);

        // Reads the echoPins, returns the sound wave travel time in ms
        duration2 = pulseIn(echoPin2, HIGH);

        // calculating the distance
        distance2 = duration2 * 0.034 / 2; //speed of sound wave divided by 2
    }
}

void setup() 
{
    main();
}

void loop() 
{
    delay(1000);
}

