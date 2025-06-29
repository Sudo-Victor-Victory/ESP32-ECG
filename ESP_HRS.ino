// These pins are for the ESP-WROOM-32.
int ecg_output_pin = 34;  // TX2
int lo_minus = 19;    // D5
int lo_plus = 18;     // D18

int ecg_voltage;
void setup() {
  Serial.begin(115200);
  pinMode(lo_minus, INPUT);
  pinMode(lo_plus, INPUT);

  analogReadResolution(12);        // Uses the ESP's 12 bit ADC
  analogSetAttenuation(ADC_11db);  // Uses ESP's default ref. voltage
}

void loop() {
  int lo_plus_val = digitalRead(lo_plus);
  int lo_minus_val = digitalRead(lo_minus);

  if (lo_plus_val == HIGH || lo_minus_val == HIGH) {
    Serial.printf("Lead detection failure: LO+ %d and LO- %d\n", lo_plus_val, lo_minus_val  );
    delay(5);
  } 
  else {
    ecg_voltage = analogRead(ecg_output_pin);
    Serial.print("Analog val ");
    Serial.print(ecg_voltage);  
    Serial.println("");
  }

  delay(20);
}