import os
import sys

# Load the compiled extension
if sys.platform.startswith("win"):
    from .vrmlxpy import *  # Windows: Imports `vrmlxpy.pyd`
elif sys.platform.startswith("linux"):
    from .vrmlxpy import *  # Linux: Imports `vrmlxpy.so`
