#!/bin/bash
#./HATrickBench -gen -pa '/mnt/data/csv-sf1' -d '!'
#rm /mnt/data/log/*.log
#systemctl stop postgresql
#sh -c "echo 3 > /proc/sys/vm/drop_caches"
#systemctl restart postgresql
#sleep 60
cd /home/azureuser/HATrickBench
./HATrickBench -init -dsn 'pgdsn'  -usr 'postgres' -pwd 'hatrickbench' -pa '/mnt/data/csv-sf1' -db postgres -t sp
./HATrickBench -frontier -dsn 'pgdsn' -usr 'postgres' -pwd 'hatrickbench' -wd 100 -td 120 -db postgres -t sp
