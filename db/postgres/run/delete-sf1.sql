\c hatrickbench;
set search_path to hat;
delete from hat.lineorder where lo_orderkey>1500000;
delete from hat.history where h_orderkey>1500000;
