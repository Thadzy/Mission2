// STEPPERMOTER1 SET PIN
const int ena = 9;
const int dir = 10;
const int step = 11; // pul
// STEPPERMOTER2 SET PIN
const int ena2 = 6;
const int dir2 = 7;
const int step2 = 8; // pul

const int led_yellow  = 3;
const int led_green  = 30;//change
const int led_red  = 5;


//Define variables  
const int microstep = 32;
const float step_angle = 1.8;

// Speed stepper change rpm
const int rpm = 10;  // 6 sec per round
const float pulse_rev = ( 360 * microstep )/ 1.8;
float pulse_sec = ( pulse_rev*rpm )/ 60; // 1066    
float delay_speed = ( 1 / ( 2 * pulse_sec )*( 1000000 ) );  //ใช้ได้แล้ว
float delay_speed_slow = ( delay_speed * 1.5 );

//degree input
float degree_want = 0 ;

//input
int limit_switch_x = 12;
int limit_switch_y = 13;//13
float value_use = 0.0;
int state = 1;

float value_degree;

unsigned long timestamp = millis();

void setup() 
{
  Serial.begin( 9600 );
  Serial.setTimeout( 10 );
  //Setup pin
  pinMode( step, OUTPUT ); 
  pinMode( dir, OUTPUT );
  pinMode( ena, OUTPUT );
  pinMode( step2, OUTPUT ); 
  pinMode( dir2, OUTPUT );
  pinMode( ena2, OUTPUT );

  pinMode( led_yellow, OUTPUT );
  pinMode( led_green, OUTPUT );
  pinMode( led_red, OUTPUT );

  pinMode( limit_switch_x, INPUT_PULLUP );
  pinMode( limit_switch_y, INPUT_PULLUP );
  

  digitalWrite( ena,LOW ); // active
  digitalWrite( ena2,HIGH );
  digitalWrite( dir, HIGH ); // LOW = CW  , HIGH = CCW 
  digitalWrite( dir2, HIGH );
  
}

void loop() 
{
  //Serial.println( degree_step );
  //Serial.println( delay_speed );
  int limit_switch_xvalue = digitalRead( limit_switch_x );
  int limit_switch_yvalue = digitalRead( limit_switch_y );
  /*if ( millis() - timestamp >= 1000 )
  {
    Serial.print( "degree_step : " );
    Serial.println( degree_step );

    Serial.print( "limit_switch_yvalue : " );
    Serial.println( limit_switch_yvalue );

    Serial.print( "limit_switch_xvalue : " );
    Serial.println( limit_switch_xvalue );

    Serial.println( "" );

    timestamp = millis();
  }*/
  
  if ( state == 1 )
  {
    state_1();
  }

  else if ( state == 2 )
  {
    state_2();
  }

  else if ( state == 3 )
  {
    state_3();
  }



}

void state_1() // sethome 
{
  int limit_switch_xvalue = digitalRead( limit_switch_x );
  int limit_switch_yvalue = digitalRead( limit_switch_y );
  if ( state == 1 and limit_switch_yvalue == 1 )// รอ define ว่า กด เป็น 0 หรือ 1 แต่อันนี้คือต้องไม่กด
  {
    digitalWrite( dir2,LOW ); //อันนี้ต้อง UP LOW = UP,DOWN ,HIGH = UP,DOWN 
    //Serial.println( "cordinate y" );
    while( 1 )
    {
      digitalWrite( led_red, LOW );
      int limit_switch_yvalue = digitalRead( limit_switch_y ); 
      //Serial.println( limit_switch_yvalue );
      digitalWrite( step2, HIGH ); 
      delayMicroseconds( delay_speed_slow ); 
      digitalWrite( step2, LOW ); 
      delayMicroseconds( delay_speed_slow ); 
      if ( limit_switch_yvalue == 0 )// รอ define ว่า กด เป็น 0 หรือ 1 แต่อันนี้คือต้องกด
      {
        digitalWrite( led_red, LOW );
        break;
      }
    }
  }
  //ปรับแกน y ชนด้านบนสุดเรียบร้อย
  digitalWrite( dir2, HIGH ); //อันนี้ต้อง DOWN LOW = UP,DOWN ,HIGH = UP,DOWN 
  for ( int i = 0; i < 200; i++ ) // หมุน value_degree องศา ตาม CW
  {
    digitalWrite( step2, HIGH ); 
    delayMicroseconds( delay_speed_slow ); 
    digitalWrite( step2, LOW ); 
    delayMicroseconds( delay_speed_slow ); 
  } 
  //ทำให้ความสูงเท่ากับ limitswitch เรียบร้อย


  //แกน x
  if ( state == 1 and limit_switch_xvalue == 1 ) // รอ define ว่า กด เป็น 0 หรือ 1 แต่อันนี้คือต้องไม่กด
  {
    digitalWrite( dir, LOW ); // หมุนทวนเข็มให้ชนลิมิต
    //Serial.println( "cordinate x" );
    while( 1 )
    {
      int limit_switch_xvalue = digitalRead( limit_switch_x ); 
      digitalWrite( step, HIGH ); 
      delayMicroseconds( delay_speed_slow ); 
      digitalWrite( step, LOW ); 
      delayMicroseconds( delay_speed_slow ); 
      if ( limit_switch_xvalue == 0 )
      {
        state = 2;
        digitalWrite( led_yellow, HIGH );
        break;
      }
    }
    

  }
  else if ( state == 1 and limit_switch_xvalue == 0 ) // กดลิมิตอยู่แล้ว
  {
    state = 2;
    digitalWrite( led_red, HIGH );
    
  }
}

