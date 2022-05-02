#include <avr/pgmspace.h>
#include "bee.h"
#include "honeycombs.h"
#include "logo.h"


#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

#define EVEN1 65042
#define EVEN2 33284
#define ODD1  65430
#define ODD2  64781
#define ODD3  45831
#define ODD4  6177

#define NOFITEMS 61 // cells in the honeycomb, should be 61

// Colour codes
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF
#define ORANGE   0b1111101111100000
#define BROWN    0b0111100111100000
#define PINK     0b1111100100000100

#define YHIFONT 65430
#define YHIBACK 45831
#define YLOFONT 64781
#define YLOBACK 6177
#define GHIFONT 0b1111111111111111
#define GHIBACK 0b0100001000001000
#define GLOFONT 0b1000010000010000
#define GLOBACK 0b0000000000000000

// Beep frequencies (Swedish tone names)
// Diatonic tone frequencies according to 
// just intonation, each chromatic inbetween
// is the square root of the two diatonic multiplied.
// A very home made temperation, which might or might not
// favour diatonic scale melodies.
#define B3       248
#define C4       264
#define CISS4    280
#define D4       297
#define DISS4    313
#define E4       330
#define F4       352
#define FISS4    373
#define G4       396
#define GISS4    417
#define A4       440
#define AISS4    467
#define B4       495
#define C5       528
#define CISS5    560
#define D5       594
#define DISS5    626
#define E5       660
#define F5       704
#define FISS5    746
#define G5       792
#define GISS5    834
#define A5       880
#define AISS5    934
#define B5       990
#define C6      1056
#define CISS6   1120
#define D6      1188
#define DISS6   1252
#define E6      1320
#define F6      1408
#define FISS6   1492
#define G6      1584
#define GISS6   1668
#define A6      1760
#define AISS6   1868
#define B6      1980
#define C7      2112
#define Z       1
#define END     0
#define TONEDELAY 70    // ms for each tone to sound
#define TONEOFFTIME 1   // ms between tones

// Some melody snippets, all based on 
// The Flight of the Bumble Bee
// By Nikolai Rimsky-Korsakoff

uint16_t intro_melody1[] = {
  B6, AISS6, A6, GISS6, A6, GISS6, G6, FISS6, G6, FISS6, F6, E6, DISS6, D6, CISS6, C6,
  END
};

uint16_t intro_melody2[] = {
  B5, AISS5, A5, GISS5, A5, GISS5, G5, FISS5, G5, FISS5, F5, E5, DISS5, D5, CISS5, C5,
  B4, Z, Z, END
};

uint16_t main_theme[] = {
  B4, AISS4, A4, GISS4, G4, C5, B4, AISS4, B4, Z, Z, END
};

uint16_t new_ball_melody[] = {
  B4, Z, G4, Z, E4, Z, C4, Z, E4, Z, G4, Z, B4, Z, Z, END 
};

uint16_t timeout_melody[] = {
  B4, C5, B4, AISS4, B4, C5, B4, AISS4, 
  B4, C5, CISS5, D5, DISS5, D5, CISS5, C5, B4, Z, Z, END
};

uint16_t end_game_melody[] = { // 52 items
  B3,C4,CISS4,D4,DISS4,E4,F4,FISS4,G4,GISS4,A4,AISS4,B4,
  C5,CISS5,D5,DISS5,E5,F5,FISS5,G5,GISS5,A5,AISS5,B5,C6,
  CISS6,DISS6,E6, Z, Z, Z, Z, Z, Z, Z, 
  DISS5, E5, Z, Z, Z, Z, Z, Z, Z,
  DISS4, E4, Z, Z, Z, Z, END
};

  




// Function prototypes
void newBall(char *msg);
void newGame(void);
uint8_t start_screen(void);
void play_melody(uint16_t *mel);
uint8_t run_menu(void);
uint8_t run_settings(void);

// Globals
GFXcanvas16 centre(44, 40);
float spead = 0.76;
float sqrspead = 0.5776;
uint8_t spead_mode = 2;
uint8_t gravity_mode = 1;
uint8_t sound_mode = 3;

