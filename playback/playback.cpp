#include <librealsense2/rs.hpp>
#include "example.hpp"
#include <chrono>
#include <windows.h>

#include <imgui.h>
#include "imgui_impl_glfw.h"

#include <sstream>
#include <iostream>
#include <iomanip>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() try
{
    // window app(1280, 720, "RealSense Playback");

    rs2::config cfg;
    cfg.enable_device_from_file("../record_and_save/realtime_recording.bag");
    rs2::pipeline pipe;
    pipe.start(cfg);
    rs2::device device = pipe.get_active_profile().get_device();
    rs2::playback playback = device.as<rs2::playback>();
    playback.set_real_time(false);

    // might use in depth image
    rs2::colorizer color_map;
    rs2::rates_printer printer;

    rs2::frameset frame;

    int curr_position;
    int last_position = -1;

    while (true) {
        // comment this part to allow play on a loop
        pipe.try_wait_for_frames(&frame);
        curr_position = frame.get_frame_number();
        std::cout << curr_position << " " << last_position << " " << std::endl;
        if (curr_position - last_position < 0) {
            break;
        }
        last_position = curr_position;
        //

        // app.show(frame.apply_filter(printer).apply_filter(color_map));
        // Sleep(33); // because recording is 30 FPS

        std::stringstream filename;
        filename << "outputPNG/" << curr_position << ".png";
        auto colorframe = frame.get_color_frame();
        stbi_write_png(filename.str().c_str(), colorframe.get_width(), colorframe.get_height(),
            colorframe.get_bytes_per_pixel(), colorframe.get_data(), colorframe.get_stride_in_bytes());
    } 

    std::cout << "done" << std::endl;

    pipe.stop();
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