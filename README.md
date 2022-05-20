# HATtrick: An HTAP Benchmark

This repository accompanies our SIGMOD 2022 paper which proposes a systematic way to evaluate HTAP databases:

[Elena Milkai, Yannis Chronis, Kevin P. Gaffney, Zhihan Guo, Jignesh M. Patel, Xiangyao Yu. How Good is MY HTAP System?, SIGMOD, 2022.](https://doi.org/10.1145/3514221.3526148)

## Build instructions

HATtrick is implemented in C++ and uses ODBC for accessing a database. The codebase supports benchmarking for Postgres, TiDB and MySQL. You canfind instructions on how to install Postgres [here](https://www.postgresql.org/docs/current/install-binaries.html), TiDB [here](https://docs.pingcap.com/tidb/stable/production-deployment-using-tiup) and MySQL [here](https://dev.mysql.com/doc/refman/8.0/en/general-installation-issues.html). To build the project, run `make all` from the top-level directory. To run an experiment you need first to generate data, second to load the data in the database and finally compute the frontier and the freshness of your database system. You can examine the options and their parameters by running `./HATtrickBench --help`. 

### Step 1: Generate the data 
```
./HATtrickBench -gen -pa <path> -d <d>
```

### Step 2: Initialize DB (i.e. connect to DB, create schema, load data)
```
./HATtrickBench -init -dsn <dsn>  -usr <user> -pwd <password> -pa <path> -db <database>
```

### Step 3: Run benchmark and get frontier and freshness results 
```
./HATtrickBench -frontier -dsn <dsn> -usr <user> -pwd <password> -wd <wd> -td <td> -db <database> -t <t>
```

## Visualization of the results 

You can use the `Results.ipynb` notebook to visualize your results after the execution of the benchmark.
