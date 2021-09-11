#include <Wire.h>
#include <HX711.h>

int ps_01=A0;
int ps_02=A2;
int ps_clock=A1;


int SD_select = 3;
int DAQ_STATUS = 0;
float REC_STATUS;
int serial_address[N_sensors];
double buffer_sensordata[N_sensors];
//LCD Output-related variables
String display_name[N_sensors] = {
  "Rec_status",
  "Temp",
  "Hum",
  "P"
};
String display_units[N_sensors] = {
  "[-]",
  "[C]",
  "[%]",
  "[Pa]"
};
int lcd_visualization_group[2] = {
  1,
  2
};


LiquidCrystal_I2C lcd(0x27,16,2);
DHT th1(TH1_PIN, TH1_TYPE);
HX711 psensor_01;
File file;
 
void setup()
{
 pinMode(DAQ_STATUS_PIN, INPUT);
 pinMode(REC_STATUS_PIN, INPUT);
 pinMode(SD_select, OUTPUT);
 
 Serial.begin(9600);
 
 lcd.init();
 lcd.setBacklight(HIGH);
 
 th1.begin();
 
 psensor_01.begin(A0, A1);
 psensor_01.set_scale(2280.f);
 psensor_01.tare(); // reset the pressure_sensor to 0

 if (!SD.begin(SD_select)) { // Initialize SD card
    Serial.println("Could not initialize SD card."); // if return value is false, something went wrong.
 }

 if (SD.exists("file.txt")) { // if "backup_log.txt" exists, fill will be deleted
    Serial.println("Backup_log exists.");
    if (SD.remove("file.txt") == true) {
      Serial.println("Successfully removed file.");
    } else {
      Serial.println("Could not removed file.");
    }
 }

 file=SD.open("file.txt",FILE_WRITE);
 for (int x = 0; x < N_sensors; x = x + 1) {
  if (file){
    file.print(display_name[x]);
    file.print(" ,");
    }
  
  }
 file.println("");
 file.close();


 
 
}
 
void loop()
{
  DAQ_STATUS=digitalRead(DAQ_STATUS_PIN);
  if (DAQ_STATUS==0){
    lcd.setCursor(0,0);
    lcd.print("DAQ in Standy-by");
    lcd.setCursor(0,1);
    lcd.print("    .........   ");
    delay(100);
    
  } else {
    for (int x = 0; x < N_sensors; x = x + 1) {
      serial_address[x] = x + 1;
      buffer_sensordata[x] = 0.00;
    }
    
    //Store sensor data in data buffer
    REC_STATUS=digitalRead(REC_STATUS_PIN);
    buffer_sensordata[0]=REC_STATUS;
    buffer_sensordata[1] = th1.readTemperature();
    buffer_sensordata[2] = th1.readHumidity();
    buffer_sensordata[3] = psensor_01.get_units();
    
    //Serial Output
    file=SD.open("file.txt",FILE_WRITE);
    for (int n = 0; n < N_sensors; n = n + 1) {
      Serial.print(serial_address[n]);
      Serial.print(": ");
      Serial.print(buffer_sensordata[n]);
      Serial.print(" ,");
      if (file){
        file.print(buffer_sensordata[n]);
        file.print(" ,");
      }
      
      
      
      
    }
    Serial.println("");
    file.println("");
    file.close();
    
    //LCD Output
    lcd.clear();
    for (int n2 = 0; n2 < 2; n2 = n2 + 1) {
      lcd.setCursor(0, n2);
      lcd.print(display_name[lcd_visualization_group[n2]] + ": ");
      lcd.print(String(buffer_sensordata[lcd_visualization_group[n2]]));
      lcd.print(" ");
      lcd.print(display_units[lcd_visualization_group[n2]]);
    }
    delay(250);
  }
}