#define spead3  1.0
#define spead2  0.76
#define spead1  0.5
#define sqrspead3  1.0
#define sqrspead2  0.5776
#define sqrspead1  0.25

int ox, oy;
float ballx, bally, oballx, obally;
float vx, vy;
float x, y, z;
int timex, timey, otimex, otimey, otime, newtime;
float a, h, h2, aold, delta;
int lx1, ly1, lx2, ly2;
int lox1, loy1, lox2, loy2;
uint32_t dist2;
float dist;
uint16_t quarantene = 10;
int8_t itemCount;
unsigned long icTime = -1;
unsigned long gameTime;
bool soundflip = true;
uint16_t sflips[2] = { 660, 495 }; // E5, B4

void setup(void) {
  
  CARRIER_CASE = false;
  carrier.begin();

  if (start_screen() == 1)
  {
    run_settings();
  }
  carrier.display.fillScreen(BLACK);
  otimex = 0;
  otimey = 0;

  // Seed the RNG with tilt values from the accelerometer.
  // RNG is used for each new ball direction in newBall()
  while (!carrier.IMUmodule.accelerationAvailable()); 
  carrier.IMUmodule.readAcceleration(x, y, z);
  randomSeed(10000 * (x + y + z));

  ox = carrier.display.width() / 2;
  oy = carrier.display.height() / 2;
  
  quarantene = 10;
  lox1=0; loy1=0; lox2=1; loy2=1;
  aold = 0;
  newGame();
  if (sound_mode == 3)
    play_melody(main_theme);
  else
    delay((TONEDELAY + TONEOFFTIME) * (sizeof(main_theme) - 2) / 2);
  newBall("New Game");
  vx = 0;
  vy = spead;
  
  carrier.display.drawRGBBitmap(ox - 21, oy - 19, centre.getBuffer(), 44, 40);
  carrier.display.setTextSize(2);
  do
  {
    if (carrier.IMUmodule.accelerationAvailable()) 
    {
      carrier.IMUmodule.readAcceleration(x, y, z);
      h2 = x*x + y*y;
      if (h2 > 0.000001)
      {
        a = atan2(y, x);
        delta = a - aold;
        if (delta < -3.1415926536) delta += 6.28318530718;
        if (delta > 3.1415926536) delta -= 6.28318530718;
        h = sqrt(h2);
        if (h > 1) h = 1;
        aold += h*delta;
  
        // calculate bat
        lx1 = ox+cos(aold-0.2)*110;
        ly1 = oy-sin(aold-0.2)*110;
        lx2 = ox+cos(aold+0.2)*110;
        ly2 = oy-sin(aold+0.2)*110;
  
        // Draw bat by first drawing over the old bat
        carrier.display.drawLine(lox1, loy1, lox2, loy2, BLACK);
        carrier.display.drawLine(lx1, ly1, lx2, ly2, WHITE);
        lox1 = lx1;
        loy1 = ly1;
        lox2 = lx2;
        loy2 = ly2;
        
        // calculate ball
        ballx += vx;
        bally += vy;
        quarantene++;
        if (quarantene == 4)
          carrier.Buzzer.noSound();
        
        dist2 = (ballx - 128)*(ballx - 128)+(bally - 128)*(bally - 128);
        dist = sqrt(dist2);
        if (dist > 129)
        {
          if (sound_mode == 3) 
            play_melody(new_ball_melody);
          else
            delay((TONEDELAY + TONEOFFTIME) * (sizeof(new_ball_melody) - 2) / 2);
          newBall("new ball");
        }
        
        // check ball vs. bat
        if ( (quarantene > 5) && (lx1 - ballx)*(ly2 - bally) - (ly1 - bally)*(lx2 - ballx) > 0) // cross product
        {
          // if cross product is positive, we've crossed the bat line
        
  
          if ((lx1 - ballx)*(lx2 - ballx) + (ly1 - bally)*(ly2 - bally) > 0) // if dot product > 0 you missed the bat
          {
            // reset
            if (sound_mode == 3) 
              play_melody(new_ball_melody);
            else 
              delay((TONEDELAY + TONEOFFTIME) * (sizeof(new_ball_melody) - 2) / 2);
            newBall("new ball");
          }
          else // You hit the bat
          {
            float nx, ny, l, dotp;
            if (sound_mode != 1) carrier.Buzzer.sound(330);
            ny = lx2 - lx1;
            nx = ly1 - ly2;
            l = sqrt(nx * nx + ny * ny);
            nx /= l;
            ny /= l;
            dotp = 2. * (vx * nx + vy * ny);
            nx *= dotp;
            ny *= dotp;
            vx -= nx;
            vy -= ny;
            quarantene = 0; // Put ball in quarantene for 5 game loops to avoid stucking
  
          }
        }
        else // Ball is still at centre side of bat line. Check for collision with hex comb.
        {
          uint8_t checkX, checkY; // corner point to check for collision
          float colx, coly;       // mean coordinate for 1 or 2 items the ball colides with
          uint8_t nOfCols; 
           
          if ((ballx > ox - 21) && (ballx < ox + 23) &&
              (bally > oy - 20) && (bally < oy + 20))
          {
            checkX = ballx - ox + 21;
            checkY = bally - oy + 19;
  
            // collisionCheck() returns true if no collision!
            // If false, increase nOfCols and add coordinate of collided item to colX, colY
            // and erase item with black rect
            colx = 0;
            coly = 0;
            nOfCols = 0;
            if (collisionCheck(checkX - 1, checkY - 1, &colx, &coly, &nOfCols))
            {
              if (collisionCheck(checkX - 1, checkY + 1, &colx, &coly, &nOfCols))
              {
                if (collisionCheck(checkX + 1, checkY - 1, &colx, &coly, &nOfCols))
                {
                  collisionCheck(checkX + 1, checkY + 1, &colx, &coly, &nOfCols);
                }
              }
            }
            if (nOfCols) // We have a collision with item
            {
              // Check if all items (61 of them) have been hit
              if (!itemCount)
              {
                carrier.display.fillScreen(BLACK);
                carrier.display.setTextColor(PINK);
                carrier.display.setCursor(20, 80);
                carrier.display.setTextSize(3);
                carrier.display.print("  Game\n completed\n  in ");
                carrier.display.print(float(int(float(millis() - 1000 - gameTime) / 100)) / 10);
                carrier.display.print(" s");
                if (sound_mode == 3)
                  play_melody(end_game_melody);
                else
                  delay((TONEDELAY + TONEOFFTIME) * (sizeof(end_game_melody) - 2) / 2);
                delay(100);
                carrier.Buzzer.noSound();
                delay(900);
                
                carrier.display.fillScreen(BLACK);
                
                newGame();
                newBall("New Game");
              }
              else
              {
                if (nOfCols == 2)
                {
                  colx /= 2;
                  coly /= 2;
                }
                // Now we have a collision between ball and coordinate colX, colY.
                // Do the same as with colliding bat
                
                float nx, ny, l, dotp;
                ny = bally - coly;
                nx = ballx - colx; // nx, ny is the vector from cell to ball
                l = sqrt(nx * nx + ny * ny);
                nx /= l;
                ny /= l;           // now its length is 1
                dotp = 2. * (vx * nx + vy * ny); // double dot product
                nx *= dotp;
                ny *= dotp;
                vx -= nx;
                vy -= ny;          // Now we have reflected the velocity against the normal
                carrier.display.drawRGBBitmap(ox - 21, oy - 19, centre.getBuffer(), 44, 40);
              }
            } // End of collision with item
            
          } // End of checking for collision in centre
        }// End of checking if ball inside centre
        
        // Erase old ball
        carrier.display.fillRect(oballx-1, obally-1, 3, 3, BLACK);
        
        // Draw new ball
        carrier.display.drawPixel(ballx, bally, WHITE);
        carrier.display.drawPixel(ballx+1, bally-1, 23275);
        carrier.display.drawPixel(ballx+1, bally+1, 23275);
        carrier.display.drawPixel(ballx-1, bally-1, 23275);
        carrier.display.drawPixel(ballx-1, bally+1, 23275);
        
        carrier.display.drawPixel(ballx-1, bally, 33808);
        carrier.display.drawPixel(ballx+1, bally, 33808);
        carrier.display.drawPixel(ballx, bally-1, 33808);
        carrier.display.drawPixel(ballx, bally-1, 33808);
  
        // Restore ball coordinates
        oballx = ballx;
        obally = bally;
        
  
        // write time
        newtime = (millis() - gameTime) / 1000;
        timex = 118 - (ballx - 128) * 60 / dist;
        timey = 120 - (bally - 128) * 60 / dist;
        
        carrier.display.setTextSize(2);
        carrier.display.setTextColor(BLACK);
        carrier.display.setCursor(otimex, otimey);
        carrier.display.print(otime);
        carrier.display.setTextColor(WHITE);
        carrier.display.setCursor(timex, timey);
        carrier.display.print(newtime);
        otimex = timex;
        otimey = timey;
        otime = newtime;

        // Deal with time running up
        if (millis() - gameTime > 197000)
        {
          if (1)
          {
            static bool lastBeep = false;
            uint16_t millisecs;
            millisecs = (millis() - gameTime) % 1000;
            if (!lastBeep)
            {
              if (millisecs < 100)
              {
                carrier.Buzzer.sound(E5);
                lastBeep = true;
              }
            }
            else
            {
              if (millisecs > 100)
              {
                carrier.Buzzer.noSound();
                lastBeep = false;
              }
            }
          }
          if (millis() - gameTime > 199950) // Assure not to go over 1000 s
          // TIME IS OUT! NEW GAME!
          {
                carrier.display.fillScreen(BLACK);
                carrier.display.setTextColor(PINK);
                carrier.display.setCursor(20, 80);
                carrier.display.setTextSize(4);
                carrier.display.print("   Time\n  is up!");
                
                if (1)
                {
                  carrier.Buzzer.sound(E6);
                  delay(100);
                  carrier.Buzzer.noSound();
                  delay(1000);
                  play_melody(timeout_melody);
                }
                else
                {
                  delay(1100);
                  delay((TONEDELAY + TONEOFFTIME) * (sizeof(timeout_melody) - 2) / 2);
                }
                delay(1000);
                carrier.display.fillScreen(BLACK);
                
                newGame();
                newBall("New Game");
          }
        }
        
      }
    }
    if (millis() - icTime > 500)
    {
      carrier.display.setTextColor(BLACK);
      carrier.display.setTextSize(3);
      carrier.display.setCursor(112, 35);
      carrier.display.print(int(itemCount));
      icTime = -1;
      
    }
  }
  while(1);
  
}

