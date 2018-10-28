/* Kelompok 4 */

main(void){
  clearscreen();
  // helloworld();
  putLogo();
  printString("Erick Wijaya   13515057",2,2,0x02);
  printString("Veren Iliana   13515078",3,2,0x04);
  printString("Zacki Zulfikar 13515147",4,2,0x06);
}

printString(char *c, int baris, int kolom, int warna){
  /* Menulis string pada baris dan kolom dengan warna sesuai parameter fungsi */
  int i, b, alamat;
  i = 0;

  alamat = (baris-1)*80*2 + 2*kolom;
  while(c[i] != '\0'){
    b = alamat;
    putInMemory(0xB000, b+0x8000, c[i]);
    putInMemory(0xB000, b+0x8001, warna);
    i = i + 1;
    alamat = alamat + 2;
  }
}

helloworld(void){
  /* Menulis hello pada baris dan kolom nol warna putih */
  char *hw = "Hello, World";
  int alamat = 0x8000;
  int i = 0;

  for(i=0; hw[i]!='\0'; i++){
    putInMemory(0xB000, alamat, hw[i]);
    putInMemory(0xB000, alamat+0x01, 0x0F);
    alamat = alamat + 2;
  }
}

clearscreen(void){
  /* Membersihkan layar dari tulisan */
  int i=0;
  for(i=0; i<25; i++)
    printString("                                                                                          ",i,0,0x02);
}

putLogo(void){
              printString("                                                                               ",0 ,0,0x02);
              printString("                                                           ___                 ",1 ,0,0x02);
              printString("                                                        ,o88888                ",2 ,0,0x02);
              printString("                                                     ,o8888888'                ",3 ,0,0x02);
              printString("                               ,:o:o:oooo.        ,8O88Pd8888'                 ",4 ,0,0x02);
              printString("                           ,.::.::o:ooooOoOoO. ,oO8O8Pd888''                   ",5 ,0,0x02);
              printString("                         ,.:.::o:ooOoOoOO8O8OOo.8OOPd8O8O'                     ",6 ,0,0x02);
              printString("                        , ..:.::o:ooOoOOOO8OOOOo.FdO8O8'                       ",7 ,0,0x02);
              printString("                       , ..:.::o:ooOoOO8O888O8O,COCOO'                         ",8 ,0,0x02);
              printString("                      , . ..:.::o:ooOoOOOO8OOOOCOCO'                           ",9 ,0,0x02);
              printString("                       . ..:.::o:ooOoOoOO8O8OCCCC'o                            ",10,0,0x02);
              printString("                          . ..:.::o:ooooOoCoCCC'o:o                            ",11,0,0x02);
              printString("                          . ..:.::o:o:,cooooCo'oo:o:                           ",12,0,0x02);
              printString("                       `   . . ..:.:cocoooo''o:o:::'                           ",13,0,0x02);
              printString("                       .`   . ..::ccccoc''o:o:o:::'                            ",14,0,0x02);
              printString("                      :.:.    ,c:cccc'':.:.:.:.:.'                             ",15,0,0x02);
              printString("                    ..:.:''`::::c:''..:.:.:.:.:.'                              ",16,0,0x02);
              printString("                  ...:.'.:.::::''    . . . . .'                                ",17,0,0x02);
              printString("                 .. . ....:.'' `   .  . . ''                                   ",18,0,0x02);
              printString("               . . . ....''                                                    ",19,0,0x02);
              printString("               .. . .''       _______ ________  _____  _  __   ________        ",20,0,0x02);
              printString("              .              / __/ _ /_  __/ / / / _ \\/ |/ /  /__ / __/       ",21,0,0x02);
              printString("                            _\\ \\/ __ |/ / / /_/ / , _/    /  //_//\\ \\        ",22,0,0x02);
              printString("                           /___/_/ |_/_/  \\____/_/|_/_/|_/  /___/___/         ",23,0,0x02);
              printString("                                                                               ",24,0,0x02);
}
