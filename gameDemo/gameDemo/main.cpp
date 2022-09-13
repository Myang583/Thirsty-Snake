#include "main.h"
#include "cmps.h"
#include "time.h"
#include "stdio.h"
#include <windowsx.h>
#include<vector>

boolean bPaintMain = false;
boolean bRestartPress = false;

const char* textfield_length_class = "Edit";
const char* textfield_time_class = "Edit";
const char* button_restart_class = "button";
const char* button_pause_class = "button";

RECT rect_food;
vector<RECT> vec_snake;
HWND hWnd_textfield_length;
HWND hWnd_textfield_time;
HWND hWnd_button_restart;
HWND hWnd_button_pause;
extern HWND hWnd;
HANDLE thread_handle;
DWORD thread_id;

int int_length = 1;
int game_time = 0;

int movedirection;


void drawMainFrame(HINSTANCE hInst, HWND hWnd, HDC hdc) {
	// draw panel
    if (bPaintMain == false) {
        
        initMainFrame(hInst, hWnd, hdc);
        bPaintMain = true;
    }
	
    if (bRestartPress == true) {

        restartGame(hdc);

        bRestartPress = false;
    }

    if (bPaintMain == true && bRestartPress == false) {

        if (movedirection != HOLD_ON) {
            ResumeThread(thread_handle);
        }

        for (int i = vec_snake.size() - 1; i >= 0; i--) {

            FillRect(hdc, &vec_snake[i], (HBRUSH)(COLOR_WINDOW + 5));
        }
        FillRect(hdc, &rect_food, (HBRUSH)(COLOR_WINDOW + 8));

        //update time text
        char  c_time[50];
        sprintf_s(c_time, "%d", game_time);
        SetWindowTextA(hWnd_textfield_time, c_time);
    }
    
}

