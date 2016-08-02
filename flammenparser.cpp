
#include "flammenparser.h"



int flamenparser(int source, int x, int y, int w, int h, double minout, double maxout)
{
    VideoCapture vid = VideoCapture();
    Mat gray1, gray2;
    Mat frame;
    Rect roi = Rect(x,y,w,h);
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
        gray2 = gray1(roi);
        // clamp to 1.0 (100%)
        double percent = min(sum(gray2)[0]/(gray2.rows*gray2.cols*256), 1.0);
#ifdef USE_INT_PERCENT
        int ipercent = (int)((minout+(maxout-minout)*percent)*100);
#else
        double ipercent = minout+(maxout-minout)*percent;
#endif
        cout << ipercent << endl;
        stringstream convs;
        convs << ipercent;
        string calls = call_percent_pre + convs.str() + call_percent_post;
        system(calls.c_str());
    }
    return 0;
}
