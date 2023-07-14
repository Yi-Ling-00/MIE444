//Ultrasonic -- in order of clockwise from FW
#include <Servo.h>

#define trigPin 26
#define U1 28//0
#define U2 29
#define U3 30//90
#define U4 31//180
#define U5 32//270
#define U6 33


//Motor controls --- A left, B right
#define IN1 46 //A
#define IN2 48 //A
#define IN3 50 //B
#define IN4 52 //B
//Motor speed
#define PWMA 2 //Left wheel
#define PWMB 3 //Right wheel

//IR Line sensors i

#define L1 8 //Left line sensor
#define L2 9 //right line sensor
//IR Distance sensors
#define IR A0


//Servos---------------------------
Servo myservo;
int pos = 0;

//Variables------------------------------------------------
bool go = false;

//Sensor distance reading
long d1=9999;
long d2=9999;
long d3=9999;
long d4=9999;
long d5=9999;
long d6=9999;

// Adjusted Distances = measured + offset from center of robot (5.5cm)
long d_1=9999;
long d_2=9999;
long d_3=9999;
long d_4=9999;
long d_5=9999;
long d_6=9999;

//prev reading
long prev1;
long prev2;
long prev3;
long prev4;
long prev5;
long prev6;

//difference reading  
long diff1;
long diff2;
long diff3;
long diff4;
long diff5;
long diff6;
long pingTime;
//speed
long mult = 1; //use for speed adjustment in case of weird battery
int s1 = 75;
int s2 = 72;

//Direction 
//-----------------------------------------------------------
//-1 Backward   0 Stop    1 Forward     2 Left    3 Right     
int dir = 0; 

//timer -----------------------------------------------------
unsigned long prevMillis;
int m = 1;
int i = 0;

//distance sensor for block
int IR_value;
int IR_dist;

// Robot location
int x = 0;
int y = 0;
int theta = 0;
int tracking_x = 0;
int tracking_y = 0;

// Robot status
int align = 0;
int centered = 0;
int localize = 0;

// Destination
int pickup = 3;
int dropoff = 5;

// Calibration values
int err = 3; // error value in cm
float check = 0; // stop and take new path when seeing 'check' number of blocks in front
float check_back = 5; // stop and take new path when seeing 'check' number of blocks behind

// IR readings
bool i1 = 0;
bool i2 = 0;

//variables to check straight motion
long side;
long left;
long right;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(U1, INPUT);
  pinMode(U2, INPUT);
  pinMode(U3, INPUT);
  pinMode(U4, INPUT);
  pinMode(U5, INPUT);
  pinMode(U6, INPUT);
  pinMode(trigPin, OUTPUT);

  pinMode(PWMA,OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  pinMode(L1, INPUT);
  pinMode(L2, INPUT);
  pinMode(IR, INPUT);
  myservo.attach(4);
  myservo.write(0);

}

