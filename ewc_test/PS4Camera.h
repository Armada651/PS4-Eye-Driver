#pragma once

#include <opencv2/opencv.hpp>

class Pyramid{
public:
        Pyramid(){};
        ~Pyramid(){};

        void init(int numPyramid = 4){
                m_numPyramid = numPyramid;
                m_pyramid.resize(m_numPyramid);
        }

        cv::Mat &getImageAt(int level = 0){
                if (m_numPyramid <= level){
                        std::cerr << "level error" << std::endl;
                }
                return m_pyramid[level];
        }
        void setImageAt(cv::Mat image, int level = 0){
                if (m_numPyramid <= level){
                        std::cerr << "level error" << std::endl;
                }
                image.copyTo(m_pyramid[level]);
        }

private:
        int m_numPyramid;
        std::vector<cv::Mat> m_pyramid;
};

class PS4Camera
{
public:
        PS4Camera();
        ~PS4Camera();
        int init(int camID = 0, int level = 1, int fps = 30);
        int Capture();

        cv::Mat &getRawImage(){
                return m_capImage;
        }
        cv::Mat &getImage(int id=0, int level = 0){
                return m_pyramid[id].getImageAt(level);
        }

private:
        int cameraID;
        cv::Mat_<cv::Vec4b> m_input4ch;
        cv::Mat_<cv::Vec3b> m_capImage;
        std::vector<Pyramid> m_pyramid;
        std::vector<cv::Rect> m_rect;

        int m_CAPTURE_WIDTH;
        int m_CAPTURE_HEIGHT;
        int m_CAPTURE_FPS;
        int m_IMAGE_WIDTH;
        int m_IMAGE_HEIGHT;
        int m_MARGIN_LEFT;
        int m_MARGIN_BOTTOM;
        int m_MARGIN_PYRAMID2;
        int m_MARGIN_PYRAMID3;
        int m_MARGIN_PYRAMID4;
};