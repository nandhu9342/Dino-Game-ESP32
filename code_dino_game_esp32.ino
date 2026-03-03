/* HELLO GUYS THIS IS NANDHU HERE
 * =============================================================
 *  DINO RUNNER GAME - ESP32 + SSD1306 OLED 0.96" (128x64)
 * =============================================================
 *  Wiring:
 *    OLED SDA  -> GPIO 21
 *    OLED SCL  -> GPIO 22
 *    OLED VCC  -> 3.3V
 *    OLED GND  -> GND
 *    JUMP BTN  -> GPIO 14 (other pin to GND)
 *    DUCK BTN  -> GPIO 27 (other pin to GND) [optional]
 *
 *  Libraries needed (install via Arduino Library Manager):
 *    - Adafruit SSD1306
 *    - Adafruit GFX Library
 * =============================================================
 */

// you need to download these libraries

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- Display Config ----------
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- Buttons ----------
#define JUMP_PIN  14
#define DUCK_PIN  27

// ---------- Game constants ----------
#define GROUND_Y       52   // y of ground line
#define DINO_X         12   // fixed x of dino

// Dino standing sprite (12x13 px)
static const uint8_t PROGMEM dino_run1[] = {
  0b00000111, 0b11000000,
  0b00000111, 0b11100000,
  0b00000111, 0b11100000,
  0b00000111, 0b11000000,
  0b00001111, 0b11000000,
  0b11101111, 0b11000000,
  0b11111111, 0b11000000,
  0b11111110, 0b00000000,
  0b11111111, 0b00000000,
  0b01111111, 0b00000000,
  0b00110110, 0b00000000,
  0b00110000, 0b00000000,
  0b01110000, 0b00000000,
};
// Dino run frame 2 (legs alternate)
static const uint8_t PROGMEM dino_run2[] = {
  0b00000111, 0b11000000,
  0b00000111, 0b11100000,
  0b00000111, 0b11100000,
  0b00000111, 0b11000000,
  0b00001111, 0b11000000,
  0b11101111, 0b11000000,
  0b11111111, 0b11000000,
  0b11111110, 0b00000000,
  0b11111111, 0b00000000,
  0b01111111, 0b00000000,
  0b00011011, 0b00000000,
  0b00001100, 0b00000000,
  0b00001100, 0b00000000,
};
// Dino duck frame 1 (16x9)
static const uint8_t PROGMEM dino_duck1[] = {
  0b00000001, 0b11110000,
  0b11100011, 0b11111000,
  0b11111111, 0b11110000,
  0b11111111, 0b00000000,
  0b01111111, 0b00000000,
  0b00111111, 0b00000000,
  0b00010110, 0b10000000,
  0b01110000, 0b00000000,
  0b00100000, 0b00000000,
};
// Dino duck frame 2
static const uint8_t PROGMEM dino_duck2[] = {
  0b00000001, 0b11110000,
  0b11100011, 0b11111000,
  0b11111111, 0b11110000,
  0b11111111, 0b00000000,
  0b01111111, 0b00000000,
  0b00111111, 0b00000000,
  0b00011010, 0b10000000,
  0b00001110, 0b00000000,
  0b00110000, 0b00000000,
};

// Cactus small (8x16)
static const uint8_t PROGMEM cactus_s[] = {
  0b00010000,
  0b00010000,
  0b10010000,
  0b11010000,
  0b11011100,
  0b11111110,
  0b01110100,
  0b00110000,
  0b00110000,
  0b00110000,
  0b00110000,
  0b00110000,
  0b00110000,
  0b00111000,
  0b00111000,
  0b00000000,
};
// Cactus large (12x24)
static const uint8_t PROGMEM cactus_l[] = {
  0b00000100, 0b00000000,
  0b00000110, 0b00000000,
  0b10000110, 0b00000000,
  0b11000110, 0b10000000,
  0b11001111, 0b10000000,
  0b11011111, 0b10000000,
  0b11111111, 0b10000000,
  0b11111110, 0b00000000,
  0b01111100, 0b00000000,
  0b00111100, 0b00000000,
  0b00011100, 0b00000000,
  0b00011100, 0b00000000,
  0b00011100, 0b00000000,
  0b00011100, 0b00000000,
  0b00011100, 0b00000000,
  0b00011100, 0b00000000,
  0b00011100, 0b00000000,
  0b00011100, 0b00000000,
  0b00011100, 0b00000000,
  0b00011100, 0b00000000,
  0b00011110, 0b00000000,
  0b00001110, 0b00000000,
  0b00001100, 0b00000000,
  0b00000000, 0b00000000,
};

