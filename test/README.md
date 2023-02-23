# UNIT TESTS

### WARNING THE MAKEFILE IS USING INCREMENTAL COMPILATION AND IN SOME SITUATIONS CANNOT UPDATE THE OBJECTS FILES. IF A WEIRD BEHAVIOR IS NOTICED, THEN RUN `MAKE CLEAN`
## HOW DO I CONFIGURE THE SYSTEM THE FIRST TIME?

-Please, check the folder cpputest has the cpputest repository. If the cpputest is empty, then clone this [repository](https://github.com/aralce/cpputest) on this folder.  
  
-If It is the first time you set up the test enviroment run the followings commands opening a terminal on this folder (LINUX ONLY)  
```
$ cd cpputest  
$ autoreconf . -i  
$ ./configure  
$ make tdd  
$ export CPPUTEST_HOME=$(pwd)  
```
***NOTE:*** The commands could you to have installed GNU autotools. In this case run `$ apt-get install automake autoconf libtool`

## HOW DO I RUN THE TESTS?
- If the system is configured, then run the following command on your terminal `$ make`. You have to run the command from this folder.

  
***NOTE***: If you run a test after generate a coverage report, you can see warnings messages. To avoid warnings you need to run: `$ make clean`
  
### makefile  
Have the recipe to run the tests.

## Important
- The system was develop under linux and the instructions target a linux system.

-----------------------------------------------------------------------------------------
# [BACK TO MAIN README](https://github.com/DroneBois/SmartBattery/blob/develop/README.md)
