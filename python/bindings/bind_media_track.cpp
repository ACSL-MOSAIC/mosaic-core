//
// Created by yhkim on 2026. 2. 4.
//

#include "bind_media_track.hpp"

#include <chrono>
#include <memory>

#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <opencv2/core.hpp>

#include "mosaic/handlers/media_track/media_track_handler.hpp"
#include "mosaic/handlers/media_track/recordable.hpp"

namespace {

using namespace mosaic::handlers;

// numpy array (HxW, HxWx3, HxWx4) → cv::Mat conversion
// py::array::c_style | forcecast ensures contiguous memory and uint8 type
cv::Mat numpy_to_mat(const py::array_t<uint8_t, py::array::c_style | py::array::forcecast>& arr) {
    if (arr.ndim() > 3 || arr.ndim() < 1) {
        throw std::runtime_error("NumPy array dimensions must be 1, 2, or 3.");
    }

    // Request buffer info
    py::buffer_info info = arr.request();

    // Determine the OpenCV data type and number of channels
    int cv_type;
    if (info.format == py::format_descriptor<unsigned char>::format()) {
        cv_type = (arr.ndim() == 3) ? CV_8UC(info.shape[2]) : CV_8UC1;
    } else if (info.format == py::format_descriptor<float>::format()) {
        cv_type = (arr.ndim() == 3) ? CV_32FC(info.shape[2]) : CV_32FC1;
    } else if (info.format == py::format_descriptor<double>::format()) {
        cv_type = (arr.ndim() == 3) ? CV_64FC(info.shape[2]) : CV_64FC1;
    } else {
        throw std::runtime_error("Unsupported NumPy data type");
    }

    // Get shape (rows, cols) and strides
    std::vector<int> shape;
    if (arr.ndim() == 3) {
        shape.push_back(info.shape[0]);  // rows
        shape.push_back(info.shape[1]);  // cols
    } else if (arr.ndim() == 2) {
        shape.push_back(info.shape[0]);  // rows
        shape.push_back(info.shape[1]);  // cols
    } else {                             // 1D array treated as single row
        shape.push_back(1);              // rows
        shape.push_back(info.shape[0]);  // cols
    }

    // Create cv::Mat with shared memory
    // The cv::Mat constructor takes a void* pointer to the data and shape information
    cv::Mat mat(shape[0], shape[1], cv_type, info.ptr);
    return mat;
}

// Trampoline class for AMediaTrackHandler
// Allows Python to implement Start() and Stop()
class PyAMediaTrackHandler : public AMediaTrackHandler {
  public:
    using AMediaTrackHandler::AMediaTrackHandler;

    void Start() override {
        PYBIND11_OVERRIDE_PURE_NAME(void,                // Return type
                                    AMediaTrackHandler,  // Parent class
                                    "start",             // Python method name
                                    Start                // C++ function name
        );
    }

    void Stop() override {
        PYBIND11_OVERRIDE_PURE_NAME(void,                // Return type
                                    AMediaTrackHandler,  // Parent class
                                    "stop",              // Python method name
                                    Stop                 // C++ function name
        );
    }

    // Helpers for accessing protected methods
    void SetRunningPublic(bool running) {
        SetRunning(running);
    }

    bool GetStopFlagPublic() const {
        return GetStopFlag();
    }

    void SetStopFlagPublic(bool stop_flag) const {
        SetStopFlag(stop_flag);
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
    // shared_ptr holder: maintains holder consistency as multiple inheritance base class of AMediaTrackHandler
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

    // IMediaTrackHandler (abstract - no constructor)
    // CreateVideoTrackSource is excluded as it uses WebRTC types
    // shared_ptr holder: passed as shared_ptr<IMediaTrackHandler> in MosaicConnector::AddMediaTrackHandler
    py::class_<IMediaTrackHandler, std::shared_ptr<IMediaTrackHandler>>(m, "IMediaTrackHandler")
        .def("get_track_name", &IMediaTrackHandler::GetTrackName)
        .def("close", &IMediaTrackHandler::Close);

    // AMediaTrackHandler (abstract, multiple inheritance: IMediaTrackHandler + Recordable)
    // Python can implement Start() and Stop() via trampoline class
    // SendFrame(VideoFrame), CreateVideoTrackSource are excluded as they use WebRTC types
    py::class_<AMediaTrackHandler,
               std::shared_ptr<AMediaTrackHandler>,
               IMediaTrackHandler,
               Recordable,
               PyAMediaTrackHandler>(m, "AMediaTrackHandler")
        .def(py::init<const std::string&, bool>(), py::arg("track_name"), py::arg("recordable"))
        .def("start", &AMediaTrackHandler::Start)
        .def("stop", &AMediaTrackHandler::Stop)
        .def("is_running", &AMediaTrackHandler::IsRunning)
        .def(
            "send_frame",
            [](AMediaTrackHandler& self, const py::array_t<uint8_t, py::array::c_style | py::array::forcecast>& frame) {
                self.SendFrame(numpy_to_mat(frame), std::chrono::steady_clock::now());
            })
        .def("set_running",
             [](AMediaTrackHandler& self, bool running) {
                 dynamic_cast<PyAMediaTrackHandler&>(self).SetRunningPublic(running);
             })
        .def("get_stop_flag",
             [](AMediaTrackHandler& self) -> bool {
                 return dynamic_cast<PyAMediaTrackHandler&>(self).GetStopFlagPublic();
             })
        .def("set_stop_flag", [](AMediaTrackHandler& self, bool stop_flag) {
            dynamic_cast<PyAMediaTrackHandler&>(self).SetStopFlagPublic(stop_flag);
        });
}