//
// Created by yhkim on 7/17/25.
//
#include "rtc_sender/handlers/media_track/media_track_handler.h"

#include <api/video/i420_buffer.h>
#include <api/video/video_frame.h>
#include <libyuv/convert.h>
#include <media/base/adapted_video_track_source.h>
#include <rtc_sender/logger/log.h>

#include <opencv2/core/mat.hpp>

using namespace rtc_sender::handlers;

namespace rtc_sender {
    namespace handlers {
        class ActualVideoTrackSource : public rtc::AdaptedVideoTrackSource {
        public:
            ActualVideoTrackSource() {
            }

            // rtc::AdaptedVideoTrackSource 인터페이스 구현
            SourceState state() const override {
                return is_running_ ? kLive : kEnded;
            }

            bool remote() const override {
                return false;
            }

            bool is_screencast() const override {
                return false;
            }

            std::optional<bool> needs_denoising() const override {
                return false;
            }

            void SendFrame(const webrtc::VideoFrame &frame) {
                if (is_running_ && !stop_flag_) {
                    OnFrame(frame);
                } else {
                }
            }

            std::atomic<bool> is_running_ = false;
            std::atomic<bool> stop_flag_ = false;
        };
    } // namespace handlers
} // namespace rtc_sender

class MediaTrackHandler::Impl {
public:
    void SendFrame(const webrtc::VideoFrame &frame) {
        if (actual_video_track_source_) {
            actual_video_track_source_->SendFrame(frame);
        }
    }

    void SendFrame(const cv::Mat &frame, std::chrono::steady_clock::time_point start_time) {
        if (actual_video_track_source_) {
            actual_video_track_source_->SendFrame(CvMat2VideoFrame(frame, start_time));
        }
    }

    bool IsRunning() const {
        return actual_video_track_source_ && actual_video_track_source_->is_running_;
    }

    webrtc::scoped_refptr<webrtc::VideoTrackSourceInterface> CreateVideoTrackSource() {
        if (!actual_video_track_source_) {
            // Create a new ActualVideoTrackSource instance if it doesn't exist
            actual_video_track_source_ =
                    webrtc::scoped_refptr<ActualVideoTrackSource>(
                        new webrtc::RefCountedObject<ActualVideoTrackSource>());
        }
        return actual_video_track_source_;
    }

    void Close() {
        actual_video_track_source_ = nullptr;
    }

    void SetRunning(bool running) {
        if (actual_video_track_source_) {
            actual_video_track_source_->is_running_ = running;
        }
    }

    bool GetStopFlag() const {
        return actual_video_track_source_ && actual_video_track_source_->stop_flag_;
    }

    void SetStopFlag(bool stop_flag) {
        if (actual_video_track_source_) {
            actual_video_track_source_->stop_flag_ = stop_flag;
        }
    }

private:
    webrtc::scoped_refptr<ActualVideoTrackSource> actual_video_track_source_;

    static webrtc::VideoFrame CvMat2VideoFrame(const cv::Mat &frame, std::chrono::steady_clock::time_point start_time) {
        // Create I420 buffer
        webrtc::scoped_refptr<webrtc::I420Buffer> buffer = webrtc::I420Buffer::Create(frame.cols, frame.rows);

        // Convert RGB to I420
        libyuv::RGB24ToI420(frame.data,
                            frame.step[0],
                            buffer->MutableDataY(),
                            buffer->StrideY(),
                            buffer->MutableDataU(),
                            buffer->StrideU(),
                            buffer->MutableDataV(),
                            buffer->StrideV(),
                            frame.cols,
                            frame.rows);

        // Calculate timestamp
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time);
        int64_t timestamp_us = elapsed.count();

        return webrtc::VideoFrame::Builder().set_video_frame_buffer(buffer).set_timestamp_us(timestamp_us).build();
    }

    static cv::Mat VideoFrame2CvMat(const webrtc::VideoFrame &frame) {
        webrtc::scoped_refptr<webrtc::VideoFrameBuffer> buffer = frame.video_frame_buffer();
        webrtc::scoped_refptr<webrtc::I420BufferInterface> i420_buffer = buffer->ToI420();

        // Get frame dimensions
        int width = i420_buffer->width();
        int height = i420_buffer->height();

        // Create temporary RGB buffer
        std::unique_ptr<uint8_t[]> rgb_data(new uint8_t[width * height * 3]);

        // Convert I420 to RGB24
        libyuv::I420ToRGB24(i420_buffer->DataY(),
                            i420_buffer->StrideY(),
                            i420_buffer->DataU(),
                            i420_buffer->StrideU(),
                            i420_buffer->DataV(),
                            i420_buffer->StrideV(),
                            rgb_data.get(),
                            width * 3, // RGB stride
                            width,
                            height);

        // Create OpenCV Mat from RGB data
        cv::Mat rgb_mat(height, width, CV_8UC3, rgb_data.get());

        // Clone the data to ensure it persists after rgb_data goes out of scope
        cv::Mat result;
        rgb_mat.copyTo(result);

        return result;
    }

    friend class MediaTrackHandler;
};

MediaTrackHandler::MediaTrackHandler(const std::string &track_name, const bool recordable)
    : IMediaTrackHandler(track_name), Recordable(recordable) {
    pImpl = std::make_unique<Impl>();
}

MediaTrackHandler::~MediaTrackHandler() {
    MediaTrackHandler::Close();
}

void MediaTrackHandler::SendFrame(const webrtc::VideoFrame &frame) {
    pImpl->SendFrame(frame);
    RecordFrame(Impl::VideoFrame2CvMat(frame));
}

void MediaTrackHandler::SendFrame(const cv::Mat &frame, std::chrono::steady_clock::time_point start_time) {
    pImpl->SendFrame(frame, start_time);
    RecordFrame(frame);
}

bool MediaTrackHandler::IsRunning() const {
    return pImpl->IsRunning();
}

webrtc::scoped_refptr<webrtc::VideoTrackSourceInterface> MediaTrackHandler::CreateVideoTrackSource() {
    auto video_source = pImpl->CreateVideoTrackSource();
    Start();
    return video_source;
}

void MediaTrackHandler::Close() {
    Stop();
    StopRecording();
    pImpl->Close();
}

void MediaTrackHandler::SetRunning(bool running) {
    pImpl->SetRunning(running);
}

bool MediaTrackHandler::GetStopFlag() const {
    return pImpl->GetStopFlag();
}

void MediaTrackHandler::SetStopFlag(bool stop_flag) {
    pImpl->SetStopFlag(stop_flag);
}
