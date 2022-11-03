// Team Awesome: Jesse Thompson, TJ Bourget, Sean Tammelleo, Craig Kimello
// COMP 350 - 001
// ProjectC

void printChar(char);
void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int,int,int,int);

void main()
{
    char userInput[80];
    char fileInput[512];

    makeInterrupt21();
    interrupt(0x21,1,userInput,0,0);
    interrupt(0x21,0,userInput,0,0);
    interrupt(0x21,2,fileInput,30,0);
    interrupt(0x21,0,fileInput,0,0);

    while (1);
}

// Prints out the provided string to the console using interrupts
void printString(char* chars)
{
    int index = 0;
    while(chars[index] != 0x0)
    {
        interrupt(0x10, 0xe * 256 + chars[index], 0, 0, 0);
        index++;
    }
}

void readString(char* letter)
{
    int index = 0;
    //getting letter and printing to screen
    letter[index] = interrupt(0x16, 0, 0, 0,0);
    printChar(letter[index]);

    //check if enter key is pressed and under string limit but not zero
    while (letter[index] != 0xd  && 0 < index < 80)
    {
        char inputLetter = interrupt(0x16, 0, 0, 0, 0);
        //if input is backspace
        if (inputLetter == 0x8)
        {
            if(index >= 0)
            {
                index--;
                printChar(0x8);
                printChar(' ');
                printChar(0x8);
            }
        }
        else{
            index++;
            letter[index] = inputLetter;
            printChar(letter[index]);
        }
    }
    //once enter key pressed
    letter[index+1] = 0xa;
    letter[index+2] = 0x0;
    printChar(0xa);
}

void printChar(char c)
{
    interrupt(0x10, 0xe*256+c, 0, 0, 0);
}

void readSector(char* buffer, int sector)
{
    int ah = 2;             // tells BIOS to read sector
    int al = 1;             // number of sectors to use
    int ax = ah * 256 + al;

    int ch = 0;             // track number
    int cl = sector + 1;    // relative sector number
    int cx = ch * 256 + cl;

    int dh = 0;             // head number
    int dl = 0x80;          // device number
    int dx = dh * 256 + dl;

    interrupt(0x13, ax, buffer, cx, dx);
}

// Chooses the proper interrupt function call based on the value of 'ax'
void handleInterrupt21(int ax, int bx, int cx, int dx)
{
    switch(ax)
    {
        case 0:
            printString(bx);
            break;
        case 1:
            readString(bx);
            break;
        case 2:
            readSector(bx, cx);
            break;
        default:
            printString("No interrupt function correlated with AX number");

    }
}
