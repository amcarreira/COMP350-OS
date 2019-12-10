int main(){
    int i = 0;
    while(1){
    char buffer[13312];
    char buffer2[13312];
    char filename[7];
    interrupt(0x21,0,"SHELL: ",0,0);
    interrupt(0x21,1,buffer,0);
    if(buffer[0] == 't' && buffer[1] == 'y' && buffer[2] == 'p' && buffer[3] == 'e'){
        filename[0] = buffer[5];
        filename[1] = buffer[6];
        filename[2] = buffer[7];
        filename[3] = buffer[8];
        filename[4] = buffer[9];
        filename[5] = buffer[10];
        filename[6] = '\0';
        interrupt(0x21,3,filename,buffer2,0);
        interrupt(0x21,0,buffer2,0);
       
    }
    else if(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'e' && buffer[3] == 'c'){
        filename[0] = buffer[5];
        filename[1] = buffer[6];
        filename[2] = buffer[7];
        filename[3] = buffer[8];
        filename[4] = buffer[9];
        filename[5] = buffer[10];
        filename[6] = '\0';
        interrupt(0x21, 4,filename, 0x2000, 0);
        }
    else{
        interrupt(0x21,0,"Bad Command, Try Again\0",0);
    }
}
    return 0;
}
