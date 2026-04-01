#include <stdio.h> 
#include <string.h>

#include "raylib.h"
#include "tinyfiledialogs.h"

#include "loadsoundicon.h"

#define VERSION_NUMBER "v0.1.0"
#define TITLE "Soundboard " VERSION_NUMBER
#define FPS 60

#define WIDTH 800
#define HEIGHT 600
#define N_ROWS 3
#define N_COLS 3
#define PADDING 10
#define N_BUTTONS_CT (N_ROWS * N_COLS)

// TODO: add button / switch stop all sounds
// TODO: enable mute group

const int N_TOTAL_BUTTONS = N_ROWS * N_COLS;
const Color BACKGROUND = {.r = 40, .g = 40, .b = 40, .a = 255};
const Color RECT_COLOR = {.r = 100,.g = 100,.b = 100,.a = 255};
const Color LOAD_SOUND_BUTTON_COLOR = {.r = 70,.g = 70, .b = 70, .a = 255};

typedef struct Button {
  Rectangle rec;
  float textX;
  float textY;
  char text[32];
  const char* soundFile;
  Sound sound;
  Rectangle pickSoundFile;
} Button;

Button buttons[N_BUTTONS_CT]; // does this need to be static?
const float BUTTON_WIDTH = (float)(WIDTH / N_ROWS - PADDING);
const float BUTTON_HEIGHT = (float)(HEIGHT / N_COLS - PADDING);
const float drawWidth = (float)BUTTON_WIDTH + PADDING;
const float drawHeight = (float)BUTTON_HEIGHT + PADDING;

bool loadSoundToButton(Button buttons[N_TOTAL_BUTTONS], int index, const char* soundFile) {
   buttons[index].soundFile = soundFile;
   buttons[index].sound = LoadSound(buttons[index].soundFile);
   bool result = IsSoundValid(buttons[index].sound);
   if (!result) {
     printf("%s is not valid sound file!\n", soundFile); 
   }
   return result;
}

void playSoundOnKeyPress(const Button buttons[N_TOTAL_BUTTONS]) {
      if (IsKeyPressed(KEY_KP_7)) {
         PlaySound(buttons[0].sound);            
      }
      else if (IsKeyPressed(KEY_KP_8)) {
         PlaySound(buttons[1].sound);            
      }
      else if (IsKeyPressed(KEY_KP_9)) {
         PlaySound(buttons[2].sound);            
      }
      else if (IsKeyPressed(KEY_KP_4)) {
         PlaySound(buttons[3].sound);            
      }
      else if (IsKeyPressed(KEY_KP_5)) {
         PlaySound(buttons[4].sound);            
      }
      else if (IsKeyPressed(KEY_KP_6)) {
         PlaySound(buttons[5].sound);            
      }
      else if (IsKeyPressed(KEY_KP_1)) {
         PlaySound(buttons[6].sound);            
      }
      else if (IsKeyPressed(KEY_KP_2)) {
         PlaySound(buttons[7].sound);            
      }
      else if (IsKeyPressed(KEY_KP_3)) {
         PlaySound(buttons[8].sound);            
      }
}

void unloadSoundsFromButtons(const Button buttons[N_TOTAL_BUTTONS], const int N_TOTAL_BUTTONS) {
   for (int i = 0; i < N_TOTAL_BUTTONS; i++) {
      UnloadSound(buttons[i].sound);
   }
}

int main() {
   
   SetTraceLogLevel(LOG_ERROR);
   SetTargetFPS(FPS);
   printf("%s\n", TITLE);
   char buttonText[32];
   char* buttonBaseText = "Button";

   int counter = 0;
   for (int j = 0; j < N_COLS; j++) {
      for (int i = 0; i < N_ROWS; i++) {
         buttons[counter].rec = (Rectangle){
            .x = drawWidth * i,
            .y = drawHeight * j,
            .width = BUTTON_WIDTH,
            .height = BUTTON_HEIGHT
         };
         buttons[counter].pickSoundFile = (Rectangle){
            .x = buttons[counter].rec.x,
            .y = buttons[counter].rec.y,
            .width = BUTTON_WIDTH / 5,
            .height = BUTTON_HEIGHT / 5
         };
         buttons[counter].textX = buttons[counter].rec.x + BUTTON_WIDTH / 2;
         buttons[counter].textY = buttons[counter].rec.y + BUTTON_HEIGHT / 2;
         int result = snprintf(buttonText, sizeof(buttonText), "%s %d", buttonBaseText, counter+1); 
         printf("%s\n", buttonText);
         if (result<0) {
           printf("Error creating text for button %d\n", counter);
         }
         strcpy(buttons[counter].text, buttonText);
         //printf("%s\n", buttons[counter].text);
         memset(buttonText, 0, sizeof(buttonText));
         counter++;
      }
   }

   Vector2 mousePosition;
   bool leftClick;

   InitWindow(WIDTH, HEIGHT, TITLE);
   InitAudioDevice();

   if(!IsAudioDeviceReady()) {
      CloseWindow();
      printf("Error initializing audio device\n");
      return 1;
   }
  
   /*
   Image loadSoundImage = Image{
     .data=assets_loadsoundicon_png, 
     .width=35,
     .height=35, 
     .mipmaps=1, 
     .format=PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA
   };
   */
   Image loadSoundImage = LoadImageFromMemory(
       ".png", 
       assets_loadsoundicon_png,
       assets_loadsoundicon_png_len
   );
   Texture2D loadSoundTexture = LoadTextureFromImage(loadSoundImage); 
   

   while(!WindowShouldClose()) {

      mousePosition = GetMousePosition();
      leftClick = IsMouseButtonPressed(0);
      for (int i = 0; i < N_TOTAL_BUTTONS; i++) {
         if (leftClick && CheckCollisionPointRec(mousePosition, buttons[i].pickSoundFile)) {
            printf("Pressed load file button %d\n", i+1);
            const char* soundFilePath = tinyfd_openFileDialog(
               "Select file", ".", 0, NULL, NULL, 0);
            loadSoundToButton(buttons,i,soundFilePath);
            continue;
         }
         if (leftClick && CheckCollisionPointRec(mousePosition, buttons[i].rec)) {
            PlaySound(buttons[i].sound);
         }

         playSoundOnKeyPress(buttons);

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
         for (int i=0; i<N_TOTAL_BUTTONS; i++) {
           if (IsSoundPlaying(buttons[i].sound)) {
              DrawRectangleRec(buttons[i].rec, RED);
           } else {
              DrawRectangleRec(buttons[i].rec, RECT_COLOR);
           }
           DrawRectangleRec(buttons[i].pickSoundFile, LOAD_SOUND_BUTTON_COLOR);
           DrawTexture(loadSoundTexture, 
               buttons[i].pickSoundFile.x, buttons[i].pickSoundFile.y, RED);
           DrawText(buttons[i].text, buttons[i].textX, buttons[i].textY, 12, RAYWHITE);
         }
      EndDrawing();

   }
   unloadSoundsFromButtons(buttons, N_TOTAL_BUTTONS);
   //UnloadImage(loadSoundImage);
   //UnloadTexture(loadSoundTexture);
   CloseWindow();
   CloseAudioDevice();
   return 0;
}
