main(){
  int i;
  int j;
  int k;
  enableInterrupts();
  for(i=0; i<100;i++){
    interrupt(0x21,0,"OS\n\0",0,0);
    for(j=0;j<1000;j++){
      for(k=0;k<1000;k++){

      }
    }
  }
}
