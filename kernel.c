
void printString(char* a);
void printChar(char b);
void readString(char* line);
void readSector(char* buffer,int sector); 
int div(int a, int b);
int mod(int a, int b);
void readFile(char* filename, char* buffer);
void handleInterrupt21(int ax, int bx, int cx, int dx);
void executeProgram(char* filename, int segment);
void terminate();
void writeSector(char *buffer, int sector);
void deleteFile(char *fileName);
void writeFile(char* name, char* buffer, int numberOfSectors);

int main(){
	char line[80];
	char buffer[13312];
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = '\0';
	makeInterrupt21();
	interrupt(0x21, 4, "shell\0", 0x2000, 0); 
	while(1);
	return 0;
}

void printString(char* a){
	int i = 0;
	while(a[i] != '\0'){
		interrupt(0x10,0xe*256+a[i],0,0,0);
		i++;
	}
	return;
}

void printChar(char b){
	interrupt(0x10,0xe*256+b,0,0,0);
	return;
}

void readString(char* line){
	int i = 0;
	while(i < 80){
		line[i] = interrupt(0x16,0,0,0,0);
		if(line[i] == '\b' && i != 0){
			line[i] = ' ';
			i--;
			continue;
		}
		else if(line[i] == '\r'){
			line[i+1] = '\n';
			line[i+2] = '\0';
			interrupt(0x10,0xe*256+'\0',0,0,0);
			interrupt(0x10,0xe*256+'\n',0,0,0);
			interrupt(0x10,0xe*256+'\r',0,0,0);
			return;
		}
		interrupt(0x10,0xe*256+line[i],0,0,0);
		i++;
}
	return;
}

void readSector(char* buffer,int sector){
	int rS = mod(sector,18) + 1;
	int head = mod(div(sector,18),2);
	int track = div(sector,36);
	interrupt(0x13,2*256+1,buffer,track*256+rS,head*256);
	}

int mod(int a, int b){
	while(a >= b){
		a = a - b;
	}
	return a;
}
int div(int a, int b){
	int q=0;
	while( q * b <= a){
		q = q + 1;
	}
	return q - 1;
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	
	if(ax == 0){
		printString(bx);
		return;
	}
	else if(ax == 1){
		readString(bx);
		return;
	}
	else if(ax == 2){
		readSector(bx,cx);
		return;
	}
	else if(ax == 3){
		readFile(bx,cx);
		return;
	}
	else if(ax == 4){
		executeProgram(bx,cx);
		return;
	}
	else if(ax == 5){
		terminate();
		return;
	}
	else if (ax == 6){
	    	writeSector((char *) bx, cx);
		return;
	} 
	else if (ax == 7){
	    deleteFile((char *) bx);
	     return;
	  } 
	else if (ax == 8){
	    writeFile((char *) bx, (char *) cx, dx);
	    return;
	  } 
	else{
		printString("Error: Invalid value of AX");
}
}

void readFile(char* filename, char* buffer){
		char directory[512];
		int match = 0;
		int i = 0;
		int j = 0;
		int seg = 6;
		int bufferplus=0;
		readSector(directory,2); 
		for(i = 0;i<512;i+=32){
			for(j = 0; j<6;j++){
				if(filename[j] == directory[i+j]){
				
				match++;
					if(match == 6){
						printString("The kernel has found the file! \0");
						while(seg !=26){
							readSector(buffer + bufferplus*512,directory[seg+i]);
							bufferplus++;
							seg++;
							
						}
						return;
					}
		
				}
			}
				match = 0;
		}	
		return;
}
void executeProgram(char* filename, int segment){
	char buffer[13312];
	int i = 0;
	readFile(filename,buffer);
	while(i!=13312){
		putInMemory(segment,i,buffer[i]);
		i++;
	}
	launchProgram(segment);
	return;
}
void terminate(){
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = '\0';
	interrupt(0x21,4,shell,0x2000);
	return;
}
void writeSector(char *buffer, int sector) {
	  interrupt(0x13, 3 * 256 + 1,
	    buffer,
	    div(sector, 36) * 256 + mod(sector, 18) + 1,
	    mod(div(sector, 18), 2) * 256 + 0);
	}
	
void deleteFile(char *fileName) {
	  char directory[512], map[512];
	  int i, j, sector;
	  readSector(map, 1);
	  readSector(directory, 2);
	
	  for (i = 0; i < 512; i += 32) {
	    for (j = 0; j < 6; j++) {
	      if (fileName[j] != directory[i + j]) {
	        break;
	      }
	    }
	    if (j == 6) {
	      for (j = 0; j < 26; j++) {
	        sector = (int) directory[i + 6 + j];
	        if (sector == 0) {
	          break;
	        }
	        map[sector] = 0x00;
	      }
	      directory[i] = 0x00;
	      break;
	    }
	  }
	
	  writeSector(map, 1);
	  writeSector(directory, 2);
	}
	
void writeFile(char* name, char* buffer, int numberOfSectors) {
	  char directory[512], map[512];
	  int i, j, sector, sectorsWritten, directoryIndex;
	  readSector(map, 1);
	  readSector(directory, 2);
	
	  for (i = 0; i < 512; i += 32) {
	    if (directory[i] == 0x00) {
	      directoryIndex = i;
	      for (j = 0; j < 6; j++) {
	        directory[i + j] = name[j];
	      }
	      break;
	    }
	  }
	
	  sectorsWritten = 0;
	  for (i = 0; i < 512 && sectorsWritten < numberOfSectors; i += 1) {
	    if (map[i] == 0x00) {
	      map[i] = 0xFF;
	      directory[directoryIndex + 6 + sectorsWritten] = i;
	      writeSector(buffer + sectorsWritten * 512, i);
	      sectorsWritten++;
	    }
	  }
	  for (i = sectorsWritten + 6; i < 32; i++) {
	    directory[directoryIndex + i] = 0x00;
	  }
	
	  writeSector(map, 1);
	  writeSector(directory, 2);
	}



