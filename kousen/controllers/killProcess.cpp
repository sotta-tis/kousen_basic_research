#include "killProcess.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>

namespace killPro {
    pid_t pid=getpid();  // このファイルで変数を実際に定義
}

void killPro::killProcess() {
    kill(pid, SIGTERM);
    std::cout << "プロセス " << pid << " を終了しました。" << std::endl;
}

//
// Created by 年澄荘多 on 24/07/05.
//
