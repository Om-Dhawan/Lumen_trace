#include <Servo.h>
#define S2 6
#define S3 7
#define sensorOut1 8
#define sensorOut2 9
#define sensorOut3 12
#define servo 13
int IN1 = 10;
int IN2 = 3;
int IN3 = 11;
int IN4 = 5;

int en1 = 1;
int en2 = 1;

Servo myservo;
int angle = 70;
// myservo.write(angle);

int sensorMap[3] = {8,9,12};

// Stores frequency read by the photodiodes

char colorName[6][10] = {"Black","Red","Yellow","Blue","Green","White"};
char colorBox[4][10] = {"Red","Yellow","Blue","Green"};


/*color code***
 * Black -0
 * RED - 1
 * YEllow - 2
 * BLUE - 3
 * Green - 4
 * White - 5
 */

// color range map
int colorMap[3][6][3]={{{160,177,141},{55,111,92},{43,49,60},{117,130,78},{81,70,80},{39,39,32}},{{145,166,126},{53,106,85},{46,55,66},{113,129,75},{78,69,76},{38,38,31}},{}};
int boxMap[4][3];

void readColor(int sensor, int &r , int &g ,int &b)
{
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  r = pulseIn(sensorMap[sensor],LOW);
  delay(10);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  g = pulseIn(sensorMap[sensor],LOW);
  delay(10);
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  b = pulseIn(sensorMap[sensor],LOW);
  delay(10);
  return ;
}

// see some predefined value (only for 4 colors)
int findBoxColor(){
  int r, g, b, color=0;
  readColor(2,r,g,b);
  for(int i=0;i<4;i++){
    if(r>boxMap[i][0]-20 && r<boxMap[i][0]+20 && g>boxMap[i][1]-20 && g<boxMap[i][1]+20 && b>boxMap[i][2]-20 && b<boxMap[i][2]+20){
      return i+1;
    }
  }
  return -1;
}

int findColor(int sensor)
{
  int r, g, b;
  readColor(sensor,r,g,b);
  // using minimum distance
  double minDis = 10000;
  int color;
  for(int clr = 0 ; clr<=5;clr++)
  {
    double dis =0;
    dis += (colorMap[sensor][clr][0] - r)*(colorMap[sensor][clr][0] - r);
    dis += (colorMap[sensor][clr][1] - g)*(colorMap[sensor][clr][1] - g);
    dis += (colorMap[sensor][clr][2] - b)*(colorMap[sensor][clr][2] - b);
    dis = sqrt(dis);

    if(dis<minDis)
    {
      color = clr;
      minDis = dis;
    }
  }
  return color;
}



// calibration function
void calibrate(int sensor)
{
  int delayy = 2000;
  int caltime = 3000;
  //delay 3 black 5 delay 3 red 5 delay 3 yellow 5 delay 3 blue 5 delay 3 green 5 delay 3 white 5  
  for(int clr =0;clr<=5;clr++)
  {
    Serial.println("pause");
    delay(delayy);
    Serial.println(colorName[clr]);
    long start = millis();
    int n =0;
    while(millis() - start<caltime)
    {
      int r, g, b;
      readColor(sensor,r,g,b);
      n++;
      boxMap[clr][0]+=r;
      boxMap[clr][0]+=g;
      boxMap[clr][0]+=b;
    }
    boxMap[clr][0]/=n;
    boxMap[clr][0]/=n;
    boxMap[clr][0]/=n;  
  }
  Serial.println("done");
}

// calibration function
void calibrate2(int sensor)
{
  int delayy = 2000;
  int caltime = 3000;
  //delay 3 black 5 delay 3 red 5 delay 3 yellow 5 delay 3 blue 5 delay 3 green 5 delay 3 white 5  
  for(int clr =0;clr<=3;clr++)
  {
    Serial.println("pause");
    delay(delayy);
    Serial.println(colorBox[clr]);
    long start = millis();
    int n =0;
    while(millis() - start<caltime)
    {
      int r, g, b;
      readColor(sensor,r,g,b);
      n++;
      colorMap[sensor][clr][0]+=r;
      colorMap[sensor][clr][1]+=g;
      colorMap[sensor][clr][2]+=b;
    }
    colorMap[sensor][clr][0]/=n;
    colorMap[sensor][clr][1]/=n;
    colorMap[sensor][clr][2]/=n;  
  }
  Serial.println("done");
}

