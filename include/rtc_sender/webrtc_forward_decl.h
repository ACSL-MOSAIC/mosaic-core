//
// Created by yhkim on 25. 7. 7.
//

#ifndef WEBRTC_FORWARD_DECL_H
#define WEBRTC_FORWARD_DECL_H

// Forward declarations for PIMPL
namespace webrtc {
    class DataChannelInterface;
    class SessionDescriptionInterface;
    class IceCandidateInterface;
    class PeerConnectionInterface;
    class PeerConnectionFactoryInterface;
    class TaskQueueFactory;
    class PeerConnectionObserver;
    class DataChannelObserver;
    struct DataBuffer;
    class VideoFrame;
    class RtpSenderInterface;
    class MediaStreamTrackInterface;
    class VideoTrackSourceInterface;
    class VideoTrackInterface;
    template<class T>
    class scoped_refptr;
} // namespace webrtc

namespace rtc {
    class Thread;
    class AdaptedVideoTrackSource;
}

#endif  // WEBRTC_FORWARD_DECL_H