void loop() {
  getDistance(); //get ultrasonic sensors
  d_1 = d1 + 5.5; // front
  d_2 = d2 + 5.5; // angled right
  d_3 = d3 + 5.5; // right
  d_4 = d4 + 5.5; // back
  d_5 = d5 + 5.5; // left
  d_6 = d6 + 5.5; // angled left


  if (Serial.available() > 0){  
    Serial.println("y to start, n to stop");
    char read = Serial.read();
    if(read == 'y'){
      go = true;
    }
    else if(read == 'n'){
      go = false;
      dir = 0;
      setMotor();
    }
    if (read == 'u'){
      Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
      Serial.print("d1: ");
      Serial.println(d_1);
      Serial.print("d2: ");
      Serial.println(d_2);
      Serial.print("d3: ");
      Serial.println(d_3);
      Serial.print("d4: ");
      Serial.println(d_4);
      Serial.print("d5: ");
      Serial.println(d_5);
      Serial.print("d6: ");
      Serial.println(d_6);
      Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    }

  }

  if (go){  
    myservo.write(90);
    x = 2;
    detectBlockPickUp();
    go = 0;
  }
    myservo.write(90);
//     adjust();
// //    straight();
//     if (align == 0){
//       alignment();
//     } else if (centered == 0) {
//       center();
//     } else if (localize == 0) {
//       localization();
//       if (localize == 1) {
//         Serial.print("localized: ");
//         Serial.print(x);
//         Serial.print(", ");
//         Serial.print(y);
//         Serial.print(", ");
//         Serial.println(theta);
//       }
//     } else if (localize == 2 && (d_3 > 30 || d_5 > 30)) {
//       stop();
//       centered = 0;
//       localize = 0;
//     } else if (pickup != 0){
//       if (pickup == 3){
//         to_pickup(); // select initial path
//       } else if (d_1 < 16 + (30*check)){
//         tracking_x = x; // update current location
//         tracking_y = y;
//         if (check == 1.5) { // when halfway into L/Z
//           stop();
//           pickup = 0;
//           Serial.println("Arrived at L/Z");
//           check = 0;
//         } else {
//           to_pickup(); // select path
//         }
//       } else {
//         straight(); // update to adjusted s1, s2 value
//       }
//     } else if (dropoff != 0){
//       if (dropoff == 5){ // acquire info
//         Serial.println("Where to?");
//         dropoff = Serial.parseInt();
//         to_dropoff(); // select path
//       } else if (d_1 < 16 + (30*check)){
//         tracking_x = x; // update current location
//         tracking_y = y;
//         if (check == 0.5) { // when halfway into B
//           stop();
//           dropoff = 0;
//           Serial.println("Arrived at B");
//           check = 0;
//           myservo.write(90);
//           detectBlockPickUp();
//         } else {
//           to_dropoff(); // select path
//         }
//       } else if (d_4 > 14 + (30*check_back)){
//         tracking_x = x; // update current location
//         tracking_y = y;
//         check_back = 5;
//         to_dropoff(); // select path
//       } else {
//         straight(); // update to adjusted s1, s2 value
//       }
//     } 
//   }
}

void straightShort(){
  dir = 1;
  s1 = 70;
  s2 = 70;
  setMotor();
  delay(1500);
  stop();
}


void straight(){
  dir = 1;
  setMotor();
}

void reverse(){
  dir = -1;
  s1 = 69;
  s2 = 70;
  setMotor();
  delay(1500);
  stop();
}

void rotateCCW180(){  //tested
  dir = 2;
  s1 = 95;//100
  s2 = 95;
  setMotor();
  delay(1500);
  stop();
  theta += 180;
}
void rotateCCW90(){  
  dir = 2;
  s1 = 75;//80
  s2 = 75;
  setMotor();
  delay(1500);
  stop();
  theta += 90;
}

void rotateCW90(){
  dir = 3;
  s1 = 75;//80
  s2 = 75;
  setMotor();
  delay(1500);
  stop();
  theta -= 90;
}


void rotateCCW30(){  
  dir = 2;
  s1 = 65;
  s2 = 65;
  setMotor();
  delay(1500);
  stop();
}

void rotateCW30(){  
  dir = 3;
  s1 = 65;
  s2 = 65;
  setMotor();
  delay(1500);
  stop();
}

void stop(){
  dir = 0;
  setMotor();
  delay(1000);
}

void setMotor(){

  analogWrite(PWMA, s1);
  analogWrite(PWMB, s2);
  if(dir == 1){//Forward
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
  }
  else if(dir == -1){//Reverse
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
  }  
  else if(dir == 2){//Right
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
  }
  else if(dir == 3){//Left
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
  }

  else{//Stop
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
  }  

  // Serial.println("--------------------------");
  // Serial.print("DIR: ");
  // Serial.println(dir);
  // Serial.print("s1: ");
  // Serial.println(s1);
  // Serial.print("s2: ");
  // Serial.println(s2);
  // Serial.println("--------------------------");
}


