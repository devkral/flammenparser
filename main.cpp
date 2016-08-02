#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include "calibrate.h"
#include "flammenparser.h"

using namespace std;


int main(int argc, char *argv[])
{
    if(argc==2)
    {
        return calibrate(atoi(argv[1]));
    }else if (argc==6)
    {
        return flamenparser(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), default_minout, default_maxout);
    }
    else if(argc==8)
    {
        return flamenparser(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atof(argv[6]), atof(argv[7]));
    }
    else
    {
        cerr << "Invalid amount parameters, needed: source [x y width height [minout maxout]]" << endl;
        return 1;
    }
}