bool collisionCheck(uint8_t x, uint8_t y, float *colx, float *coly, uint8_t *nOfCols)
{
  uint8_t fx, fy, mfx, mfy;
  float addx, addy;
  if (centre.getPixel(x, y) == 0)
    return true;
  (*nOfCols)++;
  itemCount--;

  if (sound_mode != 1) carrier.Buzzer.sound(sflips[soundflip]);
  soundflip = !soundflip;
  quarantene = 0;  
  carrier.display.setTextColor(BLACK);
  carrier.display.setTextSize(3);
  carrier.display.setCursor(112, 35);
  carrier.display.print(int(itemCount+1));
  carrier.display.setTextColor(CYAN);
  carrier.display.setTextSize(3);
  carrier.display.setCursor(112, 35);
  carrier.display.print(int(itemCount));
  icTime = millis();
  fy = y / 9;
  mfy = y % 9;
  if (mfy < 4)
  {
    fx = x / 5;
    *coly += 9 * fy + 1.5;  
    *colx += 5 * fx + 1.5;
    centre.fillRect(5 * fx, 9 * fy, 4, 4, BLACK);
  }
  else
  {
    fx = (x - 2) / 5;
    *coly += 9 * fy + 6;
    *colx += 5 * fx + 4;
    centre.fillRect(5 * fx + 2, 9 * fy + 4, 5, 5, BLACK);
  }
  return false;
}

