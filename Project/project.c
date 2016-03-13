#include "spimcore.h"

/* project.c

   Created By: Clayton Walker and Matthew Brown

   The project's coding was almost entirely done by Clayton Walker.
   Both project partners met up and discussed the project together, going through the file as a group effort. 
   This file is a result of a 70/30 effort collaboration, with Clayton doing 70% of the work and coding, and the other 30% from Matthew in helping understand what is going on in the program.
 */

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch (ALUControl) {

        // Addition
        case 0: 
            *ALUresult = A+B;
            break;

        // Subtraction
        case 1: 
            *ALUresult = A-B;
            break;

        // Less than
        case 2: 
            if ((int)A < (int)B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;
            
        // Less than unsigned
        case 3: 
            if (A < B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;

        // bitwise AND
        case 4: 
            *ALUresult = (A & B);
            break;

        // bitwise OR
        case 5: 
            *ALUresult = (A | B);
            break;
            
        // A Less than 0
        case 6:
            if ((int)A < 0)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;

        // bitwise NOT
        case 7:
            *ALUresult = ~A;
            break;
    }

    // Set Zero based on results. 1 if *ALUResult = 0; otherwise, 0.
    if (*ALUresult == 0) {
        *Zero = 1;
    }
    else {
        *Zero = 0;
    }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    
    /*Return 1 if a halt condition occurs; otherwise, return 0.*/
    if (PC > 65535 | PC & 0x3) {
        return 1;
    }
    
    /*Fetch the instruction addressed by PC from Mem and write it to instruction. bit-wise shift by 2 to get instruction from memory*/
    *instruction = Mem[PC >> 2];
    
    return 0;
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

    // Partition instruction into several parts (op, r1, r2, r3, funct, offset, jsec)
    
    /*bit-wise shift by 26 to get the upper six bits(opcode)*/
    *op = (instruction >> 26) & 0x0000003f; 
    
    /*bit-wise shift by 21 then ANDed with 0x1f (0001 1111) to only extract the first five-bits(register 1)*/
    *r1 = (instruction >> 21) & 0x0000001F;
    
    /*bit-wise shift by 16 then ANDed with 0x1f (0001 1111) to only extract the first five-bits(register 2)*/
    *r2 = (instruction >> 16) & 0x0000001F;
    
    /*bit-wise shift by 11 then ANDed with 0x1f (0001 1111) to only extract the first five-bits(register 3)*/
    *r3 = (instruction >> 11) & 0x0000001F;
    
    /*the instruction is ANDed with 0x3f (0011 1111) to extract the first six-bits(function)*/
    *funct = instruction & 0x0000003F;
    
    /*the instruction is ANDed with 0xffff (1111 1111 1111 1111) to extract the first 
     16-bits(offset)*/
    *offset = instruction & 0X0000FFFF;
    
    /*the instruction is ANDed with 0x3ffffff (0011 1111 1111 1111 1111 1111 1111) to extract the first 26-bits(jump target)*/
    *jsec = instruction & 0x03FFFFFF;

}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

    //Used for halting on unused opcode
    int haltFlag = 0;
    
    switch (op) {
        case 0x00:
            //Register Type: 
            /*ALUop is (0111) which means it is an R-type instruction, the RegDst is on(1) because there is a register destination in use, and RegWrite is on(1) because a register is also being written, the ALUSrc is zero because the second operand for the ALU is taken from a register*/
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            break;
        case 0x0d:
            //OR immediate:
            /*ALUop is (0101), the RegDst is on(1) because there is a register destination in use, and RegWrite is on(1) because a register is also being written, the ALUSrc is zero because the second operand for the ALU is taken from a register*/
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 5;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0x23:
            // Load word:
            /*MemRead is on(1) because there is date read from memory, MemtoReg is on(1) because the memory is written to a register after it is loaded, ALUSrc is 1 because the instruction is an immediate type, RegWrite is on because a register is being written, ALUop is 0 */
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0x2b:
            // Store word:
            /*MemWrite is on(1) because memory is being written, ALUSrc is 1 because the second operand for the ALU is taken from the instruction, ALUop is 0 */
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            break;
        case 0x0a:
            // set less than immediate:
            /*ALUSrc is 1 because it is an immediate instruction, the ALUop is (0010) meaning the ALU will do set less than, RegWrite is on because a register is being written */
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0x04:
            // Branch Eq:
            /*Branch is on(1) because the instruction is a branch and the equal comparator is used, ALUSrc is 1 because the second operand for the ALU is in the instruction */
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            break;
        case 0x01:
            //Branch if Equal to or Greater than 0:
            /*Branch is on(1) because the instruction is a branch and the equal comparator is used, ALUSrc is 1 because the second operand for the ALU is in the instruction*/
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 6;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            break;
        case 0x02:
            // Jump:
            /*ALUop is (0000) which is NOP or add, it does this because the lower 26 bits are shifted to the left twice then added with the four bits from the incremented PC, the ALUSrc is 1 because the second operand for the ALU is in the instruction */
            controls->RegDst = 2;
            controls->Jump = 1;
            controls->Branch = 2;
            controls->MemRead = 2;
            controls->MemtoReg = 2;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 2;
            controls->RegWrite = 0;
            break;
        default:
            // we don't handle this opcode
            haltFlag = 1;
    }
    return haltFlag;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    
// Read the registers addressed by r1 and r2 from Reg, and write the read values to
// data1 and data2 respectively.
     
    /*puts register one and register two(found from the instruction partition function) into their own arrays, setting them equal to two different data pointers for use during the ALU stage*/
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    
    /*takes the offset and checks if the 16th bit is one, if it is one it is a negative value and will be sign extended, if the 16th bit is zero it is non-negative and will be left alone */
    if (offset & 0x00008000) {
        *extended_value = offset | 0xffff0000; 
    }
    else {
        *extended_value = offset;
    }
    
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    
    // R-Type 
    //Checks if the ALUSrc is zero, if it is zero then the operation will be an R-Type
    if(ALUSrc == 0)
    {
        switch (funct) {
            case 0x21: // Addition unsigned
                ALUOp = 0;
                break;
            case 0x23: // Subtraction unsigned
                ALUOp = 1;
                break;
            case 0x24: // Bitwise and
                ALUOp = 4;
                break;
            case 0x2a: // Set on less than 
                ALUOp = 2;
                break;
            case 0x2b: // Set on less than unsigned
                ALUOp = 3;
                break;
            default: // Unknown
                return 1;
        }
        
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }
    
    // I-Type 
    //Checks if the ALUSrc is one, if it is one then the operation will be an immediate 
    else if (ALUSrc == 1) {
        //the sign extended value is passed into ALU instead of data2
        ALU(data1, extended_value, ALUOp, ALUresult, Zero);
    }
    
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    
    if ((MemWrite == 1 || MemRead == 1) && ALUresult % 4 != 0) // If address is bad, then return a halt condition
	{
		//Memory call out of range
		return 1;
	}
	//checks if MemWrite is 1. If it is, it sets memory of ALUresult to data2
	if (MemWrite == 1)
	{
		Mem[ALUresult >> 2] = data2;
	}
	//checks if MemRead is 1. If it is, it sets the memory data to memory of ALUresult shifted 2-bits
	if (MemRead == 1)
	{
		*memdata = Mem[ALUresult >> 2];	
	}
    
	return 0;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    
    if (RegWrite == 1 ) {
        if (MemtoReg == 1) {
            if (RegDst == 1) 
                Reg[r3] = memdata; //Write memdata to rd 
            else 
                Reg[r2] = memdata; // Write memdata to rt
        }
        else {
            if (RegDst == 1)
                Reg [r3] = ALUresult; // Write ALU results to rd
            else    
                Reg [r2] = ALUresult; // Write ALU results to rt
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    
    //The PC is incremented by four after every instruction
    *PC = *PC  + 4;
    
    if(Jump == 1) 
    {
        /*if jump is 1, the PC equals the jump target shifted 2-bits left ORed with the upper four bits of the PC to make 32-bits*/
		*PC = (jsec << 2) | (*PC | 0xf0000000); //dont do &
    }
    /*If branch is 1, the PC will be equal to the sign extended value shifted left two bits  */
    else if (Branch == 1 && Zero)
    {
		*PC += (extended_value << 2);
    }
}