#
# ****************************************************************************
# @attention
#
# Copyright (c) 2026 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
#
# ****************************************************************************

import configparser
import os
import sys


def get_ini_path() -> str:
    return os.path.normpath(os.path.join(os.path.dirname(__file__), "da.ini"))


def get_path_provisioning() -> str:
    config_path = get_ini_path()
    config = configparser.ConfigParser()
    config.read(config_path)
    path = config["provisioning"]["path"]
    if not os.path.isabs(path):
        path = os.path.join(os.path.dirname(config_path), path)
    return os.path.abspath(path)


path_rot_provisioning = get_path_provisioning()
launcher_path = os.path.abspath(os.path.dirname(__file__))
sys.path = [
    path for path in sys.path
    if os.path.abspath(path or os.getcwd()) != launcher_path
]
sys.path.insert(0, path_rot_provisioning)

from da.da import Provisioning
from helper.common.log import ROT_Logger
from helper.exception.rot_exception import handle_rot_exception


@handle_rot_exception
def main():
    rot_logger = ROT_Logger("da.log")
    Provisioning(get_ini_path(), rot_logger).process_args()


if __name__ == "__main__":
    main()