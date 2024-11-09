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
const float HERTZ = 1E+6f / (2.0f * PULSE_SEC); // delayspeed
// MicroSecond per Pulse (Slow)
const float HERTZ_SLOW = HERTZ * 1.5;
// Distance headpen from ground =
const float DISTANCE = (4.5)*PULSE_REV; // 6400 : 1 cm //4.2

float distance_offset;

/*
@param pin Pulse PIN
@param hertz Frequency 1 pulse per microsecond
*/
void Move(int pin, float hertz ,int dir , int pin_dir) // pin คือ เลือกแกน ที่จะขยับ + ความเร็ว 
{
  digitalWrite(pin_dir, dir); // / 10 X_DIR 0 CCW , 1 CW  / 7 Y_DIR 0 UP, 1 DOWN/
  digitalWrite(pin, HIGH); 
  delayMicroseconds(hertz); 
  digitalWrite(pin, LOW); 
  delayMicroseconds(hertz); 
}

void MovePulse(int pin, float hertz, int dir , int pin_dir , float pulse)
{
  for (float i = 0; i < pulse; i += 1.0f) Move(pin, hertz, dir, pin_dir);
}

void MovePulsereturn(int pin, float hertz, int dir , int pin_dir)
{
  while (digitalRead(LIMIT_X) != 0) Move(pin, hertz, dir, pin_dir);
}

void MoveTrapezoid(int pin, int dir , int pin_dir , float pulse) // pin คือ เลือกแกน ที่จะขยับ + ความเร็ว 
{
  float hertznew = 300;  
  for (float i = 0; i < pulse; i += 1.0f) // / 10 X_DIR 0 CCW , 1 CW  / 7 Y_DIR 0 UP, 1 DOWN/ 
  {
    Move(pin, hertznew, dir, pin_dir);
    if ( ( i < (2 * pulse) / 3 ) && pin == Y_PUL) //เร็ว 
    {
      hertznew = 1;
    } 
    else if ( ( i < (2.5*pulse) / 3 ) && pin == X_PUL) //เร็ว 
    {
      hertznew = 200; //33 Base : 150  //100  //150
    }
    else if ( ( i > (2.5*pulse) / 3 ) && pin == X_PUL) //ช้า
    {
      hertznew = 100; //600    // ช้ามากกว่าเร็ว จะหมุนไม่ถึง   เร็วมากกว่าช้า
    }
    else // ช้า
    {
      hertznew = 1;
    }

  }
  
}

void MoveTrapezoidreturn(int pin, int dir , int pin_dir , float pulse) // pin คือ เลือกแกน ที่จะขยับ + ความเร็ว 
{
  float hertznew = 300;  
  for (float i = 0; i < pulse; i += 1.0f) // / 10 X_DIR 0 CCW , 1 CW  / 7 Y_DIR 0 UP, 1 DOWN/ 
  {
    Move(pin, hertznew, dir, pin_dir);
    if ( ( i < (2.5*pulse) / 3 ) && pin == X_PUL) //เร็ว 
    {
      hertznew = 100;
    }
    else if ( ( i > (2.2*pulse) / 3 ) && pin == X_PUL) //ช้า
    {
      while (digitalRead(LIMIT_X) != 0) Move(X_PUL, 300, 0, X_DIR); // หมุนทวนเข็ม ขณะที่ยังไม่ชน limit x ให้หมุนทวนเข็มไปเรื่อยๆ
      break;
    }
  }

}
  


/*
Initialize at home
*/
void Home() //การ set home
{
  if (digitalRead(LIMIT_Y) == 1) // ไม่ชน limit y แกน 0
  { //ปรับทิศทางการหมุนเป็นหมุนขึ้น
    while (digitalRead(LIMIT_Y) != 0) Move(Y_PUL, 200, 0, Y_DIR); // หมุนขึ้น ขณะที่ยังไม่ชน limit y ให้หมุนขึ้นไปเรื่อยๆที่ y ด้วยความเร็วช้า
  }  //ปรับทิศทางการหมุนเป็นหมุนลง
  //for (int i = 0; i < 200; i++) Move(Y_PUL, HERTZ_SLOW, 1 , 7); // หมุนลง ขณะที่ยังไม่ชน limit y ให้หมุนลงไป ตาม set up //

  if (digitalRead(LIMIT_X) == 1 ) // limit แกน x ยังไม่ชน
  {
    while (digitalRead(LIMIT_X) != 0) Move(X_PUL, 420, 0, X_DIR); // หมุนทวนเข็ม ขณะที่ยังไม่ชน limit x ให้หมุนทวนเข็มไปเรื่อยๆ
  }
}

/*
Wait for the user to enter the angle and convert it to pulse
*/
float WaitForPulse() 
{
  while (Serial.available() > 0) //รอรับค่า
  {
    float degree = Serial.readString().toFloat(); //อ่านค่า degree 
    if (degree >= 0.0f) return ((PULSE_REV / (360.0 / degree)) * 3 ); 
  }
  return -1.0f;
}

/*
Move to the target and move the pen down to press and turn back to home
@param pulse Pulse at target
*/

void Score(float pulse) //การทำคะแนน
{
  distance_offset = DISTANCE + ( ( pulse * 0.00019 )*PULSE_REV ); // degree * 0.00156     5 มิล  ใส่ 180 /// 0.00007
  if (pulse == 0.0f)
  {
    float X_OFFSET = 250.0f;
    float Y_OFFSET = (2 *distance_offset) / 3;
    MovePulse(X_PUL, 600, 1, X_DIR, X_OFFSET);
    MovePulse(Y_PUL, 40, 1, Y_DIR, Y_OFFSET); //40
    MovePulse(X_PUL, 600, 0, X_DIR, X_OFFSET);
    MovePulse(Y_PUL, 300, 1, Y_DIR, distance_offset - Y_OFFSET);

    MovePulse(Y_PUL, 300, 0, Y_DIR, distance_offset - Y_OFFSET);
    MovePulse(X_PUL, 600, 1, X_DIR, X_OFFSET);
    MovePulse(Y_PUL, 40, 0, Y_DIR, Y_OFFSET);
    MovePulsereturn(X_PUL ,600 ,0 ,X_DIR);
    return;
  }
  MoveTrapezoid( X_PUL ,1 ,X_DIR ,pulse  );
  delay(250); // รอ 1.5 วิ
   // หมุนลง x cm
  MoveTrapezoid( Y_PUL ,1 ,Y_DIR ,distance_offset  );  // (int pin,int dir , int pin_dir , float pulse)
  MoveTrapezoid( Y_PUL ,0 ,Y_DIR ,distance_offset  );
  MoveTrapezoidreturn( X_PUL ,0 ,X_DIR ,pulse  );
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

float pulseS = -1.0f;
void loop() 
{
  if (pulseS == -1.0f) pulseS = WaitForPulse();
  else 
  {
    Score(pulseS);
    pulseS = -1.0f;
  }
}