#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <random>


int main()
{

    //scr width/height

    bool screenBuffer[32][64] = {};

    int PC = 512;
    int SP = 0; //STACK POINTER

    const int width = 64;
    const int height = 32;
    const int CELLSIZE = 8;

    const int INSTRUCTIONS_PER_FRAME = 1;

    //memory
    uint8_t RAM[4096] = {};
    uint16_t Stack[16] = {};

    //registers

    //SPECIAL REGISTERS
    uint16_t I = 0;
    uint8_t delay = 0x3C;

    //v0-vF
    uint8_t registers[16] = {};

    bool waiting = false;
    bool keyBool = false;
    uint8_t key = 0xFF;

    std::ifstream rom("roms/octoachip8story.ch8", std::ios::in | std::ios::binary);


    //load font data into ram by hand
    RAM[0] = 0xF0; 
    RAM[1] = 0x90;
    RAM[2] = 0x90;
    RAM[3] = 0x90;
    RAM[4] = 0xF0; //0

    RAM[5] = 0x20;
    RAM[6] = 0x60;
    RAM[7] = 0x20;
    RAM[8] = 0x20;
    RAM[9] = 0x70; //1

    RAM[10] = 0xF0;
    RAM[11] = 0x10;
    RAM[12] = 0xF0;
    RAM[13] = 0x80;
    RAM[14] = 0xF0; //2

    RAM[15] = 0xF0;
    RAM[16] = 0x10;
    RAM[17] = 0xF0;
    RAM[18] = 0x10;
    RAM[19] = 0xF0;  //3

    RAM[20] = 0x90;
    RAM[21] = 0x90;
    RAM[22] = 0xF0;
    RAM[23] = 0x10;
    RAM[24] = 0x10; //4

    RAM[25] = 0xF0;
    RAM[26] = 0x80;
    RAM[27] = 0xF0;
    RAM[28] = 0x10;
    RAM[29] = 0xF0; //5  

    RAM[30] = 0xF0;
    RAM[31] = 0x80;
    RAM[32] = 0xF0;
    RAM[33] = 0x90;
    RAM[34] = 0xF0; //6

    RAM[35] = 0xF0;
    RAM[36] = 0x10;
    RAM[37] = 0x20;
    RAM[38] = 0x40;
    RAM[39] = 0x40; //7

    RAM[40] = 0xF0;
    RAM[41] = 0x90;
    RAM[42] = 0xF0;
    RAM[43] = 0x90;
    RAM[44] = 0xF0; // 8

    RAM[45] = 0xF0;
    RAM[46] = 0x90;
    RAM[47] = 0xF0;
    RAM[48] = 0x10;
    RAM[49] = 0xF0; // 9 

    RAM[50] = 0xF0;
    RAM[51] = 0x90;
    RAM[52] = 0xF0;
    RAM[53] = 0x90;
    RAM[54] = 0x90; // A 

    RAM[55] = 0xE0;
    RAM[56] = 0x90;
    RAM[57] = 0xE0;
    RAM[58] = 0x90;
    RAM[59] = 0xE0; // B 
    

    RAM[60] = 0xF0;
    RAM[61] = 0x80;
    RAM[62] = 0x80;
    RAM[63] = 0x80;
    RAM[64] = 0xF0; // C
        

    RAM[65] = 0xE0;
    RAM[66] = 0x90;
    RAM[67] = 0x90;
    RAM[68] = 0x90;
    RAM[69] = 0xE0; // D
      

    RAM[70] = 0xF0;
    RAM[71] = 0x80;
    RAM[72] = 0xF0;
    RAM[73] = 0x80;
    RAM[74] = 0xF0; // E 

    RAM[75] = 0xF0;
    RAM[76] = 0x80;
    RAM[77] = 0xF0;
    RAM[78] = 0x80;
    RAM[79] = 0x80; // F 


    char byte;
    int index = 0;
    while (rom.get(byte) && index < 4096 - 512) {
        unsigned char ubyte = static_cast<unsigned char>(byte);
        RAM[512 + index] = ubyte;
        index++;
    }

    for (int i = 0; i < 4096; ++i) {
        std::cout << std::hex << (int)RAM[i] << " ";
    }
    std::cout << std::endl;

    rom.close();
    //infinite runtime !!!!!!


    sf::RenderWindow window(sf::VideoMode({ width * CELLSIZE, height * CELLSIZE }), "CHIP-8");


    while (window.isOpen())
    {
        uint8_t incAmount = 2;
        keyBool = false;
        key = 0xFF;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
                    key = 0x1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
                    key = 0x2;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
                    key = 0x3;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
                    key = 0xC;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
                    key = 0x4;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                    key = 0x5;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
                    key = 0x6;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                    key = 0xD;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                    key = 0x7;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                    key = 0x8;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                    key = 0x9;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
                    key = 0xE;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
                    key = 0xA;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
                    key = 0x0;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
                    key = 0xB;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V)) {
                    key = 0xF;
                }

                if (key < 0x10) {
                    keyBool = true;
                }
            }
        }

        if (delay > 0) {
            delay--;
        }


        sf::RectangleShape rectangle(sf::Vector2f(8, 8));

        uint16_t current_Instruction = (RAM[PC] << 8) | RAM[PC + 0x1];

        //decode
        for (int i = 0; i < INSTRUCTIONS_PER_FRAME; i++) {
            current_Instruction = (RAM[PC] << 8) | RAM[PC + 0x1];
            if (!waiting) {

                if (current_Instruction == 0x00E0) { //cls 00E0
                    incAmount = 0x2;
                    for (int y = 0; y < 32; y++) {
                        for (int x = 0; x < 64; x++) {
                            screenBuffer[y][x] = false;
                        }
                    }
                    std::cout << PC << " 00e0 CLEAR SCREEN" << std::endl;
                }
                else if (current_Instruction >> 12 == 0x02) { //CALL subroutine 2NNN
                    incAmount = 0x2;
                    uint32_t NNN = (current_Instruction & 0x0FFF);
                    //push current PC to stack
                    if (SP < 16) {
                        incAmount = 0;
                        Stack[SP] = PC + 2;
                        SP++;
                        PC = NNN;
                        std::cout << +PC << std::hex << " " << current_Instruction << " PUSHED PC TO STACK " << +Stack[SP - 1] << ", PC = " << +PC << std::endl;
                    }
                    else {
                        std::cout << "STACK OVERFLOW!!!";
                        break;
                    }
                }
                else if (current_Instruction == 0x00EE) {
                    incAmount = 0;
                    //pop previous PC from stack
                    if (SP > 0) {
                        SP--;
                        PC = Stack[SP];
                        std::cout << +PC << " 00ee POPPED PC " << +Stack[SP] << " FROM STACK, PC = " << +PC << std::endl;
                    }
                    else {
                        std::cout << "STACK UNDERFLOW!!!";
                        break;
                    }
                }
                else if (current_Instruction >> 12 == 0x01) { //JMP instruction 1NNN
                    incAmount = 0;
                    uint32_t NNN = current_Instruction & 0x0FFF;
                    PC = NNN;

                    std::cout << +PC << " " << std::hex << current_Instruction << " JUMP TO " << +NNN << " PC = " << +PC << std::endl;
                }
                else if (current_Instruction >> 12 == 0x03) { //3XNN //skip next opcode if vX == NN (note: on platforms that have 4 byte opcodes, like F000 on XO-CHIP, this needs to skip four bytes)
                    uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                    uint16_t NN = (current_Instruction & 0x00FF);
                    if (registers[vX] == NN) {
                        incAmount = 4;
                        std::cout << +PC << std::hex << " " << current_Instruction << " skipped X==NN, PC: " << +PC << " > " << +(PC + 0x4) << std::endl;
                    }
                }
                else if (current_Instruction >> 12 == 0x04) { //4XNN skip next opcode if vX != NN (note: on platforms that have 4 byte opcodes, like F000 on XO-CHIP, this needs to skip four bytes)
                    uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                    uint16_t NN = (current_Instruction & 0x00FF);
                    if (registers[vX] != NN) {
                        incAmount = 0x4;
                        std::cout << +PC << std::hex << " " << current_Instruction << " skipped X!=NN, PC: " << +PC << " > " << +(PC + 0x4) << std::endl;
                    }
                }
                else if (current_Instruction >> 12 == 0x05) { //5XY0 skip next opcode if vX == vY (note: on platforms that have 4 byte opcodes, like F000 on XO-CHIP, this needs to skip four bytes)
                    uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                    uint8_t vY = (current_Instruction & 0x00F0) >> 4;
                    if (registers[vX] == registers[vY]) {
                        incAmount = 0x4;
                        std::cout << +PC << std::hex << " " << current_Instruction << " skipped X==Y, PC: " << +PC << std::endl;
                    }
                }
                else if (current_Instruction >> 12 == 0x09) { //9XY0 skip next opcode if vX != vY (note: on platforms that have 4 byte opcodes, like F000 on XO-CHIP, this needs to skip four bytes)
                    uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                    uint8_t vY = (current_Instruction & 0x00F0) >> 4;
                    if (registers[vX] != registers[vY]) {
                        incAmount = 0x4;
                        std::cout << +PC << std::hex << " " << current_Instruction << " skipped X!=Y, PC: " << +PC << std::endl;
                    }
                }
                else if (current_Instruction >> 12 == 0x06) { //set VX 6XNN
                    incAmount = 2;
                    uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                    uint16_t NN = current_Instruction & 0x00FF;
                    registers[vX] = NN;
                    std::cout << +PC << std::hex << " " << current_Instruction << " SET REGISTER " << +vX << " TO " << +NN << std::endl;
                }
                else if (current_Instruction >> 12 == 0x07) { //add value to VX 7XNN
                    incAmount = 2;
                    uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                    uint16_t NN = current_Instruction & 0x00FF;
                    registers[vX] += NN;
                    std::cout << +PC << std::hex << " " << current_Instruction << " ADD VALUE " << +NN << " TO REGISTER " << +vX << std::endl;
                }
                else if (current_Instruction >> 12 == 0x0A) { //set reg I ANNN
                    incAmount = 2;
                    uint32_t NNN = current_Instruction & 0x0FFF;
                    I = NNN;
                    std::cout << +PC << std::hex << " " << current_Instruction << " SET I TO " << +NNN << std::endl;
                }
                else if (current_Instruction >> 12 == 0x0B) { //JMP to NNN + v0 BNNN
                    incAmount = 2;
                    uint32_t NNN = current_Instruction & 0x0FFF;
                    PC = NNN + registers[0];
                    std::cout << +PC << std::hex << " " << current_Instruction << " JMP TO " << +PC << std::endl;
                }
                else if (current_Instruction >> 12 == 0x0C) { //set vX to random number & NN CXNN
                    incAmount = 2;
                    uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                    uint16_t NN = current_Instruction & 0x00FF;

                    int random = rand();
                    registers[vX] = random & NN;

                    std::cout << +PC << std::hex << " " << current_Instruction << " GEN RND " << +registers[vX] << std::endl;
                }
                else if (current_Instruction >> 12 == 0x08) { //8XY_ instuction family
                    incAmount = 2;
                    if ((current_Instruction & 0x000F) == 0x0) { //set VX to VY 8XY0
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        uint8_t vY = (current_Instruction & 0x00F0) >> 4;
                        registers[vX] = registers[vY];
                        std::cout << +PC << std::hex << " " << current_Instruction << " REGISTER " << +vX << " = VALUE FROM " << +vY << " " << std::hex << +registers[vY] << std::endl;
                    }
                    else if ((current_Instruction & 0x000F) == 0x1) { //bitwise OR
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        uint8_t vY = (current_Instruction & 0x00F0) >> 4;

                        registers[vX] = registers[vY] | registers[vX];
                        registers[15] = 0x0;

                        std::cout << +PC << std::hex << " " << current_Instruction << " REGISTER " << +vX << " = OR WITH " << +vY << " " << std::hex << +registers[vY] << std::endl;
                    }
                    else if ((current_Instruction & 0x000F) == 0x2) { //bitwise AND
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        uint8_t vY = (current_Instruction & 0x00F0) >> 4;

                        registers[vX] = registers[vY] & registers[vX];
                        registers[15] = 0x0;

                        std::cout << +PC << std::hex << " " << current_Instruction << " REGISTER " << +vX << " = AND WITH " << +vY << " " << std::hex << +registers[vY] << std::endl;
                    }
                    else if ((current_Instruction & 0x000F) == 0x3) { //bitwise XOR
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        uint8_t vY = (current_Instruction & 0x00F0) >> 4;

                        registers[vX] = registers[vY] ^ registers[vX];
                        registers[15] = 0x0;

                        std::cout << +PC << std::hex << " " << current_Instruction << " REGISTER " << +vX << " = XOR WITH " << +vY << " " << std::hex << +registers[vY] << std::endl;
                    }
                    else if ((current_Instruction & 0x000F) == 0x4) { //bitwise ADD
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        uint8_t vY = (current_Instruction & 0x00F0) >> 4;
                        uint8_t temp = registers[vX];
                        int result;

                        result = registers[vY] + registers[vX];
                        registers[vX] = (result % 256);

                        if (result > 255) {
                            registers[15] = 0x1;
                            std::cout << "OVERFLOW!" << std::endl;
                        }
                        else {
                            registers[15] = 0x0;
                            std::cout << "NORMAL!" << std::endl;
                        }

                        std::cout << +PC << std::hex << " " << current_Instruction << " REGISTER " << +vX << " (" << +temp << ")" << " = ADD WITH REGISTER " << +vY << " (" << +registers[vY] << ") " << std::hex << +registers[15] << std::endl;
                    }
                    else if ((current_Instruction & 0x000F) == 0x5) { //bitwise SUB x-y
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        uint8_t vY = (current_Instruction & 0x00F0) >> 4;
                        uint8_t temp = registers[vX];

                        registers[vX] = registers[vX] - registers[vY];

                        std::cout << " RESULT SUB vX - vY = " << +registers[vX] << std::endl;
                        if (temp > registers[vY] || temp == registers[vY]) {
                            registers[15] = 0x1;
                            std::cout << "NORMAL!" << std::endl;
                        }
                        else {
                            registers[15] = 0x0;
                            std::cout << "UNDERFLOW!" << std::endl;
                        }
                        std::cout << +PC << std::hex << " " << current_Instruction << " REGISTER " << +vX << " (" << +temp << ")" << " = SUB WITH REGISTER " << +vY << " (" << +registers[vY] << ") " << std::hex << +registers[15] << std::endl;
                    }
                    else if ((current_Instruction & 0x000F) == 0x6) { // set vX to vY and shift vX one bit to the right, set vF to the bit shifted out, even if X=F!


                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        uint8_t vY = (current_Instruction & 0x00F0) >> 4;

                        registers[vX] = registers[vY];
                        uint8_t temp = registers[vX];

                        std::cout << " vX = " << +temp << std::endl;
                        std::cout << " BIT TO BE SHIFTED OUT = " << +(temp & 0b00000001) << std::endl;
                        std::cout << " vF = " << +registers[15] << std::endl;

                        registers[vX] >>= 1;
                        registers[15] = (temp & 0b00000001);

                        std::cout << +PC << std::hex << " " << current_Instruction << " REGISTER " << +vX << " (" << +temp << ")" << " SHIFTED 1 BIT TO RIGHT >> " << +registers[vX] << " | vF = " << +registers[15] << std::endl;



                    }
                    else if ((current_Instruction & 0x000F) == 0x7) { // SUB Vy-Vx
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        uint8_t vY = (current_Instruction & 0x00F0) >> 4;
                        uint8_t temp = registers[vX];

                        registers[vX] = registers[vY] - registers[vX];
                        if (registers[vY] > temp || registers[vY] == temp) {
                            registers[15] = 0x1;
                            std::cout << "NORMAL!" << std::endl;
                        }
                        else {
                            registers[15] = 0x0;
                            std::cout << "UNDERFLOW!" << std::endl;
                        }
                        std::cout << "RESULT OF SUB vY - vX = " << +registers[vX] << std::endl;
                        std::cout << +PC << std::hex << " " << current_Instruction << " REGISTER " << +vY << " (" << +registers[vY] << ")" << " = SUB WITH REGISTER " << +vX << " (" << +temp << ") " << std::hex << +registers[15] << std::endl;
                    }
                    else if ((current_Instruction & 0x000F) == 0xE) { // set vX to vY and shift vX one bit to the left, set vF to the bit shifted out, even if X=F!


                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        uint8_t vY = (current_Instruction & 0x00F0) >> 4;

                        registers[vX] = registers[vY];
                        uint8_t temp = registers[vX];


                        registers[vX] <<= 1;
                        registers[15] = (temp >> 7);


                        std::cout << +PC << std::hex << " " << current_Instruction << " REGISTER " << +vX << " (" << +temp << ")" << " SHIFTED 1 BIT TO LEFT << " << +registers[vX] << " | vF = " << +registers[15] << std::endl;


                    }
                }
                else if (current_Instruction >> 12 == 0x0D) { // display DXYN
                    incAmount = 2;
                    uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                    uint8_t vY = (current_Instruction & 0x00F0) >> 4;
                    uint8_t X = registers[vX] & 63;
                    uint8_t Y = registers[vY] & 31;
                    uint8_t N = (current_Instruction & 0x000F);
                    registers[15] = 0;
                    for (int row = 0; row < N; ++row) {
                        uint8_t spriteByte = RAM[I + row];  // get the byte for this row

                        // loop through each bit in spriteByte
                        for (int bit = 0; bit < 8; ++bit) {
                            bool pixelOn = (spriteByte >> (7 - bit)) & 1;

                            int screenX = (X + bit) % 64;
                            int screenY = (Y + row) % 32;

                            // XOR the pixel with screenBuffer
                            if (pixelOn) {
                                if (screenBuffer[screenY][screenX]) {
                                    registers[15] = 1; // collision detected
                                }
                                screenBuffer[screenY][screenX] ^= true;
                            }
                        }
                    }
                    std::cout << +PC << std::hex << " " << current_Instruction << " DRAW " << +N << " @ " << +vX << +vY << " " << +PC << " " << incAmount << std::endl;
                }
                else if (current_Instruction >> 12 == 0xF) { // Fx__ family 
                    incAmount = 2;
                    if ((current_Instruction & 0x00FF) == 0x65) { // load registers from ram v0 >> vX FX65
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        for (int i = 0; i <= vX; i++) {
                            registers[i] = RAM[I + i];


                            std::cout << +PC << std::hex << " " << current_Instruction << " SAVE VALUE FROM ADDR " << std::hex << +I + i << " TO REGISTER " << i << " " << std::hex << +registers[i] << std::endl;
                        }
                    }
                    else if ((current_Instruction & 0x00FF) == 0x55) { // save registers to ram v0 >> vX FX55
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        incAmount = 2;
                        for (int i = 0; i <= vX; i++) {
                            RAM[I + i] = registers[i];
                            std::cout << +PC << std::hex << " " << current_Instruction << " SAVE VALUE " << std::hex << +registers[i] << " TO ADDR " << std::hex << +I + i << std::endl;
                        }
                    }
                    else if ((current_Instruction & 0x00FF) == 0x33) { // save registers to ram
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        RAM[I] = registers[vX] / 100;
                        RAM[I + 1] = (registers[vX] / 10) % 10;
                        RAM[I + 2] = registers[vX] % 10;
                        std::cout << +PC << std::hex << " " << current_Instruction << " ";
                    }
                    else if ((current_Instruction & 0x00FF) == 0x1E) { // I += vX (FX1E)
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        I += registers[vX];
                    }
                    else if ((current_Instruction & 0x00FF) == 0x0A) { // stops and waits for key input FX0A
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        incAmount = 0;
                        waiting = true;
                        std::cout << +PC << " " << std::hex << current_Instruction << " Waiting for player input: " << +registers[vX] << " PC = " << +PC << std::endl;
                        std::cout << "WAITING!" << std::endl;
                    }
                    else if ((current_Instruction & 0x00FF) == 0x29) { // sets I to mem address of font for relevant hex character (0-F)
                        incAmount = 2;
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        uint8_t lowNibble = registers[vX] & 0x0F;
                        I = lowNibble * 0x5;
                        std::cout << +PC << " " << std::hex << current_Instruction << " I set to Hex Font " << +I << " " << +PC << std::endl;
                    }
                    else if ((current_Instruction & 0x00FF) == 0x15) { // sets delay to value of vX
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        delay = registers[vX];

                        std::cout << +PC << " " << std::hex << current_Instruction << " DELAY SET TO VALUE " << +registers[vX] << std::endl;
                    }
                    else if ((current_Instruction & 0x00FF) == 0x18) {

                        // FX18,   sound instruction

                    }
                    else if ((current_Instruction & 0x00FF) == 0x07) { // sets vX to value of delay timer
                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;

                        registers[vX] = delay;
                        std::cout << +PC << " " << std::hex << current_Instruction << " " << +vX << " SET TO DELAY VALUE " << +delay << std::endl;
                    }

                }
                else if (current_Instruction >> 12 == 0xE) { //EXA1, EX9E
                    if ((current_Instruction & 0x00FF) == 0xA1) {

                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        if ((registers[vX] & 0b00001111) != key) {
                            incAmount = 4;
                            std::cout << +PC << " " << std::hex << current_Instruction << " KEY " << +registers[vX] << " NOT PRESSED: SKIPPED!" << std::endl;
                        }
                        else {
                            std::cout << +PC << " " << std::hex << current_Instruction << " KEY " << +registers[vX] << " PRESSED: NOT SKIPPED!" << std::endl;
                        }
                        // skip if key in vX is NOT pressed
                    }
                    else if ((current_Instruction & 0x00FF) == 0x9E) {




                        uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                        if ((registers[vX] & 0b00001111) == key) {
                            incAmount = 4;
                            std::cout << +PC << " " << std::hex << current_Instruction << " KEY " << +registers[vX] << " PRESSED: SKIPPED!" << std::endl;
                        }
                        else {
                            std::cout << +PC << " " << std::hex << current_Instruction << " KEY " << +registers[vX] << " NOT PRESSED: NOT SKIPPED!" << std::endl;
                        }

                        // skip if key in vX is pressed
                    }
                }

                PC += incAmount;

            }
        }
        if (waiting){
            if (key >= 0 && keyBool) {
                std::cout << std::hex << current_Instruction << " KEY: " << +key << " PRESSED " << keyBool << std::endl;
                uint8_t vX = (current_Instruction & 0x0F00) >> 8;
                registers[vX] = key;
                incAmount = 2;
                PC += 2;
                waiting = false;
            }
        }

        window.clear();

        float yOffset = 0.0;

        //draw grid of pixels reflecting the screenbuffer
        for (int y = 0; y < 32; ++y) {
            float xOffset = 0.0;
            for (int x = 0; x < 64; ++x) {
                if (screenBuffer[y][x] == false) {
                    rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
                    rectangle.setFillColor(sf::Color{ 10, 10, 34, 255 });
                    window.draw(rectangle);
                    xOffset += CELLSIZE;
                }
                else {
                    rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
                    rectangle.setFillColor(sf::Color::White);
                    window.draw(rectangle);
                    xOffset += CELLSIZE;
                }
            }
            yOffset += CELLSIZE;

        }
        
        window.display();
    }
}