// Bird / Pterodactyl frames (16x9)
static const uint8_t PROGMEM bird1[] = {
  0b00000000, 0b00000000,
  0b00000001, 0b10000000,
  0b00000011, 0b11000000,
  0b11100111, 0b11100000,
  0b11111111, 0b11110000,
  0b01111110, 0b00000000,
  0b00111100, 0b00000000,
  0b00011000, 0b00000000,
  0b00000000, 0b00000000,
};
static const uint8_t PROGMEM bird2[] = {
  0b00000110, 0b00000000,
  0b00001111, 0b00000000,
  0b00000111, 0b11000000,
  0b11100001, 0b11100000,
  0b11111111, 0b11110000,
  0b01111110, 0b00000000,
  0b00111100, 0b00000000,
  0b00011000, 0b00000000,
  0b00000000, 0b00000000,
};

// Cloud (16x5)
static const uint8_t PROGMEM cloud_bmp[] = {
  0b00000110, 0b00000000,
  0b00011111, 0b10000000,
  0b11111111, 0b11100000,
  0b11111111, 0b11000000,
  0b01111111, 0b10000000,
};

// ---------- Obstacle types ----------
#define OBS_NONE    0
#define OBS_CACT_S  1
#define OBS_CACT_L  2
#define OBS_BIRD_LO 3   // bird low (duck under)
#define OBS_BIRD_HI 4   // bird high (jump over or ignore)

struct Obstacle {
  int x;
  int type;
  bool active;
};

#define MAX_OBS  4
#define MAX_CLOUDS 3

struct Cloud {
  int x, y;
  bool active;
};

// ---------- Game state ----------
enum GameState { STATE_TITLE, STATE_PLAYING, STATE_DEAD };
GameState gameState = STATE_TITLE;

// Dino
float dino_y;       // top of dino
float dino_vy;
bool  ducking;
bool  onGround;
int   animFrame;
unsigned long lastAnimTime;

// Score
int   score;
int   highScore = 0;
float speed;       // pixels per frame
unsigned long frameCount;
unsigned long lastFrameTime;

Obstacle obs[MAX_OBS];
Cloud    clouds[MAX_CLOUDS];

// Ground scroll
int groundOffset = 0;

// ---------- Sprite dims ----------
#define DINO_W    12
#define DINO_H    13
#define DINO_DUCK_W 16
#define DINO_DUCK_H  9
#define CACT_S_W   8
#define CACT_S_H  16
#define CACT_L_W  12
#define CACT_L_H  24
#define BIRD_W    16
#define BIRD_H     9
#define CLOUD_W   16
#define CLOUD_H    5

// ---------- Forward declarations ----------
void resetGame();
void updateGame();
void drawGame();
void spawnObstacle();
bool checkCollision();
void drawDino();
void drawObstacle(Obstacle &o);
void drawClouds();
void drawGround();
void drawHUD();
void drawTitle();
void drawGameOver();

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  pinMode(JUMP_PIN, INPUT_PULLUP);
  pinMode(DUCK_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  randomSeed(analogRead(0));
  gameState = STATE_TITLE;
}

// ---------- Loop ----------
void loop() {
  bool jumpPressed = (digitalRead(JUMP_PIN) == LOW);
  bool duckPressed = (digitalRead(DUCK_PIN) == LOW);

  switch (gameState) {
    case STATE_TITLE:
      drawTitle();
      if (jumpPressed) {
        delay(200);
        resetGame();
        gameState = STATE_PLAYING;
      }
      break;

    case STATE_PLAYING:
      updateGame(jumpPressed, duckPressed);
      drawGame();
      break;

    case STATE_DEAD:
      drawGameOver();
      if (jumpPressed) {
        delay(200);
        resetGame();
        gameState = STATE_PLAYING;
      }
      break;
  }
}

// ---------- Reset ----------
void resetGame() {
  dino_y   = GROUND_Y - DINO_H;
  dino_vy  = 0;
  ducking  = false;
  onGround = true;
  animFrame = 0;
  lastAnimTime = millis();
  score     = 0;
  speed     = 2.5;
  frameCount = 0;
  groundOffset = 0;

  for (int i = 0; i < MAX_OBS; i++) obs[i].active = false;
  for (int i = 0; i < MAX_CLOUDS; i++) {
    clouds[i].active = true;
    clouds[i].x = random(20, 128);
    clouds[i].y = random(8, 25);
  }
}

