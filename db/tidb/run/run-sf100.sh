#!/bin/bash
#./HATrickBench -gen -pa '/root/.tiup/data/csv-sf100' -d '!'
./HATrickBench -init -dsn 'msdsn'  -pa '/root/.tiup/data/csv-sf100' -db tidb
mysql --host 127.0.0.1 --port 4000 -u root < db/tidb/tiflashOn.sql
sleep 1800
./HATrickBench -frontier -dsn 'msdsn' -wd 400 -td 1000 -db tidb -t ps