void getDistance(){
    prev1 = d1;
    prev2 = d2;
    prev3 = d3;
    prev4 = d4;
    prev5 = d5;
    prev6 = d6;

    digitalWrite(trigPin, LOW); 
    delayMicroseconds(500); 
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);

    pingTime = pulseIn(U1, HIGH);
    d1 = pingTime*(340/2)/10000;
    if (d1 == 0){
      d1 = 85;
    }

    digitalWrite(trigPin, LOW); 
    delayMicroseconds(500); 
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);

    pingTime = pulseIn(U2, HIGH);
    d2 = pingTime*(340/2)/10000;
    if (d2 == 0){
      d2 = 85;
    }

    digitalWrite(trigPin, LOW); 
    delayMicroseconds(500); 
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);

    pingTime = pulseIn(U3, HIGH);
    d3 = pingTime*(340/2)/10000;
    if (d3 == 0){
      d3 = 85;
    }

    digitalWrite(trigPin, LOW); 
    delayMicroseconds(500); 
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);

    pingTime = pulseIn(U4, HIGH);
    d4 = pingTime*(340/2)/10000;
    if (d4 == 0){
      d4 = 85;
    }

    digitalWrite(trigPin, LOW); 
    delayMicroseconds(500); 
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);

    pingTime = pulseIn(U5, HIGH);
    d5 = pingTime*(340/2)/10000;
    if (d5 == 0){
      d5 = 85;
    }

    digitalWrite(trigPin, LOW); 
    delayMicroseconds(500); 
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);

    pingTime = pulseIn(U6, HIGH);
    d6 = pingTime*(340/2)/10000;
    if (d6 == 0){
      d6 = 85;
    }    
}

void alignment() {
  if ((d_2 > 21.55 + err || d_2 < 21.55 - err) && (d_6 > 21.55 + err || d_6 < 21.55 - err)) {
    align = 1;
    Serial.println("aligned");
  } else {
    rotateCCW30();
    //rotate by small amount for alignment ideally 15 degrees..?
  }
}

void center() {
  int a1 = d_1 / 30.48; //front
  float r1 = d_1 - (30.48 * a1); // remainder of d_1/30.48
  int a4 = d_4 / 30.48; //back
  float r4 = d_4 - (30.48 * a4); // remainder of d_4/30.48
  if (r1 > 15.24 + err*2 && r4 < 15.24 - err*2){
    straight();
    // far from front edge of block, near to back edge of block by at least 1 step size
    // move forward by 1 step
  }else if (r4 > 15.24 + err*2 && r1 < 15.24 - err*2){
    reverse();
    // far from back edge of block, near to front edge of block by at least 1 step size
    // move backward by 1 step
  }else {
    centered = 1;
    stop();
    Serial.println("centered");
  }
}

