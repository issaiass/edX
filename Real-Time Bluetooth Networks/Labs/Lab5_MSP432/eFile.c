// eFile.c
// Runs on either TM4C123 or MSP432
// High-level implementation of the file system implementation.
// Daniel and Jonathan Valvano
// September 13, 2016
#include <stdint.h>
#include "eDisk.h"

uint8_t Buff[512];
uint8_t Directory[256], FAT[256];
int32_t bDirectoryLoaded =0; // 0 means disk on ROM is complete, 1 means RAM version active
// Return the larger of two integers.
int16_t max(int16_t a, int16_t b){
  if(a > b){
    return a;
  }
  return b;
}
// if directory and FAT not loaded,
// bring it into RAM from disk
void MountDirectory(void){ 
// if bDirectoryLoaded is 0, 
//    read disk sector 255 and populate Directory and FAT
//    set bDirectoryLoaded=1
// if bDirectoryLoaded is 1, simply return
// **write this function**
  volatile uint8_t *DirAddr = (volatile uint8_t *)(EDISK_ADDR_MAX - 511); /* address of directory */
	volatile uint8_t *FATAddr = (volatile uint8_t *)(EDISK_ADDR_MAX - 255); /* address of FAT */

//	for (int i = 0; i <= 255; i++) {
//		Directory[i] = 255;
//		FAT[i]       = 255;
//		Buff[i]      = 255;
//	}
	if (bDirectoryLoaded == 0) {
    eDisk_ReadSector(Buff, 255);
	 	for (uint16_t i=0; i < 256; i++) {
			Directory[i] = Buff[i];
			FAT[i] = Buff[i + 256];
		}
		bDirectoryLoaded = 1;
	}
	return;
}

// Return the index of the last sector in the file
// associated with a given starting sector.
// Note: This function will loop forever without returning
// if the file has no end (i.e. the FAT is corrupted).
uint8_t lastsector(uint8_t start){
// **write this function**
// uint8_t m;  //index in FAT
//	
//	
//  if (start == 255) {
//    return 255;
//  }
//  m = FAT[start];
//  while (m != 255) {
// //FATindex "m" as "start", content of FAT location is m, and m points to next FAT index
//    start = m;
//    m = FAT[start];
//  }
//  return start; 
  uint8_t m;  //index in FAT
  
	
	if (start == 255) {
    return 255;
  }
  m = FAT[start];
  while (m != 255) {
 //FATindex "m" as "start", content of FAT location is m, and m points to next FAT index
    start = m;
    m = FAT[start];
  }
  return start; 
}

// Return the index of the first free sector.
// Note: This function will loop forever without returning
// if a file has no end or if (Directory[255] != 255)
// (i.e. the FAT is corrupted).
uint8_t findfreesector(void){
// **write this function**
//uint8_t i = 0;
//uint8_t ls = 0;  
//int16_t fs = -1;

//ls = lastsector(Directory[i]);
//while (ls != 255)
// {
//	fs = max(fs, ls);
//	i++;
//	ls = lastsector(Directory[i]);
// }  
// return fs+1;
  uint8_t i = 0;
  uint8_t ls = 0;  
  int16_t fs = -1;


	ls = lastsector(Directory[i]);
  while (ls != 255) {
	  fs = max(fs, ls);
	  i++;
	  ls = lastsector(Directory[i]);
  }
  return fs+1;
}

// Append a sector index 'n' at the end of file 'num'.
// This helper function is part of OS_File_Append(), which
// should have already verified that there is free space,
// so it always returns 0 (successful).
// Note: This function will loop forever without returning
// if the file has no end (i.e. the FAT is corrupted).
uint8_t appendfat(uint8_t num, uint8_t n){
// **write this function**
//  uint8_t i;
//	uint8_t m;

//	
//	i = Directory[num];
//  if (i != 255) {
//  	m = FAT[i];
//	  while (m != 255) {
//		  i = m;
//		  m = FAT[i];
//	  }
//	  FAT[i] = n;
//    return 0;
//  }
// if (i == 255) {
//   Directory[num] = n;
// }
// return 0; // replace this line
uint8_t i = 0; //content of directory at location num, FAT index number
uint8_t m = 0; //content of FAT at location i, 


  i = Directory[num];
  if (i != 255) {
	  m = FAT[i];
	  while (m != 255) {
      i = m;
		  m = FAT[i];
	  }
	  FAT[i] = n;
    return 0;
  }
  if (i == 255) {
    Directory[num] = n;
  }
  return 0; // replace this line
}

//********OS_File_New*************
// Returns a file number of a new file for writing
// Inputs: none
// Outputs: number of a new file
// Errors: return 255 on failure or disk full
uint8_t OS_File_New(void){
// **write this function**
//  uint8_t i;
//	
//	
//	i = 0;
//	while(Directory[i] != 255) {
//		i++;
//		if(i == 255) {
//			return 255;
//		}
//	}
//	return i;
  uint8_t i = 0;
  if (bDirectoryLoaded == 0) {
   MountDirectory();
  }
  while (Directory[i] != 255) {
	  i++;
	  if ( i == 255) {
		  return 255;
	  }
  }
  return i;
}