void state_2() //idle
{
  while ( Serial.available() > 0  )
  {
    //value_degree = Serial.parseInt();
    value_degree = Serial.readString().toInt(); 
    if ( value_degree > 0 )
    {
      value_use = ( pulse_rev / ( 360 /value_degree ) ) * 3; //
      Serial.print( value_use );
      Serial.print( value_degree );
    }
    digitalWrite( led_yellow, LOW );
    state = 3;
    
  }

  /*value_degree = 180;//Serial.parseInt();
  if ( value_degree > 0 )
  {
    value_use = value_degree;
    degree_step = pulse_rev / ( 360 /value_use );
    Serial.println( degree_step );
    Serial.println( value_degree );
  }
  digitalWrite( led_yellow, LOW );
  state = 3;*/


  
}

void state_3() // Process จบอย่าลืม degree = 0
{
  digitalWrite( led_green, HIGH );
  int limit_switch_xvalue = digitalRead( limit_switch_x );
  digitalWrite( dir, HIGH ); // หมุนตามเข็ม // HIGH ตามเข็ม
  for ( int i = 0; i < value_use; i++ ) // หมุน value_degree องศา ตาม CW 
  {
    digitalWrite( step, HIGH ); 
    delayMicroseconds( delay_speed ); 
    digitalWrite( step, LOW ); 
    delayMicroseconds( delay_speed ); 
  } 
  delay( 1500 );
  //หมุนถึงจุดเป้าหมาย
  /*digitalWrite( dir2, LOW );  // LOW = UP,DOWN ,HIGH = UP,DOWN 
  for ( int i = 0; i < 3200; i++ ) // หมุนลง กี่เซน ต้องคุยกับเมคคานิกแล้วจะให้หมุนกี่องศา ตาม CW
  {
    digitalWrite( step2, HIGH ); 
    delayMicroseconds( delay_speed ); 
    digitalWrite( step2, LOW ); 
    delayMicroseconds( delay_speed ); 
  } 
  //จิ้มลงแล้ว
  digitalWrite( dir2, HIGH );
  for ( int i = 0; i < 3200; i++ ) // หมุนขึ้น กี่เซน ต้องคุยกับเมคคานิกแล้วจะให้หมุนกี่องศา ตาม CW
  {
    digitalWrite( step2, HIGH ); 
    delayMicroseconds( delay_speed ); 
    digitalWrite( step2, LOW ); 
    delayMicroseconds( delay_speed ); 
  } */
  //ยกขึ้นแล้ว
  digitalWrite( dir, LOW ); // หมุนกลับทวนเข็ม
  while( 1 )
    {
      digitalWrite( led_red, HIGH );
      int limit_switch_xvalue = digitalRead( limit_switch_x ); 
      digitalWrite( step, HIGH ); 
      delayMicroseconds( delay_speed ); 
      digitalWrite( step, LOW ); 
      delayMicroseconds( delay_speed ); 
      if ( limit_switch_xvalue == 0 )
      {
        state = 2;
        digitalWrite( led_yellow, HIGH );
        digitalWrite( led_red, LOW );
        digitalWrite( led_green, LOW );
        break;
        
      }
    }

}
// การแปลงค่าองศาหลังรับมาในโค้ด แปลงเลย
//test มีการหมุนกลับ  + หมุนตามองศายังไม่ไปตามนั้น