bool moveSnake(int length_step, int direction) {
    int vec_snake_end = vec_snake.size() - 1;
    RECT rect_snake_end = vec_snake[vec_snake_end];
    
    // check if snake pass border
    if (rect_snake_end.left <= 20 || rect_snake_end.top <= 20
        || rect_snake_end.right >= 500 || rect_snake_end.bottom >= 500) {

        setRestart(true);
       
    }

    // check if snake head hit wail
    if (vec_snake.size() >= 3 && (vec_snake[0].left < vec_snake[vec_snake_end].right
        && vec_snake[0].right > vec_snake[vec_snake_end].left
        && vec_snake[0].top < vec_snake[vec_snake_end].bottom
        && vec_snake[0].bottom > vec_snake[vec_snake_end].top)) {

         setRestart(true);
         
    }


    // check if snake eat food
    if (rect_snake_end.left <= rect_food.right && rect_snake_end.right >= rect_food.left
        && rect_snake_end.top <= rect_food.bottom && rect_snake_end.bottom >= rect_food.top) {

        RECT rect_snake_new;
        
        switch (direction)
        {
        case MOVE_LEFT: {

            rect_snake_new.left = rect_snake_end.left - 15;
            rect_snake_new.right = rect_snake_end.right - 15;
            rect_snake_new.top = rect_snake_end.top;
            rect_snake_new.bottom = rect_snake_end.bottom;
            vec_snake.push_back(rect_snake_new);

            break;
        }
        case MOVE_RIGHT: {

            rect_snake_new.left = rect_snake_end.left + 15;
            rect_snake_new.right = rect_snake_end.right + 15;
            rect_snake_new.top = rect_snake_end.top;
            rect_snake_new.bottom = rect_snake_end.bottom;
            vec_snake.push_back(rect_snake_new);

            break;
        }
        case MOVE_UP: {

            rect_snake_new.top = rect_snake_end.top - 15;
            rect_snake_new.bottom = rect_snake_end.bottom - 15;
            rect_snake_new.left = rect_snake_end.left;
            rect_snake_new.right = rect_snake_end.right;
            vec_snake.push_back(rect_snake_new);

            break;
        }
        case MOVE_DOWN: {

            rect_snake_new.top = rect_snake_end.top + 15;
            rect_snake_new.bottom = rect_snake_end.bottom + 15;
            rect_snake_new.left = rect_snake_end.left;
            rect_snake_new.right = rect_snake_end.right;
            vec_snake.push_back(rect_snake_new);

            break;
        }
        default:
            break;
        }

        // update food location
        srand(time(0));
        rect_food.left = rand() % 300 + 50;
        rect_food.top = rand() % 300 + 50;
        rect_food.right = rect_food.left + 15;
        rect_food.bottom = rect_food.top + 15;

        //update length text
        char  c_length[50];
        int_length = int_length + 1;
        sprintf_s(c_length, "%d", int_length);
        SetWindowTextA(hWnd_textfield_length, c_length);
       

        return true;
    }

    
    if (movedirection != HOLD_ON) {
    
        for (int i = 0; i < vec_snake.size() - 1; i++) {

            vec_snake[i].left = vec_snake[i + 1].left;
            vec_snake[i].right = vec_snake[i + 1].right;
            vec_snake[i].top = vec_snake[i + 1].top;
            vec_snake[i].bottom = vec_snake[i + 1].bottom;

        }
    }
    
    int index_snake_head = vec_snake.size() - 1;

    switch (direction)
    {
    case MOVE_LEFT: {

        // check if sneak move is correct
        if (index_snake_head >= 2 && (vec_snake[index_snake_head].left > vec_snake[index_snake_head-2].left)
             && (vec_snake[index_snake_head - 2].top == vec_snake[index_snake_head].top)) {
        
            vec_snake[index_snake_head].left = vec_snake[index_snake_head].left + 15;
            vec_snake[index_snake_head].right = vec_snake[index_snake_head].right + 15;
        }
        else {

            vec_snake[index_snake_head].left = vec_snake[index_snake_head].left - 15;
            vec_snake[index_snake_head].right = vec_snake[index_snake_head].right - 15;
        }

        break;
    }
    case MOVE_RIGHT: {

        if (index_snake_head >= 2 &&  (vec_snake[index_snake_head - 2].left > vec_snake[index_snake_head].left)
            && (vec_snake[index_snake_head - 2].top == vec_snake[index_snake_head].top)) {

            vec_snake[index_snake_head].left = vec_snake[index_snake_head].left - 15;
            vec_snake[index_snake_head].right = vec_snake[index_snake_head].right - 15;
        }
        else {
            vec_snake[index_snake_head].left = vec_snake[index_snake_head].left + 15;
            vec_snake[index_snake_head].right = vec_snake[index_snake_head].right + 15;

        }

        break;
    }
    case MOVE_UP: {

        // check if sneak move is correct
        if (index_snake_head >= 2 && (vec_snake[index_snake_head].top > vec_snake[index_snake_head - 2].top)
            && (vec_snake[index_snake_head - 2].left == vec_snake[index_snake_head].left)) {

            vec_snake[index_snake_head].top = vec_snake[index_snake_head].top + 15;
            vec_snake[index_snake_head].bottom = vec_snake[index_snake_head].bottom + 15;
        }
        else {

            vec_snake[index_snake_head].top = vec_snake[index_snake_head].top - 15;
            vec_snake[index_snake_head].bottom = vec_snake[index_snake_head].bottom - 15;
        }

        break;
    }
    case MOVE_DOWN: {

        // check if sneak move is correct
        if (index_snake_head >= 2 && (vec_snake[index_snake_head].top < vec_snake[index_snake_head - 2].top)
            && (vec_snake[index_snake_head - 2].left == vec_snake[index_snake_head].left)) {

            vec_snake[index_snake_head].top = vec_snake[index_snake_head].top - 15;
            vec_snake[index_snake_head].bottom = vec_snake[index_snake_head].bottom - 15;
        }
        else {

            vec_snake[index_snake_head].top = vec_snake[index_snake_head].top + 15;
            vec_snake[index_snake_head].bottom = vec_snake[index_snake_head].bottom + 15;
        }

        break;
    }
    default:
        break;
    }

    return true;
}

DWORD WINAPI timeCaculator(LPVOID data) {

    clock_t  current = clock();
    
    while (true) {
        if ((clock() - current) / 1000 >= 1) {
            game_time++;
            current = clock();
        }
    }
    return NULL;
}

int getCurrentTime() {

    return game_time;
}

