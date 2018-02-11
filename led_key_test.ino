const int strobe = 7;
const int clock = 9;
const int data = 8;
uint8_t hejElsa[8] = {0x76, 0x79, 0x1E, 0x00, 0x79, 0x38, 0x6D, 0x77}; // HEJ ELSA
uint8_t hejJoar[8] = {0x76, 0x79, 0x1E, 0x00, 0x0E, 0x3F, 0x77, 0x77}; // HEJ JOAR

void setup() {
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
 
  sendCommand(0x8f);  // activate and set brightness to max
  reset();

}

void loop() {
  writeName(hejElsa); 
  writeName(hejJoar); 
}

void writeName(uint8_t* nameArray) {
   sendCommand(0x40);  // set single address

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
  shiftOut(data, clock, LSBFIRST, value); // H
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
  sendCommand(0x40); // set auto increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(data, clock, LSBFIRST, 0x00);
  }
  digitalWrite(strobe, HIGH);
}