void newBall(char *msg)
{
  carrier.display.setTextSize(2);
  carrier.display.setTextColor(BLACK);
  carrier.display.setCursor(otimex, otimey);
  carrier.display.print(otime);

  carrier.display.setTextSize(3);
  carrier.display.setTextColor(ORANGE);
  carrier.display.setCursor(60, 60);
  carrier.display.print(msg);
  delay(1000);
  
  carrier.display.setCursor(60, 60);
  carrier.display.setTextColor(BLACK);
  carrier.display.print(msg);
  
  carrier.display.fillRect(oballx-1, obally-1, 3, 3, BLACK);
  carrier.display.drawLine(lox1, loy1, lox2, loy2, BLACK);
  
#define TIMERX 112
#define TIMERY 35

  for (int8_t i = 3; i > 0; i--)
  {
    carrier.display.setCursor(TIMERX, TIMERY);
    carrier.display.setTextColor(BLACK);
    carrier.display.print(i+1);
    carrier.display.setCursor(TIMERX, TIMERY);
    carrier.display.setTextColor(RED);
    carrier.display.print(i);
    if (sound_mode != 1) carrier.Buzzer.sound(B4);
    delay(75);
    carrier.Buzzer.noSound();
    delay(925);
  }
    carrier.display.setCursor(TIMERX, TIMERY);
    carrier.display.setTextColor(BLACK);
    carrier.display.print(1);
    carrier.display.setCursor(TIMERX, TIMERY);
    carrier.display.setTextColor(RED);
    carrier.display.print(0);
    if (sound_mode != 1) carrier.Buzzer.sound(B5);
    delay(75);
    carrier.Buzzer.noSound();
    delay(75);
    carrier.display.setCursor(TIMERX, TIMERY);
    carrier.display.setTextColor(BLACK);
    carrier.display.print(0);

  ballx = 128;
  bally = 150;
  dist = 22.;
  vx = float(random(100)) / 100. - 0.5;
  vy = sqrt(sqrspead - vx * vx);
  quarantene = 5;
  carrier.display.setTextSize(2);
  
}

