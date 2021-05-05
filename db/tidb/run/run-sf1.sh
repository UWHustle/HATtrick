#!/bin/bash
#./HATrickBench -gen -pa '/root/.tiup/data/csv-sf1' -d '!'
<<COM
./HATrickBench -init -dsn 'msdsn'  -pa '/root/.tiup/data/csv-sf1' -db tidb
mysql --host 127.0.0.1 --port 4000 -u root < db/tidb/tiflashOn.sql
sleep 60
./HATrickBench -run -dsn 'msdsn' -tc 0 -ac 64 -wd 100 -td 300 -db tidb -t ps
./HATrickBench -run -dsn 'msdsn' -tc 64 -ac 0 -wd 100 -td 300 -db tidb -t ps
./HATrickBench -init -dsn 'msdsn'  -pa '/root/.tiup/data/csv-sf1' -db tidb
mysql --host 127.0.0.1 --port 4000 -u root < db/tidb/tiflashOn.sql
sleep 60
./HATrickBench -run -dsn 'msdsn' -tc 32 -ac 32 -wd 100 -td 300 -db tidb -t ps
./HATrickBench -init -dsn 'msdsn'  -pa '/root/.tiup/data/csv-sf1' -db tidb
mysql --host 127.0.0.1 --port 4000 -u root < db/tidb/tiflashOn.sql
sleep 60
./HATrickBench -run -dsn 'msdsn' -tc 12 -ac 52 -wd 100 -td 300 -db tidb -t ps
COM
./HATrickBench -init -dsn 'msdsn'  -pa '/root/.tiup/data/csv-sf1' -db tidb
 < db/tidb/tiflashOn.sql
sleep 60
./HATrickBench -frontier -dsn 'msdsn' -wd 100 -td 300 -db tidb -t ps
