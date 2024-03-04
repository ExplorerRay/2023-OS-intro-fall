#!/bin/bash

# Read parent pid, child pid
while [ "$#" -gt 0 ]; do
  case "$1" in
    --parent)
      parent="$2"
      shift 2
      ;;
    --child)
      child="$2"
      shift 2
      ;;
	--path)
	  opt="Yes"
	  shift 1
	  ;;
    *)
      echo "Unknown parameter passed: $1"
      exit 1
      ;;
  esac
done

# Check if parent or child is empty
if [ -z "$parent" ] || [ -z "$child" ]; then
  echo "Usage: $0 --parent PARENT_PID --child CHILD_PID [--path]"
  exit 1
fi

########
# TODO #
########
array=( $(pgrep -P $parent) )
rcd=( $parent )
len=${#array[@]}
declare -A visit
while [ $len != 0 ]; do
    if [ ${array[-1]} -eq $child ]; then
        echo "Yes"
        #if [[ $opt == "Yes" ]]; then
        #    echo -n ${rcd[0]}
        #    for ((i=1; i<${#rcd[@]}; i++)); do
        #        echo -n " -> ${rcd[$i]}"
        #    done
		#	echo -n " -> ${array[-1]}"
        #fi
        exit 0
    fi
    rcd+=( ${array[-1]} )
    last=${array[-1]}
	unset array[-1]
	if [[ ${visit[$last]} != 1 ]] ; then
		visit[$last]=1
		array+=( $(pgrep -P $last) )
	fi
	if [[ ${visit[${rcd[-1]}]} -eq 1 ]] && [[ ${rcd[-1]} -eq $last ]] ; then
		unset rcd[-1]
	fi
    len=${#array[@]} # update array length
done

echo "No"
exit 0

