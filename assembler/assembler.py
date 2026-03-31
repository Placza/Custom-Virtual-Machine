instructions = []
pc = 0

tags = {}
prev_tags = {}


with open('code.cvm', 'r') as f:
    for line in f:
        line = line.strip()
        if line == '' or line[0] == '#': continue
        
        ins = line.split(' ')
        
        if (ins[0][-1] == ':'):
            tag = ins[0][0:-1]
            tags[tag] = pc
            if tag in prev_tags:
                pos, cur_pc = prev_tags[tag]
                addr = pc - cur_pc + 1
                instructions[pos] = addr.to_bytes(2, 'little', signed=True)

        if (ins[0] == 'push'):
            instructions.append(b'\x00\x00')
            num = int(ins[1])
            instructions.append(num.to_bytes(2, 'little', signed=True))
            pc += 2

        if (ins[0] == 'pop'):
            instructions.append(b'\x01\x00')
            pc += 1

        if (ins[0] == 'halt'):
            instructions.append(b'\x02\x00')
            pc += 1

        if (ins[0] == 'add'):
            instructions.append(b'\x03\x00')
            pc += 1
        
        if (ins[0] == 'sub'):
            instructions.append(b'\x04\x00')
            pc += 1

        if (ins[0] == 'mul'):
            instructions.append(b'\x05\x00')
            pc += 1

        if (ins[0] == 'and'):
            instructions.append(b'\x06\x00')
            pc += 1

        if (ins[0] == 'or'):
            instructions.append(b'\x07\x00')
            pc += 1

        if (ins[0] == 'xor'):
            instructions.append(b'\x08\x00')
            pc += 1

        if (ins[0] == 'sfl'):
            instructions.append(b'\x09\x00')
            pc += 1

        if (ins[0] == 'sfr'):
            instructions.append(b'\x0A\x00')
            pc += 1

        if (ins[0] == 'jmp'):
            instructions.append(b'\x0B\x00')
            tag = ins[1]
            if tag in tags:
                addr = tags[tag] - pc - 1 
                print(tags[tag], pc)
                instructions.append(addr.to_bytes(2, 'little', signed=True))
            else:
                prev_tags[ins[1]] = (len(instructions), pc)
                instructions.append(ins[1])
            pc += 2
    

        if (ins[0] == 'cjmp'):
            instructions.append(b'\x0C\x00')
            tag = ins[1]
            if tag in tags:
                addr = tags[tag] - pc - 1
                instructions.append(addr.to_bytes(2, 'little', signed=True))
            else:
                prev_tags[ins[1]] = (len(instructions), pc)
                instructions.append(ins[1])
            pc += 2


        if (ins[0] == 'ldr'):
            instructions.append(b'\x0D\x00')
            pc += 1

        if (ins[0] == 'str'):
            instructions.append(b'\x0E\x00')
            pc += 1
        

print(instructions)

with open('code.bin', 'wb') as f:
    for i in instructions:
        f.write(i)
