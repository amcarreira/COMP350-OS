
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


int main(){
//    char array[512];
    char line[80];
    char buffer[13312];
    makeInterrupt21();
//    interrupt(0x21,3,"messag\0",buffer,0);
//    interrupt(0x21,0,buffer,0,0);
//    interrupt(0x21, 4, "tstprg\0", 0x2000, 0);
    interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);
    interrupt(0x21, 4, "shell\0", 0x2000, 0);

//    interrupt(0x21,1,line,0,0);
//     interrupt(0x21,0,line,0,0);
//    while(1);
//     interrupt(0x21,2,buffer,30,0);
//     interrupt(0x21,0,buffer,0,0);
//    printString("Enter a line: \0");
//    readString(line);
//    printString(line);
//    readSector(buffer,30);
//    printString(buffer);
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
    else{
        printString("Error: Invalid value of AX");
}
}

void readFile(char* filename, char* buffer){
        char directory[512];
        int instance = 0;
        int i = 0;
        int j = 0;
        int segment = 6;
        int bufferplus=0;
        readSector(directory,2);
        for(i = 0;i<512;i+=32){
            for(j = 0; j<6;j++){
                if(filename[j] == directory[i+j]){
               
                instance++;
                    if(instance == 6){
                        printString("The file has been found \0");
                        while(segment !=26){
                            readSector(buffer + bufferplus*512,directory[segment+i]);
                            bufferplus++;
                            segment++;
                           
                        }
                        return;
                    }
       
                }
            }
                instance = 0;
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
    //while(1);
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
