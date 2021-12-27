void setup() {
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
  Address BUS: (16-bit for now): PORTC(0..7)-PORTA(0..7)
  DATA BUS   : PORTL(0..7)
  CE : PG0
  OE : PG1
  WE : PG2
  */
  
  DDRA = DDRA | B11111111;
  DDRB = DDRB | B11111111;
  DDRL = DDRL | B11111111;
  DDRG = DDRG | 0x7; // PG0, PG1, PG2 -> output
}

char readByte(char aH, char aL) {
  // PORTL = Input (data bus)
  DDRL = DDRL | 0x0;
  // PORTA & PORTC = Output (address bus)
  DDRA = DDRA | 0xF;
  DDRC = DDRC | 0xF;
  // set address bus
  PORTC=aH;
  PORTA=aL;
  
  // WE=1, CE=1, OE=1
  PORTG=0x7;
  delay(10);
  // WE=1, CE=0, OE=1
  PORTG=0x5;
  delay(10);
  // WE=1, CE=0, OE=0
  PORTG=0x4;
  delay(10);
  char out = PORTL;
  delay(10);
  // WE=1, CE=1, OE=0
  PORTG=0x6;
  delay(10);
  // WE=1, CE=1, OE=1
  PORTG=0x7;
  delay(10);
  return out;
}

void loop() {
  // put your main code here, to run repeatedly:
  //PORTA=0xF;
  PORTL=0x2;
  //PORTL=0xF;
  delay(1000);
  //PORTA=0x0;
  PORTL=0x4;
  //PORTL=0x0;
  delay(1000);
}
