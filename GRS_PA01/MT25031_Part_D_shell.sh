#!/bin/bash
#
# MT25031 - Part D Automation Script 
#

CSV="MT25031_Part_D_CSV.csv"
PROC="./partA_process"
THREAD="./partA_thread"
WORKERS=("cpu" "mem" "io")

echo "Program+Worker,Count,CPU,Mem(KB),IO" > "$CSV"

run_and_measure () {
    PROG=$1
    LABEL=$2
    WORKER=$3
    COUNT=$4
    NAME=$(basename $PROG)

    # Start IO monitoring
    iostat -dx 1 6 > io.tmp &
    IOPID=$!

    # Run program pinned to CPU 0
    taskset -c 0 $PROG $WORKER $COUNT &
    PID=$!

    # Allow program to start
    sleep 0.3

    CPU_SUM=0
    MEM_MAX=0
    SAMPLES=0

    while kill -0 $PID 2>/dev/null
    do
        LINE=$(top -b -n 1 -w 512 | awk -v p="$NAME" '
            $12 == p && $9 ~ /^[0-9.]+$/ && $6 ~ /^[0-9]+$/ {print $9, $6; exit}
        ')

        if [ -n "$LINE" ]; then
            CPU=$(echo "$LINE" | awk '{print $1}')
            if [[ "$LABEL" == "programA" ]]; then
    MEM=$(top -b -n 1 -w 512 | awk '
        $12=="partA_process" && $6 ~ /^[0-9]+$/ { sum += $6 }
        END { print sum }
    ')
else
    MEM=$(top -b -n 1 -w 512 | awk '
        $12=="partA_thread" && $6 ~ /^[0-9]+$/ { print $6; exit }
    ')
fi


            CPU_SUM=$(echo "$CPU_SUM + $CPU" | bc)
            [ "$MEM" -gt "$MEM_MAX" ] && MEM_MAX=$MEM
            SAMPLES=$((SAMPLES + 1))
        fi

        sleep 0.5
    done

    wait $IOPID 2>/dev/null

    if [ "$SAMPLES" -eq 0 ]; then
        AVG_CPU=0
    else
        AVG_CPU=$(echo "scale=2; $CPU_SUM / $SAMPLES" | bc)
    fi

    IO_VAL=$(grep -v "^Linux" io.tmp | awk '{sum+=$10} END {print sum}')
    rm -f io.tmp

    echo "$LABEL+$WORKER,$COUNT,$AVG_CPU,$MEM_MAX,$IO_VAL" >> "$CSV"
}

# ---------- Program A: Processes ----------
for W in "${WORKERS[@]}"
do
    for N in 2 3 4 5
    do
        echo "Running Program A | $W | Processes=$N"
        run_and_measure "$PROC" "programA" "$W" "$N"
    done
done

# ---------- Program B: Threads ----------
for W in "${WORKERS[@]}"
do
    for N in 2 3 4 5 6 7 8
    do
        echo "Running Program B | $W | Threads=$N"
        run_and_measure "$THREAD" "programB" "$W" "$N"
    done
done

echo "Part D completed → $CSV generated"
