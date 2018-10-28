void prnt(char buff);
void clear(char*,int);

main(){
	char buffer[180];
	prnt("\r\n");
	prnt("shell> ");
	interrupt(0x21,1,buffer,0,0);	
	prnt(buffer);
	clear(buffer,180);
	//while(1);
}

void prnt(char* buff){
	interrupt(0x21,0,buff,0,0);

}
void clear(char* buff, int len){
	int i;
	for(i=0;i<len;i++){
		buff[i] = 0x0;
	}
}
