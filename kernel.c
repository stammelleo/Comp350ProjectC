// Jesse Thompson
// COMP 350
// ProjectC

//void printChar(char);
void printString(char*);
//void readString(char*);
//void readSector(char*, int);
void handleInterrupt21(int,int,int,int);

void main()
{
    printString("Hello World");


    while (1);
}

// Craig
void printString(char* chars)
{
    // Prints out the provided string to the console using interrupts
    int index = 0;

    while(chars[index] != 0x0)
    {
        interrupt(0x10, 0xe * 256 + chars[index], 0, 0, 0);
        index++;
    }
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
    switch(ax)
    {
        case 0:
            printString(bx);
            break;
//        case 1:
//            readString(bx);
//            break;
//        case 2:
//            readSector(bx, cx);
//            break;
        default:
            printString("No interrupt function correlated with AX number");

    }
}

