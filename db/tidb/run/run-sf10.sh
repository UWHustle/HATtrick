#!/bin/bash
#./HATrickBench -gen -pa '/root/.tiup/data/csv-sf10' -d '!'
./HATrickBench -init -dsn 'msdsn'  -pa '/root/.tiup/data/csv-sf10' -db tidb
mysql --host 127.0.0.1 --port 4000 -u root < db/tidb/tiflashOn.sql
sleep 180
./HATrickBench -frontier -dsn 'msdsn' -wd 200 -td 400 -db tidb -t ps
