#include <stdio.h>
#include <iostream>         
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "graphics.h"

std::mutex mtx;
bool Timeflag = true;

void background::BackgroundSimulate(){  

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


void background::SetColor(int loc, Color color){
    if(color == Color::RED)    {_colour.at(loc)  = cv::Scalar(0, 0, 255);}
    if(color == Color::GREEN)  {_colour.at(loc)  = cv::Scalar(110, 220, 0);}
    if(color == Color::ORANGE) {_colour.at(loc)  = cv::Scalar(0, 125, 255);}
    if(color == Color::WHITE)  {_colour.at(loc)  = cv::Scalar(255, 255, 255);}
    if(color == Color::YELLOW) {_colour.at(loc)  = cv::Scalar(0, 255, 255);}
}

background& background::operator= (const background &RHS)
{
    // do the copy
    _colour = RHS._colour;
    _points = RHS._points;
    
    // return the existing object so we can chain this operator
    return *this;
}

void background::points_init(){

    std::vector<cv::Point> DOOR_shape_points;
    DOOR_shape_points.push_back(cv::Point(155, 554));
    DOOR_shape_points.push_back(cv::Point(258, 554));
    DOOR_shape_points.push_back(cv::Point(203, 578));
    _points.push_back(DOOR_shape_points);
    

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

    std::vector<cv::Point> AIR_shape_points;
    AIR_shape_points.push_back(cv::Point(849, 354));
    AIR_shape_points.push_back(cv::Point(928, 354));
    AIR_shape_points.push_back(cv::Point(888, 374));
    _points.push_back(AIR_shape_points);

    std::vector<cv::Point> BULB_shape_points;
    BULB_shape_points.push_back(cv::Point(1017, 304));
    BULB_shape_points.push_back(cv::Point(1097, 304));
    BULB_shape_points.push_back(cv::Point(1058, 326));
    _points.push_back(BULB_shape_points);

    std::vector<cv::Point> LIGHT_shape_points;
    LIGHT_shape_points.push_back(cv::Point(1192, 254));
    LIGHT_shape_points.push_back(cv::Point(1269, 254));
    LIGHT_shape_points.push_back(cv::Point(1228, 273));
    _points.push_back(LIGHT_shape_points);

    std::vector<cv::Point> ENERGY_shape_points;
    ENERGY_shape_points.push_back(cv::Point(1361, 204));
    ENERGY_shape_points.push_back(cv::Point(1439, 204));
    ENERGY_shape_points.push_back(cv::Point(1399, 223));
    _points.push_back(ENERGY_shape_points);

    std::vector<cv::Point> WINDOW_shape_points;
    WINDOW_shape_points.push_back(cv::Point(1343, 424));
    WINDOW_shape_points.push_back(cv::Point(1361, 383));
    WINDOW_shape_points.push_back(cv::Point(1361, 465));
    _points.push_back(WINDOW_shape_points);

    std::vector<cv::Point> HEATER_shape_points;
    HEATER_shape_points.push_back(cv::Point(1343, 654));
    HEATER_shape_points.push_back(cv::Point(1361, 616));
    HEATER_shape_points.push_back(cv::Point(1361, 695));
    _points.push_back(HEATER_shape_points);
}

uint32_t background::TimeofDaySimulation(){
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    double cycleDuration = 1;
    int timetracker = 0;

     lastUpdate = std::chrono::system_clock::now();
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lck(mtx);
        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if (timeSinceLastUpdate >= cycleDuration)
        {   
            std::cout << "Time:     " << timetracker << std::endl;
            if (timetracker < 24){timetracker++;}
            else
            {
                timetracker = 0;
            }
            if(timetracker > 18 && timetracker < 24){
                std::cout << "It is night\n";
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