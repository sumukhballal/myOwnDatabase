#include "storage_mgr.h"
#include "dberror.h"

/* manipulating page files */
void initStorageManager (void)
{
    printf("Intialized Storage Manager.");
}
//Need to create a page file here
RC createPageFile (char *fileName)
{
    // Where does fileName come from? storage_manager.h? 
    
}
RC openPageFile (char *fileName, SM_FileHandle *fHandle);
RC closePageFile (SM_FileHandle *fHandle);
RC destroyPageFile (char *fileName);