
#include "calibrate.h"
#include "flammenparser.h"

Rect roi=Rect(0,0,-1,-1);

int curminout=(int)(default_minout*100);
int curmaxout=(int)(default_maxout*100);

int maxheight=-1;
int maxwidth=-1;

void setcurmin(int _curmin, void*)
{
    // better set twice than to create endless loop (createTrackbar set too curminout)
    curminout = _curmin;
    if (max(curmaxout, curminout) != curmaxout)
        setTrackbarPos("maxouttrack", "calibrateCamera", max(curmaxout, curminout));
}
void setcurmax(int _curmax, void*)
{
    // better set twice than to create endless loop (createTrackbar set too curmaxout)
    curmaxout = _curmax;
    if (min(curminout, curmaxout) != curminout)
        setTrackbarPos("minouttrack", "calibrateCamera", min(curminout, curmaxout));
}

void onMouse(int event, int x, int y, int, void* )
{
    if(event == EVENT_LBUTTONDOWN)
    {
        roi.x=x;
        roi.y=y;
        roi.width=min(roi.width,maxwidth-roi.x);
        roi.height=min(roi.height,maxheight-roi.y);
    }
    if (event == EVENT_RBUTTONDOWN)
    {
        if (roi.x>=x || roi.y>=y)
            return;
        roi.width=x-roi.x;
        roi.height=y-roi.y;
    }

}

void cleanup(VideoCapture &vid)
{
    vid.release();
    destroyWindow("calibrateCamera");
}

int calibrate(int source)
{
    VideoCapture vid = VideoCapture();
    Mat gray1;
    Mat frame;
    p_percent result;
    if (vid.open(source)==false)
    {
        cerr << "Opening source failed: " << source << endl;
        return -1;
    }
    vid.read(frame);
    maxwidth = frame.cols;
    roi.width = frame.cols;
    maxheight = frame.rows;
    roi.height = frame.rows;
    namedWindow("calibrateCamera");
    setMouseCallback("calibrateCamera", onMouse);
    createTrackbar("minouttrack", "calibrateCamera", &curminout, max(200, (int)(default_maxout*100)),setcurmin);
    createTrackbar("maxouttrack", "calibrateCamera", &curmaxout, max(200, (int)(default_maxout*100)),setcurmax);

    while(true)
    {
        vid.read(frame);
        cvtColor(frame, gray1, COLOR_BGR2GRAY);
        result = parsepercentimg(gray1(roi),curminout/100.0,curmaxout/100.0);
        rectangle(gray1,roi, Scalar(255),2);
        stringstream convs;
        convs << "Current strength: ";
        convs << result.intpercent;
        convs << "%";
        putText(gray1, convs.str(), Point(10,60),FONT_HERSHEY_SIMPLEX, 1,Scalar(255));
        imshow("calibrateCamera",gray1);
        char key = waitKey(50);
        switch(key)
        {
            case 'q':
            case 'c':
                cleanup(vid);
                cout << source << " " << roi.x << " " << roi.y << " " << roi.width << " " << roi.height << " " << curminout/100.0 << " " << curmaxout/100.0 << endl;
                return 0;
            case 's':
                cleanup(vid);
                return flamenparser(source, roi, curminout/100.0, curmaxout/100.0);
        }
    }
    return 0;
}
