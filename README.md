# Hello Database!

this is a simple database that implemented in c.

## Install

---

you need to have c compiler in your computer

```
$ make
$ make install
```

## Usage

---

```
$ hdb init mydb
```

will create a folder called mydb and you can cd into it to continue.

there are some commons to use:
* create
* select
* insert
* delete
* search

### create

```
$ hdb create table1{column1:int,column2:string}
```

int this common, we create a table that store two columns of data:
the first store int as data type, the second store string as data type

you can decide what your column or table named, ex:

```
$ hdb create 2022_A_building{name:string,age:int}
```

### select
### insert
### delete
### search

## TODO

---

since this is a small project for school, we're probably not doing future update for this program.

