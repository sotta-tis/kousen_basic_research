#pragma once

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>


namespace killPro{
    extern pid_t pid;

    void killProcess();
}
