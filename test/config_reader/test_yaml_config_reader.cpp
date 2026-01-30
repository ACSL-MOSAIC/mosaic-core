#include <filesystem>

#include <gtest/gtest.h>
#include <mosaic/auto_configurer/config_reader/yaml_config_reader.h>

using namespace mosaic::auto_configurer;

class YamlConfigReaderTest : public ::testing::Test {
  protected:
    YamlConfigReader reader;
    std::string test_yaml_path;

    void SetUp() override {
        // Get the path to the test yaml file
        std::filesystem::path current_path = std::filesystem::current_path();
        test_yaml_path = (current_path / "test" / "config_reader" / "test_config.yaml").string();
    }
};

// Test IsSupportedExtension method
TEST_F(YamlConfigReaderTest, IsSupportedExtension_YamlExtension) {
    EXPECT_TRUE(reader.IsSupportedExtension("yaml"));
}

TEST_F(YamlConfigReaderTest, IsSupportedExtension_YmlExtension) {
    EXPECT_TRUE(reader.IsSupportedExtension("yml"));
}

TEST_F(YamlConfigReaderTest, IsSupportedExtension_UnsupportedExtension) {
    EXPECT_FALSE(reader.IsSupportedExtension("json"));
    EXPECT_FALSE(reader.IsSupportedExtension("xml"));
    EXPECT_FALSE(reader.IsSupportedExtension("txt"));
    EXPECT_FALSE(reader.IsSupportedExtension(""));
}

// Test LoadConfigs method
TEST_F(YamlConfigReaderTest, LoadConfigs_ValidYamlFile) {
    mosaic::core::MosaicConfig configs = reader.LoadConfigs(test_yaml_path);

    // Test server config
    EXPECT_EQ(configs.server.ws_url, "ws://localhost:8080");

    // Test auth config
    EXPECT_EQ(configs.server.auth_config.type, "basic");
    EXPECT_EQ(configs.server.auth_config.robot_id, "robot123");
    EXPECT_EQ(configs.server.auth_config.params.size(), 2);
    EXPECT_EQ(configs.server.auth_config.params["key1"], "value1");
    EXPECT_EQ(configs.server.auth_config.params["key2"], "value2");

    // Test WebRTC config
    EXPECT_EQ(configs.server.webrtc_config.ice_servers.size(), 2);

    // Test first ICE server
    EXPECT_EQ(configs.server.webrtc_config.ice_servers[0].urls.size(), 2);
    EXPECT_EQ(configs.server.webrtc_config.ice_servers[0].urls[0], "stun:stun.l.google.com:19302");
    EXPECT_EQ(configs.server.webrtc_config.ice_servers[0].urls[1], "stun:stun1.l.google.com:19302");
    EXPECT_EQ(configs.server.webrtc_config.ice_servers[0].username, "testuser");
    EXPECT_EQ(configs.server.webrtc_config.ice_servers[0].credential, "testcred");

    // Test second ICE server
    EXPECT_EQ(configs.server.webrtc_config.ice_servers[1].urls.size(), 1);
    EXPECT_EQ(configs.server.webrtc_config.ice_servers[1].urls[0], "turn:turn.example.com:3478");
    EXPECT_EQ(configs.server.webrtc_config.ice_servers[1].username, "turnuser");
    EXPECT_EQ(configs.server.webrtc_config.ice_servers[1].credential, "turncred");

    // Test connectors
    EXPECT_EQ(configs.connectors.size(), 2);

    // Test first connector
    EXPECT_EQ(configs.connectors[0].type, "camera");
    EXPECT_EQ(configs.connectors[0].label, "camera_dc");
    EXPECT_EQ(configs.connectors[0].params.size(), 2);
    EXPECT_EQ(configs.connectors[0].params["resolution"], "1920x1080");
    EXPECT_EQ(configs.connectors[0].params["fps"], "30");

    // Test second connector
    EXPECT_EQ(configs.connectors[1].type, "microphone");
    EXPECT_EQ(configs.connectors[1].label, "mic_dc");
    EXPECT_EQ(configs.connectors[1].params.size(), 2);
    EXPECT_EQ(configs.connectors[1].params["sample_rate"], "48000");
    EXPECT_EQ(configs.connectors[1].params["channels"], "2");
}

TEST_F(YamlConfigReaderTest, LoadConfigs_NonExistentFile) {
    EXPECT_THROW(reader.LoadConfigs("/non/existent/file.yaml"), std::exception);
}
