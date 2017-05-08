# -*- coding: utf-8 -*-
# Copyright 2017 DF Automation & Robotics Sdn. Bhd.  All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

__title__ = 'zalpha-api'
__version__ = '0.0.0'
__license__ = 'Apache 2.0'
__copyright__ = 'Copyright 2017 DF Automation & Robotics Sdn. Bhd.'

from zalpha import Zalpha


def version_compatible(api_server_version):
    # strip last dot (patch version)
    lib_version = __version__[:]
    pos = lib_version.rfind('.')
    if pos >= 0:
        lib_version = lib_version[:pos]

    # strip last dot (patch version)
    server_version = api_server_version[:]
    pos = api_server_version.rfind('.')
    if pos >= 0:
        server_version = server_version[:pos]

    return lib_version == server_version
