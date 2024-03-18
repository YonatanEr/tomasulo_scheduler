def line2opcode(line):
    line = line.upper().rstrip()
    line = line.replace(" ", "").replace("[", "").replace("]", "")
    if "//" in line:
        line = line[:line.find("//")]
    if len(line) == 0:
        return ""
    if "HALT" in line:
        return "06000000"
    if "+" in line:
        opcode = 2
    if "-" in line:
        opcode = 3
    if "*" in line:
        opcode = 4
    if "/" in line:
        opcode = 5
    tokens = line.split("F")
    dst = hex(int(tokens[1][:-1]))
    src0 = hex(int(tokens[2][:-1]))
    src1 = hex(int(tokens[3][:]))
    return f"0{opcode}{dst[2:]}{src0[2:]}{src1[2:]}000"


INPUT_FILE = "test_3/inst_file.txt"
MEMIN_FILE = f"{INPUT_FILE.split('.')[0]}_meming.txt"

inp = open(INPUT_FILE, "r")
mem = open(MEMIN_FILE, "w")
lines = inp.readlines()
for line in lines:
    mline = line2opcode(line)
    if len(mline) < 8:
        continue
    mem.write(mline + "\n")
