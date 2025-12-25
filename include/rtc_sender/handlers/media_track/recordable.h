//
// Created by yhkim on 7/29/25.
//

#ifndef BA_GCS_RTC_SENDER_RECORDABLE_H
#define BA_GCS_RTC_SENDER_RECORDABLE_H

#include <memory>
#include <string>

#include "opencv_forward_decl.h"

namespace rtc_sender::handlers {
  enum RecordState { NotRecording, Recording, Paused };

  class Recordable {
  public:
    explicit Recordable(bool recordable);

    virtual ~Recordable();

    void SetRecordFilePath(const std::string &record_file_path);

    void SetVideoParameters(double fps, int width, int height);

    void StartRecording();

    void PauseRecording();

    void ResumeRecording();

    void StopRecording();

    void RecordFrame(const cv::Mat &frame);

  private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
  };
} // namespace rtc_sender::handlers

#endif  // BA_GCS_RTC_SENDER_RECORDABLE_H
