#include <iostream>
#include <sstream>
#include "raylib.h"

#define VERSION_NUMBER "v0.1.0"
#define TITLE "Soundboard " VERSION_NUMBER
#define FPS 60

int WIDTH = 800;
int HEIGHT =  600;
const int N_ROWS = 3;
const int N_COLS = 3;
const Color BACKGROUND = {.r = 40, .g = 40, .b = 40, .a = 255};
const Color RECT_COLOR = {.r = 100,.g = 100,.b = 100,.a = 255};

typedef struct Button {
  Rectangle rec;
  float textX;
  float textY;
  std::string text;
  std::string  soundFile;
  Sound sound;
} Button;

const int N_TOTAL_BUTTONS = N_ROWS * N_COLS;
Button buttons[N_TOTAL_BUTTONS];
const int PADDING = 10;
const float BUTTON_WIDTH = (float)(WIDTH / N_ROWS - PADDING);
const float BUTTON_HEIGHT = (float)(HEIGHT / N_COLS - PADDING);
const float drawWidth = (float)BUTTON_WIDTH + PADDING;
const float drawHeight = (float)BUTTON_HEIGHT + PADDING;

bool loadSoundToButton(Button (&buttons)[N_TOTAL_BUTTONS], int index, const char* soundFile) {
  buttons[index].soundFile = soundFile;
  buttons[index].sound = LoadSound(buttons[index].soundFile.c_str());
  bool result = IsSoundValid(buttons[index].sound);
  if (!result) {
     std::cout << soundFile << " is not a valid sound file!" << std::endl;    
  }
  return result;
}

int main() {
   
  SetTargetFPS(FPS);
  std::cout << TITLE << std::endl;

  std::ostringstream text;

  int counter = 0;
  for (int j = 0; j < N_COLS; j++) {
    for (int i = 0; i < N_ROWS; i++) {
       buttons[counter].rec = {
         .x = drawWidth * i,
         .y = drawHeight * j,
         .width = BUTTON_WIDTH,
         .height = BUTTON_HEIGHT
       };
       buttons[counter].textX = buttons[counter].rec.x + BUTTON_WIDTH / 2;
       buttons[counter].textY = buttons[counter].rec.y + BUTTON_HEIGHT / 2;
       text << " Button " << counter+1; // TODO: this should be the file name
       buttons[counter].text = text.str();
       text.str("");
       text.clear();
       counter++;
    }
  }

  Vector2 mousePosition;
  bool leftClick;

  InitWindow(WIDTH, HEIGHT, TITLE);
  InitAudioDevice();

  if(!IsAudioDeviceReady()) {
    CloseWindow();
    std::cout << "Error initializing audio device" << std::endl;
    return 1;
  }
  
  // TODO: implement a config file to load sounds from 
  loadSoundToButton(buttons, 0, "sounds/horns2.ogg");
  loadSoundToButton(buttons, 1, "sounds/drumloop3.ogg");
  loadSoundToButton(buttons, 2, "sounds/funk.ogg");
  loadSoundToButton(buttons, 3, "sounds/chinese6.ogg");
  loadSoundToButton(buttons, 4, "sounds/ghostssnares2.ogg");

  while(!WindowShouldClose()) {

    // TODO: play sounds when user hits key on keyboard as well
    mousePosition = GetMousePosition();
    leftClick = IsMouseButtonPressed(0);
    for (int i = 0; i < N_TOTAL_BUTTONS; i++) {
       if (leftClick && CheckCollisionPointRec(mousePosition, buttons[i].rec)) {
          std::cout << "Pressed button " << i+1 << "\n"; 
          PlaySound(buttons[i].sound);
       }
       if (IsSoundPlaying(buttons[i].sound)) {
          for (int j = 0; j < N_TOTAL_BUTTONS; j++) {
             if (j != i) {
                if (IsSoundPlaying(buttons[j].sound)) {
                    StopSound(buttons[j].sound);
                }
             }
          }
       }
    }

    BeginDrawing();
       ClearBackground(BACKGROUND);
       for (const auto& b : buttons) {
         if (IsSoundPlaying(b.sound)) {
            DrawRectangleRec(b.rec, RED);
         } else {
            DrawRectangleRec(b.rec, RECT_COLOR);
         }
         DrawText(b.text.c_str(), b.textX, b.textY, 12, RAYWHITE);
       }
    EndDrawing();

  }
  CloseWindow();
  CloseAudioDevice();
  return 0;
}
