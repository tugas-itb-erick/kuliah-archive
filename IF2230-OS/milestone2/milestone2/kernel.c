/* Operating System
   Copyright 2013 Meredith Myers

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

void printString(char*);
void readString(char*);
void readSector(char* buffer, int sector);
void writeSector(char* buffer, int sector);
int mod(int a, int b);
int div(int a, int b);
void handleInterrupt21 (int AX, int BX, int CX, int DX);
void readFile(int* fileName,int* buffer);
void printChar(char ch);
void prtInt(int i);
int strComp(int* buffer, char* fileName);
void executeProgram(char* name, int segment); 
void terminate();
void deleteFile(char* name);
void writeFile(int* name, int* buffer, int numberOfSectors);
void getDirectory();
void clear(char*,int);

main(){
	int i;
	char buff[13312];
	char b[512];
	makeInterrupt21();

	interrupt(0x21, 4, "shell\0", 0x2000, 0);	

    while(1){

    }
} 

void handleInterrupt21 (int AX, int BX, int CX, int DX){
		int ah;
	
		ah = AX >> 8;

		if (ah == 9){
			interrupt(0x10, 0xe*256+'H', 0, 0, 0);
			interrupt(0x10, 0xe*256+'e', 0, 0, 0);
			interrupt(0x10, 0xe*256+'l', 0, 0, 0);
			interrupt(0x10, 0xe*256+'l', 0, 0, 0);
			interrupt(0x10, 0xe*256+'o', 0, 0, 0);
			interrupt(0x10, 0xe*256+' ', 0, 0, 0);
			interrupt(0x10, 0xe*256+'W', 0, 0, 0);
			interrupt(0x10, 0xe*256+'o', 0, 0, 0);
			interrupt(0x10, 0xe*256+'r', 0, 0, 0);
			interrupt(0x10, 0xe*256+'l', 0, 0, 0);
			interrupt(0x10, 0xe*256+'d', 0, 0, 0);
			interrupt(0x10, 0xe*256+'!', 0, 0, 0);
		}
		else if (ah == 0x4C){
			terminate();
		}
	
        if (AX == 0){
            printString(BX);
        }
        else if (AX ==1){
            readString(BX);
        }
        else if(AX == 2){
			/* Buffer, sector */
            readSector(BX,30);
        }
		else if (AX == 3){
			/* Filename, buffer*/
			readFile(BX, CX);
		}
		else if (AX == 4){
			/* Program name, segment number */
			executeProgram(BX,CX);
		}
		else if (AX == 5){
			terminate();
		}
		else if (AX == 6){
			/* BX - address of char array, CX- sector number*/
			writeSector(BX,CX);
		}
		else if (AX == 7){
			deleteFile(BX);
		}
		else if (AX == 8){
			/* BX = address of character array holding the file name, CX = address of character array holding the file to be written, DX = number of sectors */
			writeFile(BX, CX, DX);
		}
		else if (AX == 9){
			getDirectory();
		}
		else {
            printString("Invalid interrupt!\0");
        }
}


void executeProgram(char* name, int segment){ 
	int i;
	int address;
	char readingBuffer[13312];

	readFile(name, readingBuffer);

	for (i=0; i<13312;i++){
		putInMemory(segment,i,readingBuffer[i]);
	} 

	launchProgram(segment);

}

void terminate(){
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = 0x0;
	interrupt(0x21,4,shell,0x2000,0);
}

void getDirectory(){
	char buff[512];
	char dirBuff[512];
	int i,j;
	int index = 0;
	for(i=0;i<512;i++){
		buff[i] = 0x0;
		dirBuff[i]=0x0;
	}
	/* Read in the directory sector */
    readSector(dirBuff, 2); 
	/* Look through the directory for files */
	for(i=0;i<16;i++){
		 /* If there is a file at the location */
		if (dirBuff[32*i] != 0x0){
			/* Check the first characters */
			for (j=0; j < 6; j++){
				buff[index] = dirBuff[j+32*i];
				index++;
			}
			/* Add some formatting */
			buff[index] = '\r';
			index++;
			buff[index] = '\n';
			index++;
		}
	}
	for(i=0;i<512;i++){
		printChar(buff[i]);
	}
	return;
	
}

