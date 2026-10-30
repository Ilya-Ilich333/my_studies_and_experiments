#!/bin/bash

# The progs from xattr packet don't afford to add pairs key-value with same
# keys. This script recieves a) key, b) value, c) list of the files. It checks
# old xattr keys of the files, if the files have same keys, the script finds a
# key with the biggest number (the key without number is zero key) 
# and adds the new key with (biggest + 1) number. 

# The first param is KEY (wuthout "user."-prefix).
KEY=$1
# The second param is VALUE.
VALUE=$2
# PAR_NUM is number of transferred parameters.
PAR_COUNT=$#
# Number of a individual file, which is processing.
FILE_IND=1
# A name of the file, which is processing.
PAR_IND=$(( $FILE_IND + 2 ))
CUR_FILE=${!PAR_IND}

# This function works with xattr attributes of a individual file.
function file_proc {

# 1st command prints a list of the attributes, which matches the pattern/.
# 2st command deletes the first string from the output. 
# < <(...). < is usual input redirection operator. 
# <(...) is process substitution command.  
mapfile -t \
LINE < <(getfattr -m "^user\\.$KEY[[:digit:]]*" $CUR_FILE | sed "1d")

# awk command deletes the last line from the output.
# LIN_NUM stores number of the attributes with the same key. 
LIN_NUM=\
`getfattr -m "^user\\.$KEY[[:digit:]]*" $CUR_FILE | sed "1d" | awk "NF" | wc -l`

# If LIN_NUM = 0, the new key is unique.
if [[ $LIN_NUM -eq 0 ]] 
then
	setfattr -n user.${KEY} -v $VALUE $CUR_FILE
fi

# If LIN_NUM > 0, the new key isen't unique.
if [[ $LIN_NUM -gt 0 ]] 
then
	setfattr -n user.${KEY}${LIN_NUM} -v $VALUE $CUR_FILE
fi	
}

while [[ $PAR_IND -le $PAR_COUNT ]] 
do
	file_proc
	((FILE_IND++)) 
	PAR_IND=$(( $FILE_IND + 2 ))
	CUR_FILE=${!PAR_IND}
done
