# CSCI3170project
Develop with Intellij idea IDE

File -> project structure -> Library -> add "mysql-connector-java-8.0.27.jar" to lib

# Initializing sample data
Choose administor -> create -> load -> input "sample_data" -> DONE!

# Database info.:
linux ac: db004
pw: bzt1jskf

using db48 (group no.)
mysql username: Group48 
pw: 54321

# How to run the program:
Assume that the working folder is called "csci3170"

1) Compile to .class 

javac -d . "PROJECT_LOCATION/csci3170/CSCI3170project/src/*.java

2) Run

java -classpath "PROJECT_LOCATION/csci3170:PROJECT_LOCATION/csci3170/CSCI3170project/src/CSCI3170proj/mysql-connector-java-8.0.27.jar" CSCI3170proj.Main


(separator is different in windows(;) and linux(:)
