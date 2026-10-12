#include "terminal.h"

#include "raylib.h"
#include <string.h>
#include <stdio.h>

enum{
    TERMINAL_CLOSED,
    TERMINAL_OPENED
};

int terminal_state = 0;

char* buffer[128];
char command_line[256] = "";

int total_char = 0;
int current_char = 0;

void DrawTerminal()
{
    DrawRectangleV((Vector2){ 0, 0 }, (Vector2){GetWindowPosition().x, GetWindowPosition().y/2}, (Color){ 255, 255, 255, 127});
    int buf = GetCharPressed();
    if(buf != 0)
    {
        command_line[current_char] = buf;
        total_char++;
        current_char++;
        printf("%s\n", command_line);
    }

    if(IsKeyPressed(KEY_BACKSPACE))
    {
        total_char--;
        current_char--;
        if(total_char < 0)
        {
            current_char = 0;
            total_char = 0;
        } 
        command_line[current_char] = "";
        printf("%s\n", command_line);
    }

    if(IsKeyPressed(KEY_LEFT))
    {
        current_char--;
        if(current_char < 0) current_char = 0;
    }

    if(IsKeyPressed(KEY_RIGHT))
    {
        current_char++;
        if(current_char > total_char) current_char = total_char;
    }   
}

void HandleTerminal()
{
    DrawTerminal();
}
