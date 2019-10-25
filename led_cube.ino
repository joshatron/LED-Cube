// Cube size
int xSize = 4;
int ySize = 4;
int zSize = 4;
// Layers from top to bottom
int layers[4] = {47,46,49,48};
// xy plane, with x first
int xy[4][4] = {
  {32,36,40,44},
  {33,37,41,45},
  {30,34,38,42},
  {31,35,39,43}
};
// The next frame, to be filled up
int next[4][4][4] = {
  {
    {HIGH,HIGH,HIGH,HIGH},
    {HIGH,LOW,LOW,HIGH},
    {HIGH,LOW,LOW,HIGH},
    {HIGH,HIGH,HIGH,HIGH}
  },
  {
    {HIGH,LOW,LOW,HIGH},
    {LOW,LOW,LOW,LOW},
    {LOW,LOW,LOW,LOW},
    {HIGH,LOW,LOW,HIGH}
  },
  {
    {HIGH,LOW,LOW,HIGH},
    {LOW,LOW,LOW,LOW},
    {LOW,LOW,LOW,LOW},
    {HIGH,LOW,LOW,HIGH}
  },
  {
    {HIGH,HIGH,HIGH,HIGH},
    {HIGH,LOW,LOW,HIGH},
    {HIGH,LOW,LOW,HIGH},
    {HIGH,HIGH,HIGH,HIGH}
  }
};
// Time when frame started
unsigned long frameTime = 0;
unsigned long frame = 0;

void setup() {
  // Set up layers
  for(int z = 0; z < zSize; z++) {
    pinMode(layers[z], OUTPUT);
    // Set as high to have all the lights off
    digitalWrite(layers[z], HIGH);
  }

  // Set up xy plane
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      pinMode(xy[x][y], OUTPUT);
    }
  }
}

void makeRain() {
  // Go bottom to better keep track of drops
  for(int z = zSize; z >= 0; z--) {
    for(int x = 0; x < xSize; x++) {
      for(int y = 0; y < ySize; y++) {
        // Move drops down
        if(next[z][x][y] == HIGH) {
          next[z][x][y] = LOW;
          if(z < (zSize - 1)) {
            next[z+1][x][y] = HIGH;
          }
        }
        //Randomly create drops on top
        else if(z == 0) {
          // 15% chance of creating drop
          if(random(100) < 25) {
            next[z][x][y] = HIGH;
          }
        }
      }
    }
  }
}

void fillAll() {
  for(int z = 0; z < zSize; z++) {
    for(int x = 0; x < xSize; x++) {
      for(int y = 0; y < ySize; y++) {
        next[z][x][y] = HIGH;
      }
    }
  }
}

void cycleOne() {
  for(int z = 0; z < zSize; z++) {
    for(int x = 0; x < xSize; x++) {
      for(int y = 0; y < ySize; y++) {
        if(z == frame / 16 % 4 && x == frame / 4 % 4 && y == frame % 4) {
          next[z][x][y] = HIGH;
        }
        else {
          next[z][x][y] = LOW;
        }
      }
    }
  }
}

void bouncePlane(int s, int plane) {
  int pos = (frame % (s * 2 - 2)) - s + 1;
  //abs needs to be on different line because macro, not function
  pos = abs(pos);

  for(int z = 0; z < zSize; z++) {
    for(int x = 0; x < xSize; x++) {
      for(int y = 0; y < ySize; y++) {
        if((plane == 0 && z == pos) || (plane == 1 && x == pos) || (plane == 2 && y == pos)) {
          next[z][x][y] = HIGH;
        }
        else {
          next[z][x][y] = LOW;
        }
      }
    }
  }
}

void rotatePlane(int plane) {
  int s = 2;
  float angle = ((frame % (s * 2)) / (float)s) - 1;
  int horz = (frame % (s * 4)) / (s * 2);
  
  for(int z = 0; z < zSize; z++) {
    for(int x = 0; x < xSize; x++) {
      for(int y = 0; y < ySize; y++) {
        int first;
        int second;
        switch(plane) {
          case 0:
            first = z;
            second = x;
            break;
          case 1:
            first = x;
            second = z;
            break;
          case 2:
            first = z;
            second = y;
            break;
          case 3:
            first = y;
            second = z;
            break;
          case 4:
            first = x;
            second = y;
            break;
          case 5:
            first = y;
            second = x;
        }
        
        if(horz == 0) {
          float diff = (first - 1.5) - (angle * (second - 1.5));
          diff = abs(diff);
          if(diff < .5) {
            next[z][x][y] = HIGH;
          }
          else {
            next[z][x][y] = LOW;
          }
        }
        else {
          float diff = (second - 1.5) - ((angle * -1) * (first - 1.5));
          diff = abs(diff);
          if(diff < .5) {
            next[z][x][y] = HIGH;
          }
          else {
            next[z][x][y] = LOW;
          }
        }
      }
    }
  }
}

void createWave(int s, int plane) {
  int basePos = (frame % (s * 2 - 2)) - s + 1;
  
  for(int z = 0; z < zSize; z++) {
    for(int x = 0; x < xSize; x++) {
      for(int y = 0; y < ySize; y++) {
        int subtractor;
        switch(plane) {
          case 0:
            subtractor = x;
            break;
          case 1:
            subtractor = y;
            break;
          case 2:
            subtractor = z;
            break;
        }
        int pos = ((frame - subtractor) % (s * 2 - 2)) - s + 1;
        pos = abs(pos);
        if((plane == 0 && z == pos) || (plane == 1 && x == pos) || (plane == 2 && y == pos)) {
          next[z][x][y] = HIGH;
        }
        else {
          next[z][x][y] = LOW;
        }
      }
    }
  }
}

void demo() {
  switch((frame / 30) % 6) {
    case 0:
      makeRain();
      break;
    case 1:
      bouncePlane(zSize, 0);
      break;
    case 2:
      bouncePlane(xSize, 1);
      break;
    case 3:
      rotatePlane(0);
      break;
    case 4:
      rotatePlane(5);
      break;
    case 5:
      createWave(zSize, 0);
      break;
  }
}

// Refills next frame. Currently applies 10 times a second
void fillOutNextFrame() {
  if(millis() - frameTime > 100) {
    frame++;
    demo();
    frameTime = millis();
  }
}

void loop() {
  digitalWrite(layers[zSize - 1],HIGH);
  fillOutNextFrame();
  for(int z = 0; z < zSize; z++) {
    if(z != 0) {
      digitalWrite(layers[z - 1], HIGH);
    }
    for(int x = 0; x < xSize; x++) {
      for(int y = 0; y < ySize; y++) {
        digitalWrite(xy[x][y],next[z][x][y]);
      }
    }
    digitalWrite(layers[z], LOW);
  }
}
