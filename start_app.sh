#!/bin/bash
SOCKET_FILE="/tmp/visAlgo.sock"
echo "Waiting for $SOCKET_FILE to be created..."
while [ ! -e "$SOCKET_FILE" ]; do
  sleep 1
done
echo "start visapp"
/home/stephen/ASL_Translator/application/build/bin/visapp &

exit 0