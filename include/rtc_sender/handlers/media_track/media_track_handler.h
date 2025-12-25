//
// Created by yhkim on 7/17/25.
//

#ifndef BA_GCS_RTC_SENDER_MEDIA_TRACK_HANDLER_H
#define BA_GCS_RTC_SENDER_MEDIA_TRACK_HANDLER_H

#include <chrono>
#include <memory>
#include <string>

#include "i_media_track_handler.h"
#include "rtc_sender/webrtc_forward_decl.h"
#include "recordable.h"
#include "opencv_forward_decl.h"

namespace rtc_sender::handlers {
  class MediaTrackHandler : public IMediaTrackHandler, public Recordable {
  public:
    explicit MediaTrackHandler(const std::string &track_name, bool recordable);

    ~MediaTrackHandler() override;

    virtual void Start() = 0;

    virtual void Stop() = 0;

    void SendFrame(const webrtc::VideoFrame &frame);

    void SendFrame(const cv::Mat &frame, std::chrono::steady_clock::time_point start_time);

    bool IsRunning() const;

    webrtc::scoped_refptr<webrtc::VideoTrackSourceInterface> CreateVideoTrackSource() override;

    void Close() override;

  protected:
    void SetRunning(bool running);

    bool GetStopFlag() const;

    void SetStopFlag(bool stop_flag);

  private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
  };
} // namespace rtc_sender::handlers

#endif  // BA_GCS_RTC_SENDER_MEDIA_TRACK_HANDLER_H
