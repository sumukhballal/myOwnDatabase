# CS525 : Advanced Database Organization
## Assignment 1:

### Storage Manager: Implement a storage manager that allows read/writing of blocks to/from a file on disk.
===================================================================
### Contents :
* MakeFile : The make file usesd to create a executable test_assign1.exe
* test_assign1.c : Where all the test cases are written
* All required header files and their respective executable code in respective .c files.
===================================================================
### Steps to run the code:
* open the command prompt and directs its directory where the MakeFile is located.
* Run the make command on the MakeFil which will create the .o file along with an executable test_assign1.exe file.
* Run the test_assign1.exe file by either double tap it or by command line execution.
===================================================================
### Nested content and flow of execution:
------------------------------------------------------------------- 
 * test_assign1.c
     * All header files are included
      storage_mgr.h;dberror.h;test_helper.h 
      (each header file has his respective .c file)
         - storage_mgr.c
             Functions included:
             - initStorageManager()  |
             - createpageFile()      |
             - openPageFile()        | //manipulating page files
             - closePageFile()       |
             - destroyPageFile()     |
             
             - readBlock()           |
             - getBlockPos()          |
             - readFirstBlock()      |
             - readPreviousBlock()   | // reading blocks from disc
             - readCurrentBlock()    |
             - readNextBlock()       |
             - readLatestBlock()     |
             
             - writeBlock()          |
             - writeCurrentBlock()   | // writing blocks to a page file
             - appendEmptyBlock()    |
             - ensureCapacity()      |
        - initStorageManager()
        - testCreateOpenClose()
        - testSinglePageContent()
-------------------------------------------------------------------