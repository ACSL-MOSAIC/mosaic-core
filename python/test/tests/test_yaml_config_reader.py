"""
Equivalent of test/config_reader/test_yaml_config_reader.cpp

# NOTE: YamlConfigReader 및 IConfigReader는 현재 pybind11로 바인딩되지 않아
# 아래 테스트는 실행할 수 없습니다.  바인딩이 추가되면 주석을 제거하세요.
#
# 바인딩이 필요한 클래스:
#   - mosaic::config::IConfigReader        (인터페이스)
#   - mosaic::config::YamlConfigReader     (구현, IConfigReader 상속)
#
# 바인딩 완료 시 필요한 API:
#   YamlConfigReader()                        → 생성자
#   YamlConfigReader.is_supported_extension(ext: str) -> bool
#   YamlConfigReader.load_configs(path: str)  -> MosaicConfig
"""

# import unittest
# import mosaic_core


# class TestYamlConfigReaderIsSupportedExtension(unittest.TestCase):
#     """C++ TEST_F(YamlConfigReaderTest, IsSupportedExtension)"""
#
#     def setUp(self):
#         self.reader = mosaic_core.YamlConfigReader()
#
#     def test_yaml_supported(self):
#         self.assertTrue(self.reader.is_supported_extension(".yaml"))
#
#     def test_yml_supported(self):
#         self.assertTrue(self.reader.is_supported_extension(".yml"))
#
#     # C++ 테스트에서 지원하지 않는 확장자 목록
#     def test_json_not_supported(self):
#         self.assertFalse(self.reader.is_supported_extension(".json"))
#
#     def test_xml_not_supported(self):
#         self.assertFalse(self.reader.is_supported_extension(".xml"))
#
#     def test_txt_not_supported(self):
#         self.assertFalse(self.reader.is_supported_extension(".txt"))
#
#     def test_empty_not_supported(self):
#         self.assertFalse(self.reader.is_supported_extension(""))


# class TestYamlConfigReaderLoadConfigs(unittest.TestCase):
#     """C++ TEST_F(YamlConfigReaderTest, LoadConfigs)
#
#     C++ 테스트는 test/config_reader/test_config.yaml 파일을 읽어
#     MosaicConfig 구조체의 모든 필드를 검증합니다.
#     Python 테스트도 동일한 YAML 파일을 사용하여 검증합니다.
#     """
#
#     def setUp(self):
#         self.reader = mosaic_core.YamlConfigReader()
#         # C++ 테스트와 동일한 경로 (실행 CWD를 고려해 조정 필요)
#         self.config_path = "../../test/config_reader/test_config.yaml"
#
#     def test_load_configs(self):
#         config = self.reader.load_configs(self.config_path)
#
#         # server
#         self.assertEqual(config.server.ws_url, "wss://example.com")
#         # server.auth
#         self.assertEqual(config.server.auth_config.type, "token")
#         self.assertEqual(config.server.auth_config.robot_id, "robot-001")
#         self.assertEqual(config.server.auth_config.params, {"token": "abc123"})
#         # server.webrtc.ice_servers (2개)
#         self.assertEqual(len(config.server.webrtc_config.ice_servers), 2)
#         self.assertEqual(config.server.webrtc_config.ice_servers[0].urls, ["stun:stun.l.google.com:19302"])
#         self.assertEqual(config.server.webrtc_config.ice_servers[1].urls, ["turn:turn.example.com:3478"])
#         self.assertEqual(config.server.webrtc_config.ice_servers[1].username, "user")
#         self.assertEqual(config.server.webrtc_config.ice_servers[1].credential, "pass")
#         # connectors (2개)
#         self.assertEqual(len(config.connectors), 2)
#         self.assertEqual(config.connectors[0].type, "echo")
#         self.assertEqual(config.connectors[0].label, "echo_channel")
#         self.assertEqual(config.connectors[0].params, {"key1": "value1"})
#         self.assertEqual(config.connectors[1].type, "sensor")
#         self.assertEqual(config.connectors[1].label, "sensor_channel")
#         self.assertEqual(config.connectors[1].params, {"key2": "value2"})
#
#     def test_load_configs_file_not_found(self):
#         """C++ TEST_F(YamlConfigReaderTest, LoadConfigs) — non-existent file throws"""
#         with self.assertRaises(Exception):
#             self.reader.load_configs("nonexistent_file.yaml")


# if __name__ == "__main__":
#     unittest.main()