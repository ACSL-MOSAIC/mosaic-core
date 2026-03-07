//
// Created by yhkim on 2026. 2. 14.
//

#ifndef MOSAIC_CORE_OPENCV_CAMERA_MT_HANDLER_HPP
#define MOSAIC_CORE_OPENCV_CAMERA_MT_HANDLER_HPP

#include <memory>
#include <thread>

#include <opencv2/opencv.hpp>

#include "../../handlers/media_track/media_track_handler.hpp"
#include "../connector/configurable_connectors.hpp"

namespace mosaic::auto_configurer::impl {
class OpenCV_V4L2_CameraMediaTrackConfigurer : public AMTHandlerConfigurer {
  public:
    OpenCV_V4L2_CameraMediaTrackConfigurer() = default;

    std::string GetConnectorType() const override {
        return "opencv-sender-v4l2-camera";
    }

    void Configure() override;
};

class OpenCV_V4L2_CameraMediaTrack : public handlers::AMediaTrackHandler {
  public:
    explicit OpenCV_V4L2_CameraMediaTrack(const std::string& track_name,
                                          const float frame_rate,
                                          const int camera_id,
                                          const int width,
                                          const int height)
        : AMediaTrackHandler(track_name, false),
          frame_rate_(frame_rate),
          camera_id_(camera_id),
          width_(width),
          height_(height) {}

    ~OpenCV_V4L2_CameraMediaTrack() override {
        OpenCV_V4L2_CameraMediaTrack::Stop();
    }

    void Start() override;

    void Stop() override;

  private:
    void FrameLoop();

    float frame_rate_;
    int camera_id_;
    int width_;
    int height_;
    cv::VideoCapture capture_;
    std::chrono::steady_clock::time_point start_time_;
    std::shared_ptr<std::thread> frame_loop_thread_;
};
}  // namespace mosaic::auto_configurer::impl

#endif  // MOSAIC_CORE_OPENCV_CAMERA_MT_HANDLER_HPP
