//
// Created by yhkim on 25. 7. 2.
//

#include <api/audio_codecs/builtin_audio_decoder_factory.h>
#include <api/audio_codecs/builtin_audio_encoder_factory.h>
#include <api/create_peerconnection_factory.h>
#include <api/task_queue/default_task_queue_factory.h>
#include <api/video_codecs/video_decoder_factory_template.h>
#include <api/video_codecs/video_decoder_factory_template_dav1d_adapter.h>
#include <api/video_codecs/video_decoder_factory_template_libvpx_vp8_adapter.h>
#include <api/video_codecs/video_decoder_factory_template_libvpx_vp9_adapter.h>
#include <api/video_codecs/video_decoder_factory_template_open_h264_adapter.h>
#include <api/video_codecs/video_encoder_factory_template.h>
#include <api/video_codecs/video_encoder_factory_template_libaom_av1_adapter.h>
#include <api/video_codecs/video_encoder_factory_template_libvpx_vp8_adapter.h>
#include <api/video_codecs/video_encoder_factory_template_libvpx_vp9_adapter.h>
#include <api/video_codecs/video_encoder_factory_template_open_h264_adapter.h>
#include <mosaic/configs_decl.h>
#include <mosaic/core/connector_state_manager.h>
#include <mosaic/core/peer_connection_manager.h>
#include <mosaic/core/peer_connection_observer.h>
#include <mosaic/logger/log.h>
#include <mosaic/signaling/i_signaling_client.h>

namespace mosaic::core {

webrtc::PeerConnectionInterface::IceServer GetIceServer(const IceServerConfig& ice_config);

class CreateSdpAnswerObserver : public webrtc::CreateSessionDescriptionObserver {
  public:
    explicit CreateSdpAnswerObserver(const std::shared_ptr<PeerConnectionManager>& pc_manager)
        : pc_manager_(pc_manager) {}

    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override {
        pc_manager_->AfterCreateSdpAnswer(desc);
    }

    void OnFailure(webrtc::RTCError error) override {
        MOSAIC_LOG_ERROR("Failed to create SDP answer: {}", error.message());
    }

  private:
    std::shared_ptr<PeerConnectionManager> pc_manager_;
};

class SimpleSetLocalDescriptionObserver : public webrtc::SetSessionDescriptionObserver {
  public:
    explicit SimpleSetLocalDescriptionObserver(const std::shared_ptr<PeerConnectionManager>& pc_manager,
                                               webrtc::SessionDescriptionInterface* desc)
        : pc_manager_(pc_manager), desc_(desc) {}

    void OnSuccess() override {
        if (!desc_) {
            MOSAIC_LOG_ERROR("No session description set for local description observer.");
            return;
        }
        pc_manager_->AfterSetLocalDescription(desc_);
    }

    void OnFailure(webrtc::RTCError error) override {
        MOSAIC_LOG_ERROR("Failed to set local description: {}", error.message());
    }

  private:
    std::shared_ptr<PeerConnectionManager> pc_manager_;
    webrtc::SessionDescriptionInterface* desc_;
};

class SimpleSetRemoteDescriptionObserver : public webrtc::SetSessionDescriptionObserver {
  public:
    explicit SimpleSetRemoteDescriptionObserver(const std::shared_ptr<PeerConnectionManager>& pc_manager)
        : pc_manager_(pc_manager) {}

    void OnSuccess() override {
        MOSAIC_LOG_INFO("Set remote description with SDP offer Success");
        pc_manager_->AfterSetSessionDescription();
    }

    void OnFailure(const webrtc::RTCError error) override {
        MOSAIC_LOG_ERROR("Failed to set session description: {}", error.message());
    }

  private:
    std::shared_ptr<PeerConnectionManager> pc_manager_;
};

class PeerConnectionManager::Impl {
  public:
    Impl(const std::shared_ptr<MosaicConnector>& client,
         const std::shared_ptr<ConnectorStateManager>& state_manager,
         const std::shared_ptr<ISignalingClient>& signaling_client,
         const std::shared_ptr<WebRtcConfig>& web_rtc_config)
        : client_(client),
          state_manager_(state_manager),
          signaling_client_(signaling_client),
          web_rtc_config_(web_rtc_config) {}

