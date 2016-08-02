
#include "flammenparser.h"


p_percent parsepercentimg(Mat grayinp, double minout, double maxout)
{
    p_percent p;
    double percent = min(sum(grayinp)[0]/(grayinp.rows*grayinp.cols*256), 1.0);
    p.floatpercent = minout+(maxout-minout)*percent;
    p.intpercent = (int)(p.floatpercent*100);
    return p;
}

int flamenparser(int source, Rect roi, double minout, double maxout)
{
    VideoCapture vid = VideoCapture();
    Mat gray1;
    Mat frame;
    bool initsize = true;
    if (vid.open(source)==false)
    {
        cerr << "Opening source failed: " << source << endl;
        return -1;
    }
    while(true)
    {
        vid.read(frame);
        cvtColor(frame, gray1, COLOR_BGR2GRAY);
        if (initsize)
        {
            if (roi.height<0)
            {
                roi.height = gray1.rows-roi.y;
            }
            if (roi.width<0)
            {
                roi.width = gray1.cols-roi.x;
            }
            if(roi.x+roi.width>gray1.cols)
            {
                cerr << "Invalid roi width or x" << endl;
                return -1;
            }
            if(roi.y+roi.height>gray1.rows)
            {
                cerr << "Invalid roi height or y" << endl;
                return -1;
            }
            initsize=false;
        }
        p_percent calcp = parsepercentimg(gray1(roi), minout, maxout);
        stringstream convs;
#ifdef USE_INT_PERCENT
        convs << calcp.intpercent;
#else
        convs << calcp.floatpercent;
#endif
        string calls = call_percent_pre + convs.str() + call_percent_post;
        system(calls.c_str());
    }
    return 0;
}
