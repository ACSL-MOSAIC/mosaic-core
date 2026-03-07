//
// Created by yhkim on 2026. 2. 14.
//

#include "mosaic/auto_configurer/impl/opencv_v4l2_camera_mt_handler.hpp"

#include <chrono>

using namespace mosaic::auto_configurer;

void impl::OpenCV_V4L2_CameraMediaTrackConfigurer::Configure() {
    const auto frame_rate =
        !connector_config_->params["fps"].empty() ? std::stof(connector_config_->params["fps"]) : 30.0f;
    const auto camera_id =
        !connector_config_->params["camera_id"].empty() ? std::stoi(connector_config_->params["camera_id"]) : 0;
    const auto width =
        !connector_config_->params["width"].empty() ? std::stoi(connector_config_->params["width"]) : 640;
    const auto height =
        !connector_config_->params["height"].empty() ? std::stoi(connector_config_->params["height"]) : 480;
    handler_ =
        std::make_shared<OpenCV_V4L2_CameraMediaTrack>(connector_config_->label, frame_rate, camera_id, width, height);
}

void impl::OpenCV_V4L2_CameraMediaTrack::Start() {
    if (IsRunning()) {
        return;
    }
    if (!capture_.open(camera_id_, cv::CAP_V4L2)) {
        return;
    }
    capture_.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

    capture_.set(cv::CAP_PROP_FRAME_WIDTH, width_);
    capture_.set(cv::CAP_PROP_FRAME_HEIGHT, height_);
    capture_.set(cv::CAP_PROP_FPS, frame_rate_);

    SetRunning(true);
    SetStopFlag(false);
    start_time_ = std::chrono::steady_clock::now();
    frame_loop_thread_ = std::make_shared<std::thread>(std::thread([this]() {
        while (!GetStopFlag()) {
            try {
                this->FrameLoop();
            } catch (const cv::Exception& e) {
                MOSAIC_LOG_ERROR("OpenCV error: {}", e.what());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0f / this->frame_rate_)));
        }
    }));
}

void impl::OpenCV_V4L2_CameraMediaTrack::Stop() {
    if (!IsRunning()) {
        return;
    }

    SetStopFlag(true);
    if (frame_loop_thread_ != nullptr && frame_loop_thread_->joinable()) {
        frame_loop_thread_->join();
    }
    frame_loop_thread_.reset();

    SetRunning(false);
}

void impl::OpenCV_V4L2_CameraMediaTrack::FrameLoop() {
    cv::Mat frame;
    if (!capture_.read(frame)) {
        return;
    }
    SendFrame(frame, start_time_);
}