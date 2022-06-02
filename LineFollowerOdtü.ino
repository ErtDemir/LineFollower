unsigned long now,last;
//MOTOR1
int motor1_left =  12;
int motor1_right =  13;
int enable1 =  11;
int motorright_speed;
int spd1 = 100;
//////////////////////////////////////////////
//MOTOR2
int motor2_left =8 ;
int motor2_right =9 ;
int enable2 =10 ;
int motorleft_speed;
int spd2 =150;
/////////////////////////////////////////////
//OTR-8A
int sensor0 = A2;
int sensor1 = A3;
int sensor2 = A4;
int sensor3 = A5;
int sensor4 = A6;
int sensor5 = A7;
/////////////////////////////////////////////
//PID


int  difference,lastdifference, output, difference2,lastdifference2, output2,D,I,D2,I2;
int Kp = 20;
int Ki = 15;
int Kd = 10;
int firsttime,lasttime,timechange;

int pos = 0 ;
int pos2 = 0;
int reference = 300;


int sayac = 0;
int sayac2 = 0;
int sayac3 = 0;
int sayac4 = 0;
int gate_speed_sag = 50;
int gate_speed_sol = 75;
int list[6];
/////////////////////////////////////////////////////////
//mz-80
int mz_80 = 2;
int value = 0;
///////////////////////////////////////////
//infraRed
int infraRed = 3;

/////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:

  pinMode(motor1_left, OUTPUT);
  pinMode(motor1_right, OUTPUT);
  pinMode(enable1, OUTPUT);
  /////////////////////////////////////////
  pinMode(motor2_right, OUTPUT);
  pinMode(motor2_left, OUTPUT);
  pinMode(enable2, OUTPUT);
  //////////////////////////////////////////
  pinMode(sensor0, INPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
  ////////////////////////////////////////////
  pinMode(2, INPUT);
  analogWrite(enable1,0 );
  analogWrite(enable2,0 );
  delay(200);



 Serial.begin(9600);

}
//////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){
  


  read_function();
  mz_read();

  
if (value == 0)    //engel varsa

    {
       if ( sayac == 0 )
       {
        turn_function();
        sayac2 = 1;
        
        }
       if ( sayac == 1 ) {
        gate_function();
        delay(5000);
        sayac2 = 2;
        }
       if ( sayac == 2 )
          {
              gate_function();
                  if (list[0] == 0 && list[1] == 0 && list[2] == 0 && list[3] == 0 && list[4] == 0 && list[5] == 0)
                  {
                    turn_function2();
                    sayac2 = 3; 
                   }
                   
           }

        sayac+=1;
        if (sayac > 2) {sayac = 2;}
        
    }



if (value == 1)    //engel yoksa

   {
     

      if (list[0] == 0 && list[1] == 0 && list[2] == 0 && list[3] == 0 && list[4] == 0 && list[5] == 0 && sayac2 == 3)
      {
        go(gate_speed_sag,gate_speed_sol);
        delay(400);
        gate_function();
        gate_speed_sag = 0;
        gate_speed_sol = 0;
        sayac2 = 4;
        }
         if (sayac2 != 4)
         {
           if((sayac == 1) && (list[0] == 0 && list[1] == 0 && list[2] == 1 && list[3] == 1 && list[4] == 1 && list[5] == 1))

            {
                 while(list[1] == 0)
                  {
                    go_geri(50,75);
                    read_function();
                    }
                    
                    while((list[0] != 1 && list[1] != 1 && list[2] != 1 && list[3] != 1 && list[4] != 0 && list[5] != 0))
                  {
                    followLine();
                    }
                   while(list[5] == 0)
                  {
                    go_geri(50,75);
                    read_function();
                    }
                    
          
              }
          

            followLine();
          }
            

}
        
      


        

    

}




////////////////////////////////////////////
void read_function()
{
  list[0] = analogRead(sensor0);
  list[1] = analogRead(sensor1);
  list[2] = analogRead(sensor2);
  list[3] = analogRead(sensor3);
  list[4] = analogRead(sensor4);
  list[5] = analogRead(sensor5);

  for (int i = 0 ; i < 6 ; i++ )
  {
    if (list[i] > referen
ce) {
      list[i] = 0;
    }
    else   {
      list[i] = 1;
    }
  }

}
///////////////////////////
void read_function2()
{
  list[0] = analogRead(sensor0);
  list[1] = analogRead(sensor1);
  list[2] = analogRead(sensor2);
  list[3] = analogRead(sensor3);
  list[4] = analogRead(sensor4);
  list[5] = analogRead(sensor5);

  for (int i = 0 ; i < 6 ; i++ )
  {
    if (list[i] < reference) {
      list[i] = 0;
    }
    else   {
      list[i] = 1;
    }



  }

}
///////////////////////////////////////////////

