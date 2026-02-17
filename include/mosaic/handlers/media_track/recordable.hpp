//
// Created by yhkim on 7/29/25.
//

#ifndef MOSAIC_CORE_RECORDABLE_HPP
#define MOSAIC_CORE_RECORDABLE_HPP

#include <memory>
#include <string>

#include "opencv_forward_decl.hpp"

namespace mosaic::handlers {
enum RecordState { NotRecording, Recording, Paused };

class Recordable {
  public:
    explicit Recordable(bool recordable);

    virtual ~Recordable();

    void SetRecordFilePath(const std::string& record_file_path);

    void SetVideoParameters(double fps, int width, int height);

    void StartRecording();

    void PauseRecording();

    void ResumeRecording();

    void StopRecording();

    void RecordFrame(const cv::Mat& frame);

  private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
}  // namespace mosaic::handlers

#endif  // MOSAIC_CORE_RECORDABLE_HPP