// ---------- Update ----------
void updateGame(bool jump, bool duck) {
  unsigned long now = millis();
  if (now - lastFrameTime < 20) return;  // ~50fps cap
  lastFrameTime = now;
  frameCount++;

  // Increase speed every 500 frames
  if (frameCount % 500 == 0) speed += 0.3;
  // Score
  if (frameCount % 6 == 0) score++;

  // Duck / jump input
  ducking = duck && onGround;

  if (jump && onGround) {
    dino_vy  = -8.5;
    onGround = false;
    ducking  = false;
  }

  // Gravity
  if (!onGround) {
    dino_vy += 0.55;
    dino_y  += dino_vy;
    int groundLimit = GROUND_Y - (ducking ? DINO_DUCK_H : DINO_H);
    if (dino_y >= groundLimit) {
      dino_y   = groundLimit;
      dino_vy  = 0;
      onGround = true;
    }
  }

  // Animate legs
  if (now - lastAnimTime > 120) {
    animFrame = (animFrame + 1) % 2;
    lastAnimTime = now;
  }

  // Scroll ground
  groundOffset = (groundOffset + (int)speed) % 16;

  // Scroll clouds
  for (int i = 0; i < MAX_CLOUDS; i++) {
    clouds[i].x -= 1;
    if (clouds[i].x < -CLOUD_W) {
      clouds[i].x = 128 + random(10, 40);
      clouds[i].y = random(8, 25);
    }
  }

  // Spawn obstacles
  // Find rightmost active obstacle
  int rightmost = 0;
  for (int i = 0; i < MAX_OBS; i++) {
    if (obs[i].active && obs[i].x > rightmost) rightmost = obs[i].x;
  }
  // Spawn if rightmost far enough or no active
  bool anyActive = false;
  for (int i = 0; i < MAX_OBS; i++) if (obs[i].active) anyActive = true;
  int minGap = max(60, (int)(120 - speed * 5));
  if (!anyActive || (rightmost < SCREEN_WIDTH - minGap)) {
    // find free slot
    for (int i = 0; i < MAX_OBS; i++) {
      if (!obs[i].active) {
        obs[i].active = true;
        obs[i].x = SCREEN_WIDTH + random(10, 30);
        // Pick type; birds appear after score > 100
        int r = random(0, 4);
        if (score < 100) {
          obs[i].type = (r < 2) ? OBS_CACT_S : OBS_CACT_L;
        } else {
          if (r == 0)      obs[i].type = OBS_BIRD_LO;
          else if (r == 1) obs[i].type = OBS_BIRD_HI;
          else             obs[i].type = (random(0,2) == 0) ? OBS_CACT_S : OBS_CACT_L;
        }
        break;
      }
    }
  }

  // Move obstacles
  for (int i = 0; i < MAX_OBS; i++) {
    if (!obs[i].active) continue;
    obs[i].x -= (int)speed;
    int obsW = (obs[i].type == OBS_CACT_L) ? CACT_L_W :
               (obs[i].type == OBS_BIRD_LO || obs[i].type == OBS_BIRD_HI) ? BIRD_W : CACT_S_W;
    if (obs[i].x < -obsW) obs[i].active = false;
  }

  // Collision
  if (checkCollision()) {
    if (score > highScore) highScore = score;
    gameState = STATE_DEAD;
  }
}

// ---------- Collision ----------
bool checkCollision() {
  int dw, dh, dx, dy;
  dx = DINO_X;
  dy = (int)dino_y;
  if (ducking) {
    dw = DINO_DUCK_W; dh = DINO_DUCK_H;
  } else {
    dw = DINO_W; dh = DINO_H;
  }
  // Shrink hitbox a little for fairness
  int margin = 2;
  int dx1 = dx + margin, dy1 = dy + margin;
  int dx2 = dx + dw - margin, dy2 = dy + dh - margin;

  for (int i = 0; i < MAX_OBS; i++) {
    if (!obs[i].active) continue;
    int ox = obs[i].x, ow, oh, oy;
    switch (obs[i].type) {
      case OBS_CACT_S: ow=CACT_S_W; oh=CACT_S_H; oy=GROUND_Y-CACT_S_H; break;
      case OBS_CACT_L: ow=CACT_L_W; oh=CACT_L_H; oy=GROUND_Y-CACT_L_H; break;
      case OBS_BIRD_LO: ow=BIRD_W; oh=BIRD_H; oy=GROUND_Y-BIRD_H-4; break;
      case OBS_BIRD_HI: ow=BIRD_W; oh=BIRD_H; oy=GROUND_Y-BIRD_H-18; break;
      default: continue;
    }
    int ox1=ox, oy1=oy, ox2=ox+ow, oy2=oy+oh;
    if (dx1<ox2 && dx2>ox1 && dy1<oy2 && dy2>oy1) return true;
  }
  return false;
}

// ---------- Draw ----------
void drawGame() {
  display.clearDisplay();
  drawClouds();
  drawGround();
  drawDino();
  for (int i = 0; i < MAX_OBS; i++) {
    if (obs[i].active) drawObstacle(obs[i]);
  }
  drawHUD();
  display.display();
}

