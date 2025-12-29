//
// Created by yhkim on 7/17/25.
//

#ifndef MOSAIC_RTC_CORE_A_MEDIA_TRACK_HANDLER_H
#define MOSAIC_RTC_CORE_A_MEDIA_TRACK_HANDLER_H

#include <chrono>
#include <memory>
#include <string>

#include <mosaic_rtc_core/handlers/media_track/i_media_track_handler.h>
#include <mosaic_rtc_core/handlers/media_track/recordable.h>
#include <mosaic_rtc_core/webrtc_forward_decl.h>

#include "opencv_forward_decl.h"

namespace mosaic::handlers {
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

#endif  // MOSAIC_RTC_CORE_A_MEDIA_TRACK_HANDLER_H