void newGame(void)
{
  centre.fillScreen(0);

  // Create the honeycomb graphic
  for (int i = 0; i < 5; i++)
  {
    int x0, y0;
    int margin = 2 - i;
    if (margin < 0) margin = -margin;
    for (int j = margin; j < 9-margin; j++)
    {
      x0 = 5*j;
      y0 = 9*i;
      centre.drawPixel(x0 + 1, y0, EVEN2);
      centre.drawPixel(x0 + 2, y0, EVEN2);
      centre.drawPixel(x0 + 1, y0+3, EVEN2);
      centre.drawPixel(x0 + 2, y0+3, EVEN2);
      centre.drawPixel(x0, y0+1, EVEN2);
      centre.drawPixel(x0, y0+2, EVEN2);
      centre.drawPixel(x0 + 3, y0+1, EVEN2);
      centre.drawPixel(x0 + 3, y0+2, EVEN2);

      centre.drawPixel(x0+1, y0+1, EVEN1);
      centre.drawPixel(x0+1, y0+2, EVEN1);
      centre.drawPixel(x0+2, y0+1, EVEN1);
      centre.drawPixel(x0+2, y0+2, EVEN1);
    }
  }
  for (int i = 0; i < 4; i++)
  {
    int x0, y0, margin;
    if (i == 0 || i == 3)
      margin = 1;
    else
      margin = 0;
    for (int j = margin; j < 8-margin; j++)
    {
      x0 = 5*j+3;
      y0 = 9*i+5;
      centre.drawPixel(x0 + 1, y0 - 1, ODD4);
      centre.drawPixel(x0 - 1, y0 + 1, ODD4);
      centre.drawPixel(x0 + 3, y0 + 1, ODD4);
      centre.drawPixel(x0 + 1, y0 + 3, ODD4);

      centre.drawPixel(x0, y0, ODD3);
      centre.drawPixel(x0+2, y0, ODD3);
      centre.drawPixel(x0, y0+2, ODD3);
      centre.drawPixel(x0+2, y0+2, ODD3);

      centre.drawPixel(x0+1, y0, ODD2);
      centre.drawPixel(x0+1, y0+2, ODD2);
      centre.drawPixel(x0, y0+1, ODD2);
      centre.drawPixel(x0+2, y0+1, ODD2);

      centre.drawPixel(x0+1, y0+1, ODD1);
    }
  }
  // Honeycomb graphic ready
  itemCount = NOFITEMS;
  carrier.display.drawRGBBitmap(ox - 21, oy - 19, centre.getBuffer(), 44, 40);
  gameTime = millis();
  oballx = 0;
  obally = 0;
}

