#include <Arduino.h>

int main(void)
{
    Serial.begin(115200);


    while(1)
    {
        printf("Test: \n");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    return 0;
}




void setup() 
{
    main();
}

void loop() 
{
    delay(1000);
}