    void InitializeWebRTC() {
        // Initialize WebRTC components, such as threads and factory.
        network_thread_ = rtc::Thread::CreateWithSocketServer();
        worker_thread_ = rtc::Thread::Create();
        signaling_thread_ = rtc::Thread::Create();

        network_thread_->Start();
        worker_thread_->Start();
        signaling_thread_->Start();

        task_queue_factory_ = webrtc::CreateDefaultTaskQueueFactory();
        const auto dummy_adm =
            webrtc::AudioDeviceModule::Create(webrtc::AudioDeviceModule::kDummyAudio, task_queue_factory_.get());

        // Set the threads for the PeerConnectionFactory.
        peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(
            network_thread_.get(),
            worker_thread_.get(),
            signaling_thread_.get(),
            dummy_adm,
            webrtc::CreateBuiltinAudioEncoderFactory(),
            webrtc::CreateBuiltinAudioDecoderFactory(),
            std::make_unique<webrtc::VideoEncoderFactoryTemplate<webrtc::LibvpxVp8EncoderTemplateAdapter,
                                                                 webrtc::LibvpxVp9EncoderTemplateAdapter,
                                                                 webrtc::OpenH264EncoderTemplateAdapter,
                                                                 webrtc::LibaomAv1EncoderTemplateAdapter>>(),
            std::make_unique<webrtc::VideoDecoderFactoryTemplate<webrtc::LibvpxVp8DecoderTemplateAdapter,
                                                                 webrtc::LibvpxVp9DecoderTemplateAdapter,
                                                                 webrtc::OpenH264DecoderTemplateAdapter,
                                                                 webrtc::Dav1dDecoderTemplateAdapter>>(),
            nullptr,  // audio_mixer
            nullptr   // audio_processing
        );

        if (!peer_connection_factory_) {
            MOSAIC_LOG_ERROR("Failed to create PeerConnectionFactory");
        }

        state_manager_->SetState(ConnectorStateManager::READY_TO_CONNECT);
        MOSAIC_LOG_DEBUG("WebRTC Client initialized successfully");
    }

    void CreatePeerConnection(const std::shared_ptr<PeerConnectionManager>& outer_this_) {
        MOSAIC_LOG_DEBUG("RobotWebRTCClient::CreatePeerConnection()");
        webrtc::PeerConnectionInterface::RTCConfiguration configuration;
        configuration.ice_connection_receiving_timeout = 30000;
        for (const auto& ice_server : web_rtc_config_->ice_servers) {
            configuration.servers.push_back(GetIceServer(ice_server));
        }

        // Observer 생성
        peer_connection_observer_ = std::make_shared<PeerConnectionObserver>(client_, outer_this_);

        // PeerConnection 생성
        auto c = peer_connection_factory_->CreatePeerConnectionOrError(
            configuration, webrtc::PeerConnectionDependencies(peer_connection_observer_.get()));
        if (c.ok()) {
            peer_connection_ = c.value();
        } else {
            // Handle error
            MOSAIC_LOG_ERROR("Failed to create PeerConnection: {}", c.error().message());
            // TODO
            return;
        }
    }

    void HandleSdpOffer(webrtc::SessionDescriptionInterface* sdp_offer,
                        const std::shared_ptr<PeerConnectionManager>& outer_this_) {
        MOSAIC_LOG_DEBUG("Handle SDP offer");
        // Close existing connection if exists
        if (peer_connection_) {
            ClosePeerConnection();
        }

        if (!state_manager_->IsState(ConnectorStateManager::READY_TO_CONNECT)) {
            MOSAIC_LOG_ERROR("Cannot handle SDP offer in current state: {}", state_manager_->GetStateString());
            return;
        }

        state_manager_->SetState(ConnectorStateManager::RTC_CONNECTING);
        // Create a new PeerConnection
        CreatePeerConnection(outer_this_);
        MOSAIC_LOG_DEBUG("Peer Connection created successfully");

        if (!peer_connection_) {
            MOSAIC_LOG_ERROR("Peer connection is null, cannot handle SDP offer");
            state_manager_->SetState(ConnectorStateManager::RTC_FAILED);
            return;
        }

        // Set remote description (SDP offer)
        const auto set_remote_observer = webrtc::scoped_refptr<SimpleSetRemoteDescriptionObserver>(
            new webrtc::RefCountedObject<SimpleSetRemoteDescriptionObserver>(outer_this_));

        // Go to AfterSetSessionDescription after setting remote description
        peer_connection_->SetRemoteDescription(set_remote_observer.get(), sdp_offer);
        MOSAIC_LOG_DEBUG("Set remote description with SDP offer");
    }

