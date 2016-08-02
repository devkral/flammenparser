
#include "calibrate.h"
#include "flammenparser.h"

Rect roi=Rect(0,0,-1,-1);

int curminout=(int)(default_minout*100);
int curmaxout=(int)(default_maxout*100);

int maxheight=-1;
int maxwidth=-1;

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

void cleanup()
{
    destroyWindow("calibrateCamera");
}

int calibrate(int source)
{
    VideoCapture vid = VideoCapture();
    Mat gray1;
    Mat frame;
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
    createTrackbar("minouttrack", "calibrateCamera", &curminout, max(200, (int)(default_maxout*100)));
    createTrackbar("maxouttrack", "calibrateCamera", &curmaxout, max(200, (int)(default_maxout*100)));

    while(true)
    {
        vid.read(frame);
        cvtColor(frame, gray1, COLOR_BGR2GRAY);
        rectangle(gray1,roi, Scalar(255),2);
        imshow("calibrateCamera",gray1);
        char key = waitKey(50);
        switch(key)
        {
            case 'q':
            case 'c':
                cleanup();
                cout << roi.x << " " << roi.y << " " << roi.width << " " << roi.height << " " << curminout/100.0 << " " << curmaxout/100.0 << endl;
                return 0;
            case 's':
                cleanup();
                return flamenparser(source, roi.x, roi.y, roi.width, roi.height, curminout/100.0, curmaxout/100.0);
        }
    }
    return 0;
}
