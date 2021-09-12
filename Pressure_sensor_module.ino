#include <Wire.h>
#include <HX711.h>

int t_daq=250;
int ps01_port=A0;
int ps02_port=A2;
int ps_clock=A1;
float ps01_reading;
float ps02_reading;

HX711 ps01;
HX711 ps02;

 
void setup()
{
 
 Serial.begin(9600);
 Serial.println("Initializing pressure sensor 01...");
 
 ps01.begin(ps01_port, ps_clock);
 ps01.set_scale(2280.f);
 ps01.tare(); // reset the pressure_sensor to 0
 
 Serial.println("Sensor has been zeroed");


 Serial.println("Initializing pressure sensor 02..."); 
 
 ps02.begin(ps02_port, ps_clock);
 ps02.set_scale(2280.f);
 ps02.tare(); // reset the pressure_sensor to 0
 
 Serial.println("Sensor has been zeroed");
 
 Serial.println("Sensors intialized sucessfully."); 

}
 
void loop(){

    ps01_reading = ps01.get_units();
    ps02_reading = ps02.get_units();

    Serial.print("|| ");
    Serial.print("PS01 : ");
    Serial.print(ps01_reading);
    Serial.print(" | ");
    Serial.print("PS02 : ");
    Serial.print(ps02_reading);
    Serial.print(" || ");

    Serial.println("");
    delay(t_daq);
}
