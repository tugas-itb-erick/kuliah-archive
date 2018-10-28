main()
{
    interrupt(0x21,0,"WooHoo! I'm a user program!\r\n\0",0,0);
    interrupt(0x21,4,"shell",0x2000,0);
} 
