//
// Created by yhkim on 7/29/25.
//

#ifndef RECORDABLE_H
#define RECORDABLE_H

#include <memory>
#include <string>

namespace cv {
  class Mat;
}

namespace rtc_sender {
  namespace handlers {
    enum RecordState { NotRecording, Recording, Paused };

    class Recordable {
    public:
      explicit Recordable(bool recordable);

      ~Recordable();

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
  } // namespace handlers
} // namespace rtc_sender

#endif  // RECORDABLE_H
