#include <Arduino.h>
#include <BlynkSimpleEsp32.h>

/* DEFINES */
#define HELP_BUTTON 23
#define VIBRATE_PIN_1 2
#define VIBRATE_PIN_2 22

//ultrasonic sensor 1
#define trigPin1 4
#define echoPin1 16

//ultrasonic sensor 2
#define trigPin2 18
#define echoPin2 5

#define DISTANCE_CHECK_STACK_SIZE 1000
#define DISTANCE_CHECK_PRIORITY 4

/* DECLARATIONS */
void GPIO_Open(void);
void getDistance(int);
void wirelessOpen(void);
void vibrateSignal(int);
void tasksOpen(void);
void vDistanceCheck(void * parameter);
void vVibrate2(void * parameter);


/* GLOBALS */
bool helpPressed = false;

// Wi-Fi
// char ssid[] = "Choma 5G";
// char pass[] = "Kaiser99";
char ssid[] = "jakey";
char pass[] = "12345678";
// auth token for blynk servers
char auth[] = "6aW_cmjPMMIvBDqwQ20cJh3kkwYCj-7y";

int distance1; // for the distance measurement
int distance2;

int main(void)
{
    Serial.begin(115200);

    GPIO_Open();
    // wirelessOpen();


    tasksOpen();


    while(1)
    {
        // printf("Test: \n");
        // Blynk.run();

        if(helpPressed)
        {
            printf("Help Button Pressed \n");
            helpPressed = false; 
            Blynk.notify("EMERGENCY BUTTON PRESSED");
        }
        
        // delay(1000);

        vibrateSignal(10);

       delay(10); 
        // helpPressed = true;
    }

    return 0;
}

void tasksOpen()
{
    printf("Opening FreeRTOS Tasks: \n");
    
    xTaskCreate(vDistanceCheck, "Distance Check", DISTANCE_CHECK_STACK_SIZE, NULL, DISTANCE_CHECK_PRIORITY, NULL); 
    xTaskCreate(vVibrate2, "Vibrate 2", DISTANCE_CHECK_STACK_SIZE, NULL, DISTANCE_CHECK_PRIORITY, NULL); 
}

void vDistanceCheck(void * parameter)
{
    for(;;)
    {
        vTaskDelay(250 / portTICK_PERIOD_MS);
        getDistance(1);
        getDistance(2);
        // Serial.print("distance 1: ");
        // Serial.println(distance1);
        // Serial.print("\n");
        // Serial.print("distance 2: ");
        // Serial.print(distance2);
        // Serial.print("\n");
    }
}

void vVibrate2(void * parameter)
{
    for(;;)
    {
        if(distance2 >= 130)
        {
            digitalWrite(VIBRATE_PIN_2, HIGH);
            delay(150);
            digitalWrite(VIBRATE_PIN_2, LOW);
            delay(1000);
        }
        else if(90 <= distance2 && distance2 < 130)
        {
            digitalWrite(VIBRATE_PIN_2, HIGH);
            delay(500);
            digitalWrite(VIBRATE_PIN_2, LOW);
            delay(500);
        }
        else if(50 <= distance2 && distance2 < 90)
        {
            digitalWrite(VIBRATE_PIN_2, HIGH);
            delay(250);
            digitalWrite(VIBRATE_PIN_2, LOW);
            delay(250);
        }
        else if(0 <= distance2 && distance2 < 50)
        {
            Serial.println("Smallest Distance Option");
            digitalWrite(VIBRATE_PIN_2, HIGH);
            delay(100);
            digitalWrite(VIBRATE_PIN_2, LOW);
            delay(100);
        }
        else
        {
            digitalWrite(VIBRATE_PIN_2, LOW);
            delay(1000);
        }
    }

}

void vibrateSignal(int distance)
{
    if(distance1 >= 130)
    {
        digitalWrite(VIBRATE_PIN_1, HIGH);
        delay(150);
        digitalWrite(VIBRATE_PIN_1, LOW);
        delay(1000);
    }
    else if(90 <= distance1 && distance1 < 130)
    {
        digitalWrite(VIBRATE_PIN_1, HIGH);
        delay(500);
        digitalWrite(VIBRATE_PIN_1, LOW);
        delay(500);
    }
    else if(50 <= distance1 && distance1 < 90)
    {
        digitalWrite(VIBRATE_PIN_1, HIGH);
        delay(250);
        digitalWrite(VIBRATE_PIN_1, LOW);
        delay(250);
    }
    else if(0 <= distance1 && distance1 < 50)
    {
        Serial.println("Smallest Distance Option");
        digitalWrite(VIBRATE_PIN_1, HIGH);
        delay(100);
        digitalWrite(VIBRATE_PIN_1, LOW);
        delay(100);
    }
    else
    {
        digitalWrite(VIBRATE_PIN_1, LOW);
        delay(1000);
    }

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

    pinMode(VIBRATE_PIN_2, OUTPUT);
    pinMode(VIBRATE_PIN_1, OUTPUT);

    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
}

void getDistance(int ultrasonic)
{
    long duration1; // for the duration of the wave travel
    long duration2;

    if(ultrasonic == 1)
    {
        digitalWrite(trigPin1, LOW);
        delayMicroseconds(2);
        // set the trigPin high for 10 uS
        digitalWrite(trigPin1, HIGH);
        
        delayMicroseconds(10);
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
        // set the trigPin high for 10 uS
        digitalWrite(trigPin2, HIGH);

        delayMicroseconds(10);
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

