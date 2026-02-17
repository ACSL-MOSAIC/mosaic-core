//
// Created by yhkim on 7/17/25.
//

#ifndef MOSAIC_CORE_A_MEDIA_TRACK_HANDLER_HPP
#define MOSAIC_CORE_A_MEDIA_TRACK_HANDLER_HPP

#include <chrono>
#include <memory>
#include <string>

#include <opencv2/opencv.hpp>

#include "../../rtc/webrtc_forward_decl.hpp"
#include "recordable.hpp"

namespace mosaic::handlers {
class IMediaTrackHandler {
  public:
    explicit IMediaTrackHandler(std::string track_name) : track_name_(std::move(track_name)) {}

    virtual ~IMediaTrackHandler() = default;

    virtual webrtc::scoped_refptr<webrtc::VideoTrackSourceInterface> CreateVideoTrackSource() = 0;

    virtual void Close() = 0;

    std::string GetTrackName() const {
        return track_name_;
    }

  protected:
    std::string track_name_;
};

class AMediaTrackHandler : public IMediaTrackHandler, public Recordable {
  public:
    explicit AMediaTrackHandler(const std::string& track_name, bool recordable);

    ~AMediaTrackHandler() override;

    virtual void Start() = 0;

    virtual void Stop() = 0;

    void SendFrame(const webrtc::VideoFrame& frame);

    void SendFrame(const cv::Mat& frame, std::chrono::steady_clock::time_point start_time);

    [[nodiscard]] bool IsRunning() const;

    webrtc::scoped_refptr<webrtc::VideoTrackSourceInterface> CreateVideoTrackSource() override;

    void Close() override;

  protected:
    void SetRunning(bool running) const;

    [[nodiscard]] bool GetStopFlag() const;

    void SetStopFlag(bool stop_flag) const;

  private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
}  // namespace mosaic::handlers

#endif  // MOSAIC_CORE_A_MEDIA_TRACK_HANDLER_HPP
