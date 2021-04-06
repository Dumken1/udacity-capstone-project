#include <stdio.h>
#include <iostream>         
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "graphics.h"

// Global time used to simulate the time change
bool Timeflag = true; 

std::mutex g_mtx; // used to lock the console display output


//Display Background image
void Background::BackgroundSimulate(){  
    _image = cv::imread(_home_image, cv::IMREAD_COLOR);

    for (size_t i = 0; i < _points.size(); i++){
        cv::fillPoly(_image, _points.at(i), _colour.at(i), 8);
    }

    if (_image.empty()){
        std::cerr << "image not found\n";
    }

    const std::string window_name{"Home automation"};
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::imshow(window_name, _image);
    cv::waitKey(33);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    _image = cv::imread(_home_image, cv::IMREAD_COLOR);
    cv::imshow(window_name, _image);
    cv::waitKey(33);

    std::this_thread::sleep_for(std::chrono::seconds(1));
}


void Background::SetColor(int loc, Color color){
    std::unique_lock<std::mutex> lck(_mtx);
    if(color == Color::RED)    {_colour.at(loc)  = cv::Scalar(0, 0, 255);}
    if(color == Color::GREEN)  {_colour.at(loc)  = cv::Scalar(110, 220, 0);}
    if(color == Color::ORANGE) {_colour.at(loc)  = cv::Scalar(0, 125, 255);}
    if(color == Color::WHITE)  {_colour.at(loc)  = cv::Scalar(255, 255, 255);}
    if(color == Color::YELLOW) {_colour.at(loc)  = cv::Scalar(0, 255, 255);}
    if(color == Color::BLACK)  {_colour.at(loc)  = cv::Scalar(0, 0, 0);}
}

Background& Background::operator=(const Background &RHS)
{
    // do the copy
    _colour = RHS._colour;
    _points = RHS._points;
    
    // return the existing object so we can chain this operator
    return *this;
}


// Set the shape points in the image
void Background::points_init(){  
    std::vector<cv::Point> TEMPSENSOR_shape_points;
    TEMPSENSOR_shape_points.push_back(cv::Point(326, 507));
    TEMPSENSOR_shape_points.push_back(cv::Point(406, 507));
    TEMPSENSOR_shape_points.push_back(cv::Point(363, 529));
    _points.push_back(TEMPSENSOR_shape_points);

    std::vector<cv::Point> WEATHER_shape_points;
    WEATHER_shape_points.push_back(cv::Point(639, 330));
    WEATHER_shape_points.push_back(cv::Point(713, 331));
    WEATHER_shape_points.push_back(cv::Point(675, 350));
    _points.push_back(WEATHER_shape_points);

    std::vector<cv::Point> BULB_shape_points;
    BULB_shape_points.push_back(cv::Point(1017, 304));
    BULB_shape_points.push_back(cv::Point(1097, 304));
    BULB_shape_points.push_back(cv::Point(1058, 326));
    _points.push_back(BULB_shape_points); 

    std::vector<cv::Point> ENERGY_shape_points;
    ENERGY_shape_points.push_back(cv::Point(1361, 204));
    ENERGY_shape_points.push_back(cv::Point(1439, 204));
    ENERGY_shape_points.push_back(cv::Point(1399, 223));
    _points.push_back(ENERGY_shape_points);

    std::vector<cv::Point> HEATER_shape_points;
    HEATER_shape_points.push_back(cv::Point(1343, 654));
    HEATER_shape_points.push_back(cv::Point(1361, 616));
    HEATER_shape_points.push_back(cv::Point(1361, 695));
    _points.push_back(HEATER_shape_points);
}


void Background::TimeofDaySimulation(){
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    double cycleDuration = 1;
    int timetracker = 0;

     lastUpdate = std::chrono::system_clock::now();
    while (true)
    {

        std::this_thread::sleep_for(std::chrono::seconds(1));
        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if (timeSinceLastUpdate >= cycleDuration)
        {   
            std::unique_lock<std::mutex> lck(g_mtx);
            std::cout << "Time:     " << timetracker << std::endl;
            lck.unlock();

            if (timetracker < 24){timetracker++;} 
            else
            {
                timetracker = 0;
            }

            //Simulating night effect
            if((timetracker >= 18 && timetracker <= 24)){
                std::unique_lock<std::mutex> lck(g_mtx);
                std::cout << "It is night\n";
                lck.unlock();
                Timeflag = false;
            }
            else
            {
                Timeflag = true;
            }
        }
        lastUpdate = std::chrono::system_clock::now();
    }
}