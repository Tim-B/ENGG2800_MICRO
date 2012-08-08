int val = 0;
int codeIncomming = 0;

void setup()  {
  Serial.begin(9600);
  Serial.println("App starting");
  delay(1000);
  pinMode(36, OUTPUT);  // declare LED as output
  pinMode(2, INPUT); 
  pinMode(13, OUTPUT); 
  digitalWrite(36, HIGH);
  monitorIR();
}

void loop(){   
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}

void processIR() {
    noInterrupts();
    delayMicroseconds(2400);
    if(digitalRead(2) != 0) {
       interrupts();
       return; 
    }
    int val;
    int signal[127];
    for(int i = 0; i < 128; i++) {
      val = digitalRead(2); 
      signal[i] = val;
      delayMicroseconds(600);
    }
    delay(5000);
    interrupts();
    print_out(128, signal);
}

void monitorIR() {
  Serial.println("Attaching intterupt");
  attachInterrupt(0, processIR, CHANGE);
}

void print_out(int count, int values[]) {
    for(int i = 0; i < count; i++) {
        Serial.print(values[i]);
    }
    Serial.println();
}
