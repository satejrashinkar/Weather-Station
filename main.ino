#include <Adafruit_ST7735.h> 
#include <SPI.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>	 

#define TFT_CS     10        
#define TFT_DC     9      
// Serial Peripherals 
//I2C devices BUS Interface 
Adafruit_BMP085 bmp;	// Creating Object bmp of class Adafruit_BMP085
DHT dht(7, DHT11);		// Connected to IO7
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC);	

float pulse_count = 0;
float avg_wind_speed = 0;
float rpm;
int previous_time_ms=0;
int calculated_time_ms;

void countPulse() 
{
   pulse_count++;
   digitalWrite(8, !digitalRead(8));
}
void setup() {
	bmp.begin();
	dht.begin();
	tft.initR(INITR_BLACKTAB);  
	tft.setRotation(1);
    tft.fillScreen(ST7735_BLACK);
	attachInterrupt(digitalPinToInterrupt(2), countPulse, RISING);	//attachInterrupt(digitalPinToInterrupt(pin), ISR, mode)
}

void loop(){
	tft.fillScreen(ST7735_BLACK);
	tft.setCursor(0,0);
	tft.setTextColor(ST7735_CYAN);
	tft.println("      Weather Station      ");
	tft.print("\n");

	tft.print("Temperature:");
	tft.setTextColor(ST7735_GREEN);
    tft.print(((analogRead(A0)*500)/1023));
	tft.println("C");
	tft.setTextColor(ST7735_CYAN);
	tft.print("\n");

	tft.print("Humidity   :");
	tft.setTextColor(ST7735_GREEN);
    tft.print(dht.readHumidity(), 2);
	tft.println("%");
	tft.setTextColor(ST7735_CYAN);
	tft.print("\n");

	tft.print("Pressure   :");
	tft.setTextColor(ST7735_GREEN);
    tft.print(bmp.readPressure());
	tft.println("Pa");
	tft.setTextColor(ST7735_CYAN);
	tft.print("\n");

	tft.print("Altitude   :");
	tft.setTextColor(ST7735_GREEN);
    tft.print(bmp.readAltitude());
	tft.println("m");
	tft.setTextColor(ST7735_CYAN);
	tft.print("\n");

	tft.print("Cloud Cover:");
	//tft.print(String(map(analogRead(A1),1023,601,0,100)));
	tft.setTextColor(ST7735_GREEN);
	tft.print(String(map(analogRead(A1),601,1023,0,100)));		//Reads the value from the specified analog pin.
//map(): Re-maps a number from one range to another. That is, a value of fromLow would get mapped to toLow, a value of fromHigh to toHigh, values in-between to values in-between, etc.
//map(value, fromLow, fromHigh, toLow, toHigh)	
	tft.println("%");
	tft.setTextColor(ST7735_CYAN);
	tft.print("\n");

	tft.print("Wind Speed :");
	calculated_time_ms = millis() - previous_time_ms;  	//millis():Returns the number of milliseconds passed since the Arduino board began running the current program.
	rpm = (pulse_count/calculated_time_ms) * 60000;   
	previous_time_ms = millis(); 
	//avg_wind_speed = avg_wind_speed==0?rpm/42.7833:(((rpm/42.7833)+avg_wind_speed)/2);
	avg_wind_speed = (((rpm*0.01885)+avg_wind_speed)/2);
	//delay(500);
	tft.setTextColor(ST7735_GREEN);
	tft.print(avg_wind_speed);
	tft.println("km/hr");
	tft.setTextColor(ST7735_CYAN);
	tft.print("\n");
	detachInterrupt(digitalPinToInterrupt(2)); 

	delay(3000);
}