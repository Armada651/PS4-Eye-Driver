#include <iostream>
#include <string.h>

#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

#include "ps4eye.h"

using namespace ps4eye;

class PS4EyeFrame
{
public:
	PS4EyeFrame(cv::Mat raw)
	{
		assert(raw.channels() == 2);
		cv::cvtColor(raw, m_frame, cv::COLOR_YUV2BGR_YUY2);

		int height = m_frame.rows - MARGIN_BOTTOM;
		int width = (height * 16) / 10;
		assert(width == 1280 || width == 640 || width == 320);

		int x = MARGIN_LEFT;
		int y = 0;
		for (int i = 0; i < LEVELS; i++)
		{
			for (int j = 0; j < EYES; j++)
			{
				// Calculate the region of this level in the frame.
				cv::Rect level(x, y, width >> i, height << i);

				if (i > 0)
				{
					// Clone the part of the frame that contains the pixels for this level.
					// This is necessary because the rows are cut to pieces and interleaved,
					// so we can't simply use differing strides for each level.
					m_levels[i][j] = m_frame(level).clone();
				}
				else
				{
					// The first level doesn't need to be cloned, since the rows aren't interleaved.
					m_levels[i][j] = m_frame(level);
				}

				// The copy has merged the rows in memory again, so now we can simply
				// manipulate the stride and set the actual size.
				m_levels[i][j].step[0] <<= i;
				m_levels[i][j].cols = width;
				m_levels[i][j].rows = height;

				// Move to the next image
				x += level.width;
			}

			// Calculate the header margin
			y |= 1 << i;

			// Just like mip-maps every level is half the size of the previous level.
			width >>= 1;
			height >>= 1;
		}
	}

	~PS4EyeFrame() { }

	cv::Mat GetImage(bool right_eye = false, int level = 0)
	{
		if (level >= LEVELS)
			return cv::Mat();

		// Don't worry, this is not a deep-copy
		return m_levels[level][right_eye];
	}

private:
	static const int MARGIN_LEFT = 48;
	static const int MARGIN_BOTTOM = 8;
	static const int LEVELS = 4;
	static const int EYES = 2;

	cv::Mat m_frame;
	cv::Mat m_levels[LEVELS][EYES];
};

int main() {
	std::vector<PS4EYECam::PS4EYERef> devices(PS4EYECam::getDevices());

	if (devices.size() == 0)
		return -1;

	ps4eye::PS4EYECam::PS4EYERef eye = devices.at(0);
	eye->firmware_path = "resources/firmware.bin";
	eye->firmware_upload();

	// TODO: Only select the PS4 camera, preferably the one we just uploaded firmware to
	cv::VideoCapture capture(0 + cv::CAP_MSMF);

	cv::namedWindow("left");
	cv::namedWindow("right");

	cv::Mat raw;
	char keyCode = 0;
	while (keyCode != 27 /* ESC */) {
		capture >> raw;
		if (raw.empty())
			break;

		PS4EyeFrame frame(raw);
		cv::imshow("left", frame.GetImage(false));
		cv::imshow("right", frame.GetImage(true));

		keyCode = (char)cv::waitKey(1);
	}

	return 0;
}
