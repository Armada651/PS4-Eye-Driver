#include "PS4Camera.h"

int main()
{
        PS4Camera cam;
        cam.init(1);
        while (true){
                if (cam.Capture() == false){
                        std::cerr << "cap_error" << std::endl;
                }
                cv::Mat raw = cam.getRawImage();
                cv::Mat left = cam.getImage(0);
                cv::Mat right = cam.getImage(1);
                cv::Mat left2 = cam.getImage(0,1);
                cv::Mat right2 = cam.getImage(1,1);
                cv::Mat left3 = cam.getImage(0, 2);
                cv::Mat right3 = cam.getImage(1, 2);
                cv::Mat left4 = cam.getImage(0, 3);
                cv::Mat right4 = cam.getImage(1, 3);
                //cv::imshow("raw", raw);
                //cv::imshow("Left", left);
                //cv::imshow("Right", right);
                cv::imshow("Left2", left2);
                cv::imshow("Right2", right2);
                cv::imshow("Left3", left3);
                cv::imshow("Right3", right3);
                cv::imshow("Left4", left4);
                cv::imshow("Right4", right4);
                
                
                char key = cv::waitKey(1);
                if (key == 'q'){
                        break;
                }
                else if (key == 's'){
                        cv::imwrite("raw.png", raw);
                        cv::imwrite("left.png", left);
                        cv::imwrite("right.png", right);
                        cv::imwrite("left2.png", left2);
                        cv::imwrite("right2.png", right2);
                        cv::imwrite("left3.png", left3);
                        cv::imwrite("right3.png", right3);
                        cv::imwrite("left4.png", left4);
                        cv::imwrite("right4.png", right4);
                        break;
                }

        }
        return 0;
}