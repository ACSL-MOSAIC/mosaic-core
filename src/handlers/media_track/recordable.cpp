//
// Created by yhkim on 7/29/25.
//

#include "mosaic_rtc_core/handlers/media_track/recordable.h"

#include <filesystem>

#include <opencv2/core/mat.hpp>

#include "mosaic_rtc_core/handlers/media_track/video_recorder.h"
#include "mosaic_rtc_core/logger/log.h"

using namespace rtc_sender::handlers;

class Recordable::Impl {
public:
    explicit Impl(const bool recordable) : recordable_(recordable) {
        if (recordable_) {
            const auto default_record_file_path = "./recordings";
            if (IsValidRecordFilePath(default_record_file_path)) {
                video_recorder_ = std::make_shared<VideoRecorder>(default_record_file_path);
            } else {
                RTC_SENDER_LOG_ERROR("Recordable becomes false due to invalid record file path");
                recordable_ = false;
            }
        }
    }

    void SetRecordFilePath(const std::string &record_file_path) {
        if (!recordable_ || record_state_ != NotRecording) {
            RTC_SENDER_LOG_ERROR("Cannot change record file path while recording or paused");
        }
        if (IsValidRecordFilePath(record_file_path)) {
            video_recorder_->SetRecordFilePath(record_file_path);
        }
    }

    void SetVideoParameters(const double fps, const int width, const int height) {
        if (!recordable_ || record_state_ != NotRecording) {
            RTC_SENDER_LOG_ERROR("Cannot change video parameters while recording or paused");
        }
        video_recorder_->SetVideoParameters(fps, width, height);
    }

    void StartRecording() {
        if (recordable_ && video_recorder_) {
            video_recorder_->Start();
            record_state_ = Recording;
        }
    }

    void PauseRecording() {
        if (recordable_ && record_state_ == Recording) {
            record_state_ = Paused;
        }
    }

    void ResumeRecording() {
        if (recordable_ && record_state_ == Paused) {
            record_state_ = Recording;
        }
    }

    void StopRecording() {
        if (recordable_ && video_recorder_) {
            video_recorder_->Stop();
            record_state_ = NotRecording;
        }
    }

    void RecordFrame(const cv::Mat &frame) {
        if (recordable_ && record_state_ == Recording) {
            video_recorder_->SaveFrame(frame);
        }
    }

private:
    bool recordable_ = false;
    std::shared_ptr<VideoRecorder> video_recorder_ = nullptr;
    RecordState record_state_ = NotRecording;

    static bool IsValidRecordFilePath(const std::string &record_file_path) {
        // 빈 문자열이면 에러
        if (record_file_path.empty()) {
            RTC_SENDER_LOG_ERROR("Record file path is empty");
            return false;
        }

        // 디렉토리가 존재하지 않으면 생성 시도
        if (!std::filesystem::exists(record_file_path)) {
            try {
                std::filesystem::create_directories(record_file_path);
                RTC_SENDER_LOG_INFO("Created directory: " + record_file_path);
            } catch (const std::filesystem::filesystem_error &ex) {
                RTC_SENDER_LOG_ERROR("Failed to create directory: " + record_file_path + ", Error: " + ex.what());
                return false;
            }
        }

        if (!std::filesystem::is_directory(record_file_path)) {
            RTC_SENDER_LOG_ERROR("Path is not a directory: " + record_file_path);
            return false;
        }

        auto perms = std::filesystem::status(record_file_path).permissions();
        if ((perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none &&
            (perms & std::filesystem::perms::group_write) == std::filesystem::perms::none &&
            (perms & std::filesystem::perms::others_write) == std::filesystem::perms::none) {
            RTC_SENDER_LOG_ERROR("No write permission for directory: " + record_file_path);
            return false;
        }
        return true;
    }
};

Recordable::Recordable(const bool recordable) {
    pImpl = std::make_unique<Impl>(recordable);
}

Recordable::~Recordable() = default;

void Recordable::SetRecordFilePath(const std::string &record_file_path) {
    pImpl->SetRecordFilePath(record_file_path);
}

void Recordable::SetVideoParameters(const double fps, const int width, const int height) {
    pImpl->SetVideoParameters(fps, width, height);
}

void Recordable::StartRecording() {
    pImpl->StartRecording();
}

void Recordable::PauseRecording() {
    pImpl->PauseRecording();
}

void Recordable::ResumeRecording() {
    pImpl->ResumeRecording();
}

void Recordable::StopRecording() {
    pImpl->StopRecording();
}

void Recordable::RecordFrame(const cv::Mat &frame) {
    pImpl->RecordFrame(frame);
}
