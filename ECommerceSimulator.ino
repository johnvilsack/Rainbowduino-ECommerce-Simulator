#include <Rainbowduino.h>

float Customers = 60000;
float Pageviews = 300000;
float CartAdds = 5000;
float Transactions = 600;
float pctBounceRate = 50.00;
float pctExit = 25.00;
float pctAddCart;
float pctChanceToBuy = (Transactions/Pageviews)*100;

// Number of customers against numbers of minutes in a day against number of bulbs in array
int pctChanceLit = (Customers/1440)/64*100;

void setup() {
  Rb.init();
  Serial.begin(9600);
}

unsigned char x,y, gridPos, cellState;
int width = 8;

unsigned char grid[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void loop() {

x = random(8);
y = random(8);
gridPos = (x + y * width);

if ((Customers >= 1) && (Pageviews >= 1)) {
    // Determine current cell
  for (int i=0; i<64; i++) {
    if((grid[i] >= 50) && (grid[i] <= 51)) {
      grid[i]++;
      if(grid[i] == 51) {
        Blue();
        grid[i] = 2;
      }
    } else if((grid[i] >= 60) && (grid[i] <= 62)) {
      grid[i]++;
      if(grid[i] == 62) {
        sessionReset();
      }
    }
  }

    Browser();
  }
else
  Rb.blankDisplay();
}

// ACTUAL BROWSER SIMULATOR
void Browser() {
  cellState = grid[gridPos];

  // Socket Handler
  switch (cellState) {
    case 0:
      sessionBroker();
      break;
    case 1:
      sessionBrowse();
      break;
    case 2:
      sessionBrowse();
      break;
    case 3:
      sessionBrowse();
      break;
    case 99:
      sessionReset();
      break;
  }
}




///////////////////////////////// SESSION MANAGEMENT ///////////////////////////////////////////////

// This is how we establish new connections, based on some of the analytics.
void sessionBroker() {
    if (pctChanceLit >= random(100)) { // Does a visitor make attempt?
      if (pctBounceRate <= random(100)) { // Did they bounce right away?
        sessionBounce();
      } else { // A wild Visitor appears.
        sessionCreate();
      }  
    }  
    Customers--;
    Pageviews--;
}

// Establishing an actual user
void sessionCreate() {
  Blue();
  grid[gridPos] = 1;
}

// Bounces should flash and keep socket open for users
void sessionBounce() {
  lowBlue();
  Black();
}

// Establishing an actual user
void sessionExit() {
  lowRed();
  grid[gridPos] = 99;
}

// How to clean up a socket
void sessionReset() {
  Black();
  grid[gridPos] = 0;
}


///////////////////////////////// SESSION BROWSING ///////////////////////////////////////////////
void sessionBrowse() {
  Pageviews--;
  pctAddCart = (CartAdds/Pageviews)*100;
 
 if (grid[gridPos] == 1) {
   Blue();
   grid[gridPos] = 3;
 }
 
  if ((grid[gridPos] == 2) && (pctChanceToBuy >= random(100))){
    Green();
    grid[gridPos] = 60;
    CartAdds = CartAdds - 5;
    Pageviews = Pageviews - 5;
    return;
  }
  
  if (pctAddCart >= random(100)) {
    Yellow();
    grid[gridPos] = 50;
    CartAdds--;
    return;
  } else if (pctExit >= random(100)) {
    sessionExit();
    return;
  }
}


///////////////////////////////// COLORS ///////////////////////////////////////////////
void Green() {
  Rb.setPixelXY(x,y,0,255,0);
}

void lowRed() {
  Rb.setPixelXY(x,y,50,0,0);
}


void Blue() {
  Rb.setPixelXY(x,y,2,5,22);
}

void lowBlue() {
  Rb.setPixelXY(x,y,0,0,5);
}

void Black() {
  Rb.setPixelXY(x,y,0,0,0);
}

void lowWhite() {
  Rb.setPixelXY(x,y,10,10,10);
}

void Yellow() {
  Rb.setPixelXY(x,y,250,250,0);
}


