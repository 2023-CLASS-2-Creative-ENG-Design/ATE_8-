#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial BTSerial(2, 3);

int pinLB = 4;
int pinLF = 5;
int pinRB = 6;
int pinRF = 7;

int inputPin = 11;
int outputPin = 10;

int Fspeedd = 0;
int Rspeedd = 0;
int Lspeedd = 0;
int directionn = 0;

Servo myservo;

int delay_time = 250;

int Fgo = 8;
int Rgo = 6;
int Lgo = 4;
int Bgo = 2;

bool autonomousMode = false; 

void setup()
{
  BTSerial.begin(9600);
  Serial.begin(9600);

  pinMode(pinLB, OUTPUT);
  pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT);
  pinMode(pinRF, OUTPUT);

  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);

  myservo.attach(9);
}

void handleBluetoothCommands()
{
  if (BTSerial.available())
  {
    char cmd = (char)BTSerial.read();
    Serial.println(cmd);

    if (cmd == 'F')
    {
      forward(1);
    }
    else if (cmd == 'R')
    {
      right(1);
    }
    else if (cmd == 'L')
    {
      left(1);
    }
    else if (cmd == 'B')
    {
      back(1);
    }
    else if (cmd == 'S')
    {
      stopp(1);
      autonomousMode = false;
    }
    else if (cmd == 'A')
    {
      autonomousMode = !autonomousMode;
    }
  }
}
void forward(int a) {
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);

  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);

  delay(a * 1);
}

void right(int b) {
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, LOW);

  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);

  delay(b * 100);
}

void left(int c) {
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);

  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, LOW);

  delay(c * 100);
}

void turnR(int d) {
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);

  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, LOW);

  delay(d * 50);
}

void turnL(int e) {
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, LOW);

  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);

  delay(e * 50);
}

void stopp(int f) {
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, LOW);

  delay(f * 100);
}

void back(int g) {
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, LOW);

  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, LOW);

  delay(g * 100);
}

void detection() {
  int delay_time = 200;
  ask_pin_F();
  
  if (Fspeedd < 10) {
    stopp(1);
    back(5);
  }

  if (Fspeedd < 20) {
    stopp(1);
    ask_pin_L();
    
    delay(delay_time);
    
    ask_pin_R();
    delay(delay_time);
    
    if (Lspeedd > Rspeedd) {
      directionn = Lgo;
    }

    if (Lspeedd <= Rspeedd) {
      directionn = Rgo;
    }

    if (Lspeedd < 15 && Rspeedd < 15) {
      directionn = Bgo;
    }
  }

  if (Fspeedd >= 20) {
    directionn = Fgo;
  }
}

void ask_pin_F() {
  myservo.write(90);

  digitalWrite(outputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW);

  float Fdistance = pulseIn(inputPin, HIGH);
  Fdistance = Fdistance / 5.8 / 10;

  Serial.print("F distance:");
  Serial.println(Fdistance);

  Fspeedd = Fdistance;
}

void ask_pin_L() {
  myservo.write(5);

  delay(delay_time);

  digitalWrite(outputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW);

  float Ldistance = pulseIn(inputPin, HIGH);
  Ldistance = Ldistance / 5.8 / 10;

  Serial.print("L distance:");
  Serial.println(Ldistance);

  Lspeedd = Ldistance;
}

void ask_pin_R() {
  myservo.write(177);

  delay(delay_time);

  digitalWrite(outputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW);

  float Rdistance = pulseIn(inputPin, HIGH);
  Rdistance = Rdistance / 5.8 / 10;

  Serial.print("R distance:");
  Serial.println(Rdistance);

  Rspeedd = Rdistance;
}
void autonomousModeControl()
{
  myservo.write(90);
  detection();

  if (directionn == 2)
  {
    back(10);
    turnL(2);
    Serial.print(" Reverse ");
  }
  else if (directionn == 6)
  {
    turnR(2);
    Serial.print(" Right ");
  }
  else if (directionn == 4)
  {
    turnL(2);
    Serial.print(" Left ");
  }
  else if (directionn == 8)
  {
    forward(1);
    Serial.print(" Forward ");
    Serial.print(" ");
  }
}
void loop()
{
  handleBluetoothCommands();

  if (autonomousMode)
  {
    autonomousModeControl();
  }
}