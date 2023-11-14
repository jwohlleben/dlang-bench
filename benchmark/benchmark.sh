#!/bin/bash

ensure_file_existing()
{
	if [ ! -f $1 ]
	then
		echo "[!] Can not find file '$1'"
		exit 1
	fi	
}

ensure_dir_existing()
{
	if [ ! -d $1 ]
	then
		echo "[!] Can not find directory '$1'"
		exit 1
	fi	
}

get_config_data()
{
	echo "$1" | cut -d '=' -f 2
}

cut_output()
{
	echo "$1" | sed 's/^[^.]*.\///' | sed 's/ /_/g' | sed 's/\//#/g'
}

if [ $# -ne 1 ]
then
	echo "Usage: $0 config_file"
	exit 1
fi

config_file=$1
ensure_file_existing $config_file
config_file_abs=`readlink -f "$config_file"`
base_dir=`dirname "$config_file_abs"`

cd "$base_dir"

echo "[*] Parsing config"

iterations=10
warmup_threads=16
warmup_time_initial=30s
warmup_time_in_between="none"
output_dir=out
csv_header="none"

parsing_done=false

while read line
do
	if [ $parsing_done = true -a "$line" != "" ]
	then
		out_name=`cut_output "${line}.csv"`
		if [ "$csv_header" != "none" ]
		then
			echo "$csv_header" > "$output_dir/$out_name"
		fi
		if [ "$warmup_time_in_between" != "none" ]
		then
			echo "[*] Starting warmup for $warmup_time_in_between"
			stress-ng -q -c "$warmup_threads" -t "$warmup_time_in_between"
		fi
		for iteration in `seq 1 $iterations`
		do
			echo -n -e "\r[+] [$iteration/$iterations] Running $line"
			$line >> "$output_dir/$out_name"
			if [ $? -ne 0 ]
			then
				echo "[!] An error occurred. Exiting."
				exit 1
			fi
		done
		echo ""
	else
		case $line in
		iterations=*)
			iterations=`get_config_data "$line"`
			;;
		warmup_threads=*)
			warmup_threads=`get_config_data "$line"`
			;;
		warmup_time_initial=*)
			warmup_time_initial=`get_config_data "$line"`
			;;
		warmup_time_in_between=*)
			warmup_time_in_between=`get_config_data "$line"`
			;;
		output_dir=*)
			output_dir=`get_config_data "$line"`
			;;
		csv_header=*)
			csv_header=`get_config_data "$line"`
			;;
		run:)
			parsing_done=true
			echo "[+] Iterations = $iterations"
			echo "[+] Warmup threads = $warmup_threads"
			echo "[+] Warmup time inital = $warmup_time_initial"
			echo "[+] Warmup time in between = $warmup_time_in_between"
			echo "[+] Base directory = $base_dir"
			echo "[+] Output directory = $output_dir"
			echo "[+] CSV header = $csv_header"
			echo "[*] Checking directories"
			ensure_dir_existing "$output_dir"
			echo "[*] Starting warmup for $warmup_time_initial"
			stress-ng -q -c "$warmup_threads" -t "$warmup_time_initial"
			echo "[*] Starting benchmark"
			;;
		"")
			;;
		*)
			echo "[!] Could not parse '$line'"
			exit 1
		;;
		esac
	fi
done < "$config_file_abs"

echo "[*] Done benchmarking"
