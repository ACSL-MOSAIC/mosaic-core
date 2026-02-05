"""
Equivalent of test/logger/test_logging_level.cpp

C++ tests use a custom LoggingLevelTester : ILogger that counts how many times
LOG() is called per LogLevel.  The filtering (IsDebugEnabled / IsInfoEnabled …)
lives in the non-virtual LOG_DEBUG / LOG_INFO helpers, so the counter only
increments when the level gate passes.
"""

import unittest
from collections import defaultdict

from mosaic_core.logging import ILogger, set_logger, LogLevel, set_log_level


class LoggingLevelTester(ILogger):
    """Python equivalent of the C++ LoggingLevelTester helper."""

    def __init__(self):
        super().__init__()
        self.counts: dict[LogLevel, int] = defaultdict(int)

    def log(self, message: str, log_level: LogLevel) -> None:
        self.counts[log_level] += 1


class TestLoggingLevel(unittest.TestCase):
    def setUp(self):
        self.tester = LoggingLevelTester()
        set_logger(self.tester)

    # --- SimpleLogging: level INFO, log at INFO → reaches LOG() ---
    # C++: TEST_F(LoggingLevelTest, SimpleLogging)
    def test_simple_logging(self):
        set_log_level(LogLevel.INFO)
        self.tester.log_info("hello")
        self.assertEqual(self.tester.counts[LogLevel.INFO], 1)

    # --- LevelInfoLogDebug: level INFO, log at DEBUG → filtered out ---
    # C++: TEST_F(LoggingLevelTest, LevelInfoLogDebug)
    def test_level_info_log_debug(self):
        set_log_level(LogLevel.INFO)
        self.tester.log_debug("should be filtered")
        self.assertEqual(self.tester.counts[LogLevel.DEBUG], 0)

    # --- LevelDebugLogInfo: level DEBUG, log at INFO → passes ---
    # C++: TEST_F(LoggingLevelTest, LevelDebugLogInfo)
    def test_level_debug_log_info(self):
        set_log_level(LogLevel.DEBUG)
        self.tester.log_info("should pass")
        self.assertEqual(self.tester.counts[LogLevel.INFO], 1)

    # --- LevelDebugLogDebug: level DEBUG, log at DEBUG → passes ---
    # C++: TEST_F(LoggingLevelTest, LevelDebugLogDebug)
    def test_level_debug_log_debug(self):
        set_log_level(LogLevel.DEBUG)
        self.tester.log_debug("should pass")
        self.assertEqual(self.tester.counts[LogLevel.DEBUG], 1)


if __name__ == "__main__":
    unittest.main()
