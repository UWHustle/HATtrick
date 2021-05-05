#!/bin/bash
#./HATrickBench -gen -pa '/mnt/data/csv-sf10' -d '!'
#rm /mnt/data/log/*.log
#systemctl stop postgresql
#sh -c "echo 3 > /proc/sys/vm/drop_caches"
#systemctl restart postgresql
#sleep 60
cd /home/azureuser/HATrickBench
./HATrickBench -init -dsn 'pgdsn'  -usr 'postgres' -pwd 'hatrickbench' -pa '/mnt/data/csv-sf10' -db postgres -t sp
./HATrickBench -run -dsn 'pgdsn' -usr 'postgres' -pwd 'hatrickbench' -tc 0 -ac 64 -wd 180 -td 240 -db postgres -t sp

