#include <PCD8544.h>
#include <PID_v1.h>

// Defino los pines de los LEDs
#define BLUE 8
#define GREEN 12
#define RED 13

// Implementación de la pantalla
PCD8544 lcd;

// Implementación del PID
double Input, Output, Setpoint;

PID myPID(&Input, &Output, &Setpoint, 15, 0.5, 1, DIRECT);

// SETUP
void setup() {
  Serial.begin(9600);
  // Establezco el tipo de pin que quiero 
  pinMode(BLUE, OUTPUT); pinMode(GREEN, OUTPUT); pinMode(RED, OUTPUT); // LEDs

  // Activo el LCD
  lcd.begin(); // default resolution is 84x48

  // Inicializo el PID
  myPID.SetOutputLimits(-125, 125);
  myPID.SetMode(AUTOMATIC);
}

// LOOP
void loop() {
  LCD_state();

  // Definición del SETPOINT
  int value = analogRead(A0);
  float temp = map(value, 0, 1022, 20, 80); // Rango de operación de la incubadora: [20, 80] °C

  Setpoint = temp;

  // Definición de INPUT
  float tempWatts = (int)Output*25.0/255.0;
  Input = simPlant(tempWatts);

  // Se llama al PID
  myPID.Compute();

  // Mostrar en LCD
  lcd.setCursor(0, 1);
  lcd.print("SP: "); lcd.print(temp); 
  
  lcd.setCursor(0, 2);
  lcd.print("Control: "); lcd.print(Output);

  lcd.setCursor(0, 3);
  lcd.print("Temp: "); lcd.print(Input);

  int commSwitch = analogRead(A2);

  // Interrupción de las comunicaciones
  if(commSwitch > 700){
    Serial.print(Setpoint);
    Serial.print(",");
    Serial.print(Output);
    Serial.print(",");
    Serial.println(Input); 
    delay(500);
  }
  delay(500);

  // Activación de los LEDs indicadores de temperatura
  if(Input < 30){
    digitalWrite(BLUE, HIGH); 
    digitalWrite(RED, LOW); 
    digitalWrite(GREEN, LOW);
  } else if(Input > 42){
    digitalWrite(RED, HIGH); 
    digitalWrite(GREEN, LOW); 
    digitalWrite(BLUE, LOW);
  } else{
    digitalWrite(GREEN, HIGH); 
    digitalWrite(RED, LOW); 
    digitalWrite(BLUE, LOW);
  }
}
// Inicio de funciones adicionales

void LCD_state(){
  int switchState = analogRead(A1);  
  if (switchState > 700){
    lcd.setPower(1);
  } else{
    lcd.setPower(0);
  }
}

float simPlant(float Q) { // heat input in W (or J/s)
  // simulate a 1x1x2cm aluminum block with a heater and passive ambient cooling
 // float C = 237; // W/mK thermal conduction coefficient for Al
  float h = 5; // W/m2K thermal convection coefficient for Al passive
  float Cps = 0.89; // J/g°C
  float area = 1e-4; // m2 area for convection
  float mass = 10; // g
  float Tamb = 25; // °C
  static float T = Tamb; // °C
  static uint32_t last = 0;
  uint32_t interval = 100; // ms

  if (millis() - last >= interval) {
    last += interval;
    // 0-dimensional heat transfer
    T = T + Q * interval / 1000 / mass / Cps - (T - Tamb) * area * h;
  }
  return T;
}