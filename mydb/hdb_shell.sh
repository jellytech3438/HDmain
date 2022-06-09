hdb create -t table1 -c c1:int c2:string c3:float
hdb insert table1 values 1 a 1
hdb insert table1 values 2 b 2
hdb insert table1 values 3 c 3
hdb alter table1 drop c1
hdb search table1 where INDEX=0
