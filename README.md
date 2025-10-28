# WHITE
# SNN - Based Malware CLassification using Program Trace

> PreRequisite : <b>bpftrace</b> 
<br>For logging system calls of the program and building a program trace 

>PreRequisite : <b>BootStrap</b><br>
For building fake directory system directory for isolation of program to from main directory <br><br>
<b>Do not Use Command<br> sudo apt update / upgrade</b> <br>
it can take significat amount of time to update insted use specific installation commands <br><br>
<b>⚠️ installation command</b> <br>
will only work for the fake directory if the progam is running and it is mounted, main system terminal might cause installation in the main system files!




SETUP WHITE
```
mkdir build
cd build
cmake ..


#Windows->
cmake --build .
cd debug
./WHITE

#Linux->
make
sudo ./WHITE
```

