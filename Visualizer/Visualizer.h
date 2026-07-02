#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#include <vector>
#include <string>

#include <sstream>
#include <iomanip>

#include "../Entities/Particle.h"
#include "../SimulationParameters/SimulationParameters.h"

using namespace std;
using namespace cv;

class Visualizer
{
public:

    static void saveImage(
    const vector<vector<Particle>>& lattice,
    const SimulationParameters& params,
    double energy,
    double magnetization,
    int runNumber)
    {
        int scale = 8;

        int width = params.latticeSize * scale;
        int latticeHeight = params.latticeSize * scale;

        int infoHeight = 140;

        int y = latticeHeight + 25;
        stringstream ss;
        ss << fixed << setprecision(2);


        Mat image(
            latticeHeight + infoHeight,
            width,
            CV_8UC3,
            Scalar(255,255,255)
);
        for (int i = 0; i < params.latticeSize; i++)
        {
            for (int j = 0; j < params.latticeSize; j++)
            {
                Scalar color;

                if (lattice[i][j].spin == 1)
                {
                    color = Scalar(255, 255, 255); // White
                }
                else
                {
                    color = Scalar(0, 0, 0); // Black
                }

                rectangle(
                    image,
                    Point(j * scale, i * scale),
                    Point((j + 1) * scale, (i + 1) * scale),
                    color,
                    FILLED
                );
            }
        }
        line(
        image,
        Point(0, latticeHeight),
        Point(width, latticeHeight),
        Scalar(150,150,150),
        2
    );

        ss.str("");
        ss << "Run Number : " << runNumber;

        putText(
            image,
            ss.str(),
            Point(10, y),
            FONT_HERSHEY_SIMPLEX,
            0.5,
            Scalar(0,0,0),
            1
        );

        y += 25;

        ss.str("");
        ss.clear();
        ss << "Temperature : " << params.temperature;

        putText(
            image,
            ss.str(),
            Point(10, y),
            FONT_HERSHEY_SIMPLEX,
            0.5,
            Scalar(0,0,0),
            1
        );
        y += 25;

        ss.str("");
        ss.clear();
        ss << "Energy : " << energy;

        putText(
            image,
            ss.str(),
            Point(10, y),
            FONT_HERSHEY_SIMPLEX,
            0.5,
            Scalar(0,0,0),
            1
        );

        y += 25;

        ss.str("");
        ss.clear();
        ss << "Magnetization : " << magnetization;

        putText(
            image,
            ss.str(),
            Point(10, y),
            FONT_HERSHEY_SIMPLEX,
            0.5,
            Scalar(0,0,0),
            1
        );








        string filename =
    "Images/run_" + to_string(runNumber) + ".png";

        imwrite(filename, image);
    }
};