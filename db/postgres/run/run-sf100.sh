#!/bin/bash
#./HATrickBench -gen -pa '/mnt/data/csv-sf100' -d '!'
#rm /mnt/data/log/*.log
#systemctl stop postgresql
#sh -c "echo 3 > /proc/sys/vm/drop_caches"
#systemctl restart postgresql
#sleep 60
cd /home/azureuser/HATrickBench
./HATrickBench -init -dsn 'pgdsn'  -usr 'postgres' -pwd 'hatrickbench' -pa '/mnt/data/csv-sf100' -db postgres -t sp
./HATrickBench -frontier -dsn 'pgdsn' -usr 'postgres' -pwd 'hatrickbench' -wd 300 -td 600 -db postgres -t sp

