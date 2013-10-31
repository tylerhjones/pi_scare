#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "GPIOClass.h"
using namespace std;
 
void sig_handler(int sig);
 
bool ctrl_c_pressed = false;
 
int main (void)
{
 
    struct sigaction sig_struct;
    sig_struct.sa_handler = sig_handler;
    sig_struct.sa_flags = 0;
    sigemptyset(&sig_struct.sa_mask);
 
    if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
        cout << "Problem with sigaction" << endl;
        exit(1);
    }
 
    string inputstate;
    GPIOClass* gpio4 = new GPIOClass("4");
 
    gpio4->export_gpio();
 
    cout << " GPIO pins exported" << endl;
 
    gpio4->setdir_gpio("in");
 
    cout << " Set GPIO pin directions" << endl;
 
    while(1)
    {
        usleep(500000); // wait 0.5 seconds
        gpio4->getval_gpio(inputstate);
        cout << "Current input pin state is " << inputstate  <<endl;
        if(inputstate == "1")
        {
           usleep(20000); // wait 20ms
                   cout << "Checking again ....." << endl;
                   gpio4->getval_gpio(inputstate);
            if(inputstate == "1")
            {
                cout << "motion definately detected. playing sound" << endl;
                system("omxplayer blurb.mp3 -o local");
                sleep(2); //wait 2 seconds after audio plays.
            }
            else
                cout << "motion not detected. That was just noise." <<endl;
 
        }
        gpio4->setval_gpio("0");
 
         if(ctrl_c_pressed)
                    {
                        cout << "Ctrl^C Pressed" << endl;
                        cout << "unexporting pin" << endl;
                        gpio4->unexport_gpio();
                        cout << "deallocating GPIO Object" << endl;
                        delete gpio4;
                        gpio4 = 0;
                        break;
 
                    }
 
    }
    cout << "Exiting....." << endl;
    return 0;
}
 
void sig_handler(int sig)
{
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
}