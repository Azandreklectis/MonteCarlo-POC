#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

#include <vector>
#include <string>

#include <sstream>
#include <iomanip>
#include <filesystem>

#include "../Entities/Particle.h"
#include "../SimulationParameters/SimulationParameters.h"

using namespace std;
using namespace cv;

class Visualizer
{
private:
    static constexpr int SCALE = 8;

    static void drawLattice(
        Mat& image,
        const vector<vector<Particle>>& lattice,
        int latticeSize,
        int scale)
    {
        for (int i = 0; i < latticeSize; i++)
        {
            for (int j = 0; j < latticeSize; j++)
            {
                Scalar color;

                if (lattice[i][j].spin == 1)
                    color = Scalar(255,255,255);
                else
                    color = Scalar(0,0,0);

                rectangle(
                    image,
                    Point(j * scale, i * scale),
                    Point((j + 1) * scale, (i + 1) * scale),
                    color,
                    FILLED
                );
            }
        }
    }

public:

    static void saveImage(
        const vector<vector<Particle>>& lattice,
        const SimulationParameters& params,
        double energy,
        double magnetization,
        int runNumber)
    {
        filesystem::create_directories("../Dataset/Images");


        int width = params.latticeSize * SCALE;
        int latticeHeight = params.latticeSize * SCALE;
        int infoHeight = 140;

        Mat image(
            latticeHeight + infoHeight,
            width,
            CV_8UC3,
            Scalar(255,255,255)
        );

        drawLattice(
            image,
            lattice,
            params.latticeSize,
            SCALE
        );

        line(
            image,
            Point(0,latticeHeight),
            Point(width,latticeHeight),
            Scalar(150,150,150),
            2
        );

        int y = latticeHeight + 25;

        stringstream ss;
        ss << fixed << setprecision(2);

        ss.str("");
        ss.clear();
        ss << "Run : " << runNumber;

        putText(
            image,
            ss.str(),
            Point(10,y),
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
            Point(10,y),
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
            Point(10,y),
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
            Point(10,y),
            FONT_HERSHEY_SIMPLEX,
            0.5,
            Scalar(0,0,0),
            1
        );

        y += 25;

        ss.str("");
        ss.clear();
        ss << "Magnetic Field : " << params.magneticField;

        putText(
            image,
            ss.str(),
            Point(10,y),
            FONT_HERSHEY_SIMPLEX,
            0.5,
            Scalar(0,0,0),
            1
        );

        string filename =
            "../Dataset/Images/run_" +
            to_string(runNumber) +
            ".png";

        imwrite(filename,image);
    }

    static void saveFrame(
        const vector<vector<Particle>>& lattice,
        const SimulationParameters& params,
        int runNumber,
        int step)
    {
        filesystem::create_directories(
            "../Dataset/Frames/Run_" + to_string(runNumber)
        );



        int width = params.latticeSize * SCALE;
        int latticeHeight = params.latticeSize * SCALE;
        int infoHeight = 40;

        Mat image(
            latticeHeight + infoHeight,
            width,
            CV_8UC3,
            Scalar(255,255,255)
        );

        drawLattice(
            image,
            lattice,
            params.latticeSize,
            SCALE
        );

        line(
            image,
            Point(0,latticeHeight),
            Point(width,latticeHeight),
            Scalar(150,150,150),
            2
        );

        putText(
            image,
            "Monte Carlo Step : " + to_string(step),
            Point(10,latticeHeight + 25),
            FONT_HERSHEY_SIMPLEX,
            0.5,
            Scalar(0,0,0),
            1
        );

        stringstream name;

        name << "../Dataset/Frames/Run_"
             << runNumber
             << "/frame_"
             << setw(4)
             << setfill('0')
             << step
             << ".png";

        imwrite(name.str(), image);

    }
};