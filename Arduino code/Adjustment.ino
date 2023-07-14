// Measured distances
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

//speed
long mult = 1; //use for speed adjustment in case of weird battery
int s1 = 80*mult; //left
int s2 = 80*mult; //right

// Calibration values
int err = 1; // error value in cm

//variables to check straight motion
long side;
long left;
long right;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  d_1 = d1 + 5.5; // front
  d_2 = d2 + 5.5; // angled right
  d_3 = d3 + 5.5; // right
  d_4 = d4 + 5.5; // back
  d_5 = d5 + 5.5; // left
  d_6 = d6 + 5.5; // angled left

  //reset variables
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
      //default s1, s2 values
    } 
  } else if (side == d_6) {// check for the left wall
    if (side > 21.55 + err/2){ //left wall too far
      s1 -= 2;
      s2 += 2;
      left_ = true;
    } else if (side < 21.55 + err/2){ //left wall too near
      s1 += 2;
      s2 -= 2;
      right_ = true;
    } else {
      //default s1, s2 values
    }
  }

  if (right_ == true && right > 15.48 + err && left < 15.48 - err) {
    //turning to the right too much
    s1 -= 1;
    s2 += 1;
  } else if (left_ == true && left > 15.48 + err && right < 15.48 - err) {
    //turning to the right too much
    s1 += 1;
    s2 -= 1;
  }

  //Set motor to adjusted value
}
