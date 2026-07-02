#pragma once

#include "gif.h"

#include <opencv2/opencv.hpp>
#include <filesystem>
#include <sstream>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

class GIFGenerator
{
public:

    static void createGIF(
        int runNumber,
        int width,
        int height,
        int frameInterval)
    {
        filesystem::create_directories("../Dataset/GIFs");

        GifWriter writer;
        string gifName =
                            "../Dataset/GIFs/run_" +
                          to_string(runNumber) +
                         ".gif";

        if (!GifBegin(
        &writer,
        gifName.c_str(),
        width,
        height,
        5))
        {
            cout << "Failed to create GIF!" << endl;
            return;
        }

        for(int step = 0; ; step += frameInterval)
        {
            stringstream frameName;

            frameName << "../Dataset/Frames/Run_"
                      << runNumber
                      << "/frame_"
                      << setw(4)
                      << setfill('0')
                      << step
                      << ".png";

            Mat frame = imread(frameName.str());

            if(frame.empty())
            {
                break;
            }

            Mat rgba;

            cvtColor(
                frame,
                rgba,
                COLOR_BGR2RGBA
            );

            GifWriteFrame(
                &writer,
                rgba.data,
                width,
                height,
                5
            );
        }

        GifEnd(&writer);

        // We'll add the rest here step by step...
    }
};