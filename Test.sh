#!/bin/sh

RED='\033[0;31m'
GREEN='\033[0;32m'
NOC='\033[0m'

test_file() {
  FILENAME=$1
  AND_FILE=`sed -n '1p' $FILENAME`
  SEED=`sed -n '2p' $FILENAME`
  EXPECTED_RESULT=`sed -n '3,$p' $FILENAME`

  echo "Testing: $FILENAME"

  OUTPUT=`cat $AND_FILE | ./Andersen $SEED`

  if [ "$OUTPUT" = "$EXPECTED_RESULT" ]
  then
    printf "${GREEN}OK!${NOC}\n"
  else
    printf "${RED}ERROR!\nEXPECTED:${NOC}\n"
    echo "$EXPECTED_RESULT"
    printf "${RED}GOT:${NOC}\n"
    echo "$OUTPUT"
  fi
}

for FILENAME in tests/*; do
test_file $FILENAME
done
