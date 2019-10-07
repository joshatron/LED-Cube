int levels[4] = {46,47,48,49};
int locs[16] = {30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45};

void setup() {
  // put your setup code here, to run once:
  for(int i = 30; i < 50; i++) {
    pinMode(i, OUTPUT);
  }
  digitalWrite(40,HIGH);
  digitalWrite(46,HIGH);
  digitalWrite(48,HIGH);
  digitalWrite(47,HIGH);
}

void turnOnLevel(int level) {
  for(int i = 0; i < 4; i++) {
    if(i == level) {
      digitalWrite(levels[i],LOW);
    }
    else {
      digitalWrite(levels[i],HIGH);
    }
  }
}

void resetLocs() {
  for(int i = 0; i < 16; i++) {
    digitalWrite(locs[i],LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 16; j++) {
      turnOnLevel(i);
      resetLocs();
      digitalWrite(locs[j],HIGH);
      delay(1000);
    }
  }
}
