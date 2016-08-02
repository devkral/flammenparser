
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


int flamenparser(int source, int x, int y, int w, int h, double minout, double maxout);

#endif
