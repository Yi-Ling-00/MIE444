// Robot location
int x = 0;
int y = 0;
int theta = 0;
int x_tracking = 0;
int y_tracking = 0;

// Robot status
int align = 0;
int centered = 0;
int localize = 0;
int drive = 0;

// Calibration values
int err = 1; // error value in cm
float step_size = 2.54; // distance traveled by each loop, 1 inch

// Destination
int pickup = 3;
int dropoff = 1;
int counter = 12; // length of one block / step size

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
// IR readings
bool i1 = false; //left
bool i2 = false; //right

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  d_1 = d1 + 5.5; // front
  d_2 = d2 + 5.5; // angled right
  d_3 = d3 + 5.5; // right
  d_4 = d4 + 5.5; // back
  d_5 = d5 + 5.5; // left
  d_6 = d6 + 5.5; // angled left

  if (Serial.available() > 0){  
    if (align == 0){
      alignment();
    } else if (centered == 0) {
      center();
    } else if (localize == 0) {
      localization();
      if (localize == 1) {
        Serial.print("localized: ");
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.print(", ");
        Serial.println(theta);
      }
    } else if (pickup != 0) {
      
    } else if (dropoff != 0){
      
    }
  }
  
}

void alignment() {
  if ((d_2 > 21.55 + err || d_2 < 21.55 - err) && (d_6 > 21.55 + err || d_6 < 21.55 - err)) {
    align = 1;
    Serial.println("aligned");
  } else {
    //rotate by small amount for alignment ideally 15 degrees..?
  }
}

void center() {
  int a1 = d_1 / 30.48; //front
  float r1 = d_1 - (30.48 * a1); // remainder of d_1/30.48
  int a4 = d_4 / 30.48; //back
  float r4 = d_4 - (30.48 * a4); // remainder of d_4/30.48
  if (r1 > 15.24 + step_size && r4 < 15.24 - step_size){
    // far from front edge of block, near to back edge of block by at least 1 step size
    // move forward by 1 step
  }else if (r4 > 15.24 + step_size && r1 < 15.24 - step_size){
    // far from back edge of block, near to front edge of block by at least 1 step size
    // move backward by 1 step
  }else {
    centered = 1;
    Serial.println("centered");
  }
}

void localization(){
  int w[] = {0,0,0,0}; // wall array, 1 if wall 0 if open path
  int count = 0; // count # walls for categorization
  int ind1 = 0; // index for minimum distance
  int ind2 = 0; // index for maximum distance
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
    if (b[ind2] == 3) {
      x = 3;
      y = 1;
      theta = (2 - ind1) * 90;
    } else if (b[ind2] == 4) {
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
    }
  } else if ((w[0] == 1 && w[2] == 1) || (w[1] == 1 && w[3] == 1)){
    // Opposite walls, to many options needs to move to a new block
    localize = 2;
  } else { // 2 Adjacent walls
    localize = 1;
      if (b[ind2] == 1) {
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
      } else if (b[ind2] == 4) {
        x = 4;
        y = 3;
        if (w[1] == 1 && w[2] == 1){
          theta = 0;
        } else if (w[0] == 1 && w[3] == 1){
          theta = 180;
        } else if (w[0] == 1 && w[1] == 1){
          theta = 90;
        } else {
          theta = 270;
        }
      } else if (b[ind2] == 3) {
        if (b[0]+b[1]+b[2]+b[3] < 5){
          x = 4;
          y = 4;
          if (w[1] == 1 && w[2] == 1){
            theta = 180;
          } else if (w[0] == 1 && w[3] == 1){
            theta = 0;
          } else if (w[0] == 1 && w[1] == 1){
            theta = 270;
          } else {
            theta = 90;
          }
        } else {
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
        }
      } else {
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
