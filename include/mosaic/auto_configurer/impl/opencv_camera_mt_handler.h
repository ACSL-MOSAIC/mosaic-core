//
// Created by yhkim on 2026. 2. 14.
//

#ifndef MOSAIC_CORE_OPENCV_CAMERA_MT_HANDLER_H
#define MOSAIC_CORE_OPENCV_CAMERA_MT_HANDLER_H

#include <memory>
#include <thread>

#include <opencv2/videoio.hpp>

#include "mosaic/auto_configurer/connector/a_mt_handler_configurer.h"
#include "mosaic/handlers/media_track/a_media_track_handler.h"

namespace mosaic::auto_configurer::impl {
class OpenCVCameraMediaTrackConfigurer : public AMTHandlerConfigurer {
  public:
    OpenCVCameraMediaTrackConfigurer() = default;

    std::string GetConnectorType() const override {
        return "opencv-sender-camera";
    }

    void Configure() override;
};

class OpenCVCameraMediaTrack : public handlers::AMediaTrackHandler {
  public:
    explicit OpenCVCameraMediaTrack(const std::string& track_name,
                                    const float frame_rate,
                                    const int camera_id,
                                    const int width,
                                    const int height)
        : AMediaTrackHandler(track_name, false),
          frame_rate_(frame_rate),
          camera_id_(camera_id),
          width_(width),
          height_(height) {}

    ~OpenCVCameraMediaTrack() override {
        OpenCVCameraMediaTrack::Stop();
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

#endif  // MOSAIC_CORE_OPENCV_CAMERA_MT_HANDLER_H