/* BX = address of character array holding the file name, CX = address of character array holding the file to be written, DX = number of sectors */
void writeFile(char* name,char* buffer, int numberOfSectors) {
	char map[512];
	char directory[512];
	int directoryLine,j,k, index, diff;
	int nameLen = 0;
	int sectorNum;
	char subBuff[512];
	int iterator = 0;
	int foundFree = 0;
	int nameInts[7];
	int i,h;
	int kVal;

	/*1.Load the Map and Directory sectors into buffers*/
	readSector(map,1);
	readSector(directory,2);
	
	/*2.Find a free directory entry (one that begins with 0x00)*/
     	for (directoryLine = 0; directoryLine < 16; directoryLine++){
		 /* If there isnt a file at the location */
		if (directory[32*directoryLine] == 0x00){
			foundFree = 1;
			break;
		}
	}
	if (foundFree == 0){
		printString("Didn't find empty location for file.");
		return;
	}

	/*3.Copy the name to that directory entry.  If the name is less than 6 bytes, fill in the remaining bytes with 0x00*/
	/* Get the name length */	
	while(name[nameLen] != '\0' && name[nameLen] != 0x0){
		nameLen++;
	}
	/* Write in the name*/
	for (j=0;j<nameLen;j++){
		directory[32*directoryLine+j] = name[j];
	}
	/* Check if the file name is less than 6 chars. If so, fill the remainder with 0x0s */
	if (nameLen < 6){
		diff = 6-nameLen;
		for(j=0;j<diff;j++){
			index = j+nameLen;
			directory[32*directoryLine+index] = 0x0;
		}
	}

	/*4.For each sector making up the file:*/
	for (k = 0; k < numberOfSectors; k++){

		/*5.Find a free sector by searching through the Map for a 0x00*/
		sectorNum = 0;
		while(map[sectorNum] != 0x0){
			sectorNum++;
		}
		if (sectorNum==26)
		{
			printString("Not enough space in directory entry for file\n");
			return;
		}
		/*6.Set that sector to 0xFF in the Map*/
		map[sectorNum] = 0xFF;
		/*7.Add that sector number to the file's directory entry*/
		directory[32*directoryLine+6+k] = sectorNum;
		/*8.Write 512 bytes from the buffer holding the file to that sector*/
		for(j=0;j<512;j++){
			kVal = k+1;
			subBuff[j] = buffer[j*kVal];
		}
		writeSector(subBuff,sectorNum);
	}
	/*9.Fill in the remaining bytes in the directory entry to 0x00*/
	/*for(j=0;j<32-numberOfSectors;j++){*/
	
	/*10.Write the Map and Directory sectors back to the disk*/
	writeSector(map,1);
	writeSector(directory,2);
}

void deleteFile(char* name){
	char map[512];
	char directory[512];
	int sectors[27];
	int sectorCount = 0;
	int i, j, k, fileFound, index;
	int sectNum;
	int sector;
	
	/*1.Load the Directory and Map to 512 byte character arrays*/
	readSector(map,1);
	readSector(directory,2);


	/*2.Search through the directory and try to find the file name.*/
	fileFound = strComp(directory,name);

	if (fileFound!=0){

		/*3. Clear the directory. Set the first byte of the file name to 0x00.*/
		
		for(i=0;i<6;i++){
			directory[fileFound*32+i] = 0x00;
		}
		index = fileFound*32+6;
		for (j=0;j<26;j++){
		/*while(directory[index]!=0x0){*/
			sectors[j] = directory[index+j];
			directory[index+j] = 0x00;
			sectorCount++;
			/*index++;*/
		
		}
		sectors[26] = 0x0;

		/*4.Step through the sectors numbers listed as belonging to the file.  For each sector, set the corresponding Map byte to 0x00.  For example, 
		 if sector 7 belongs to the file, set the 7th Map byte to 0x00 (actually you should set the 8th, since the Map starts at sector 0).*/


		for(k=0;k<sectorCount;k++){
			sector = sectors[k];
			if(sector == 0){
				break;
			}

			map[sector] = 0x00;

		}
	}
	else{
		printString("File not found!");
	}
	
	/*5.Write the character arrays holding the Directory and Map back to their appropriate sectors.*/
	writeSector(map,1);
	writeSector(directory,2);
}

