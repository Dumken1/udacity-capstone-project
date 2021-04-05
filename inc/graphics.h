#ifndef _GRAPHICS_
#define _GRAPHICS_

#include <iostream>
#include <vector>
#include <stdint.h>
#include <thread>
#include <chrono>
#include <mutex> 
#include <memory>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


extern bool Timeflag; 
extern std::mutex g_mtx;

enum class Color {RED, GREEN, ORANGE, WHITE, YELLOW, BLACK};

class Background{ 
    public:
        void BackgroundSimulate();
        void points_init();
        void SetColor(int loc, Color color);
        Background& operator=(const Background &RHS);
        void TimeofDaySimulation();

    private:
        cv::Mat _image;
        std::string _home_image = "/home/dumken/Documents/udacity_capstone_project/home_pics.png";
        std::vector<std::vector<cv::Point>> _points;
        std::vector<cv::Scalar> _colour{0,0,0,0,0};
        std::mutex _mtx;
};

#endif // !_GRAPHICS_

