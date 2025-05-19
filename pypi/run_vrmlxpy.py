from vrmlxpy.vrmlxpy import *
import sys

if len(sys.argv) != 4:
    print(f"Usage: {sys.argv[0]} <input_file> <output_file> <config_file>")
    sys.exit(1)

input_filename = sys.argv[1]
output_filename = sys.argv[2]
config_filename = sys.argv[3]

vrmlxpy.vrmlxpy.print_version()
vrmlxpy.vrmlxpy.convert_vrml(input_filename, output_filename, config_filename)