void readFile(char* fileName,char* buffer){ 
    int fileFound;
    int nameCt = 0;
	int index, k,h;
	int sectors[27];
	int j = 0;
	int i;
	int buffAddress = 0;
	

    /* Read in the directory sector */
    readSector(buffer, 2);  

    /* Try to find the file name */
	fileFound = strComp(buffer,fileName);

	if (fileFound!=0){

		/* 1.Using the sector numbers in the directory, load the file, sector by sector, into the buffer array.
			You should add 512 to the buffer address every time you call readSector*/
		/* Get the line that the file info is on from the buffer */
		index = fileFound*32+6;
		for (j=0;j<26;j++){
			sectors[j] = buffer[index+j];
			
		}

		sectors[26] = 0;
		k = 0;
		while(sectors[k]!=0x0){
			readSector(buffer+buffAddress,sectors[k]);
			buffAddress += 512;
			k++;
		}
		
	}
	else{
		printString("File Not Found!");
		return;
	}		

}
/* BX - address of char array, CX- sector number*/
void writeSector(char* buffer, int sector){
	
       /* relative sector = ( sector MOD 18 ) + 1*/
        int relSector = mod(sector,18) + 1;
        /*head = ( sector / 18 ) MOD 2		(this is integer division, so the result should be rounded down)*/
        int op = div(sector,18);
        int head = mod(op,2);
        /*track = ( sector / 36 )*/
        int track = div(sector,36);
        /* call interrupt 0x13 */
        interrupt(0x13,3*256+1,buffer,track*256+relSector,head*256+0);
}

void readSector(char* buffer, int sector){ 

       /* relative sector = ( sector MOD 18 ) + 1*/
        int relSector = mod(sector,18) + 1;
        /*head = ( sector / 18 ) MOD 2		(this is integer division, so the result should be rounded down)*/
        int op = div(sector,18);
        int head = mod(op,2);
        /*track = ( sector / 36 )*/
        int track = div(sector,36);
        /* call interrupt 0x13 */
        interrupt(0x13,2*256+1,buffer,track*256+relSector,head*256+0);


}

int strComp(char* buffer, char* fileName){ 
	int i, j;

	int checkFound = 0;


     for (i = 0; i < 16; i++)
 		{
		 /* If there is a file at the location */
		if (buffer[32*i] != 0x0){
			/* Check the first characters */
			for (j=0; j < 6; j++){
				/* This is a match */
				/*printString("Checking: ");
				printChar(buffer[j+32*i]);
				printString(" with ");
				printChar(fileName[j]);*/

				if (buffer[j+32*1] == 0x0 || buffer[j+32*1] == '\r' || buffer[j+32*1] == '\n' || fileName[j] == 0x0 || fileName[j] == '\r' || fileName[j] == '\n'){
					break;
				}
				else if (buffer[j+32*i] == fileName[j]){
					checkFound = 1;	
				}
				else {
					checkFound = 0;
					break;
				}
				
			}
		 	
			if (checkFound == 1){

				 return i;
			}
			else{
				/*printString("Next check");*/
			
			}
		}
		}
			 if (checkFound == 0){
				 for (i=0;i<13312;i++){
					buffer[i] = 0x0;
				 }
				
				 
				return 0;
			 }

	 

}

int mod(int a, int b){
    while(a >= b){
        a = a - b;
    }
    return a;
}

int div(int a, int b){
    int q = 0;
    while(q*b <=a){
        q = q+1;
    }
    return q-1;

}

void readString(char* buff){
    int dashn = 0xa;
    int endStr = 0x0;
    int enter = 0xd;
    int backsp = 0x8;
    int dashr = 0xd;
    int loop = 1;
    int count = 2;
    buff[0] = dashr;
    buff[1] = dashn;
    while(loop){
            /* Call interrupt 0x16 */
            /* interrupt #, AX, BX, CX, DX */
            int ascii = interrupt(0x16,0,0,0,0);
            if (ascii == enter){              
				buff[count] = 0x0;
                buff[count+1] = dashr;
                buff[count+2] = dashn;
                return;
            }
            else if (ascii == backsp){
                if (count > 1){
                    buff[count] = 0x0;
                    count--;
                    interrupt(0x10,0xe*256+0x8,0,0,0);
                    count++;
                    interrupt(0x10,0xe*256+0x0,0,0,0);
                    count--;
                    interrupt(0x10,0xe*256+0x8,0,0,0);
                    
               }
            }
            else{
                buff[count] = ascii;
                interrupt(0x10, 0xe*256+ascii, 0, 0, 0);
                count++;
            }     
    }
}
    
void printString(char* chars){

   
    int i = 0;
    while(chars[i] != '\0'){
            int ch = chars[i];
            /* interrupt #, AX, BX, CX, DX */
		        interrupt(0x10, 0xe*256+ch, 0, 0, 0);
            i++;
    }


}
void clear(char* buff, int len){
	int i;
	for(i=0;i<len;i++){
		buff[i] = 0x0;
	}
}
void printChar(char ch){
	char* chars[2];
	chars[0] = ch;
	chars[1] = '\0';
	printString(chars);
}
void prtInt(int i){
	int* chars[2];
	chars[0] = i;
	chars[1] = '\0';
	printString(chars);
	
}


