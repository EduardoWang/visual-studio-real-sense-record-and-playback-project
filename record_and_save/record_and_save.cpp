#include <librealsense2/rs.hpp>
#include "example.hpp"
#include <chrono>
#include <signal.h>
#include <Windows.h>

#include <imgui.h>
#include "imgui_impl_glfw.h"

#include <sstream>
#include <iostream>
#include <iomanip>

bool flag = true;

void signalHandler(int signal);

int main() try
{
    typedef void (*SignalHandlerPointer)(int);
    SignalHandlerPointer previousHandler;
    previousHandler = signal(SIGINT, signalHandler);

    auto pipe = std::make_shared<rs2::pipeline>();
    pipe->start();
    rs2::device device = pipe->get_active_profile().get_device();
    while (flag) {
        if (!device.as<rs2::playback>() && !device.as<rs2::recorder>()) {
            pipe->stop();
            pipe = std::make_shared<rs2::pipeline>();
            rs2::config cfg;
            cfg.enable_record_to_file("a.bag");
            pipe->start(cfg);
            device = pipe->get_active_profile().get_device();
            std::cout << "begin recording" << std::endl;
        }
        Sleep(10);
    }

    pipe->stop();
    pipe = std::make_shared<rs2::pipeline>();
    pipe->start();
    device = pipe->get_active_profile().get_device();

    std::cout << "end recording" << std::endl;

    exit(0);
}
catch (const rs2::error& e)
{
    std::cout << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}

void signalHandler(int Signal) {
    if (Signal == SIGINT) {
        flag = false;
    }
}