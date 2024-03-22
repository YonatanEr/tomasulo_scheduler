F = [float(i) for i in range(16)]

def line2opcode(line):
    '''   DST = SRC0 OP SRC0    ->    instruction 
    line examples:  
        F1 = F2 + F3
        F1 =    F2   +      F3
        F1 =    F2   -      F3
        F1 =    F2   *      F3
        F1 =    F2   /      F3
        HALT
    '''
    line = line.upper().rstrip()
    line = line.replace(" ", "").replace("[", "").replace("]", "")
    if "//" in line:
        line = line[:line.find("//")]
    if len(line) == 0:
        return ""
    if "HALT" in line:
        return "06000000"
    tokens = line.split("F")
    dst = hex(int(tokens[1][:-1]))
    src0 = hex(int(tokens[2][:-1]))
    src1 = hex(int(tokens[3][:]))
    if "+" in line:
        F[int(dst, 0)] = F[int(src0, 0)] + F[int(src1, 0)]
        opcode = 2
    if "-" in line:
        F[int(dst, 0)] = F[int(src0, 0)] - F[int(src1, 0)]
        opcode = 3
    if "*" in line:
        F[int(dst, 0)] = F[int(src0, 0)] * F[int(src1, 0)]
        opcode = 4
    if "/" in line:
        F[int(dst, 0)] = F[int(src0, 0)] / F[int(src1, 0)]
        opcode = 5
    return f"0{opcode}{dst[2:]}{src0[2:]}{src1[2:]}000"


INPUT_FILE = "test_5/inst_file.txt"
MEMIN_FILE = f"{INPUT_FILE.split('.')[0]}_meming.txt"
REGOUT_FILE = f"{INPUT_FILE.split('.')[0]}_expected_regout.txt"
inp = open(INPUT_FILE, "r")
mem = open(MEMIN_FILE, "w")
lines = inp.readlines()
for line in lines:
    mline = line2opcode(line)
    if len(mline) < 8:
        continue
    mem.write(mline + "\n")
inp.close()
mem.close()

reg = open(REGOUT_FILE, "w")
for f in F:
    reg.write(f"{f}\n")
reg.close()