void localization(){
  int w[] = {0,0,0,0}; // wall array, 1 if wall 0 if open path
  int count = 0; // count # walls for categorization
  int ind1 = 0; // index for minimum distance
  int ind2 = 0; // index for maximum distance
  int ind3 = 0;
  int ind4 = 0;
  int b[] = {0,0,0,0};
  b[0] = {d_1 / 30.48}; // # of blocks in front direction
  if (b[0] == 0){
    w[0] = 1;
    count += 1;
  }
  b[1] = d_5 / 30.48; // # of blocks in left direction
  if (b[1] == 0){
    w[1] = 1;
    count += 1;
  }
  b[2] = d_4 / 30.48; // blocks in back direction
  if (b[2] == 0){
    w[2] = 1;
    count += 1;
  }
  b[3] = d_3 / 30.48; // blocks in right direction
  if (b[3] == 0){
    w[3] = 1;
    count += 1;
  }

  // find direction (index) of minimum distance
  if (b[0] < b[1] && b[0] < b[2] && b[0] < b[3]){
    ind1 = 0;
  } else if (b[1] < b[0] && b[1] < b[2] && b[1] < b[3]) {
    ind1 = 1;
  } else if (b[2] < b[0] && b[2] < b[1] && b[2] < b[3]) {
    ind1 = 2;
  } else {
    ind1 = 3;
  }

  // find direction (index) of maximum distance
  if (b[0] > b[1] && b[0] > b[2] && b[0] > b[3]){
    ind2 = 0;
  } else if (b[1] > b[0] && b[1] > b[2] && b[1] > b[3]) {
    ind2 = 1;
  } else if (b[2] > b[0] && b[2] > b[1] && b[2] > b[3]) {
    ind2 = 2;
  } else {
    ind2 = 3;
  }

  // Categorize by # of walls: 0, 1, 2, 3
  if (count == 0) { // (6,3)
    x = 6;
    y = 3;
    theta = (-ind1) * 90;
    localize = 1;
  } else if (count == 1) { // (1,3), (2,4), (3,1), (8,3)
    localize = 1;
    if (ind1 == 3) {
      ind3 = 0;
    } else {
      ind3 = ind1 + 1;
    }
    if (b[ind3] == 3) { // 1, 3, 0, 2
      x = 3;
      y = 1;
      theta = (2 - ind1) * 90;
    } else if (b[ind3] == 1) { //1, 0, 2, 4
      x = 8;
      y = 3;
      theta = (3 - ind1) * 90;
    } else if ((ind1 - ind2 == 1) || (ind1 - ind2 == -3)){
      x = 2;
      y = 4;
      theta = (-ind1) * 90;
    } else {
      x = 1;
      y = 3;
      theta = (1 - ind1) * 90;
    }
  } else if (count == 3){ // (3,2), (6,4), (8,1), (8,4)
    if (b[ind2] == 1){
      x = 3;
      y = 2;
      theta = (2 - ind1)*90;
      localize = 1;
    } else if (ind2 == 0){
      if (i1 == true && i2 == false){// left white, right black
        x = 6;
        y = 4;
        theta = 180;
      } else if (i1 == true && i2 == true){
        x = 8;
        y = 4;
        theta = 180;
      } else {
        x = 8;
        y = 1;
        theta = 0;
      }
    } else {
      localize = 3; // rotate such that ind2 becomes 0
      for (int k = 1; k < ind2+1; k++){
        rotateCCW90();
      }
      Serial.println("THREE WALLS");
    }
  } else if ((w[0] == 1 && w[2] == 1) || (w[1] == 1 && w[3] == 1)){
    // Opposite walls, to many options needs to move to a new block
    localize = 2;
    Serial.println("TWO OPPOSITE WALLS");
    for (int k = 1; k < ind2+1; k++){
      rotateCCW90(); // turn to longer path
    }
    straight(); //move until left or right sensor reads opening then realign
  } else { // 2 Adjacent walls (1,1), (1,4), (2,3), (4,3), (4,4), (6,1)
    // b >= 3; (4,3) vs. (4,4) ... (1,4)
    // b = 3, 5; (6,1) vs. (1,1)
    localize = 1;
    if (ind2 == 3) {
      ind4 = 0;
    } else {
      ind4 = ind2 + 1;
    }
    if (b[ind2] == 1 && pickup == 0) {
      x = 2;
      y = 3;
      if (w[1] == 1 && w[2] == 1){
        theta = 90;
      } else if (w[0] == 1 && w[3] == 1){
        theta = 270;
      } else if (w[0] == 1 & w[1] == 1){
        theta = 180;
      } else {
        theta = 0;
      }
    } else if (b[0]+b[1]+b[2]+b[3] == 6 && pickup == 0) { // (1,4) L/Z
        x = 1;
        y = 4;
        if (w[1] == 1 && w[2] == 1){
          theta = 270;
        } else if (w[0] == 1 && w[3] == 1){
          theta = 90;
        } else if (w[0] == 1 && w[1] == 1){
          theta = 0;
        } else {
          theta = 270;
        }
    } else if (b[ind4] == 1) { 
      // (4,3) OR (4,4) mirror pattern, cannot identify
      Serial.println("TWO ADJACENT WALLS: (4,3) OR (4,4)");
      for (int k = 1; k < ind2+1; k++){
        rotateCCW90(); //rotate to face longer path
      }
      straight(); //forward until left or right sensor reads two walls
    } else { //(1,1) OR (6,1)
      if (w[0] == 1 && w[1] == 1) {
        if (i2 == true) { //right reads white
          x = 6;
          y = 1;
          theta = 180;
        } else {
          x = 1;
          y = 1;
          theta = 90;
        }
      } else if (w[1] == 1 && w[2] == 1) {
        if (i1 == true) { //left reads white
          x = 1;
          y = 1;
          theta = 0;
        } else {
          x = 6;
          y = 1;
          theta = 90;
        }
      } else if (w[2] == 1 && w[3] == 1) {
        if (i2 == true) { //right reads white
          x = 1;
          y = 1;
          theta = 270;
        } else {
          x = 6;
          y = 1;
          theta = 0;
        }
      } else {
        if (i1 == true) { //left reads white
          x = 6;
          y = 1;
          theta = 270;
        } else {
          x = 1;
          y = 1;
          theta = 180;
        }
      }
    }
  }
}

