#include <librealsense2/rs.hpp>
#include "example.hpp"
#include <chrono>
#include <windows.h>

#include <imgui.h>
#include "imgui_impl_glfw.h"

#include <sstream>
#include <iostream>
#include <iomanip>

int main() try
{
    window app(1280, 720, "RealSense Playback");

    rs2::config cfg;
    cfg.enable_device_from_file("../record_and_save/a.bag");
    rs2::pipeline pipe;
    pipe.start(cfg);

    rs2::colorizer color_map;
    rs2::rates_printer printer;

    while (app) {
        rs2::frameset frames;
        if (pipe.poll_for_frames(&frames)) {
            app.show(frames.apply_filter(printer).apply_filter(color_map));
            Sleep(33);
        }
    }
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