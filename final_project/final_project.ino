/*
18-095 Final Project
By: Taylor Roberts

This project is a flappy bird-esque game using a slide potentiometer as the primary input device,
as part of 18-095: Introduction to Electronics.
*/

#include <TFT_HX8357.h> // Hardware-specific library
#include <SPI.h>

//digital pins
#define BUTTON_1 2
#define BUTTON_2 3

// analog pins
#define SLIDE_POT 8

TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library

bool debug = false;

int16_t h = 320;
int16_t w = 480;

int16_t game_min_h = 40;
int16_t game_max_h = 320;
int16_t game_h = 280;

int16_t dly = 100;

int16_t pipeHeights[5] = {140, 160, 100, 80, 280};

// player and pipe x and y refer to the CENTER of the objects
int16_t player_y = 140;
int16_t player_x = 100;
int16_t player_size = 50;

int16_t pipe_x = 400;
int16_t pipe_y = 170;
int16_t pipe_w = 50;
int16_t gap = 100;

int16_t speed = 10;
int16_t max_speed = 100;
int16_t score = 0;
int16_t high_score = 0;

int16_t player_color = TFT_YELLOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);

  randomSeed(analogRead(0)); 

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_CYAN);
  tft.setTextColor(TFT_WHITE);
  Serial.println("Initialized!");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(dly);

  drawScore();
  drawPipes();
  drawPlayer();
  checkCollision();

}

void resetGame() {
  high_score = max(score,high_score);
  score = 0;
  speed = 10;
  tft.fillRect(pipe_x-pipe_w/2,game_min_h,pipe_w,game_max_h,TFT_CYAN);
  pipe_x = 400;

}

void checkCollision() {
  int player_l = player_x - player_size/2;
  int player_r = player_x + player_size/2;
  int player_top = player_y - player_size/2;
  int player_bot = player_y + player_size/2;

  int pipe_l = pipe_x - pipe_w/2;
  int pipe_r = pipe_x + pipe_w/2;
  int pipe_top = pipe_y - gap/2;
  int pipe_bot = pipe_y + gap/2;

  if (player_l < pipe_r &&  player_r > pipe_l) {
    // Serial.println("X overlap!");
    if (player_top < pipe_top || player_bot > pipe_bot) {
      // Serial.println("Collision!");
      // tft.fillRect(400,5,30,30,TFT_ORANGE);
      resetGame();
    }
  }

}

void drawPipes() {
  pipe_x -= speed;
  if (pipe_x - pipe_w/2 < 0) {
    tft.fillRect(0,game_min_h,pipe_w+speed,game_max_h,TFT_CYAN);
    pipe_y = random(90,270);
    pipe_x = w - pipe_w/2;
    score += 1;
    speed = min(speed+2, max_speed);
  }
  drawPipe(pipe_x, pipe_y);
}

// player_y will always be in range (70,280)
void drawPlayer() {
  tft.fillRect(player_x - player_size/2, player_y - player_size/2, player_size, player_size, TFT_CYAN);
  player_y = map(analogRead(SLIDE_POT),0,1023,70,290);
  tft.fillRect(player_x - player_size/2, player_y - player_size/2, player_size, player_size, player_color);
}

// x must be in range (0, w)
// y must be in range (90, 270)
void drawPipe(int16_t x, int16_t y) {
  tft.fillRect(x - pipe_w/2 + speed, game_min_h, pipe_w, game_max_h, TFT_CYAN);
  tft.fillRect(x - pipe_w/2, game_min_h,
               pipe_w,       y - game_min_h - gap/2, TFT_GREEN);

  tft.fillRect(x - pipe_w/2,      y + gap/2, 
              pipe_w, game_max_h - y - gap/2, TFT_GREEN);
}

void drawScore() {

  tft.fillRect(0,0,w,game_min_h,TFT_BLACK);

  // if (debug) {
    if (digitalRead(BUTTON_1)) {
      // Serial.println("\tButton 1 pressed!");
      player_color = TFT_BLUE;
      // tft.fillRect(300,5,30,30,TFT_RED);
    // } else {
      // tft.fillRect(300,5,30,30,TFT_BLACK);
    }
    if (digitalRead(BUTTON_2)) {
      // Serial.println("\tButton 2 pressed!");
      // tft.fillRect(350,5,30,30,TFT_BLUE);
      player_color = TFT_YELLOW;
    // } else {
      // tft.fillRect(350,5,30,30,TFT_BLACK);
    }
  // }

  // int s = analogRead(SLIDE_POT);

  // Serial.print("Slide Pot:");
  // Serial.println(s);

  tft.setCursor(10,10);
  tft.setTextSize(3);
  tft.print("Score: ");
  tft.print(score);

  tft.setCursor(200,10);
  tft.setTextSize(3);
  tft.print("High Score: ");
  tft.print(high_score);
}