void loop()
{
  
}

#define HCX 109
#define HCY 111

uint8_t start_screen(void)
{
  bool touched = false;
  uint32_t waitTime;
  carrier.display.drawRGBBitmap(37, 19, logo, 128, 77);
  
  delay(2000);
  // Turn on 1st cell
  carrier.display.drawRGBBitmap(HCX, HCY, hccell, hccell_mask, 26, 29);
  // Turn on 2nd cell
  carrier.display.drawRGBBitmap(HCX + 27, HCY, hccell, hccell_mask, 26, 29);
  
  if (sound_mode == 3)
    play_melody(intro_melody1);
  else
    delay((TONEDELAY + TONEOFFTIME) * (sizeof(intro_melody1) - 2) / 2);

  // Turn on 3rd cell
  carrier.display.drawRGBBitmap(HCX + 54, HCY, hccell, hccell_mask, 26, 29);
  // Turn on 4th cell
  carrier.display.drawRGBBitmap(HCX + 14, HCY+21, hccell, hccell_mask, 26, 29);

  if (sound_mode == 3)
    play_melody(intro_melody2);
  else
    delay((TONEDELAY + TONEOFFTIME) * (sizeof(intro_melody2) - 2) / 2);

  // Turn on 5th cell
  carrier.display.drawRGBBitmap(HCX + 41, HCY+21, hccell, hccell_mask, 26, 29);
  // Turn on 6th cell
  carrier.display.drawRGBBitmap(HCX -14, HCY-21, hccell, hccell_mask, 26, 29);

  // Two options during start screen
  carrier.display.setCursor(174, 184);
  carrier.display.setTextColor(CYAN);
  carrier.display.setTextSize(1);
  carrier.display.print("menu");
  carrier.display.setCursor(110, 230);
  carrier.display.print("PLAY");

  // Bee flies in from right
  for (int i = 256; i > 169; i-= 3)
  {
      //carrier.display.fillRect(i - 20, 139, 20, 57, BLACK);
      carrier.display.drawRGBBitmap(i, map(i, 256, 169, 90, 102), littlebee, littlebee_maskl, 48, 32);
      delay(1);
      carrier.Buttons.update();
      if (carrier.Button1.getTouch())
        return 1;
      if (carrier.Button2.getTouch())
        return 2;
  }
  if (sound_mode != 1) carrier.Buzzer.sound(B4);
  carrier.display.drawBitmap(HCX+54, HCY, hccell_mask, 26, 29, BLACK);
  // Bee flies out at top
  for (int i = 102; i > -22; i-= 3)
  {
      //carrier.display.fillRect(i - 20, 139, 20, 57, BLACK);
      if (i == 96) carrier.Buzzer.noSound();
      carrier.display.drawRGBBitmap(map(i, 102, -22, 169, 176), i, littlebee, littlebee_masku, 48, 32);
      delay(1);
      carrier.Buttons.update();
      if (carrier.Button1.getTouch())
        return 1;
      if (carrier.Button2.getTouch())
        return 2;
  }
  waitTime = millis();
  do
  {
    delay(5);
    carrier.Buttons.update();
    if (carrier.Button1.getTouch())
      return 1;
    if (carrier.Button2.getTouch())
      return 2;
  }
  while (millis()- waitTime < 3000);
  // Bee flies in from left
  for (int i = -50; i < 65; i+= 3)
  {
      //carrier.display.fillRect(i - 20, 139, 20, 57, BLACK);
      //if (i == 124) carrier.Buzzer.noSound();
      carrier.display.drawRGBBitmap(i, 102, littlebee, littlebee_maskr, 48, 32);
      delay(1);
      carrier.Buttons.update();
      if (carrier.Button1.getTouch())
        return 1;
      if (carrier.Button2.getTouch())
        return 2;
  }
  carrier.display.drawBitmap(HCX - 14, HCY-21, hccell_mask, 26, 29, BLACK);
  if (sound_mode != 1) carrier.Buzzer.sound(E5);
  carrier.display.drawRGBBitmap(65, 106, littlebee, littlebee_maskd, 48, 32);
  carrier.display.drawBitmap(HCX, HCY, hccell_mask, 26, 29, BLACK);
  carrier.display.drawRGBBitmap(68, 108, littlebee, littlebee_maskd, 48, 32);
  if (sound_mode != 1) carrier.Buzzer.sound(B4);
  for (int i = 110; i < 259; i+= 3)
  {
      //carrier.display.fillRect(i - 20, 139, 20, 57, BLACK);
      if (i == 116) carrier.Buzzer.noSound();
      carrier.display.drawRGBBitmap(map(i, 110, 259, 66, 48), i, littlebee, littlebee_maskd, 48, 32);
      delay(1);
      carrier.Buttons.update();
      if (carrier.Button1.getTouch())
        return 1;
      if (carrier.Button2.getTouch())
        return 2;
  }
  
  waitTime = millis();
  do
  {
    delay(5);
    carrier.Buttons.update();
    if (carrier.Button1.getTouch())
      return 1;
    if (carrier.Button2.getTouch())
      return 2;
  }
  while (millis()- waitTime < 5000);
    
  return 2;
  
}

