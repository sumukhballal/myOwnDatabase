#include "storage_mgr.h"
#include "dberror.h"
#include <stdlib.h>
#include <stdio.h>
//  File Header
SM_FileHandle fHeader;
// File pointer , keeps pointing to where the file is stored on disc.
FILE *fPtr;
/* manipulating page files */
void initStorageManager (void)
{
    printf("Storage Manager started.\n");
}
//Need to create a page file here
RC createPageFile (char *fileName)
{
    //create page file from file name

    //set page size memory in heap
    SM_PageHandle pHeader=(SM_PageHandle)calloc(PAGE_SIZE,sizeof(char));
    //Open file and create it since we run it in w+ mode
    fPtr=fopen(fileName,"w+");
    if(fPtr == NULL)
        return RC_FILE_HANDLE_NOT_INIT;
    //Assign FileHeader some initial values
    fHeader.fileName=fileName;
    fHeader.curPagePos=0;
    fHeader.totalNumPages=1;

    fwrite(&fHeader, sizeof(struct SM_FileHandle), 1, fPtr);
    if(fwrite(pHeader,sizeof(char),PAGE_SIZE,fPtr)!=0)
        printf("Page created. \n");
    else
        return RC_WRITE_FAILED;
    
    //write
    //fwrite(pHeader,sizeof(char),PAGE_SIZE,fPtr);

    //Close the file and deallocate memory
    fclose(fPtr);
    fPtr=NULL;
    free(pHeader);
    return RC_OK;
}


RC openPageFile (char *fileName, SM_FileHandle *fHandle)
{
    fPtr=fopen(fileName,"r+");
    if(fPtr==NULL)
        return RC_FILE_NOT_FOUND;
        
    fread(&fHeader,sizeof(struct SM_FileHandle),1,fPtr);

    	fHandle->curPagePos = fHeader.curPagePos;
		fHandle->fileName = fHeader.fileName;
		fHandle->mgmtInfo = fHeader.mgmtInfo;
		fHandle->totalNumPages = fHeader.totalNumPages;
    printf("Page opened. \n");
    return RC_OK;
}
RC closePageFile (SM_FileHandle *fHandle){
    fclose(fPtr);
    fPtr=NULL;
    printf("Page closed. \n");
    return RC_OK;
}
RC destroyPageFile (char *fileName){
    remove(fileName);
    printf("Page destroyed. \n");
    return RC_OK;
}

/* READING */

RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    // Lets read after opening file
    if(openPageFile(fHandle->fileName,fHandle)==RC_OK)
    {

        fPtr=fopen(fHandle->fileName,"r+"); // intialize the file pointer
        fseek(fPtr,0,SEEK_SET); // Move the file pointer to the start of the file
        fread(&fHeader,sizeof(SM_FileHandle),1,fPtr); // Read file header
        //fHandle->fileName=fHeader.fileName;
        fHandle->curPagePos=fHeader.curPagePos;
        fHandle->totalNumPages=fHeader.totalNumPages;
        fHandle->mgmtInfo=fHeader.mgmtInfo;

        fseek(fPtr, sizeof(struct SM_FileHandle) + (pageNum * PAGE_SIZE), SEEK_SET);
        fread(memPage,sizeof(char),PAGE_SIZE,fPtr);
        fHandle->curPagePos=pageNum;
        fwrite(fHandle, sizeof(struct SM_FileHandle), 1, fPtr);
        closePageFile(fHandle);
        return RC_OK;
    }
    else
        return RC_FILE_NOT_FOUND;

}
int getBlockPos (SM_FileHandle *fHandle)
{
    fPtr=fopen(fHandle->fileName,"r+");
    fread(&fHeader,sizeof(struct SM_FileHandle),1,fPtr);

    return fHeader.curPagePos;
}
RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(0,fHandle,memPage);
}
RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(getBlockPos(fHandle)-1,fHandle,memPage);
}
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
      return readBlock(getBlockPos(fHandle),fHandle,memPage);
}
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
      return readBlock(getBlockPos(fHandle)+1,fHandle,memPage);
}
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(fHandle->totalNumPages - 1, fHandle, memPage);
}


/* WRITING */

RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    fPtr=fopen(fHandle->fileName,"r+");
    fseek(fPtr,0,SEEK_SET);
    fread(&fHeader,sizeof(SM_FileHandle),1,fPtr);
    fHandle->curPagePos=fHeader.curPagePos;
    fHandle->totalNumPages=fHeader.totalNumPages;
    fHandle->mgmtInfo=fHeader.mgmtInfo;

    fseek(fPtr, sizeof(struct SM_FileHandle) + (pageNum * PAGE_SIZE), SEEK_SET);
    fwrite(memPage,sizeof(char),PAGE_SIZE,fPtr);
    fHandle->curPagePos=pageNum;
    fHandle->totalNumPages = (ftell(fPtr) - sizeof(struct SM_FileHandle)) / PAGE_SIZE;

    // Write updated file handle to file
    fseek(fPtr,0, SEEK_SET);
    fwrite(fHandle,sizeof(SM_FileHandle),1,fPtr);
    return RC_OK;

}
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    //Get current page pos
    int pagepos = getBlockPos(fHandle);
    //RC_OK will only be returned if writing is successful
    return writeBlock(pagepos,fHandle,memPage);
}

RC appendEmptyBlock (SM_FileHandle *fHandle) 
{
    SM_PageHandle emptyPage=(SM_PageHandle)calloc(PAGE_SIZE,sizeof(char)); // creates an empty char pointer
    fPtr=fopen(fHandle->fileName,"r+");
    if((fseek(fPtr,sizeof(struct SM_FileHandle) + (fHandle->totalNumPages * PAGE_SIZE), SEEK_SET)==0))
    {
        fwrite(emptyPage,sizeof(char),PAGE_SIZE,fPtr);
        int totPages=(ftell(fPtr)-sizeof(SM_FileHandle))/PAGE_SIZE;

        fHandle->curPagePos=totPages-1;
        fHandle->totalNumPages=totPages;
        return RC_OK;
    }
    //empty page creation
    return RC_WRITE_FAILED;
}
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
    //check if numberOfPages > TotalNumPages, if true then we need to append page
    if (numberOfPages > fHandle->totalNumPages)
    {
        while (numberOfPages > fHandle->totalNumPages)
        {
            //Append new pages
            appendEmptyBlock(fHandle);
        }
        return RC_OK;
    }
    else
    {
        //no. of pages < total pages so no pages need to be appended
        return RC_OK;
    }
}