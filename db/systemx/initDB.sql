DROP DATABASE hatrickbench;
  
CREATE DATABASE hatrickbench;
ALTER DATABASE hatrickbench
SET MEMORY_OPTIMIZED_ELEVATE_TO_SNAPSHOT = ON;

ALTER DATABASE hatrickbench
ADD FILEGROUP hatrickbench_filegroup CONTAINS MEMORY_OPTIMIZED_DATA;

ALTER DATABASE hatrickbench
ADD FILE
(
        name='hatrickbench_dataContainer',
        filename='/mnt/data/sql/data/hatrickbench_dataContainer'
)
TO FILEGROUP hatrickbench_filegroup