void play_melody(uint16_t *mel)
{
  for (uint16_t *p = mel; *p != END; p++)
  {
    if (*p != Z)
      carrier.Buzzer.sound(*p);
    delay(TONEDELAY); 
    carrier.Buzzer.noSound();
    delay(TONEOFFTIME);
  }
}

uint8_t run_menu(void)
{
  carrier.display.fillScreen(BLACK);
  carrier.display.setCursor(55, 30);
  carrier.display.setTextColor(ODD2);
  carrier.display.setTextSize(3);
  carrier.display.println("BreakIn");
  carrier.display.setTextSize(1);
  carrier.display.println("\n     Created by Johan Halmen 2021");
  carrier.display.println("   Music by Nikolai Rimsky-Korsakoff\n");
  
  carrier.display.setTextColor(CYAN);
  carrier.display.setTextSize(1);
  carrier.display.setCursor(168, 184);
  carrier.display.print("settings");
  carrier.display.setCursor(110, 230);
  carrier.display.print("PLAY");

  do
  {
    carrier.Buttons.update();
  }
  while(carrier.Button1.getTouch());


  while(1)
  {
    carrier.Buttons.update();
    if (carrier.Button1.getTouch())
      return 1; // settings
    if (carrier.Button2.getTouch())
      return 2; // PLAY
  }
}