    void AfterSetSessionDescription(const std::shared_ptr<PeerConnectionManager>& outer_this_) const {
        // TODO: Need to delete
        client_->CreateAllMediaTracks();

        // Create and set answer
        const auto create_answer_observer = webrtc::scoped_refptr<CreateSdpAnswerObserver>(
            new webrtc::RefCountedObject<CreateSdpAnswerObserver>(outer_this_));

        // 생성된 응답을 시그널링 서버로 전송하는 부분은 create_answer_observer 에서 진행
        peer_connection_->CreateAnswer(create_answer_observer.get(),
                                       webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
        MOSAIC_LOG_DEBUG("Created SDP answer observer and called CreateAnswer");
    }

    void AfterCreateSdpAnswer(webrtc::SessionDescriptionInterface* desc,
                              const std::shared_ptr<PeerConnectionManager>& outer_this_) const {
        MOSAIC_LOG_DEBUG("Sdp Answer Created");
        // Create observer for SetLocalDescription
        const auto set_local_observer = webrtc::scoped_refptr<SimpleSetLocalDescriptionObserver>(
            new webrtc::RefCountedObject<SimpleSetLocalDescriptionObserver>(outer_this_, desc));

        // Set local description
        peer_connection_->SetLocalDescription(set_local_observer.get(), desc);
    }

    void AfterSetLocalDescription(const webrtc::SessionDescriptionInterface* desc) const {
        MOSAIC_LOG_DEBUG("Set local description with SDP answer");
        signaling_client_->SendSdpAnswer(desc);
    }

    void HandleIceCandidateFromSignaling(const webrtc::IceCandidateInterface* candidate) const {
        if (peer_connection_) {
            // Convert raw pointer to unique_ptr
            std::unique_ptr<webrtc::IceCandidateInterface> candidate_unique_ptr(
                const_cast<webrtc::IceCandidateInterface*>(candidate));

            // Add ice candidate with callback for error handling
            peer_connection_->AddIceCandidate(std::move(candidate_unique_ptr), [](const webrtc::RTCError& error) {
                if (!error.ok()) {
                    MOSAIC_LOG_ERROR("Failed to add ICE candidate: ", error.message());
                } else {
                    MOSAIC_LOG_DEBUG("Successfully added ICE candidate");
                }
            });
        }
    }

    void OnConnectionConnected() const {
        MOSAIC_LOG_DEBUG("PeerConnectionManager::OnConnectionConnected()");
        // state manager will automatically notify signaling server about connected state
        state_manager_->SetState(ConnectorStateManager::RTC_CONNECTED);
    }

    void OnConnectionDisconnected() const {
        MOSAIC_LOG_DEBUG("PeerConnectionManager::OnConnectionDisconnected()");
        // state manager will automatically notify signaling server about disconnected state
        state_manager_->SetState(ConnectorStateManager::READY_TO_CONNECT);
    }

    void SendIceCandidate(const webrtc::IceCandidateInterface* candidate) const {
        signaling_client_->SendIceCandidate(candidate);
    }

    void StopSignalingServer() {
        if (signaling_client_) {
            signaling_client_->Stop();
            signaling_client_ = nullptr;
        }
    }

    void ClosePeerConnection() {
        if (!state_manager_->IsState(ConnectorStateManager::RTC_CONNECTED)) {
            return;  // 연결 중이 아닌 경우 종료하지 않습니다.
        }

        state_manager_->SetState(ConnectorStateManager::RTC_DISCONNECTING);
        try {
            MOSAIC_LOG_INFO("Closing Media Tracks and Data Channels...");
            client_->CloseAllMediaTracks();
            client_->CloseAllDataChannels();

            MOSAIC_LOG_INFO("Closing Peer Connection...");
            // 피어 연결 종료
            if (peer_connection_) {
                peer_connection_->Close();
                peer_connection_ = nullptr;
            }
        } catch (const std::exception& e) {
            MOSAIC_LOG_ERROR("Exception caught: {}", e.what());
        }

        // 연결 해제 알림
        OnConnectionDisconnected();
    }

    void DestroyPeerConnectionManager() {
        MOSAIC_LOG_INFO("Destroying Peer Connection, Observer and Factory...");
        // finally 블록과 동일한 정리 작업
        peer_connection_ = nullptr;
        peer_connection_observer_ = nullptr;

        peer_connection_factory_ = nullptr;

        MOSAIC_LOG_INFO("Destroying Threads...");
        // 아래 스레드 종료 및 파괴 작업은 peer_connection_factory 의 파괴 이후에 수행해야 합니다.
        if (network_thread_) {
            network_thread_->Stop();
            network_thread_ = nullptr;
        }
        if (worker_thread_) {
            worker_thread_->Stop();
            worker_thread_ = nullptr;
        }
        if (signaling_thread_) {
            signaling_thread_->Stop();
            signaling_thread_ = nullptr;
        }
    }

  private:
    std::shared_ptr<MosaicConnector> client_;
    std::shared_ptr<ConnectorStateManager> state_manager_;
    std::shared_ptr<ISignalingClient> signaling_client_;
    std::shared_ptr<WebRtcConfig> web_rtc_config_;

    webrtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory_ = nullptr;
    webrtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_ = nullptr;
    std::shared_ptr<webrtc::PeerConnectionObserver> peer_connection_observer_ = nullptr;

    std::unique_ptr<webrtc::TaskQueueFactory> task_queue_factory_ = nullptr;
    std::unique_ptr<rtc::Thread> network_thread_ = nullptr;
    std::unique_ptr<rtc::Thread> worker_thread_ = nullptr;
    std::unique_ptr<rtc::Thread> signaling_thread_ = nullptr;

    [[nodiscard]] webrtc::scoped_refptr<webrtc::VideoTrackInterface> CreateVideoTrack(
        const webrtc::scoped_refptr<webrtc::VideoTrackSourceInterface>& source,
        const std::string& label) const {
        return peer_connection_factory_->CreateVideoTrack(source, label);
    }

    [[nodiscard]] webrtc::scoped_refptr<webrtc::RtpSenderInterface> AddTrack(
        const webrtc::scoped_refptr<webrtc::MediaStreamTrackInterface>& track,
        const std::vector<std::string>& stream_ids) const {
        auto add_track_result = peer_connection_->AddTrack(track, stream_ids);
        if (!add_track_result.ok()) {
            MOSAIC_LOG_ERROR("Failed to add track: {}", add_track_result.error().message());
            return nullptr;
        }
        MOSAIC_LOG_DEBUG("Track added successfully: {}", track->id());
        return add_track_result.value();
    }

    friend class PeerConnectionManager;
    friend class MosaicConnector;
};

PeerConnectionManager::PeerConnectionManager(const std::shared_ptr<MosaicConnector>& client,
                                             const std::shared_ptr<ConnectorStateManager>& state_manager,
                                             const std::shared_ptr<ISignalingClient>& signaling_client,
                                             const std::shared_ptr<WebRtcConfig>& web_rtc_config) {
    pImpl = std::make_shared<Impl>(client, state_manager, signaling_client, web_rtc_config);
}

void PeerConnectionManager::InitializeWebRTC() const {
    pImpl->InitializeWebRTC();
}

void PeerConnectionManager::CreatePeerConnection() {
    pImpl->CreatePeerConnection(shared_from_this());
}

void PeerConnectionManager::HandleSdpOffer(webrtc::SessionDescriptionInterface* sdp_offer) {
    pImpl->HandleSdpOffer(sdp_offer, shared_from_this());
}

void PeerConnectionManager::AfterSetSessionDescription() {
    pImpl->AfterSetSessionDescription(shared_from_this());
}

void PeerConnectionManager::AfterCreateSdpAnswer(webrtc::SessionDescriptionInterface* desc) {
    pImpl->AfterCreateSdpAnswer(desc, shared_from_this());
}

void PeerConnectionManager::AfterSetLocalDescription(const webrtc::SessionDescriptionInterface* desc) const {
    pImpl->AfterSetLocalDescription(desc);
}

void PeerConnectionManager::HandleIceCandidateFromSignaling(const webrtc::IceCandidateInterface* candidate) const {
    pImpl->HandleIceCandidateFromSignaling(candidate);
}

void PeerConnectionManager::OnConnectionConnected() const {
    pImpl->OnConnectionConnected();
}

void PeerConnectionManager::OnConnectionDisconnected() const {
    pImpl->OnConnectionDisconnected();
}

void PeerConnectionManager::SendIceCandidate(const webrtc::IceCandidateInterface* candidate) const {
    pImpl->SendIceCandidate(candidate);
}

void PeerConnectionManager::StopSignalingServer() const {
    pImpl->StopSignalingServer();
}

void PeerConnectionManager::ClosePeerConnection() const {
    pImpl->ClosePeerConnection();
}

void PeerConnectionManager::DestroyPeerConnectionManager() const {
    pImpl->DestroyPeerConnectionManager();
}

webrtc::scoped_refptr<webrtc::VideoTrackInterface> PeerConnectionManager::CreateVideoTrack(
    const webrtc::scoped_refptr<webrtc::VideoTrackSourceInterface>& source,
    const std::string& label) const {
    return pImpl->CreateVideoTrack(source, label);
}

webrtc::scoped_refptr<webrtc::RtpSenderInterface> PeerConnectionManager::AddTrack(
    const webrtc::scoped_refptr<webrtc::MediaStreamTrackInterface>& track,
    const std::vector<std::string>& stream_ids) const {
    return pImpl->AddTrack(track, stream_ids);
}

webrtc::scoped_refptr<webrtc::PeerConnectionInterface> PeerConnectionManager::GetPeerConnection() const {
    return pImpl->peer_connection_;
}

webrtc::PeerConnectionInterface::IceServer GetIceServer(const IceServerConfig& ice_config) {
    webrtc::PeerConnectionInterface::IceServer server;
    if (ice_config.urls.empty() || ice_config.username.empty() || ice_config.credential.empty()) {
        // TODO
        return server;  // Return an empty IceServer
    }
    // Create an IceServer with the TURN URL, username, and credential
    for (const auto& url : ice_config.urls) {
        server.urls.push_back("turn:" + url);
    }
    server.username = ice_config.username;
    server.password = ice_config.credential;

    return server;
}

}  // namespace mosaic::core
