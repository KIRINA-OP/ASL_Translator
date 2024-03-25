#!/bin/bash

# Source Python virtual environment
source /home/stephen/ASL_Translator/fydp/bin/activate 

# Run Python script
cd /home/stephen/ASL_Translator/python_lib/ && python /home/stephen/ASL_Translator/python_lib/classify_v2.py &


# Wait for all background jobs to finish (remove if you want the script to exit immediately)
exit 0 