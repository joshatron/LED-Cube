// Cube size
int xSize = 4;
int ySize = 4;
int zSize = 4;
// Layers from top to bottom
int layers[4] = {46,47,48,49};
// xy plane, with x first
int xy[4][4] = {
  {30,31,32,33},
  {34,35,36,37},
  {38,39,40,41},
  {42,43,44,45}
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
          if(random(100) < 15) {
            next[z][x][y] = HIGH;
          }
        }
      }
    }
  }
}

// Refills next frame. Currently only applies 4 times a second
void fillOutNextFrame() {
  if(millis() - frameTime > 250) {
    makeRain();
    frameTime = millis();
  }
}

void loop() {
  fillOutNextFrame();
  digitalWrite(layers[zSize - 1],HIGH);
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
