// THIS WILL BE MOVED TO A STANDALONE PROGRAM EVENTUALLY. ONLY IN KERNEL TO TEST GUI AND MULTITASKING.
#include "graphics.h"
#include "gui.h"
#include "terminal.h"
#include "pong.h"
#include "mem.h"

#define WIDTH 600
#define HEIGHT 400

typedef struct {
    int x;
    int y;
    int xVelocity;
    int yVelocity;
    int color;
    int radius;
} Ball;

typedef struct {
    int x;
    int y;
    int height;
    int width;
    char upPressed;
    char downPressed;
    int color;
} Player;

char collide(Ball ball, Player player) {
    return (ball.x > player.x) && (ball.x < player.width + player.x) && (ball.y > player.y) && (ball.y < player.height + player.y);
}

void initPong() {
    int pongId = initWindow(WIDTH, HEIGHT, "Pong");
    Framebuffer backbuffer = {
        .width = WIDTH,
        .height = HEIGHT,
        .data = malloc(WIDTH*HEIGHT*4)
    };

    Ball ball;
    ball.x = 300;
    ball.y = 200;
    ball.xVelocity = 1;
    ball.yVelocity = 1;
    ball.color = 0x00ffffff;
    ball.radius = 5;

    Player player1;
    player1.x = 10;
    player1.y = 175;
    player1.width = 10;
    player1.height = 50;
    player1.upPressed = 0;
    player1.downPressed = 0;
    player1.color = 0x00ffffff;

    Player player2;
    player2.x = 580;
    player2.y = 175;
    player2.width = 10;
    player2.height = 50;
    player2.upPressed = 0;
    player2.downPressed = 0;
    player2.color = 0x00ffffff;

    while (1) {
        // read events and handle them
        /*Event event;
        while (checkWindowEvent(&event)) {
            if (event.type == WINDOW_CLOSE) {
                // the window has been closed, and memory freed. Do whatever cleanup you need and then exit.
                return 0;
            }
            if (event.type == KEY_CHANGE) {
                if (event.keychange.key == 17) {
                    player1.upPressed = event.keychange.state;
                }
                if (event.keychange.key == 31) {
                    player1.downPressed = event.keychange.state;
                }
                if (event.keychange.key == 103) {
                    player2.upPressed = event.keychange.state;
                }
                if (event.keychange.key == 108) {
                    player2.downPressed = event.keychange.state;
                }
            }
        }*/
       player1.y = ball.y - 25;
       player2.y = ball.y - 25;
        if (player1.upPressed) {
            player1.y -= 5;
        }
        if (player1.downPressed) {
            player1.y += 5;
        }
        if (player2.upPressed) {
            player2.y -= 5;
        }
        if (player2.downPressed) {
            player2.y += 5;
        }
        if ((ball.y > 395) || (ball.y < 5)) {
            ball.yVelocity = -ball.yVelocity;
        }
        if (ball.x < 5) {
            print("Player 2 wins!\r\n");
            break;
        }
        if (ball.x > 595) {
            print("Player 1 wins!\r\n");
            break;
        }
        if (collide(ball, player1) || collide(ball, player2)) {
            ball.xVelocity = -ball.xVelocity;
        }
        ball.x += ball.xVelocity;
        ball.y += ball.yVelocity;

        clearStruct(backbuffer);
        rectangleStruct(player1.x, player1.y, player1.width, player1.height, player1.color, backbuffer);
        rectangleStruct(player2.x, player2.y, player2.width, player2.height, player2.color, backbuffer);
        circleStruct(ball.x, ball.y, ball.radius, ball.color, backbuffer);
        updateWindow(pongId, backbuffer);
    }

    while(1);
}