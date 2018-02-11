const int strobe = 7;
const int clock = 9;
const int data = 8;
                          /*0*/ /*1*/ /*2*/ /*3*/ /*4*/ /*5*/ /*6*/ /*7*/ /*8*/ /*9*/
const uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f };
int movements = 0;
uint8_t values[] = { 0,0,0,0,0,0,0,0 };
int theDigits[] = { 0,0,0,0,0,0,0,0 };

int ledPin = 13;
int inputPin = 2;
int val = 0;
int pirState = LOW; 

void setup() {
  Serial.begin(9600);
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
 
  sendCommand(0x8f);  // activate and set brightness to max
  reset();
  
  pinMode(inputPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
//  writeName(hejElsa); 
//  writeName(hejJoar); 

  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      movements++;
      printNumber(movements);
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}

void printNumber(long number) {
 /*
  Serial.print("Sent number: ");
  Serial.println(number);
  */
  for(int i = 7; i >= 0; i--) {
    theDigits[i] = number % 10;
    number /= 10;
  }
/*
  Serial.print("split number: ");
  for(int i = 0; i < 8; i++) {
    Serial.print(theDigits[i]);
  }
  Serial.println();
  */
  boolean notZero = false;
  for(int i = 0; i < 8; i++) {
    if(!notZero && theDigits[i] > 0) {
      notZero = true;  
    }
    if(notZero) {
      values[i] = digits[theDigits[i]];
    } else {
      values[i] = 0x00;
    }
  }
/*
 Serial.print("hex number: ");
  for(int i = 0; i < 8; i++) {
    Serial.print(values[i]);
  }
  Serial.println();
  */
  sendCommand(0x40);  // continous
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0); // 1st digit

  for(uint8_t i = 0; i < 8; i++) {
     displayChar(values[i]);   
  }
  
  digitalWrite(strobe, HIGH);
}

void writeName(uint8_t* nameArray) {
   sendCommand(0x40);  // continous

  for(uint8_t j = 0; j < 8; j++) {
    digitalWrite(strobe, LOW);
    shiftOut(data, clock, LSBFIRST, 0xc0); // 1st digit
    for(uint8_t i = 0; i < 8; i++) {
      if(i<=j) {
        displayChar(nameArray[(i)]);
      } else {
        displayChar(0x00);
      }
    }
    digitalWrite(strobe, HIGH);
    delay(100);
  }
  for(uint8_t j = 0; j < 8; j++) {
    digitalWrite(strobe, LOW);
    shiftOut(data, clock, LSBFIRST, 0xc0); // 1st digit
    for(uint8_t i = 0; i < 8; i++) {
      if(i>j) {
        displayChar(nameArray[(i)]);
      } else {
        displayChar(0x00);
      }
    }
    digitalWrite(strobe, HIGH);
    delay(100);
  }
  delay(500);
}

void displayChar(uint8_t value) {
  shiftOut(data, clock, LSBFIRST, value); 
  shiftOut(data, clock, LSBFIRST, 0x00);
}

void sendCommand(uint8_t value)
{
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}
 
void reset()
{
  sendCommand(0x40); // continous
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(data, clock, LSBFIRST, 0x00);
  }
  digitalWrite(strobe, HIGH);
}
