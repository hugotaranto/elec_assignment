#include "stdio.h"
#include "stdlib.h"
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "time.h"

#include "formulas.h"
#include "wall.h"
#include "robot.h"

int done = 0;


int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return 1;
    }

    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, 0);

    struct Robot robot;
    struct Wall_collection *head = NULL;
    int front_left_sensor, front_right_sensor, left_side_sensor, right_side_sensor=0;
    clock_t start_time, end_time;
    int msec;

    // SETUP MAZE
    // You can create your own maze here. line of code is adding a wall.
    // You describe position of top left corner of wall (x, y), then width and height going down/to right
    // Relative positions are used (OVERALL_WINDOW_WIDTH and OVERALL_WINDOW_HEIGHT)
    // But you can use absolute positions. 10 is used as the width, but you can change this.
    // Overall window height = 480
    // Overall window width = 640

    insertAndSetFirstWall(&head, 1,  0, 0, 10, OVERALL_WINDOW_HEIGHT);
    insertAndSetFirstWall(&head, 2,  80, OVERALL_WINDOW_HEIGHT - 400, 10, 400 );
    insertAndSetFirstWall(&head, 3,  80, OVERALL_WINDOW_HEIGHT - 400, 150, 10);
    insertAndSetFirstWall(&head, 4,  300, 0, 10, 410);
    insertAndSetFirstWall(&head, 5,  160, OVERALL_WINDOW_HEIGHT - 320, 150, 10);
    insertAndSetFirstWall(&head, 6,  0, 0, OVERALL_WINDOW_WIDTH, 10);
    insertAndSetFirstWall(&head, 7,  160, OVERALL_WINDOW_HEIGHT - 240, 140, 10);
    insertAndSetFirstWall(&head, 8,  160, OVERALL_WINDOW_HEIGHT - 160, 150, 10);
    insertAndSetFirstWall(&head, 9,  80, OVERALL_WINDOW_HEIGHT - 80, 150, 10);
    insertAndSetFirstWall(&head, 10,  0, OVERALL_WINDOW_HEIGHT-10, OVERALL_WINDOW_WIDTH, 10);
    insertAndSetFirstWall(&head, 11,  380, 80, 10, 410);
    insertAndSetFirstWall(&head, 12,  460, 80, 10, 90);
    insertAndSetFirstWall(&head, 13,  550, 80, 100, 10);
    insertAndSetFirstWall(&head, 14,  OVERALL_WINDOW_WIDTH - 10, 0, 10, OVERALL_WINDOW_HEIGHT - 80);
    insertAndSetFirstWall(&head, 15,  380, 80, 90, 10);
    insertAndSetFirstWall(&head, 16,  460, 160, 90, 10);
    insertAndSetFirstWall(&head, 16,  460, 240, 180, 10);
    insertAndSetFirstWall(&head, 16,  460, 320, 100, 10);
    insertAndSetFirstWall(&head, 13,  550, 320, 10, 80);
    insertAndSetFirstWall(&head, 13,  550, 400, 90, 10);
    insertAndSetFirstWall(&head, 13,  460, 320, 10, 90);


    setup_robot(&robot);
    updateAllWalls(head, renderer);

    SDL_Event event;
    while(!done){
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        //Move robot based on user input commands/auto commands
        if (robot.auto_mode == 1)
            robotAutoMotorMove(&robot, front_left_sensor, front_right_sensor, left_side_sensor, right_side_sensor);
        robotMotorMove(&robot);

        //Check if robot reaches endpoint. and check sensor values
        if (checkRobotReachedEnd(&robot, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT/2+100, 10, 100)){
            end_time = clock();
            msec = (end_time-start_time) * 1000 / CLOCKS_PER_SEC;
            robotSuccess(&robot, msec);
        }
        else if(checkRobotHitWalls(&robot, head))
            robotCrash(&robot);
        //Otherwise compute sensor information
        else {
            front_left_sensor = checkRobotSensorFrontLeftAllWalls(&robot, head);
            // if (front_left_sensor>0)
            //     printf("Getting close on the front left. Score = %d\n", front_left_sensor);

            front_right_sensor = checkRobotSensorFrontRightAllWalls(&robot, head);
            // if (front_right_sensor>0)
            //     printf("Getting close on the front right. Score = %d\n", front_right_sensor);

            left_side_sensor = checkRobotSensorLeftAllWalls(&robot, head);
            // if (left_side_sensor>0)
            //     printf("Getting close on the left. Score = %d\n", left_side_sensor);

            right_side_sensor = checkRobotSensorRightAllWalls(&robot, head);
            // if (right_side_sensor>0)
            //     printf("Getting close on the right. Score = %d\n", right_side_sensor);
        }
        robotUpdate(renderer, &robot);
        updateAllWalls(head, renderer);

        // Check for user input
        SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                done = 1;
            }
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            if(state[SDL_SCANCODE_UP] && robot.direction != DOWN){
                robot.direction = UP;
            }
            if(state[SDL_SCANCODE_DOWN] && robot.direction != UP){
                robot.direction = DOWN;
            }
            if(state[SDL_SCANCODE_LEFT] && robot.direction != RIGHT){
                robot.direction = LEFT;
            }
            if(state[SDL_SCANCODE_RIGHT] && robot.direction != LEFT){
                robot.direction = RIGHT;
            }
            if(state[SDL_SCANCODE_SPACE]){
                setup_robot(&robot);
            }
            if(state[SDL_SCANCODE_RETURN]){
                robot.auto_mode = 1;
                start_time = clock();
            }
        }

        SDL_Delay(120);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    printf("DEAD\n");
}
