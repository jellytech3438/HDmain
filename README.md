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
* insert
* update
* delete
* search
* rename

### create

```
$ hdb create table table_name -c column1:int column2:string
```

int this common, we create a table that store two columns of data:
the first store int as data type, the second store string as data type

you can decide what your column or table named, ex:

```
$ hdb create 2022_A_building -c {0..9}":int" "1"{0..9}":string"
```

### update
### insert
### delete
### search
### rename

## TODO

---

since this is a small project for school, we're probably not doing future update for this program.