uint8_t run_settings(void)
{
  const char labels[3][5] =
  {
    "123x",
    "1234",
    "012x"
  };
  uint8_t row, col;
  row = 2;
  col = 1;
  
  carrier.display.fillScreen(BLACK);
  carrier.display.setCursor(50, 38);
  carrier.display.setTextColor(ODD2);
  carrier.display.setTextSize(3);
  carrier.display.print("settings");

  carrier.display.setTextSize(1);
  carrier.display.setTextColor(ODD3);
  carrier.display.setCursor(35, 80);
  carrier.display.print("speed");
  carrier.display.setCursor(93, 70);
  carrier.display.print("gravity");
  carrier.display.setCursor(93, 78);
  carrier.display.print(" mode");
  carrier.display.setCursor(165, 76);
  carrier.display.print("sound,");
  carrier.display.setCursor(165, 84);
  carrier.display.print("music");

  carrier.display.setCursor(188, 181);
  carrier.display.setTextSize(2);
  carrier.display.setTextColor(GREEN);
  carrier.display.print("x");
  carrier.display.setCursor(110, 230);
  carrier.display.setTextSize(1);
  carrier.display.print("PLAY");

  carrier.display.setTextSize(2);
  do
  {
    // Speed column
    for (int i = 0; i < 3; i++)
    {
      if (col == 0 && row == i)
      {
        if (spead_mode == i)
          carrier.display.setTextColor(YHIFONT, YHIBACK);
        else
          carrier.display.setTextColor(YLOFONT, YLOBACK);
      }
      else
      {
        if (spead_mode == i)
          carrier.display.setTextColor(GHIFONT, GHIBACK);
        else
          carrier.display.setTextColor(GLOFONT, GLOBACK);
      }
      carrier.display.setCursor(60, 25 * i + 105);
      carrier.display.print(labels[0][i]);
    }
    // Gravity column
    for (int i = 0; i < 4; i++)
    {
      if (col == 1 && row == i)
      {
        if (gravity_mode == i)
          carrier.display.setTextColor(YHIFONT, YHIBACK);
        else
          carrier.display.setTextColor(YLOFONT, YLOBACK);
      }
      else
      {
        if (gravity_mode == i)
          carrier.display.setTextColor(GHIFONT, GHIBACK);
        else
          carrier.display.setTextColor(GLOFONT, GLOBACK);
      }
      carrier.display.setCursor(110, 25 * i + 93);
      carrier.display.print(labels[1][i]);
    }
    // sound column
    for (int i = 0; i < 3; i++)
    {
      if (col == 2 && row == i)
      {
        if (sound_mode == i)
          carrier.display.setTextColor(YHIFONT, YHIBACK);
        else
          carrier.display.setTextColor(YLOFONT, YLOBACK);
      }
      else
      {
        if (sound_mode == i)
          carrier.display.setTextColor(GHIFONT, GHIBACK);
        else
          carrier.display.setTextColor(GLOFONT, GLOBACK);
      }
      carrier.display.setCursor(160, 25 * i + 105);
      carrier.display.print(labels[2][i]);
    }

    // Deal with tilting
    float x, y, z, h2, a;
    uint8_t sextant;
    bool rested;
    
    while (!carrier.IMUmodule.accelerationAvailable()); 
    carrier.IMUmodule.readAcceleration(x, y, z);
    h2 = x*x + y*y;
    // While we're waiting for the tilt, let's read the buttons
    if (h2 < 0.1)
    {
      rested = true;
      carrier.Buttons.update();
      if (carrier.Buttons.onTouchUp(TOUCH1))
      {
        switch (col)
        {
          case 0 :
            spead_mode = row;
            break;
          case 1 :
            gravity_mode = row;
            break;
          case 2 :
            sound_mode = row + 1;
            break;
        }
      }
      if (carrier.Buttons.onTouchUp(TOUCH2))
      {
        switch (spead_mode)
        {
          case 0 : 
            spead = spead1;
            sqrspead = sqrspead1;
            break;
          case 1 : 
            spead = spead2;
            sqrspead = sqrspead2;
            break;
          case 2 : 
            spead = spead3;
            sqrspead = sqrspead3;
            break;
        }
        return 1;
      }
    }
    else
    {
      a = atan2(-y, -x) + 3.14159265359;
      sextant = 6. * a / 3.14159265359 / 2.;
      if (sextant == 6) sextant = 0;
      if (rested && h2 > 0.1)
      {
        rested = false;
        switch (sextant)
        {
          case 0 : 
            switch(col)
            {
              case 0:
                col++;
                break;
              case 1:
                col++;
                if (row > 0)
                  row--;
                break;
              case 2:
                break;
            }
            break;
          case 1 :
            if (row > 0)
              row--;
            break;
          case 2 :
            switch (col)
            {
              case 0 :
                break;
              case 1 :
                col--;
                if (row > 0)
                  row--;
                break;
              case 2 :
                col--;
                break;
            }
            break;
          case 3 :
            switch (col)
            {
              case 0 :
                break;
              case 1 : 
                col--;
                if (row == 3)
                  row = 2;
                break;
              case 2 :
                col--;
                row++;
                break;
            }
            break;
          case 4 :
            if (col == 1)
            {
              if (row < 3)
                row++;
            }
            else
              if (row < 2)
                row++;
            break;
          case 5 :
            switch (col)
            {
              case 0 :
                col++;
                row++;
                break;
              case 1 :
                col++;
                if (row < 3)
                  row++;
                break;
              case 2 :
                break;
            }
            break;
        }
      }
    }
    //delay(500);      
  }
  while (1);
}
