//
// Created by yhkim on 7/29/25.
//

#ifndef BA_GCS_RTC_SENDER_VIDEO_RECORDER_H
#define BA_GCS_RTC_SENDER_VIDEO_RECORDER_H

#include <memory>
#include <string>
#include "opencv_forward_decl.h"

namespace rtc_sender {
  namespace handlers {
    class VideoRecorder {
    public:
      explicit VideoRecorder(const std::string &record_file_path);

      ~VideoRecorder();

      void SetRecordFilePath(const std::string &record_file_path);

      void SetVideoParameters(double fps, int width, int height);

      void Start();

      void Stop();

      void SaveFrame(const cv::Mat &frame);

    private:
      class Impl;
      std::unique_ptr<Impl> pImpl;
    };
  } // namespace handlers
} // namespace rtc_sender

#endif  // BA_GCS_RTC_SENDER_VIDEO_RECORDER_H