void adjust(){
  //readjust theta values
  if (theta < 0) {
    theta += 360;
  } else if (theta > 300) {
    theta -=360;
  }
  //reset variables
  s1 = 75;
  s2 = 72;
  bool left_ = false;
  bool right_ = false;
  side = min(d_2, d_6);
  int a5 = d_5 / 30.48;
  left = d_5 - (30.48 * a5);
  int a3 = d_5 /30.48;
  right = d_3 - (30.48 * a3);
  if (side == d_2){// check for the right wall
    if (side > 21.55 + err/2){ //right wall too far
      s1 += 2;
      s2 -= 2;
      right_ = true;
    } else if (side < 21.55 + err/2){ //right wall too near
      s1 -= 2;
      s2 += 2;
      left_ = true;
    } else {
      s1 = 75;
      s2 = 72;
    } 
  } else if (side == d_6) {// check for the left wall
    if (side > 21.55 + err){ //left wall too far
      s1 -= 2;
      s2 += 2;
      left_ = true;
    } else if (side < 21.55 + err){ //left wall too near
      s1 += 2;
      s2 -= 2;
      right_ = true;
    } else {
      s1 = 75;
      s2 = 72;
    }
  }

  if (right_ == true && right > 15.48 + err/2 && left < 15.48 - err/2) {
    //turning to the right too much
    s1 -= 1;
    s2 += 1;
  } else if (left_ == true && left > 15.48 + err/2 && right < 15.48 - err/2) {
    //turning to the left too much
    s1 += 1;
    s2 -= 1;
  }
}

