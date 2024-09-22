#include <Arduino.h>
// libs for display
#include <HardwareSerial.h>
#include "Diablo_Serial_4DLib.h"
#include "Diablo_Const4D.h"
#include "Node.h"
#define RESETLINE D3

Node node;

HardwareSerial DisplaySerial(1);
Diablo_Serial_4DLib Display(&DisplaySerial);
word text_72_Hndl;
word text_50_Hndl;

// put function declarations here:
void mycallback(int ErrCode, unsigned char Errorbyte);
void draw_capOutline(void);
void draw_capStatus(int bars);
void draw_Text(void);
void draw_voltageVal(String value);
void draw_currentVal(String value);
void draw_socVal(String value);

// helper functions
void offsetRectangleFilled(word x1, word y1, word x2, word y2, word colour, word x_off, word y_off);

void setup() {
  // resets the display
  pinMode(RESETLINE, OUTPUT);  //
  digitalWrite(RESETLINE, 0);  //
  delay(100);
  digitalWrite(RESETLINE, 1);  //
  delay(5000);
  // finished reseting display

  Serial.begin(9600);
  Serial.println("In setup function: Display is initialising");

  DisplaySerial.begin(9600, SERIAL_8N1, RX, TX);
  Display.TimeLimit4D = 5000;  // 5 second timeout on all commands
  Display.Callback4D = mycallback;
  Display.gfx_ScreenMode(LANDSCAPE);

  Display.gfx_Cls();
  Display.gfx_BGcolour(BLACK);
  Display.file_Mount();
  // load file handles for images and text
  text_72_Hndl = Display.file_LoadImageControl("newFonts.d01", "newFonts.g01", 1);
  text_50_Hndl = Display.file_LoadImageControl("newFonts.d01", "newFonts.g01", 1);
  draw_capOutline();
  draw_capStatus(5);
  draw_Text();
  draw_voltageVal("20");
  draw_currentVal("20");
  draw_socVal("20");

  node.begin(GPIO_NUM_3, GPIO_NUM_2, 1);  // CAN_RX, CAN_TX, CAN_STBY, CAN_LISTEN_MODE = true
  // Add multiple expected messages
  std::vector<ExpectedMessage> messages = {
    { 10, FLOAT, FLOAT },
    { 14, FLOAT, NONE }  // Example of a message with only one variable
  };
  node.addExpectedMessages(messages);
}

void loop() {
  auto parsedMessage = node.parseReceivedMessage();
  if (parsedMessage.first != 0) {  // Check if a valid message ID was returned
    uint32_t messageId = parsedMessage.first;
    String var1 = parsedMessage.second.first;
    String var2 = parsedMessage.second.second;
    if (messageId == 10) {
      draw_socVal(var1);
      draw_voltageVal(var2);
      printf("SOC: %s and Volt: %s updated\n", var1.c_str(), var2.c_str());
    } else if (messageId == 10) {
      draw_currentVal(var1);
      printf("Current: %s updated\n", var1.c_str());
    } else {
      // Now users can use messageId, var1, and var2 as needed
      printf("Unrecognised Message ID: %u\n", messageId);
    }
  }
}
// put function definitions here:
void mycallback(int ErrCode, unsigned char Errorbyte) {
  const char *Error4DText[] = { "OK\0", "Timeout\0", "NAK\0", "Length\0", "Invalid\0" };
  Serial.print(F("Serial 4D Library reports error "));
  Serial.print(Error4DText[ErrCode]);
  if (ErrCode == Err4D_NAK) {
    Serial.print(F(" returned data= "));
    Serial.println(Errorbyte);
  } else
    Serial.println(F(""));
  while (1)
    ;  // you can return here, or you can loop
}

void offsetRectangleFilled(word x1, word y1, word x2, word y2, word colour, word x_off, word y_off) {
  Display.gfx_RectangleFilled(x1 + x_off, y1 + y_off, x2 + x_off, y2 + y_off, colour);
}