//********OS_File_Size*************
// Check the size of this file
// Inputs:  num, 8-bit file number, 0 to 254
// Outputs: 0 if empty, otherwise the number of sectors
// Errors:  none
uint8_t OS_File_Size(uint8_t num){
// **write this function**
  uint8_t next;
  uint8_t count=0;

	
	next = Directory[num];
	if(next == 255) {
    return 0;
  }
  count++;
  while(FAT[next]!=255) {
    next = FAT[next];
    count++;
  }
  return count; 
}

//********OS_File_Append*************
// Save 512 bytes into the file
// Inputs:  num, 8-bit file number, 0 to 254
//          buf, pointer to 512 bytes of data
// Outputs: 0 if successful
// Errors:  255 on failure or disk full
uint8_t OS_File_Append(uint8_t num, uint8_t buf[512]){
// **write this function**
//	uint8_t n;
//	
//	
//	n = findfreesector();
//	if(n == 255) {
//		return 255;
//	}
//	if(eDisk_WriteSector(buf, num) == RES_OK) {
//	  appendfat(num, n);
//	  return 0;		
//	}
//	return RES_ERROR;
  uint8_t n;

	
  if(bDirectoryLoaded == 0) {
    MountDirectory();
	}
  n = findfreesector();
	if (n == 255) {	
   return 255;
	} else {
	 if (eDisk_WriteSector(buf, n) != RES_OK) {
     return 255;
	 }
	 appendfat(num, n);
	 return 0;
 }
}

//********OS_File_Read*************
// Read 512 bytes from the file
// Inputs:  num, 8-bit file number, 0 to 254
//          location, logical address, 0 to 254
//          buf, pointer to 512 empty spaces in RAM
// Outputs: 0 if successful
// Errors:  255 on failure because no data
uint8_t OS_File_Read(uint8_t num, uint8_t location,
                     uint8_t buf[512]){
// **write this function**
//uint8_t sector;
//uint8_t j;
//uint8_t temp;
//		
//temp=Directory[num];											 
//if (temp!=255){
//	sector=FAT[temp];
//	for (j=1;j<location;j++){
//		sector=FAT[sector];
//	}
//	eDisk_ReadSector(buf,sector);
//}	
//  return 255; // replace this line
											 
	uint8_t next = 0;
	uint8_t count = 0;
											 
	next = Directory[num];
	if (num == 255 || location == 255 || next == 255)
		 return 255;
										 
	while(count != location)//cycle through FAT 
	{
			next = FAT[next]; //get next index for FAT
			if (next != 255) //We are not at the end of the file so increment count to match with location
			{
				count++;
			}
			else
				return next;
	}
	return eDisk_ReadSector(buf,next);	//read
}


//********OS_File_Flush*************
// Update working buffers onto the disk
// Power can be removed after calling flush
// Inputs:  none
// Outputs: 0 if success
// Errors:  255 on disk write failure
uint8_t OS_File_Flush(void){
// **write this function**
//	uint16_t i = 0;	
//	for (i = 0; i < 256; i++) {
//   		Buff[i] = Directory[i];
//   		Buff[i+256] = FAT[i];
//	}
//	if (eDisk_WriteSector(Buff, 255) != RES_OK) {
//    return 255;	
//	} else {
//    bDirectoryLoaded = 0;
//	}		
//	return 0; // replace this line
  uint16_t i;

	
	if (bDirectoryLoaded) {	
    for (i = 0; i < 256; i++) {
	    Buff[i] = Directory[i];
	    Buff[i+256] = FAT[i];
    }
    if (eDisk_WriteSector(Buff, 255) != RES_OK) {
	    return 255;
    }
  }
  return 0;
}

//********OS_File_Format*************
// Erase all files and all data
// Inputs:  none
// Outputs: 0 if success
// Errors:  255 on disk write failure
uint8_t OS_File_Format(void){
// call eDiskFormat
// clear bDirectoryLoaded to zero
// **write this function**
//	uint8_t status;

//	
//  status = eDisk_Format();
//  for(int i = 0; i <= 255; i++) {
//    Directory[i] = 255;
//		FAT[i] = 255;
//	}		
//	bDirectoryLoaded = 0;
//  return status; // replace this line
  uint16_t i;

	
	if (eDisk_Format() != RES_OK) {
    return 255;
  }
  for (i=0;i<256;i++) {  //write 0xFF to Directory and FAT
	  Directory[i]=0xFF;  //0-255 for Dir, 256-511 for FAT 
	  FAT[i+256]=0xFF;
  }
  bDirectoryLoaded = 0;
  return 0;
}
