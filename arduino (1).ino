// X-axis stepper motor PIN
const int X_ENA = 9,
          X_DIR = 10,
          X_PUL = 11;

// Y-axis stepper motor PIN
const int Y_ENA = 6,
          Y_DIR = 7,
          Y_PUL = 8;

// Limit switch PIN
const int LIMIT_X = 12,
          LIMIT_Y = 13;

// LED PIN
const int LED_YELLOW = 3,
          LED_GREEN = 30,
          LED_RED = 5;

const float MICRO_STEP = 32.0f;
// Revolution per Minute
const float RPM = 10.0f;
// Angle per Step
const float ANGLE_STEP = 1.8;
// Pulse per Revolution
const float PULSE_REV = (360.0f * MICRO_STEP) / ANGLE_STEP;
// Pulse per Second
const float PULSE_SEC = (PULSE_REV * RPM) / 60.0f;
// MicroSecond per Pulse
const float HERTZ = (1.0f / (2.0f * PULSE_SEC)) * 1E-6;
// MicroSecond per Pulse (Slow)
const float HERTZ_SLOW = HERTZ * 1.5;

/*
@param pin Pulse PIN
@param hertz Frequency 1 pulse per microsecond
*/
void Move(int pin, float hertz)
{
  digitalWrite(pin, HIGH); 
  delayMicroseconds(hertz); 
  digitalWrite(pin, LOW); 
  delayMicroseconds(hertz); 
}

/*
Initialize at home
*/
void Home()
{
  if (digitalRead(LIMIT_Y) == 1)
  {
    digitalWrite(Y_DIR, LOW);
    while (digitalRead(LIMIT_Y) != 0) Move(Y_PUL, HERTZ_SLOW);
  }

  digitalWrite(Y_DIR, HIGH);
  for (int i = 0; i < 200; i++) Move(Y_PUL, HERTZ_SLOW);


  if (digitalRead(LIMIT_X) == 1 )
  {
    digitalWrite(X_DIR, LOW);
    while (digitalRead(LIMIT_X) != 0) Move(X_PUL, HERTZ_SLOW);
  }
}

/*
Wait for the user to enter the angle and convert it to pulse
*/
float WaitForPulse()
{
  float pulse = 0.0f;
  while (Serial.available() > 0)
  {
    float degree = Serial.readString().toInt(); 
    if (degree > 0.0f) pulse = (PULSE_REV / (360.0 /degree)) * 3;
    return pulse;
  }
  return 0.0;
}

/*
Move to the target and move the pen down to press and turn back to home
@param pulse Pulse at target
*/
void Score(float pulse)
{
  digitalWrite(X_DIR, HIGH);
  for (float i = 0.0; i < pulse; i += 1.0f) Move(X_PUL, HERTZ);
  delay(1500);

  digitalWrite(X_DIR, LOW);
  while (digitalRead(LIMIT_X) != 0) Move(X_PUL, HERTZ);
}

void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(10);

  // Set the x-axis stepper motor PIN
  pinMode(X_ENA, OUTPUT);
  pinMode(X_DIR, OUTPUT);
  pinMode(X_PUL, OUTPUT); 

  // Set the y-axis stepper motor PIN
  pinMode(Y_ENA, OUTPUT);
  pinMode(Y_DIR, OUTPUT);
  pinMode(Y_PUL, OUTPUT); 

  // Set LED PIN
  pinMode(LIMIT_X, INPUT_PULLUP);
  pinMode(LIMIT_Y, INPUT_PULLUP);

  // Set Limit switch PIN
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  
  // ENA | LOW = Active, HIGH = InActive 
  // DIR | LOW = CW, HIGH = CCW 
  digitalWrite(X_ENA, LOW);
  digitalWrite(X_DIR, HIGH); 
  digitalWrite(Y_ENA, HIGH);
  digitalWrite(Y_DIR, HIGH);

  Home();
}

void loop() 
{
  float pulse = 0.0f;
  
  if (pulse == 0.0f) pulse = WaitForPulse();
  else 
  {
    Score(pulse);
    pulse = 0.0f;
  }
}