void to_pickup(){
  tracking_x = x;
  tracking_y = y;
  if (y == 4) {
    pickup = 2;
    if (x > 5){//within drop off zone, escape by moving downwards.
      tracking_y = 3;
      if (theta == 90){
        rotateCCW90();
      } else if (theta == 270){
        rotateCW90();
      } else if (theta == 0){
        rotateCCW180();
      }
      straight(); 
      check = 2; //stop when detecting two blocks in front
    } else {//move leftward to reach L/Z
      tracking_x = 2;
      if (theta == 0){
        rotateCCW90();
      } else if (theta == 180){
        rotateCW90();
      } else if (theta == 270){
        rotateCCW180();
      }
      straight();
      check = 1.5; // stop halfway into L/Z
    }
  } else if (y == 3){
    pickup = 2;
    if (x != 4) { // move to the left until wall
      tracking_x = 4;
      if (theta == 0){
        rotateCCW90();
      } else if (theta == 180){
        rotateCW90();
      } else if (theta == 270){
        rotateCCW180();
      }
      straight();
      check = 0; //don't stop until detecting a wall
    } else {// move upwards until wall
      tracking_y = 4;
      if (theta == 270){
        rotateCCW90();
      } else if (theta == 90){
        rotateCW90();
      } else if (theta == 180){
        rotateCCW180();
      }
      straight(); 
      check = 0; //don't stop until detecting a wall
    }
  } else if (y == 2) {
    if (x == 3){ // move downwards
      tracking_y = 1; 
      if (theta == 90){
        rotateCCW90();
      } else if (theta == 270){
        rotateCW90();
      } else if (theta == 0){
        rotateCCW180();
      }
      straight(); 
      check = 0;
    } else {// move upwards
      tracking_y = 3; 
      if (theta == 270){
        rotateCCW90();
      } else if (theta == 90){
        rotateCW90();
      } else if (theta == 180){
        rotateCCW180();
      }
      straight(); 
      if (x == 1){
        check = 1.5; // stop halfway into L/Z
        pickup = 1;
      }else {
        check = 1; // stop when detecting one full block (dropoff zone)
        pickup = 2;
      }
    }
  } else { // y = 1
    if (x == 8){ // move up until (8,3), before entering dropoff zone
      pickup = 2;
      tracking_y = 3; 
      if (theta == 270){
        rotateCCW90();
      } else if (theta == 90){
        rotateCW90();
      } else if (theta == 180){
        rotateCCW180();
      }
      straight();
      check = 1; // stop when detecting one full block (dropoff zone)
    } else if (x != 1) { // move to left of map until reaching wall
      pickup = 1;
      tracking_x = 1;
      if (theta == 0){
        rotateCCW90();
      } else if (theta == 180){
        rotateCW90();
      } else if (theta == 270){
        rotateCCW180();
      }
      straight();
      check = 0; //don't stop until detecting a wall
    } else { // at (1,1) go up to enter L/Z, end drive when reaching (1,3).
      pickup = 1;
      tracking_y = 3;
      if (theta == 270){
        rotateCCW90();
      } else if (theta == 90){
        rotateCW90();
      } else if (theta == 180){
        rotateCCW180();
      }
      straight();
      check = 1.5; // stop when detecting 1.5 blocks in front, halfway inside the L/Z.
    }
  }
}

void to_dropoff(){ // assuming robot is somewhere within the L/Z (1,4),(1,3),(2,3),(2,4)
  if (dropoff == 1){ // to B at (3,2)
    // leave L/Z through bottom left exit at (1,3)
    if (x > 1 && y > 2){ // move to left of map until x = 1
      tracking_x = 1;
      if (theta == 0){
        rotateCCW90();
      } else if (theta == 180){
        rotateCW90();
      } else if (theta == 270){
        rotateCCW180();
      }
      straight();
      check = 0; //don't stop until detecting a wall
    } else if (y > 1){ // move down until y = 1
      tracking_y = 1; 
      if (theta == 90){
        rotateCCW90();
      } else if (theta == 270){
        rotateCW90();
      } else if (theta == 0){
        rotateCCW180();
      }
      straight(); 
      check = 0;
    } else if (x == 1 && y == 1) { // at x = 1 & y = 1, facing down
      // move right until x = 3
      tracking_x = 3;
      rotateCCW90();
      straight();
      check_back = 2; //don't stop until detecting 2 blocks behind.
    } else { // y = 1, x = 3, facing right
      tracking_y = 2;
      rotateCCW90();
      straight();
      check = 0.5; // stop when detecting 0.5 blocks in front, halfway inside the B.
    }
  } else { // otherwise exit L/Z from the top right exit at (2,4)
    if (x < 3 && y > 2) { //within L/Z
      if (y != 4) { // go up until wall (x,4)
        tracking_y = 4;
      if (theta == 270){
        rotateCCW90();
      } else if (theta == 90){
        rotateCW90();
      } else if (theta == 180){
        rotateCCW180();
      }
      straight();
      check = 0; // stop when wall detected
      } else { //go right until wall (4,4)
        tracking_x = 4;
        if (theta == 180){
          rotateCCW90();
        } else if (theta == 0){
          rotateCW90();
        } else if (theta == 90){
          rotateCCW180();
        }
        straight();
        check = 0; // stop when wall
      }
    } else if (x == 4 && y == 4) {//facing to the right
      tracking_y = 3;
      rotateCW90(); //turn to face bottom
      straight();
      check = 0; // stop when wall
    } else if (x == 4 && y == 3) {//facing bottom
      rotateCCW90(); //turn to face right
      straight();
      if (dropoff == 2){
        tracking_x = 6;
        check = 2;
        check_back = 2;
      } else {
        tracking_x = 8;
        check = 0;
      }
    } else if (dropoff == 2 && x == 6) {
      tracking_y = 4;
      rotateCCW90();
      straight();
      check = 0.5; // stop halfway into B
    } else if (dropoff == 3 && x == 8) {
      tracking_y = 4;
      rotateCCW90(); //turn to top
      straight();
      check = 0.5; // stop halfway into B
    } else if (dropoff == 4 && x == 8) {
      tracking_y = 4;
      rotateCW90(); //turn to bottom
      straight();
      check = 0.5; // stop halfway into B
    }
  }
}