void drawDino() {
  int dy = (int)dino_y;
  if (ducking) {
    const uint8_t *frame = (animFrame == 0) ? dino_duck1 : dino_duck2;
    display.drawBitmap(DINO_X, dy, frame, DINO_DUCK_W, DINO_DUCK_H, WHITE);
  } else {
    if (!onGround) {
      // mid-air: fixed frame
      display.drawBitmap(DINO_X, dy, dino_run1, DINO_W, DINO_H, WHITE);
    } else {
      const uint8_t *frame = (animFrame == 0) ? dino_run1 : dino_run2;
      display.drawBitmap(DINO_X, dy, frame, DINO_W, DINO_H, WHITE);
    }
  }
}

void drawObstacle(Obstacle &o) {
  switch (o.type) {
    case OBS_CACT_S:
      display.drawBitmap(o.x, GROUND_Y - CACT_S_H, cactus_s, CACT_S_W, CACT_S_H, WHITE);
      break;
    case OBS_CACT_L:
      display.drawBitmap(o.x, GROUND_Y - CACT_L_H, cactus_l, CACT_L_W, CACT_L_H, WHITE);
      break;
    case OBS_BIRD_LO: {
      const uint8_t *f = (animFrame == 0) ? bird1 : bird2;
      display.drawBitmap(o.x, GROUND_Y - BIRD_H - 4, f, BIRD_W, BIRD_H, WHITE);
      break;
    }
    case OBS_BIRD_HI: {
      const uint8_t *f = (animFrame == 0) ? bird1 : bird2;
      display.drawBitmap(o.x, GROUND_Y - BIRD_H - 18, f, BIRD_W, BIRD_H, WHITE);
      break;
    }
  }
}

void drawClouds() {
  for (int i = 0; i < MAX_CLOUDS; i++) {
    if (clouds[i].active) {
      display.drawBitmap(clouds[i].x, clouds[i].y, cloud_bmp, CLOUD_W, CLOUD_H, WHITE);
    }
  }
}

void drawGround() {
  // Main ground line
  display.drawLine(0, GROUND_Y, SCREEN_WIDTH, GROUND_Y, WHITE);
  // Scrolling ground bumps
  for (int x = -groundOffset; x < SCREEN_WIDTH; x += 16) {
    display.drawPixel(x + 4,  GROUND_Y + 1, WHITE);
    display.drawPixel(x + 5,  GROUND_Y + 1, WHITE);
    display.drawPixel(x + 12, GROUND_Y + 2, WHITE);
  }
}

void drawHUD() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  // Score right-aligned
  char buf[10];
  sprintf(buf, "%05d", score);
  display.setCursor(90, 0);
  display.print(buf);
  // HI label
  display.setCursor(68, 0);
  display.print("HI");
  char hbuf[10];
  sprintf(hbuf, "%05d", highScore);
  // We just show score; hi on same line but we have limited space - just score
  display.setCursor(76, 0);
  display.print(hbuf);
}

void drawTitle() {
  display.clearDisplay();
  // Draw static dino
  display.drawBitmap(DINO_X, GROUND_Y - DINO_H, dino_run1, DINO_W, DINO_H, WHITE);
  display.drawLine(0, GROUND_Y, SCREEN_WIDTH, GROUND_Y, WHITE);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(32, 4);
  display.print("DINO");
  display.setTextSize(1);
  display.setCursor(28, 38);
  display.print("Press");
  display.display();
  delay(50);
}

void drawGameOver() {
  display.clearDisplay();
  drawGround();
  // Draw dead dino (fixed pose)
  display.drawBitmap(DINO_X, GROUND_Y - DINO_H, dino_run1, DINO_W, DINO_H, WHITE);
  // Dark X eyes effect - draw two X marks on dino head area
  display.drawLine(DINO_X+6, GROUND_Y-DINO_H+1, DINO_X+8, GROUND_Y-DINO_H+3, WHITE);
  display.drawLine(DINO_X+8, GROUND_Y-DINO_H+1, DINO_X+6, GROUND_Y-DINO_H+3, WHITE);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(38, 10);
  display.print("GAME OVER");
  char sbuf[20];
  sprintf(sbuf, "SCORE: %d", score);
  display.setCursor(34, 24);
  display.print(sbuf);
  sprintf(sbuf, "BEST:  %d", highScore);
  display.setCursor(34, 34);
  display.print(sbuf);
  display.setCursor(28, 48);
  display.print(".");
  display.display();
  delay(50);
}

//           thanks to use my code
// follow on instagram @explorer_nandhu


