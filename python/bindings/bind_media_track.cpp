//
// Created by yhkim on 2026. 2. 4.
//

#include "bind_media_track.h"

#include <chrono>
#include <memory>

#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <opencv2/core.hpp>

#include "mosaic/handlers/media_track/a_media_track_handler.h"
#include "mosaic/handlers/media_track/recordable.h"

namespace {

using namespace mosaic::handlers;

// numpy array (HxW, HxWx3, HxWx4) → cv::Mat 변환
// py::array::c_style | forcecast로 연속 메모리와 uint8 타입 보장
cv::Mat numpy_to_mat(const py::array_t<uint8_t, py::array::c_style | py::array::forcecast>& array) {
    auto buf = array.request();
    int type;
    if (buf.ndim == 2) {
        type = CV_8UC1;
    } else if (buf.ndim == 3 && buf.shape[2] == 3) {
        type = CV_8UC3;
    } else if (buf.ndim == 3 && buf.shape[2] == 4) {
        type = CV_8UC4;
    } else {
        throw std::runtime_error("Unsupported array shape: expected HxW, HxWx3, or HxWx4");
    }
    return cv::Mat(buf.shape[0], buf.shape[1], type, buf.ptr);
}

// AMediaTrackHandler의 protected 메서드에 접근하기 위한 접근자 클래스
// pybind11에는 등록하지 않음 — static_cast 대상으로만 사용
class ProtectedAccessor : public AMediaTrackHandler {
  public:
    void Start() override {}
    void Stop() override {}

    static void CallSetRunning(AMediaTrackHandler& self, bool running) {
        dynamic_cast<ProtectedAccessor&>(self).SetRunning(running);
    }
    static bool CallGetStopFlag(AMediaTrackHandler& self) {
        return dynamic_cast<ProtectedAccessor&>(self).GetStopFlag();
    }
    static void CallSetStopFlag(AMediaTrackHandler& self, bool stop_flag) {
        dynamic_cast<ProtectedAccessor&>(self).SetStopFlag(stop_flag);
    }
};

}  // namespace

void bind_media_track(py::module_& m) {
    using namespace mosaic::handlers;

    // RecordState enum
    py::enum_<RecordState>(m, "RecordState")
        .value("NOT_RECORDING", NotRecording)
        .value("RECORDING", Recording)
        .value("PAUSED", Paused);

    // Recordable
    // shared_ptr holder: AMediaTrackHandler의 다중상속 기본 클래스로서 holder 일관성 유지
    py::class_<Recordable, std::shared_ptr<Recordable>>(m, "Recordable")
        .def(py::init<bool>(), py::arg("recordable"))
        .def("set_record_file_path", &Recordable::SetRecordFilePath)
        .def("set_video_parameters",
             &Recordable::SetVideoParameters,
             py::arg("fps"),
             py::arg("width"),
             py::arg("height"))
        .def("start_recording", &Recordable::StartRecording)
        .def("pause_recording", &Recordable::PauseRecording)
        .def("resume_recording", &Recordable::ResumeRecording)
        .def("stop_recording", &Recordable::StopRecording)
        .def("record_frame",
             [](Recordable& self, const py::array_t<uint8_t, py::array::c_style | py::array::forcecast>& frame) {
                 self.RecordFrame(numpy_to_mat(frame));
             });

    // IMediaTrackHandler (abstract - 생성자 없음)
    // CreateVideoTrackSource는 WebRTC 타입이므로 제외
    // shared_ptr holder: MosaicConnector::AddMediaTrackHandler에서 shared_ptr<IMediaTrackHandler>로 전달됨
    py::class_<IMediaTrackHandler, std::shared_ptr<IMediaTrackHandler>>(m, "IMediaTrackHandler")
        .def("get_track_name", &IMediaTrackHandler::GetTrackName)
        .def("close", &IMediaTrackHandler::Close);

    // AMediaTrackHandler (abstract, 다중상속 IMediaTrackHandler + Recordable - 생성자 없음)
    // SendFrame(VideoFrame), CreateVideoTrackSource는 WebRTC 타입이므로 제외
    py::class_<AMediaTrackHandler, std::shared_ptr<AMediaTrackHandler>, IMediaTrackHandler, Recordable>(m, "AMediaTrackHandler")
        .def("start", &AMediaTrackHandler::Start)
        .def("stop", &AMediaTrackHandler::Stop)
        .def("is_running", &AMediaTrackHandler::IsRunning)
        .def(
            "send_frame",
            [](AMediaTrackHandler& self, const py::array_t<uint8_t, py::array::c_style | py::array::forcecast>& frame) {
                self.SendFrame(numpy_to_mat(frame), std::chrono::steady_clock::now());
            })
        .def("set_running",
             [](AMediaTrackHandler& self, bool running) { ProtectedAccessor::CallSetRunning(self, running); })
        .def("get_stop_flag", [](AMediaTrackHandler& self) -> bool { return ProtectedAccessor::CallGetStopFlag(self); })
        .def("set_stop_flag",
             [](AMediaTrackHandler& self, bool stop_flag) { ProtectedAccessor::CallSetStopFlag(self, stop_flag); });
}