void detectBlockPickUp(){
  delay(1000);
  irDist();
  getDistance();
  Serial.println("IR DISTANCE");
  Serial.println(IR_dist+3);
  Serial.println("D1 DISTANCE");
  Serial.println(d1+5.5);
  x = 2;
  if (!((IR_dist + 3)  + 3 >= (d1 + 5.5) && (IR_dist + 3 )-3 <= (d1 + 5.5) )){
    Serial.println("1111111111111111111111");
    //figure out how to go towards the block; some kinda while loop
    while(IR_dist > 6){
      Serial.println("?????????????????");
      Serial.print("IR_dist: ");
      Serial.println(IR_dist);
      straightShort();
      irDist();
    }
 
    myservo.write(5); //close scoop
    // servoDown();
    Serial.println("PICKED UP BLOCK!");   
    delay(7000); 
    return; //gtfo
  }
 
  //if block not found straight; then keep turning 30 degrees
  int loop = 0;
  while(loop <=5){
    loop += 1;
    Serial.println("222222222222222222");
    
    if (x==1){
      Serial.println("CAse 1");
      rotateCCW30();
    }
    else{
      Serial.println("CAse 2");
      rotateCW30(); //some kind of small increment...?
    }
    
    //get sensor readings
    irDist();
    getDistance();
    Serial.println("IR DISTANCE");
    Serial.println(IR_dist+3);
    Serial.println("D1 DISTANCE");
    Serial.println(d1+5.5);

    if (!((IR_dist + 3)  + 3 >= (d1 + 5.5) && (IR_dist + 3 )-3 <= (d1 + 5.5) )){
      Serial.println("333333333333333");
      //figure out how to go towards the block; some kinda while loop
      while(IR_dist > 6){
        Serial.println("4444444444444444444");
        Serial.print("IR_dist: ");
        Serial.println(IR_dist);
        straightShort(); //keep going straight until the IR_dist is ~ 3 
        irDist();
      }
      Serial.println("338059483053");
      //myservo.write(5);// close scoop
      //servoDown();
      myservo.write(5);
      Serial.println("PICKED UP BLOCK!");    
      return; //gtfo
    }
  }  
  Serial.println("Done loop");
  //if block is directly right against the plate
}
 
void irDist(){
  IR_value = analogRead(IR);
  IR_dist =  5978.2*pow(IR_value, -1.171); //should be in cm?
}

void servoDown(){

  for (pos = 90; pos >= 5; pos -= 1){
    myservo.write(pos);
    delay(20);
  }
  
}
