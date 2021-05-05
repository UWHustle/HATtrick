


make clean
make all

./HATtrickBench --help / -h

	Options:
	1. Generate initial data:
	    [-gen]
	    -pa <output path>
	    -d <delimiter>

	2. Initialize DB:
	    [-init]
	    -dsn <data source name>
	    -usr <DB username>
	    -pwd <DB password>
	    -pa <data path>
		-db <database>

	3. Run benchmark for a specific hybrid workload:
	    [-run]
	    -dsn <data source name>
	    -usr <DB username>
	    -pwd <DB password>
	    -tc <number of transactional clients>
	    -ac <number of analytical clients>
	    -wd <warm up duration in sec>
	    -td <test duration in sec>
		-db <database>
		-t <type: store procedure(sp) or prepared statements(ps)>

	4. Run benchmark and get frontier:
	    [-frontier]
	    -dsn <data source name>
	    -usr <DB username>
	    -pwd <DB password>
	    -wd <warm up duration in sec>
	    -td <test duration in sec>
		-db <database>
		-t <type: store procedure(sp) or prepared statements(ps)>		

Option 1 -- Generate the data files
./HATtrickBench -gen -pa 'path' -d '!'

Option 2 -- Initialize DB (i.e. connect to DB, create schema, load data)
./HATtrickBench -init -dsn 'dsn'  -usr 'user' -pwd 'password' -pa 'path' -db 'database'

Option 3 -- Run benchmark, get results
./HATtrickBench -run -dsn 'dsn' -usr 'user' -pwd 'password' -tc 'tc' -ac 'ac' -wd 'wd' -td 'td' -db 'database' -t 't'

Option 4 -- Run benchmark and get the frontier 
./HATtrickBench -frontier -dsn 'dsn' -usr 'user' -pwd 'password' -wd 'wd' -td 'td' -db 'database' -t 't'

