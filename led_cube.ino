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
int frameTime = 0;
// To keep track of current layer lit up
int on = 0;

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

// Turns off current layer and sets up next one
void cycleDownLayer() {
  digitalWrite(layers[on], HIGH);
  on = (on + 1) % zSize;
}

// Turns the next layer on, once xy is ready
void cycleUpLayer() {
  digitalWrite(layers[on], LOW);
}

// Refills next frame;
void fillOutNextFrame() {
  frameTime = millis();
}

void loop() {
  on = zSize - 1;
  for(int z = 0; z < zSize; z++) {
    cycleDownLayer();
    for(int x = 0; x < xSize; x++) {
      for(int y = 0; y < ySize; y++) {
        digitalWrite(xy[x][y],next[z][x][y]);
      }
    }
    cycleUpLayer();
  }
}
