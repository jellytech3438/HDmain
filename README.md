# Hello Database!

This is a simple database that implemented in c. We implement some sql language to help operate with the database. This project is still under developement, so if you find any bugs, please help us report it.

## Install

> You need to have c compiler and make in your os.

```
$ make
$ make install
```

## Cookbook

For the tutorial and other informations, check out the [hdb cookbook](https://710242.github.io/HDcookbook/)

## Accomplish

> Below are the finish part of implementing a databse system. We will add more function in the future version.

### database operation
- [x] init
- [ ] inner-join
- [ ] left-join
- [ ] right-join
- [ ] full-join
- [ ] self-join
- [ ] ...

### table operation
- [x] create
- [x] alter
  - [x] add
  - [x] drop
  - [x] modify
  - [ ] ...
- [x] rename
- [ ] ...

### data operation
- [x] insert
- [x] update
- [x] delete
- [x] search
- [ ] ...

### other operation
- [x] where
  - [x] NOT
  - [x] AND/OR
  - [x] INDEX
  - [ ] ANY
  - [x] ALL
  - [ ] MIN
  - [ ] MAX
- [ ] ...

## TODO

* io_uring

I have tried to rewrite our File I/O into io_uring through liburing api, but i keep failing since the write size of the structure. I hope i can complete this in the future.

* history

There is a file will be created while initialize a new database called table_history. This is alse a future update for recording all the operation in the current database.

* more  functions

As those commands that not been clicked in the Accomplish part, i will complete those functions in the future, hopefully?
