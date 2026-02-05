"""
Equivalent of test/config_reader/test_config_reader_resolver.cpp

# NOTE: ConfigReaderResolver 및 IConfigReader는 현재 pybind11로 바인딩되지 않아
# 아래 테스트는 실행할 수 없습니다.  바인딩이 추가되면 주석을 제거하세요.
#
# 바인딩이 필요한 클래스 / 함수:
#   - mosaic::config::IConfigReader            (인터페이스)
#   - mosaic::config::ConfigReaderResolver     (singleton)
#
# ConnectorResolver와 동일한 패턴으로 자유함수를 추가하면 사용 가능:
#   register_config_reader(factory)            → ConfigReaderResolver에 팩토리 등록
#   is_supported_config_file(filename: str)    → 등록된 리더 중 확장자 지원 여부 확인
#   get_config_reader(filename: str)           -> IConfigReader   → 해당 파일을 읽을 리더 반환
"""

# import unittest
# import mosaic_core


# class TestConfigReaderResolverIsSupportedFile(unittest.TestCase):
#     """C++ TEST_F(ConfigReaderResolverTest, IsSupportedFile)"""
#
#     # YamlConfigReader가 기본 등록되어 있다고 가정 (C++ 테스트와 동일)
#     def test_yaml_supported(self):
#         self.assertTrue(mosaic_core.is_supported_config_file("config.yaml"))
#
#     def test_yml_supported(self):
#         self.assertTrue(mosaic_core.is_supported_config_file("config.yml"))
#
#     def test_json_not_supported(self):
#         self.assertFalse(mosaic_core.is_supported_config_file("config.json"))
#
#     def test_xml_not_supported(self):
#         self.assertFalse(mosaic_core.is_supported_config_file("config.xml"))
#
#     def test_txt_not_supported(self):
#         self.assertFalse(mosaic_core.is_supported_config_file("config.txt"))
#
#     def test_no_extension_not_supported(self):
#         self.assertFalse(mosaic_core.is_supported_config_file("config"))


# class TestConfigReaderResolverGetConfigReader(unittest.TestCase):
#     """C++ TEST_F(ConfigReaderResolverTest, GetConfigReader)"""
#
#     def test_get_reader_for_yaml(self):
#         reader = mosaic_core.get_config_reader("config.yaml")
#         self.assertIsNotNone(reader)
#
#     def test_get_reader_for_unsupported_throws(self):
#         """지원되지 않는 파일 타입은 예외 발생"""
#         with self.assertRaises(Exception):
#             mosaic_core.get_config_reader("config.json")


# class TestConfigReaderResolverRegisterConfigReader(unittest.TestCase):
#     """C++ TEST_F(ConfigReaderResolverTest, RegisterConfigReader)
#
#     C++ 테스트는 커스텀 IConfigReaderFactory를 여러 번 등록하여
#     IsSupportedFile / GetConfigReader 동작을 검증합니다.
#     Python에서도 IConfigReaderFactory 트램폴린이 필요합니다.
#     """
#
#     # def test_register_custom_factory(self):
#     #     class MyReaderFactory(mosaic_core.IConfigReaderFactory):
#     #         def get_supported_extension(self) -> str:
#     #             return ".custom"
#     #         def create_reader(self) -> mosaic_core.IConfigReader:
#     #             ...  # 커스텀 IConfigReader 반환
#     #
#     #     mosaic_core.register_config_reader(MyReaderFactory())
#     #     self.assertTrue(mosaic_core.is_supported_config_file("test.custom"))
#     pass


# if __name__ == "__main__":
#     unittest.main()