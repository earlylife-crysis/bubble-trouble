//Program modified by Yashas M Salian, 210070095

#include <simplecpp>
#include <stdlib.h> //for exit function
#include <cmath> //For sqrt
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int RIGHT_MARGIN = 430;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

//Function to check collisions and to make bullets disappear and bubbles change size or disappear
void check_collision(vector<Bullet> &bullets, vector<Bubble> &bubbles)
{
    for(unsigned int i = 0; i < bullets.size(); i++)
        for(unsigned int j = 0; j < bubbles.size(); j++)
        {
            if(sqrt((bullets[i].get_center_x() - bubbles[j].get_center_x())*(bullets[i].get_center_x() - bubbles[j].get_center_x())
               + (bullets[i].get_center_y() - bubbles[j].get_center_y())*(bullets[i].get_center_y() - bubbles[j].get_center_y())) <= 5.0 + bubbles[j].get_radius())
               {
                   bullets.erase(bullets.begin() + i);
                   if(bubbles[j].get_radius() == 10)
                   {
                       bubbles.erase(bubbles.begin() + j);
                   }
                   else
                   {
                       bubbles.push_back(Bubble(bubbles[j].get_center_x(), bubbles[j].get_center_y(), (bubbles[j].get_radius()) / 2.0, -1.2 * BUBBLE_DEFAULT_VX, bubbles[j].get_vy(), bubbles[j].get_color()));
                       bubbles.push_back(Bubble(bubbles[j].get_center_x(), bubbles[j].get_center_y(), (bubbles[j].get_radius()) / 2.0, 1.2 * BUBBLE_DEFAULT_VX, bubbles[j].get_vy(), bubbles[j].get_color()));
                       bubbles.erase(bubbles.begin() + j);
                   }
               }
        }
}

//Function to check collisions between the shooter and the bubbles
void check_shooter_death(Shooter &shooter, vector<Bubble> &bubbles)
{
    for(unsigned int i = 0; i < bubbles.size(); i++)
    {
        if(bubbles[i].get_center_y() > (394.0 - bubbles[i].get_radius()) && //440 is the y coordinate of base and 46 is the height of the shooter
           bubbles[i].get_center_x() > shooter.get_body_center_x() - 9.0 - bubbles[i].get_radius() && //9 is half the width of the body of the shooter
           bubbles[i].get_center_x() < shooter.get_body_center_x() + 9.0 + bubbles[i].get_radius()) //Checking both sides of shooter
        {
            Text game_over(WINDOW_X / 2, WINDOW_Y / 2, "Game Over");
            game_over.setColor(COLOR(255, 0, 0));
            wait(5);
            exit(0);
        }
    }
}

//Takes level as the argument, and creates bubbles accordingly, as well as the level screens for levels 2 and 3
vector<Bubble> create_bubbles(int level)
{
    vector<Bubble> bubbles;
    if(level == 1)
    {
        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    }

    if(level == 2)
    {
        bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, 4 * BUBBLE_DEFAULT_RADIUS, 1.2 * BUBBLE_DEFAULT_VX, 0, COLOR(0,0,255)));
        bubbles.push_back(Bubble((3 * WINDOW_X)/4.0, BUBBLE_START_Y, 2 * BUBBLE_DEFAULT_RADIUS, -1.2 * BUBBLE_DEFAULT_VX, 0, COLOR(0, 0, 255)));
        bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, 1.2 * BUBBLE_DEFAULT_VX, 0, COLOR(0, 0, 255)));
        Text level2_prompt(WINDOW_X / 2, WINDOW_Y / 2, "Level 2"); //Level 2 Screen
        level2_prompt.setColor(COLOR(0, 0, 255));
        wait(5);
        level2_prompt.setColor(COLOR(255, 255, 255));
        level2_prompt.moveTo(WINDOW_X, WINDOW_Y);
    }

    if(level == 3)
    {
        bubbles.push_back(Bubble(WINDOW_X/5.0, BUBBLE_START_Y, 4 * BUBBLE_DEFAULT_RADIUS, 1.7 * BUBBLE_DEFAULT_VX, 0, COLOR(255, 0, 0)));
        bubbles.push_back(Bubble((2 * WINDOW_X)/5.0, BUBBLE_START_Y + 20, 1 * BUBBLE_DEFAULT_RADIUS, 1.7 * BUBBLE_DEFAULT_VX, 0, COLOR(255, 0, 0)));
        bubbles.push_back(Bubble((3 * WINDOW_X)/5.0, BUBBLE_START_Y + 10, 2 * BUBBLE_DEFAULT_RADIUS, 1.8 * BUBBLE_DEFAULT_VX, 0, COLOR(255, 0, 0)));
        bubbles.push_back(Bubble((4 * WINDOW_X)/5.0, BUBBLE_START_Y, 4 * BUBBLE_DEFAULT_RADIUS, 1.7 * BUBBLE_DEFAULT_VX, 0, COLOR(255, 0, 0)));
        bubbles.push_back(Bubble((2.5 * WINDOW_X)/5.0, BUBBLE_START_Y + 50, 4 * BUBBLE_DEFAULT_RADIUS, 1.7 * BUBBLE_DEFAULT_VX, 0, COLOR(255, 0, 0)));
        Text level3_prompt(WINDOW_X / 2, WINDOW_Y / 2, "Level 3"); //Level 3 Screen
        level3_prompt.setColor(COLOR(0, 0, 255));
        wait(5);
        level3_prompt.setColor(COLOR(255, 255, 255));
        level3_prompt.moveTo(WINDOW_X, WINDOW_Y);
    }
    return bubbles;
}


int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");

    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    //Level variable
    int level = 1;

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles(level);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    //Level Indicator
    Text level_ind(WINDOW_X / 2, BOTTOM_MARGIN, "Level 1/3");

    //Level 1 Screen
    Text level1_prompt(WINDOW_X / 2, WINDOW_Y / 2, "Level 1");
    level1_prompt.setColor(COLOR(0, 0, 255));
    wait(5);
    level1_prompt.setColor(COLOR(255, 255, 255));
    level1_prompt.moveTo(WINDOW_X, WINDOW_Y);

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }


        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        //Check collisions between bubbles and bullets
        check_collision(bullets, bubbles);

        //Check collision between shooter and bubbles
        check_shooter_death(shooter, bubbles);

        //Check if there are any bubbles remaining
        if(bubbles.size() == 0)
        {
            if(level == 3)
            {
                Text congrats(WINDOW_X / 2, WINDOW_Y / 2, "Congratulations!!");
                congrats.setColor(COLOR(0,255,0));
                wait(5);
                return 0;
            }
            else
            {
                level++;
                bubbles = create_bubbles(level);
                //Update level indicator
                if(level == 2)
                    level_ind.setMessage("Level 2/3");
                if(level == 3)
                    level_ind.setMessage("Level 3/3");
            }
        }
        wait(STEP_TIME);
    }
}
