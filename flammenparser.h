
#ifndef FLAMENPARSER_H
#define FLAMENPARSER_H
#include <iostream>
#include <cstdio>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
#define USE_INT_PERCENT 1


const double default_minout=0.15;
const double default_maxout=1;
const string call_percent_pre = "/usr/bin/pactl set-sink-volume @DEFAULT_SINK@ ";
const string call_percent_post = "%";

struct p_percent{
    double floatpercent;
    int intpercent;
};


p_percent parsepercentimg(Mat grayinp, double minout, double maxout);

int flamenparser(int source, Rect roi, double minout, double maxout);

#endif