void setup() {
  // Setting the outputs
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut1, INPUT);
  pinMode(sensorOut2, INPUT);
  pinMode(sensorOut3, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  myservo.attach(servo);
  myservo.write(angle);
  
  // Setting frequency scaling to 20%

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
   // Begins serial communication 
  Serial.begin(9600);
  // calibrate(0);
  // calibrate(1);
  // calibrate2(2);
}
 
void moveForward()
{
  analogWrite(en1,100);
  analogWrite(en2,100);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  // analogWrite(IN1,100);
  // analogWrite(IN2,0);
  // analogWrite(IN3,100);
  // analogWrite(IN4,0);
 }
void turnLeft()
{
  analogWrite(en1,100);
  analogWrite(en2,100);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  // analogWrite(IN1,100);
  // analogWrite(IN2,0);
  // analogWrite(IN3,0);
  // analogWrite(IN4,100);
}
void turnRight()
{
  analogWrite(en1,100);
  analogWrite(en2,100);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  // analogWrite(IN1,0);
  // analogWrite(IN2,100);
  // analogWrite(IN3,100);
  // analogWrite(IN4,0);
}
void moveBack()
{
  analogWrite(en1,100);
  analogWrite(en2,100);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}
void dropBox()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  myservo.write(50);
  moveBack();
  delay(1000);
  turnRight();
  delay(300);
  // while(c2!=color){
  //   turnRight();
  // }
}

void followColoredLine(int color)
{
  
  int c1 = findColor(0);
  int c2 = findColor(1);

  Serial.print("sensor 1 : ");
  Serial.println(colorName[c1]);
  Serial.print("sensor 2 : ");
  Serial.println(colorName[c2]);

  if(c1==0&&c2==0)
  {
    Serial.println("Moving Forward");
    moveForward();
  }
  if(c1==color&&c2==color)
  {
    Serial.println("putting box and stop");
    dropBox();
    while(findColor(0)!=color){
      turnRight();
    }
    
      followBackLine(color);
    
    return ;
  }
  else if(c1==color)
  {
    Serial.println("turn right correction");
    turnRight();
  }
  else if(c2==color)
  {
    Serial.println("turn left correction");
    turnLeft();
  }
  else if(c1==0&&(c2!=0&&c2!=color))
  {
    Serial.println("turn right correction");
    turnRight();
  }
  else if(c2==0&&(c1!=0&&c1!=color))
  {
    Serial.println("turn left correction");
    turnLeft();
  }
  else
  {
    Serial.println("move forward");
    moveForward();
  }
  delay(50);
  followColoredLine(color);
}

void stop(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  myservo.write(70);
  delay(20);
  moveBack();
  delay(500);
  turnRight();
  delay(800);
}
void stopRev(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}

void followBackLine(int color)
{
  
  int c1 = findColor(0);
  int c2 = findColor(1);

  Serial.print("sensor 1 : ");
  Serial.println(colorName[c1]);
  Serial.print("sensor 2 : ");
  Serial.println(colorName[c2]);

  if(c1==0&&c2==0)
  {
    Serial.println("stop");
    stopRev();
    return;
  }
  if(c1==color&&c2==color)
  {
    Serial.println("Forward");
    moveForward();
  }
  else if(c1==0)
  {
    Serial.println("turn right correction");
    turnRight();
  }
  else if(c2==0)
  {
    Serial.println("turn left correction");
    turnLeft();
  }
  else if(c1==color &&(c2!=0&&c2!=color))
  {
    Serial.println("turn right correction");
    turnRight();
  }
  else if(c2==color &&(c1!=0&&c1!=color))
  {
    Serial.println("turn left correction");
    turnLeft();
  }
  else
  {
    Serial.println("move forward");
    moveForward();
  }
  delay(50);
  followBackLine(color);
}

void loop() {
  Serial.print("sensor 1 : ");
  Serial.println(colorName[findColor(0)]);
  Serial.print("sensor 2 : ");
  Serial.println(colorName[findColor(1)]);

  // int boxColor = findBoxColor(2);
  // if(boxColor>=0){
  //   followColoredLine(boxColor);
  // }
  followColoredLine(1); 
  delay(10000);
  /*color code***
 * Black -0
 * RED - 1
 * YEllow - 2
 * BLUE - 3
 * Green - 4
 * White - 5
 */
}