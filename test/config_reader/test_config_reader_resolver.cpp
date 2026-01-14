//
// Created by yhkim on 12/30/25.
//

#include <filesystem>

#include <gtest/gtest.h>
#include <mosaic/auto_configurer/config_reader/config_reader_factory.h>
#include <mosaic/auto_configurer/config_reader/config_reader_resolver.h>
#include <mosaic/auto_configurer/config_reader/yaml_config_reader.h>

using namespace mosaic::auto_configurer;

class ConfigReaderResolverTest : public ::testing::Test {
  protected:
    std::string test_yaml_path;

    void SetUp() override {
        // Get the path to the test yaml file
        std::filesystem::path current_path = std::filesystem::current_path();
        test_yaml_path = (current_path / "test" / "config_reader" / "test_config.yaml").string();

        // Register YamlConfigReader factory
        ConfigReaderResolver::GetInstance().RegisterConfigReader<YamlConfigReader>();
    }
};

// Test IsSupportedFile method
TEST_F(ConfigReaderResolverTest, IsSupportedFile_YamlExtension) {
    EXPECT_TRUE(ConfigReaderResolver::GetInstance().IsSupportedFile(test_yaml_path));
}

TEST_F(ConfigReaderResolverTest, IsSupportedFile_YmlExtension) {
    std::string yml_path = "/path/to/config.yml";
    EXPECT_TRUE(ConfigReaderResolver::GetInstance().IsSupportedFile(yml_path));
}

TEST_F(ConfigReaderResolverTest, IsSupportedFile_UnsupportedExtension) {
    EXPECT_FALSE(ConfigReaderResolver::GetInstance().IsSupportedFile("/path/to/config.json"));
    EXPECT_FALSE(ConfigReaderResolver::GetInstance().IsSupportedFile("/path/to/config.xml"));
    EXPECT_FALSE(ConfigReaderResolver::GetInstance().IsSupportedFile("/path/to/config.txt"));
}

TEST_F(ConfigReaderResolverTest, IsSupportedFile_NoExtension) {
    EXPECT_FALSE(ConfigReaderResolver::GetInstance().IsSupportedFile("/path/to/config"));
}

// Test GetConfigReader method - IConfigReader 타입으로 저장
TEST_F(ConfigReaderResolverTest, GetConfigReader_YamlFile) {
    // ConfigReaderResolver로부터 IConfigReader 타입으로 받기
    std::shared_ptr<IConfigReader> reader = ConfigReaderResolver::GetInstance().GetConfigReader(test_yaml_path);

    ASSERT_NE(reader, nullptr);

    // IConfigReader 인터페이스를 통해 yaml 파일 읽기
    mosaic::core::ConnectorConfigs configs = reader->LoadConfigs(test_yaml_path);

    // Test server config
    EXPECT_EQ(configs.server.ws_url, "ws://localhost:8080");

    // Test auth config
    EXPECT_EQ(configs.server.auth_config.type, "basic");
    EXPECT_EQ(configs.server.auth_config.robot_id, "robot123");
    EXPECT_EQ(configs.server.auth_config.user_id, "user456");

    // Test connectors
    EXPECT_EQ(configs.connectors.size(), 2);
    EXPECT_EQ(configs.connectors[0].type, "camera");
    EXPECT_EQ(configs.connectors[1].type, "microphone");
}

TEST_F(ConfigReaderResolverTest, GetConfigReader_UnsupportedExtension) {
    EXPECT_THROW(ConfigReaderResolver::GetInstance().GetConfigReader("/path/to/config.json"), std::invalid_argument);
}

TEST_F(ConfigReaderResolverTest, GetConfigReader_NoExtension) {
    EXPECT_THROW(ConfigReaderResolver::GetInstance().GetConfigReader("/path/to/config"), std::invalid_argument);
}

// Test multiple factories
TEST_F(ConfigReaderResolverTest, MultipleFactories_SameType) {
    // 같은 타입의 factory를 여러 개 등록해도 동작하는지 확인
    ConfigReaderResolver::GetInstance().RegisterConfigReader<YamlConfigReader>();

    EXPECT_TRUE(ConfigReaderResolver::GetInstance().IsSupportedFile(test_yaml_path));

    std::shared_ptr<IConfigReader> reader = ConfigReaderResolver::GetInstance().GetConfigReader(test_yaml_path);
    ASSERT_NE(reader, nullptr);
}

// Test IConfigReader interface usage
TEST_F(ConfigReaderResolverTest, IConfigReader_Interface_IsSupportedExtension) {
    std::shared_ptr<IConfigReader> reader = ConfigReaderResolver::GetInstance().GetConfigReader(test_yaml_path);

    ASSERT_NE(reader, nullptr);

    // IConfigReader 인터페이스를 통해 IsSupportedExtension 호출
    EXPECT_TRUE(reader->IsSupportedExtension("yaml"));
    EXPECT_TRUE(reader->IsSupportedExtension("yml"));
    EXPECT_FALSE(reader->IsSupportedExtension("json"));
}