#define RL0 49 
#define RL7 42

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
/*
W29C020C-90B Timings
---------------------

[Read Cycle Time Parameters]

Read Cycle Time                 : 90 - xx
Chip Enable Access Time         : xx - 90
Adress Access Time              : xx - 90
Output Enable Access Time       : xx - 40
CE High to High-Z Output        : xx - 25
OE High to High-Z Output        : xx - 25
Output Hold from Address change : 0 - xx

*/
  /*
  Address BUS: (16-bit for now): PORTC(7..0)-PORTA(7..0)
  DATA BUS   : PORTL(7..0)
  CE : PG0
  OE : PG1
  WE : PG2
  */
  
  DDRA = DDRA | B11111111;
  DDRC = DDRC | B11111111;
  DDRL = DDRL | B11111111;
  DDRG = DDRG | 0x7; // PG0, PG1, PG2 -> output

// WE, OE, CE

  // start serial
  Serial.begin(9600);
  while(!Serial) {}
 
  Serial.println("Serial OK ...");
}
char rcTime=90;

void setDigitalOut() {
  for (int i=RL7; i<=RL0; i++) {
    pinMode(i, OUTPUT);
  }
}

void setDigitalIn() {
  for (int i=RL7; i<=RL0; i++) {
    pinMode(i, INPUT);
  }
}


char setByte(byte data) {
  PORTL = data;
}

char setAddr(unsigned long addr) {
  PORTC=(addr&0xff00)>>8;
  PORTA=(addr&0x00ff);
  return 0;
  }

byte readData(unsigned long addr) {
  byte data=0;

  setDigitalIn();

  setAddr(addr);
  PORTG=0x6; // CE=0, WE=OE=1
  delayMicroseconds(10);
  PORTG=0x4; // WE=1;
  delayMicroseconds(10);
  data=PINL;
  PORTG=0x6;
  delayMicroseconds(10);
  return data;
}

void writeByte(byte data, unsigned long addr) {
  setDigitalOut();
  PORTG=0x6; // we=1; oe=1; ce=0
  
  setAddr(addr);
  setByte(data);
    delayMicroseconds(1);

  PORTG=0x2; // we=0; oe=1; ce=0;
  delayMicroseconds(1);
  PORTG=0x6; // we=1; oe=1; ce=0
  delayMicroseconds(1);
}

void writeData(byte data, unsigned long addr) {

   writeByte(0xAA, 0x5555);
   writeByte(0x55, 0x2AAA);
   writeByte(0xA0, 0x5555);
   writeByte(data, addr);

   delayMicroseconds(30);
}

// Short usage demo ... :)
void loop() {
  Serial.println("main starting ...");

Serial.println("writing ..:");
writeData(0x1F,0);

for(int i=1; i<127; i++) {
  writeByte(i, i);  
}

Serial.println("Done writing ...");

delay(1000);
PORTL=0x00;

setDigitalIn();

delay(1000);

Serial.println("Reading ..:");

for (int i=0;i<10;i++) {
  byte a=readData(i);
  Serial.print(i);
  Serial.print(" : ");
  Serial.println(a);
  }
Serial.println("done reading ...");
while(1);
}
