#include <opencv2/opencv.hpp>
#include "ewclib.h"     //include after including opencv
#include "PS4Camera.h"

PS4Camera::PS4Camera(){
}
PS4Camera::~PS4Camera(){
        EWC_Close(0);
}


int PS4Camera::init(int camID, int level, int fps){
        if (level == 1){
                m_CAPTURE_WIDTH = 3448;
                m_CAPTURE_HEIGHT = 808;
        }
        else if (level == 2){
                m_CAPTURE_WIDTH = 1748;
                m_CAPTURE_HEIGHT = 408;
        }
        else if (level == 3){
                m_CAPTURE_WIDTH = 898;
                m_CAPTURE_HEIGHT = 200;
        }
        m_CAPTURE_FPS = fps;
        m_IMAGE_WIDTH = 1280 / level;
        m_IMAGE_HEIGHT = 800 / level;
        m_MARGIN_LEFT = 48;
        m_MARGIN_BOTTOM = 8;
        m_MARGIN_PYRAMID2 = 320 * 3 / level;
        m_MARGIN_PYRAMID3 = 240 * 3 / level;
        m_MARGIN_PYRAMID4 = 140 * 3 / level;


        cameraID = EWC_Open(0, m_CAPTURE_WIDTH, m_CAPTURE_HEIGHT, m_CAPTURE_FPS, camID);    //??????
        EWC_SetValue(cameraID, EWC_FOCUS, 5.0);


        printf("cameraID;%d", cameraID);
        if (cameraID){
                printf("not found(%d)\n", cameraID);
                std::exit(1);
        }
        m_input4ch = cv::Mat_<cv::Vec4b>(m_CAPTURE_HEIGHT, m_CAPTURE_WIDTH);
        m_capImage = cv::Mat_<cv::Vec3b>(m_CAPTURE_HEIGHT, m_CAPTURE_WIDTH);
        m_pyramid.resize(2);
        m_pyramid[0].init();
        m_pyramid[1].init();

        m_rect.resize(8);
        m_rect[0] = cv::Rect(m_MARGIN_LEFT, 0, m_IMAGE_WIDTH, m_CAPTURE_HEIGHT - m_MARGIN_BOTTOM);
        m_rect[1] = cv::Rect(m_MARGIN_LEFT + m_IMAGE_WIDTH, 0,          m_IMAGE_WIDTH,           m_CAPTURE_HEIGHT - m_MARGIN_BOTTOM);
        m_rect[2] = cv::Rect(m_MARGIN_LEFT + m_IMAGE_WIDTH * 2, 0,      m_IMAGE_WIDTH * 0.25,    m_CAPTURE_HEIGHT);
        m_rect[3] = cv::Rect(m_MARGIN_LEFT + m_IMAGE_WIDTH*2.25, 0,     m_IMAGE_WIDTH * 0.25,    m_CAPTURE_HEIGHT);
        m_rect[4] = cv::Rect(m_MARGIN_LEFT + m_IMAGE_WIDTH*2.5, 0,      m_IMAGE_WIDTH * 0.0625,  m_CAPTURE_HEIGHT);
        m_rect[5] = cv::Rect(m_MARGIN_LEFT + m_IMAGE_WIDTH*2.5625, 0,   m_IMAGE_WIDTH * 0.0625,  m_CAPTURE_HEIGHT);
        m_rect[6] = cv::Rect(m_MARGIN_LEFT + m_IMAGE_WIDTH*2.625, 0,    m_IMAGE_WIDTH * 0.015625, m_CAPTURE_HEIGHT);
        m_rect[7] = cv::Rect(m_MARGIN_LEFT + m_IMAGE_WIDTH*2.640625, 0, m_IMAGE_WIDTH * 0.015625, m_CAPTURE_HEIGHT);

        EWC_SetBuffer(0, m_input4ch.data); 
        return 0;
}

int PS4Camera::Capture() {
        
        int count = 0;
        while (EWC_IsCaptured(0) != true){
                Sleep(1);
                count++;
                if (count > 1000){
                        return false;
                }
        }
        cv::cvtColor(m_input4ch, m_capImage, CV_RGBA2RGB);
        
        m_pyramid[0].setImageAt(cv::Mat(m_capImage, m_rect[0]), 0);
        m_pyramid[1].setImageAt(cv::Mat(m_capImage, m_rect[1]), 0);
        cv::Mat image2 = cv::Mat(m_capImage, m_rect[2]).clone();
        cv::Mat image3 = cv::Mat(m_capImage, m_rect[3]).clone();
        cv::Mat image4 = cv::Mat(m_capImage, m_rect[4]).clone();
        cv::Mat image5 = cv::Mat(m_capImage, m_rect[5]).clone();
        cv::Mat image6 = cv::Mat(m_capImage, m_rect[6]).clone();
        cv::Mat image7 = cv::Mat(m_capImage, m_rect[7]).clone();

        m_pyramid[0].setImageAt(cv::Mat(cv::Size(m_IMAGE_WIDTH / 2, m_IMAGE_HEIGHT / 2), CV_8UC3, image2.data + m_MARGIN_PYRAMID2), 1);
        m_pyramid[1].setImageAt(cv::Mat(cv::Size(m_IMAGE_WIDTH / 2, m_IMAGE_HEIGHT / 2), CV_8UC3, image3.data + m_MARGIN_PYRAMID2), 1);
        m_pyramid[0].setImageAt(cv::Mat(cv::Size(m_IMAGE_WIDTH / 4, m_IMAGE_HEIGHT / 4), CV_8UC3, image4.data + m_MARGIN_PYRAMID3), 2);
        m_pyramid[1].setImageAt(cv::Mat(cv::Size(m_IMAGE_WIDTH / 4, m_IMAGE_HEIGHT / 4), CV_8UC3, image5.data + m_MARGIN_PYRAMID3), 2);
        m_pyramid[0].setImageAt(cv::Mat(cv::Size(m_IMAGE_WIDTH / 8, m_IMAGE_HEIGHT / 8), CV_8UC3, image6.data + m_MARGIN_PYRAMID4), 3);
        m_pyramid[1].setImageAt(cv::Mat(cv::Size(m_IMAGE_WIDTH / 8, m_IMAGE_HEIGHT / 8), CV_8UC3, image7.data + m_MARGIN_PYRAMID4), 3);

        return true;
}