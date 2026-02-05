import sys

from ._mosaic_core import logging, configs, handlers, core, auto_configurer

# pybind11 def_submodule은 sys.modules에 자동 등록하지 않음.
# "from mosaic_core.logging import ..." 등의 import가 동작하려면 수동 등록 필요.
sys.modules[__name__ + '.logging'] = logging
sys.modules[__name__ + '.configs'] = configs
sys.modules[__name__ + '.handlers'] = handlers
sys.modules[__name__ + '.core'] = core
sys.modules[__name__ + '.auto_configurer'] = auto_configurer

__all__ = [
    'logging',
    'configs',
    'handlers',
    'core',
    'auto_configurer'
]

__version__ = '1.0.0'