int get_position()
{
  read_function();

  int a0 = list[0];
  int a1 = list[1];
  int a2 = list[2];
  int a3 = list[3];
  int a4 = list[4];
  int a5 = list[5];

  if (a0 = 0 && a1 == 0 && a2 == 0 && a3 == 0 && a4 == 0 && a5 == 1  ) { pos = 5;}       //000001
  else if (a0 == 0 && a1 == 0 && a2 == 0 && a3 == 0 && a4 == 1 && a5 == 1  ) { pos = 4;} //000011
  else if (a0 == 0 && a1 == 0 && a2 == 0 && a3 == 0 && a4 == 1 && a5 == 0  ) { pos = 3;} //000010
  else if (a0 == 0 && a1 == 0 && a2 == 0 && a3 == 1 && a4 == 1 && a5 == 0  ) { pos = 2;} //000110
  else if (a0 == 0 && a1 == 0 && a2 == 0 && a3 == 1 && a4 == 0 && a5 == 0  ) { pos = 1;} //000100
  else if (a0 == 0 && a1 == 0 && a2 == 1 && a3 == 1 && a4 == 0 && a5 == 0  ) {pos = 0; } //001100
  else if (a0 == 0 && a1 == 0 && a2 == 1 && a3 == 0 && a4 == 0 && a5 == 0  ) {pos = -1;} //001000
  else if (a0 == 0 && a1 == 1 && a2 == 1 && a3 == 0 && a4 == 0 && a5 == 0  ) {pos = -2;} //011000
  else if (a0 == 0 && a1 == 1 && a2 == 0 && a3 == 0 && a4 == 0 && a5 == 0  ) {pos = -3;} //010000
  else if (a0 == 1 && a1 == 1 && a2 == 0 && a3 == 0 && a4 == 0 && a5 == 0  ) {pos = -4;} //110000
  else if (a0 == 1 && a1 == 0 && a2 == 0 && a3 == 0 && a4 == 0 && a5 == 0  ) {pos = -5;} //100000
  //else if (a0 == 1 && a1 == 1 && a2 == 1 && a3 == 1 && a4 == 1 && a5 == 0  ) {pos = 2;} //100000
  //else if (a0 == 1 && a1 == 1 && a2 == 1 && a3 == 1 && a4 == 0 && a5 == 0  ) {pos = 2;} //100000
  //else if (a0 == 0 && a1 == 1 && a2 == 1 && a3 == 1 && a4 == 1 && a5 == 1  ) {pos = 2;} //100000
  else if (a0 == 0 && a1 == 0 && a2 == 0 && a3 == 0 && a4 == 0 && a5 == 0  )             //000000
  {
    if (pos < 0) {
      pos = -5;
    }

    else if (pos > 0) {
      pos = 5;
    }

  }
  return pos ;

}

int get_position2()
{
  read_function2();

  int a0 = list[0];
  int a1 = list[1];
  int a2 = list[2];
  int a3 = list[3];
  int a4 = list[4];
  int a5 = list[5];

  if (a0 = 0 && a1 == 0 && a2 == 0 && a3 == 0 && a4 == 0 && a5 == 1  ) { pos = 7;}       //000001
  else if (a0 == 0 && a1 == 0 && a2 == 0 && a3 == 0 && a4 == 1 && a5 == 1  ) { pos = 6;} //000011
  else if (a0 == 0 && a1 == 0 && a2 == 0 && a3 == 1 && a4 == 1 && a5 == 1  ) { pos = 5;} //000111
  else if (a0 == 0 && a1 == 0 && a2 == 0 && a3 == 0 && a4 == 1 && a5 == 0  ) { pos = 4;} //000010
  else if (a0 == 0 && a1 == 0 && a2 == 0 && a3 == 1 && a4 == 1 && a5 == 0  ) { pos = 3;} //000110
  else if (a0 == 0 && a1 == 0 && a2 == 1 && a3 == 1 && a4 == 1 && a5 == 0  ) { pos = 2;} //001110
  else if (a0 == 0 && a1 == 0 && a2 == 0 && a3 == 1 && a4 == 0 && a5 == 0  ) { pos = 1;} //000100
  else if (a0 == 0 && a1 == 0 && a2 == 1 && a3 == 1 && a4 == 0 && a5 == 0  ) {pos = 0; } //001100
  else if (a0 == 0 && a1 == 0 && a2 == 1 && a3 == 0 && a4 == 0 && a5 == 0  ) {pos = -1;} //001000
  else if (a0 == 0 && a1 == 1 && a2 == 1 && a3 == 1 && a4 == 0 && a5 == 0  ) { pos = -2;} //011100
  else if (a0 == 0 && a1 == 1 && a2 == 1 && a3 == 0 && a4 == 0 && a5 == 0  ) {pos = -3;} //011000
  else if (a0 == 0 && a1 == 1 && a2 == 0 && a3 == 0 && a4 == 0 && a5 == 0  ) {pos = -4;} //010000
  else if (a0 == 1 && a1 == 1 && a2 == 1 && a3 == 0 && a4 == 0 && a5 == 0  ) { pos = -5;} //111000
  else if (a0 == 1 && a1 == 1 && a2 == 0 && a3 == 0 && a4 == 0 && a5 == 0  ) {pos = -6;} //110000
  else if (a0 == 1 && a1 == 0 && a2 == 0 && a3 == 0 && a4 == 0 && a5 == 0  ) {pos = -7;} //100000
  else if (a0 == 0 && a1 == 0 && a2 == 0 && a3 == 0 && a4 == 0 && a5 == 0  )             //000000
  {
    if (pos < 0) {
      pos = -7;
    }

    else if (pos > 0) {
      pos = 7;
    }

  }
return pos ;

}