void restartGame(HDC hdc) {

    int vec_snake_end = vec_snake.size() - 1;
    RECT rect_snake_end = vec_snake[vec_snake_end];

    // check if snake pass border
    if (rect_snake_end.left <= 20 || rect_snake_end.top <= 20
        || rect_snake_end.right >= 500 || rect_snake_end.bottom >= 500) {


        char message[30] = "failed! your snake hit wall.";
        MessageBoxA(hWnd, message, NULL, MB_OK);

    }

    // check if snake head hit wail
    if (vec_snake.size() >= 3 && (vec_snake[0].left < vec_snake[vec_snake_end].right
        && vec_snake[0].right > vec_snake[vec_snake_end].left
        && vec_snake[0].top < vec_snake[vec_snake_end].bottom
        && vec_snake[0].bottom > vec_snake[vec_snake_end].top)) {

        char message[35] = "failed! your snake hit its wailer.";
        MessageBoxA(hWnd, message, NULL, MB_OK);

    }

    //initialize snake and food
    RECT rect_snake;

    rect_snake.left = 260;
    rect_snake.top = 260;
    rect_snake.right = 275;
    rect_snake.bottom = 275;
    FillRect(hdc, &rect_snake, (HBRUSH)(COLOR_WINDOW + 5));

    vec_snake.erase(vec_snake.begin(), vec_snake.end());
    vec_snake.push_back(rect_snake);

    srand(time(0));
    rect_food.left = rand() % 300 + 50;
    rect_food.top = rand() % 300 + 50;
    rect_food.right = rect_food.left + 15;
    rect_food.bottom = rect_food.top + 15;
    FillRect(hdc, &rect_food, (HBRUSH)(COLOR_WINDOW + 8));

    //update time text
    game_time = 0;
    char  c_time[50];
    sprintf_s(c_time, "%d", game_time);
    SetWindowTextA(hWnd_textfield_time, c_time);
    SuspendThread(thread_handle);

    //update length text
    int_length = 1;
    char  c_length[50];
    sprintf_s(c_length, "%d", int_length);
    SetWindowTextA(hWnd_textfield_length, c_length);

    movedirection = HOLD_ON;
}


void initMainFrame(HINSTANCE hInst, HWND hWnd, HDC hdc) {

    RECT rect;
    rect.left = 20;
    rect.top = 20;
    rect.right = 500;
    rect.bottom = 500;
    DrawEdge(hdc, &rect, BDR_SUNKENINNER | BDR_SUNKENOUTER, BF_RECT);

    //draw time display label
    TextFrameManager tfm_label_time(hdc);
    char time_char[8] = "时间:";  // or {'', '', '', .....}
    tfm_label_time.setOutput(time_char);
    RECT rect_label_time;
    rect_label_time.left = 550;
    rect_label_time.right = 600;
    rect_label_time.top = 100;
    rect_label_time.bottom = 150;
    tfm_label_time.setRect(rect_label_time);
    tfm_label_time.setTextColor(0x00000000);
    tfm_label_time.drawText(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

    //draw time textfield
    const char* textfield_time_content = "0";
    hWnd_textfield_time = CreateWindowExA(NULL, textfield_time_class, textfield_time_content, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_RIGHT
        , 620, 118, 80, 20, hWnd, NULL, hInst, NULL);
    //create time caculator thread
    thread_handle = CreateThread(NULL, 0, timeCaculator, (LPVOID)1, CREATE_SUSPENDED, &thread_id);


    //draw length label
    TextFrameManager tfm_label_length(hdc);
    char length[8] = "身长:";  // or {'', '', '', .....}
    tfm_label_length.setOutput(length);
    RECT rect_label_length;
    rect_label_length.left = 550;
    rect_label_length.right = 600;
    rect_label_length.top = 200;
    rect_label_length.bottom = 250;
    tfm_label_length.setRect(rect_label_length);
    tfm_label_length.setTextColor(0x00000000);
    tfm_label_length.drawText(DT_CENTER | DT_SINGLELINE | DT_VCENTER);

    //draw length textfield
    const char* textfield_length_content = "1";
    hWnd_textfield_length = CreateWindowExA(NULL, textfield_length_class, textfield_length_content, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_RIGHT
        , 620, 216, 80, 20, hWnd, NULL, hInst, NULL);

    //create restart button
    const char* textfield_button_restart = "重新开始";
    hWnd_button_restart = CreateWindowExA(NULL, button_restart_class, textfield_button_restart, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE
        , 550, 300, 150, 50, hWnd, (HMENU)90, hInst, NULL);


    //create end button
    const char* textfield_button_pause = "暂停";
    hWnd_button_pause = CreateWindowExA(NULL, button_pause_class, textfield_button_pause, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE
        , 550, 400, 150, 50, hWnd, (HMENU)91, hInst, NULL);

    //initialize snake and food
    RECT rect_snake;
    rect_snake.left = 260;
    rect_snake.top = 450;
    rect_snake.right = 275;
    rect_snake.bottom = 465;
    FillRect(hdc, &rect_snake, (HBRUSH)(COLOR_WINDOW + 5));
    vec_snake.push_back(rect_snake);

    srand(time(0));
    rect_food.left = rand() % 300 + 50;
    rect_food.top = rand() % 300 + 50;
    rect_food.right = rect_food.left + 15;
    rect_food.bottom = rect_food.top + 15;
    FillRect(hdc, &rect_food, (HBRUSH)(COLOR_WINDOW + 8));

    movedirection = HOLD_ON;

}

void setRestart(bool bRestart){

    bRestartPress = bRestart;
}

void setDirection(int direction) {

    movedirection = direction;
}

int getDirection() {

    return movedirection;
}