void draw_capOutline(void) {
  // origin offset
  word x = 30;
  word y = 37;
  // charge increment rectangle width and height
  int incr_width = 81;
  int incr_height = 71;

  // reactangle widths are 3
  int width = 3;

  // vertical columns
  offsetRectangleFilled(0, 0, 0 + 3, 373, LIGREYGHT, x, y);                                                                    // left column
  offsetRectangleFilled(0 + width + incr_width, 0, 0 + 2 * width + incr_width, 6 * width + 5 * incr_height, LIGREYGHT, x, y);  // right column

  // horizontal rows
  for (int i = 0; i < 6; ++i) {
    offsetRectangleFilled(0, i * (width + incr_height), 2 * width + incr_width, i * (width + incr_height) + width, LIGREYGHT, x, y);
  }
}
// args must be 1 to 5 (inclusive)
void draw_capStatus(int bars) {
  // origin offset
  int x = 30;
  int y = 37 + 373;  // add because the first charge increment is the bottom one
  // charge increment rectangle width and height
  int incr_width = 81;
  int incr_height = 71;

  // reactangle widths are 3
  int width = 3;

  if (-1 < bars < 6)  // checks for valid input
  {
    for (int i = 0; i < 5; ++i) {
      if (i < bars) {
        offsetRectangleFilled((width), -(width + incr_height + i * (width + incr_height)), (width + incr_width), -(width + i * (width + incr_height)), CYAN, x, y);
      } else {
        offsetRectangleFilled((width), -(width + incr_height + i * (width + incr_height)), (width + incr_width), -(width + i * (width + incr_height)), BLACK, x, y);
      }
    }
  }
}

void draw_Text(void) {
  Display.txt_FontID(text_50_Hndl);

  int text_size = 1;
  Display.txt_Height(text_size);
  Display.txt_Width(text_size);
  Display.txt_FGcolour(ORCHID);

  Display.gfx_MoveTo(157, 35);
  Display.putstr("Voltage");

  Display.gfx_MoveTo(157, 150);
  Display.putstr("Current");

  Display.gfx_MoveTo(157, 265);
  Display.putstr("Supcap");

  Display.gfx_MoveTo(157, 345);
  Display.putstr("SOC");

  Display.gfx_MoveTo(0, 0);
}

void draw_voltageVal(String value) {
  value = value + " V";
  Display.txt_FontID(text_72_Hndl);

  Display.gfx_MoveTo(522, 18);
  int text_size = 1;
  Display.txt_Height(text_size);
  Display.txt_Width(text_size);
  Display.txt_FGcolour(CYAN);
  // value.c_str() returns a const char * but putstr() requires char *. cbuffer is a char *. strcpy copies the elements of one into the other.
  char cbuffer[10] = "";  // ANSI C string (char* array); adjust size appropriately
  strcpy(cbuffer, value.c_str());
  Display.putstr(cbuffer);

  Display.gfx_MoveTo(0, 0);
}

void draw_currentVal(String value) {
  value = value + " A";
  Display.txt_FontID(text_72_Hndl);

  Display.gfx_MoveTo(522, 145);
  int text_size = 1;
  Display.txt_Height(text_size);
  Display.txt_Width(text_size);
  Display.txt_FGcolour(CYAN);
  // value.c_str() returns a const char * but putstr() requires char *. cbuffer is a char *. strcpy copies the elements of one into the other.
  char cbuffer[10] = "";  // ANSI C string (char* array); adjust size appropriately
  strcpy(cbuffer, value.c_str());
  Display.putstr(cbuffer);

  Display.gfx_MoveTo(0, 0);
}

void draw_socVal(String value) {
  value = value + " %";
  Display.txt_FontID(text_72_Hndl);

  Display.gfx_MoveTo(522, 280);
  int text_size = 1;
  Display.txt_Height(text_size);
  Display.txt_Width(text_size);
  Display.txt_FGcolour(CYAN);
  // value.c_str() returns a const char * but putstr() requires char *. cbuffer is a char *. strcpy copies the elements of one into the other.
  char cbuffer[10] = "";  // ANSI C string (char* array); adjust size appropriately
  strcpy(cbuffer, value.c_str());
  Display.putstr(cbuffer);

  Display.gfx_MoveTo(0, 0);
}