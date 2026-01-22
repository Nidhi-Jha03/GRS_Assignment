#!/bin/bash
#
# MT25031 - Part C Automation Script
#
# Measures CPU, Memory, I/O, and Execution Time for:
#   A+cpu, A+mem, A+io
#   B+cpu, B+mem, B+io
#
# Tools used:
#   - taskset (CPU pinning)
#   - top (CPU%, RES memory)
#   - iostat (disk statistics)
#   - time (execution time)
#

CSV="MT25031_Part_C_CSV.csv"
PROC="./partA_process"
THREAD="./partA_thread"
WORKERS=("cpu" "mem" "io")

echo "Program+Function,CPU%,Memory(KB),IO,Time(s)" > "$CSV"

for W in "${WORKERS[@]}"
do
    echo "Running A+$W"

    # Start I/O monitoring
    iostat -dx 1 5 > io_A.tmp &
    IOPID=$!

    # Run process-based program
    /usr/bin/time -f "%e" taskset -c 0 $PROC $W 2> time_A.tmp &
    PID=$!

    # Allow process to start (IMPORTANT)
    sleep 0.2

    CPU_SUM=0
    MEM_MAX=0
    COUNT=0

    # Sample top while process is alive
    while kill -0 $PID 2>/dev/null
    do
        LINE=$(top -b -n 1 -w 512 | grep "[p]artA_process" | head -n 1)

        if [ -n "$LINE" ]; then
            CPU=$(echo "$LINE" | awk '{print $9}')   # %CPU
            MEM=$(echo "$LINE" | awk '{print $6}')   # RES (KB)

            CPU_SUM=$(echo "$CPU_SUM + $CPU" | bc)
            [ "$MEM" -gt "$MEM_MAX" ] && MEM_MAX=$MEM
            COUNT=$((COUNT + 1))
        fi
        sleep 1
    done

    wait $IOPID 2>/dev/null

    TIME_A=$(cat time_A.tmp)

    if [ "$COUNT" -eq 0 ]; then
        AVG_CPU=0
    else
        AVG_CPU=$(echo "scale=2; $CPU_SUM / $COUNT" | bc)
    fi

    IO_A=$(grep -v "^Linux" io_A.tmp | awk '{sum+=$10} END {print sum}')

    echo "A+$W,$AVG_CPU,$MEM_MAX,$IO_A,$TIME_A" >> "$CSV"

    rm -f io_A.tmp time_A.tmp


    echo "Running B+$W"

    iostat -dx 1 5 > io_B.tmp &
    IOPID=$!

    /usr/bin/time -f "%e" taskset -c 0 $THREAD $W 2> time_B.tmp &
    PID=$!

    # Allow thread program to start
    sleep 0.2

    CPU_SUM=0
    MEM_MAX=0
    COUNT=0

    while kill -0 $PID 2>/dev/null
    do
        LINE=$(top -b -n 1 -w 512 | grep "[p]artA_thread" | head -n 1)

        if [ -n "$LINE" ]; then
            CPU=$(echo "$LINE" | awk '{print $9}')
            MEM=$(echo "$LINE" | awk '{print $6}')

            CPU_SUM=$(echo "$CPU_SUM + $CPU" | bc)
            [ "$MEM" -gt "$MEM_MAX" ] && MEM_MAX=$MEM
            COUNT=$((COUNT + 1))
        fi
        sleep 1
    done

    wait $IOPID 2>/dev/null

    TIME_B=$(cat time_B.tmp)

    if [ "$COUNT" -eq 0 ]; then
        AVG_CPU=0
    else
        AVG_CPU=$(echo "scale=2; $CPU_SUM / $COUNT" | bc)
    fi

    IO_B=$(grep -v "^Linux" io_B.tmp | awk '{sum+=$10} END {print sum}')

    echo "B+$W,$AVG_CPU,$MEM_MAX,$IO_B,$TIME_B" >> "$CSV"

    rm -f io_B.tmp time_B.tmp
done

echo "Part C completed → $CSV generated"
