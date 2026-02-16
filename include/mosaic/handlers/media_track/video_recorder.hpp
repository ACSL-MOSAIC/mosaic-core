//
// Created by yhkim on 7/29/25.
//

#ifndef MOSAIC_CORE_VIDEO_RECORDER_HPP
#define MOSAIC_CORE_VIDEO_RECORDER_HPP

#include <memory>
#include <string>

#include <opencv2/opencv.hpp>

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

#endif  // MOSAIC_CORE_VIDEO_RECORDER_HPP
