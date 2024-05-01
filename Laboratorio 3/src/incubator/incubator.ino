#include <PCD8544.h>
#include <PID_v1_bc.h>

// Defino los pines de los LEDs
#define BLUE 8
#define GREEN 12
#define RED 13

PCD8544 lcd;

double input, output, setpoint;

// PID myPID(&input, &output, &setpoint, )
// Implementación del PID

void LCD_state(){
  int switchState = analogRead(A1);  
  if (switchState > 700){
    lcd.setPower(1);
  } else{
    lcd.setPower(0);
  }
}

void setup() {
  // Establezco el tipo de pin que quiero 
  pinMode(BLUE, OUTPUT); pinMode(GREEN, OUTPUT); pinMode(RED, OUTPUT); // LEDs

  // Activo el LCD
  lcd.begin(); // default resolution is 84x48
}

void loop() {
  LCD_state();
  lcd.clear();

  int value = analogRead(A0);
  float temp = map(value, 0, 1022, 20, 80); // Rango de operación de la incubadora: [20, 80] °C

  // Mostrar en LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp); delay(500);

  if(temp < 30){
    digitalWrite(BLUE, HIGH); digitalWrite(RED, LOW); digitalWrite(GREEN, LOW);
  } else if(temp > 42){
    digitalWrite(RED, HIGH); digitalWrite(GREEN, LOW); digitalWrite(BLUE, LOW);
  } else{
    digitalWrite(GREEN, HIGH); digitalWrite(RED, LOW); digitalWrite(BLUE, LOW);
  }
}