void followLine() {

   firsttime = millis();
  double timechange = (double)(firsttime - lasttime)/1000;
  pos = get_position();
  difference = 0 - pos;
  
  D = (difference - lastdifference)/timechange;
  I += (difference*timechange);
  
  output = Kp * difference + Kd*D + Ki*I;
  lastdifference = difference;
  lasttime = firsttime;
  
  
  motorright_speed = spd1 + output;
  motorleft_speed = spd2 - output;
  go(motorright_speed, motorleft_speed);

}

void followLine2 () {

  pos = get_position2();

  difference = 0 - pos;

  output = Kp * difference;
  motorright_speed = spd1 + output;
  motorleft_speed = spd2 - output;
  go(motorright_speed, motorleft_speed);
  
}

void go (int motor1, int motor2)

{
  if (motor1 > 255){motor1 = 255;}
  if (motor2 > 255){motor2 = 255;}

  analogWrite(enable1, motor1);
  analogWrite(enable2, motor2);
 digitalWrite(motor1_right, HIGH);
  digitalWrite(motor1_left, LOW);
  digitalWrite(motor2_right, HIGH);
  digitalWrite(motor2_left, LOW);
  
}

void go_geri (int motor1, int motor2)

{
  if (motor1 > 255){motor1 = 255;}
  if (motor2 > 255){motor2 = 255;}

  analogWrite(enable1, motor1);
  analogWrite(enable2, motor2);
 digitalWrite(motor1_right, LOW);
  digitalWrite(motor1_left, HIGH);
  digitalWrite(motor2_right, HIGH);
  digitalWrite(motor2_left, LOW);
  
}
void go_geri_2 (int motor1, int motor2)

{
  if (motor1 > 255){motor1 = 255;}
  if (motor2 > 255){motor2 = 255;}

  analogWrite(enable1, motor1);
  analogWrite(enable2, motor2);
 digitalWrite(motor1_right, HIGH);
  digitalWrite(motor1_left, LOW);
  digitalWrite(motor2_right, LOW);
  digitalWrite(motor2_left, HIGH);
  
}
///////////////////////////////////////////////////////
//MZ-80 READ
int mz_read()
{
  value = digitalRead(mz_80);

}
////////////////////////////////////////////

int turn_function()
{analogWrite(enable1,100 );
       digitalWrite(motor1_right, HIGH);
       digitalWrite(motor1_left, LOW);

       analogWrite(enable2,80 );
       digitalWrite(motor2_right, HIGH);
       digitalWrite(motor2_left, LOW);
       delay(400);
       read_function();
       while(list[5] == 0  )
       {
        go(50,75);
        read_function();
       }
       read_function();
       while(list[1] == 0)
       {
        go(30,90);
        read_function();
       }
       
        while(list[5] == 0)
       {
        go(50,90);
        read_function();
       }
       
      



  
}

 void turn_function2()
 {
analogWrite(enable1,100 );
       digitalWrite(motor1_right, HIGH);
       digitalWrite(motor1_left, LOW);

       analogWrite(enable2,80 );
       digitalWrite(motor2_right, HIGH);
       digitalWrite(motor2_left, LOW);
       delay(300);
       read_function();
       while(list[5] == 0  )
       {
        go(50,75);
        read_function();
       }
       read_function();
       while(list[1] == 0)
       {
        go(30,90);
        read_function();
       }


  }
  
 

void gate_function()
  {

    analogWrite(enable1,50 );
    digitalWrite(motor1_right, HIGH);
    digitalWrite(motor1_left, HIGH);

    analogWrite(enable2,50 );
    digitalWrite(motor2_right, HIGH);
    digitalWrite(motor2_left, HIGH);
    
   }
