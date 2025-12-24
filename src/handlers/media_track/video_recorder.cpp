//
// Created by yhkim on 7/29/25.
//

#include "rtc_sender/handlers/media_track/video_recorder.h"

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <sstream>

#include <opencv2/videoio.hpp>

using namespace rtc_sender::handlers;

class VideoRecorder::Impl {
public:
    explicit Impl(const std::string &record_file_path) : record_file_path_(record_file_path) {
        video_writer_ = std::make_unique<cv::VideoWriter>();
    }

    void SetRecordFilePath(const std::string &record_file_path) {
        record_file_path_ = record_file_path;
    }

    void SetVideoParameters(const double fps, const int width, const int height) {
        fps_ = fps;
        width_ = width;
        height_ = height;
    }

    void Start() {
        if (!video_writer_->isOpened()) {
            const auto file_name = GetFilename();
            video_writer_->open(
                file_name, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps_, cv::Size(width_, height_), true);
            if (!video_writer_->isOpened()) {
                throw std::runtime_error("Failed to open video writer");
            }
        }
    }

    void Stop() {
        if (video_writer_->isOpened()) {
            video_writer_->release();
        }
    }

    void SaveFrame(const cv::Mat &frame) {
        if (video_writer_->isOpened()) {
            video_writer_->write(frame);
        } else {
            throw std::runtime_error("Video writer is not opened");
        }
    }

private:
    std::string record_file_path_;
    double fps_ = 30.0; // Default FPS
    int width_ = 640; // Default width
    int height_ = 480; // Default height
    std::unique_ptr<cv::VideoWriter> video_writer_;

    std::string GetFilename() {
        // record file name format: "recording_file_path/recording_YYYYDDMM_HHMMSS.avi"

        const auto now = std::chrono::system_clock::now();
        const auto time_t = std::chrono::system_clock::to_time_t(now);
        const auto tm = *std::localtime(&time_t);

        std::ostringstream filename_stream;
        filename_stream << record_file_path_ << "/recording_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".avi";

        return filename_stream.str();
    }
};

VideoRecorder::VideoRecorder(const std::string &record_file_path) {
    pImpl = std::make_unique<Impl>(record_file_path);
}

VideoRecorder::~VideoRecorder() {
    Stop();
}

void VideoRecorder::SetRecordFilePath(const std::string &record_file_path) {
    pImpl->SetRecordFilePath(record_file_path);
}

void VideoRecorder::SetVideoParameters(const double fps, const int width, const int height) {
    pImpl->SetVideoParameters(fps, width, height);
}

void VideoRecorder::Start() {
    pImpl->Start();
}

void VideoRecorder::Stop() {
    pImpl->Stop();
}

void VideoRecorder::SaveFrame(const cv::Mat &frame) {
    pImpl->SaveFrame(frame);
}
