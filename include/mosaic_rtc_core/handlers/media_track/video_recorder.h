//
// Created by yhkim on 7/29/25.
//

#ifndef MOSAIC_RTC_CORE_VIDEO_RECORDER_H
#define MOSAIC_RTC_CORE_VIDEO_RECORDER_H

#include <memory>
#include <string>

#include "opencv_forward_decl.h"

namespace mosaic::handlers {
class VideoRecorder {
  public:
    explicit VideoRecorder(const std::string& record_file_path);

    ~VideoRecorder();

    void SetRecordFilePath(const std::string& record_file_path) const;

    void SetVideoParameters(double fps, int width, int height) const;

    void Start() const;

    void Stop() const;

    void SaveFrame(const cv::Mat& frame) const;

  private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
}  // namespace mosaic::handlers

#endif  // MOSAIC_RTC_CORE_VIDEO_RECORDER_H
