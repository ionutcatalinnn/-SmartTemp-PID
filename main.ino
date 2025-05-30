#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>

#define DHTPIN 2     // Pinul de ieșire al senzorului DHT-11
#define DHTTYPE DHT11  // Tipul senzorului DHT11
const int mosfetPin = 9; // Pinul digital conectat la modulul HW-042
DHT dht(DHTPIN, DHTTYPE);

#define BUTTON_OK 11
#define BUTTON_CANCEL 12
#define BUTTON_NEXT 13
#define BUTTON_PREV 10

enum BUTTONS {
  EV_OK,
  EV_CANCEL,
  EV_NEXT,
  EV_PREV,
  EV_NONE
};

enum MENUS {
  MENU_TEMP_TIME,
  MENU_SETPOINT
};

bool button_pressed = false;
MENUS current_menu = MENU_TEMP_TIME;


float kp =2.30;  
float ki = 0.01; 
float kd = 3;   


float setpoint = 28;  
float error, lastError, integral, derivative, output;
float tempAnterioara = setpoint;
float tempSetata = tempAnterioara;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(mosfetPin, OUTPUT);

  lcd.init();
  lcd.backlight();

  pinMode(BUTTON_OK, INPUT);
  pinMode(BUTTON_CANCEL, INPUT);
  pinMode(BUTTON_NEXT, INPUT);
  pinMode(BUTTON_PREV, INPUT);

  
  while (!Serial);

  
  Serial.println("Temperature(°C)\t\tPID Output");

 
  delay(1000);

 
  setTime(16, 23, 0, 18, 12, 2024); 
}

void loop() {
  delay(50);  

  float temperature = dht.readTemperature();

  
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    return;
  }

  BUTTONS button_event = getButtonEvent();
  handleButtonEvent(button_event);

  displayCurrentMenu(temperature);

  
  error = tempSetata - temperature;
  integral += error;
  derivative = error - lastError;

  
  output = kp * error + ki * integral + kd * derivative;

  
  if (output > 0) {
    digitalWrite(mosfetPin, HIGH);
  } else {
    digitalWrite(mosfetPin, LOW);
  }

  
  Serial.print(temperature);
  Serial.print("\t\t\t");
  Serial.println(output);

  
  lastError = error;
}

BUTTONS getButtonEvent() {
  if (digitalRead(BUTTON_OK)) return EV_OK;
  if (digitalRead(BUTTON_CANCEL)) return EV_CANCEL;
  if (digitalRead(BUTTON_NEXT)) return EV_NEXT;
  if (digitalRead(BUTTON_PREV)) return EV_PREV;
  return EV_NONE;
}

void handleButtonEvent(BUTTONS button) {
  switch (button) {
    case EV_OK:
      if (current_menu == MENU_TEMP_TIME) {
        current_menu = MENU_SETPOINT;
      } else if (current_menu == MENU_SETPOINT) {
        tempSetata = tempAnterioara;
      }
      break;
    case EV_NEXT:
      if (current_menu == MENU_SETPOINT) {
        tempAnterioara += 1;
      }
      break;
    case EV_PREV:
      if (current_menu == MENU_SETPOINT) {
        tempAnterioara -= 1;
      }
      break;
    case EV_CANCEL:
      if (current_menu == MENU_SETPOINT) {
        tempAnterioara = tempSetata;
        current_menu = MENU_TEMP_TIME;
      }
      break;
    default:
      break;
  }
}

void displayCurrentMenu(float temperature) {
  lcd.clear();
  if (current_menu == MENU_TEMP_TIME) {
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(hour());
    lcd.print(":");
    lcd.print(minute());
    lcd.print(":");
    lcd.print(second());
  } else if (current_menu == MENU_SETPOINT) {
    lcd.setCursor(0, 0);
    lcd.print("Set Temp: ");
    lcd.print(tempAnterioara);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Modifica cu -/+, OK pt setare, Cancel pt revenire");
  }
}
