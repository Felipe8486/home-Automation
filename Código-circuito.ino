#include <WiFi.h>
#include <ThingESP.h>

ThingESP32 thing("felipe8486", "ConnectEsp8266", "123456789123456789");

int D4 = 4;  // Ventilador connected to pin 4
int D2 = 2;  // Cafeteira connected to pin 2

unsigned long previousTime = 0;
const long FiveMinutes = 300000;  // 5 minutes = 300000 milliseconds

void setup()
{
    Serial.begin(115200);

    pinMode(D4, OUTPUT);
    pinMode(D2, OUTPUT);

    thing.SetWiFi("Felipe", "class56/8");

    thing.initDevice();
}

String HandleResponse(String query)
{
    unsigned long currentTime = millis();

    if (query == "ventilador" && digitalRead(D4)==0)
    {
        digitalWrite(D4, 1);
        return "Ventilador ligado";
    }
    else if (query == "ventilador" && digitalRead(D4))
    {
        digitalWrite(D4, 0);
        return "Ventilador";
    }
    else if (query == "ventilador status")
        return digitalRead(D2) ? "Ventilador esta ligado" : "Ventilador esta desligado";

    ///////////////
    else if (query == "cafeteira" && digitalRead(D2)==0)
    {
        digitalWrite(D2, 1);
        previousTime = currentTime;
        return "Cafeteira ficara ligada por 5min";
    }
    else if (query == "cafeteira" && digitalRead(D2))
    {
        digitalWrite(D2, 0);
        return "Cafeteira desligada";
    }
    else if (query == "cafeteira status")
        return digitalRead(D2) ? "cafeteira esta ligada" : "cafeteira esta desligada";
    else
        return "Entrada Invalida";
}

void loop()
{
    unsigned long currentTime = millis();

    if (digitalRead(D2) && currentTime - previousTime >= FiveMinutes)  // if cafeteira is on and 5 minutes have passed
    {
        digitalWrite(D2, 0);  // turn off the cafeteira
        previousTime = currentTime;  // reset the timer
    }

    thing.Handle();
}
