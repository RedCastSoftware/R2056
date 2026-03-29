#R2056.py by Eddie Hudgins
import time
import numpy as np
inp = 2
tc = 0
carry = 0
registers  = {
    "A": 0,
    "X": 0,
    "Y": 0,
    "SP": 0,
    "P": 0
}
icarus_mem = np.zeros(65536)
def oA9(inp,carry):
    inp%=256
    registers["A"] = inp
    return carry
def oA2(inp,carry):
    inp%=256
    registers["X"] = inp
    return carry
def oA0(inp,carry):
    inp%=256
    registers["Y"] = inp
    return carry
def o8D(inp,carry):
    icarus_mem[int(inp)] = registers["A"]
    return carry
def o86(inp,carry):
    inp%=256
    icarus_mem[int(inp)] = registers["X"]
    return carry
def o84(inp,carry):
    inp%=256
    icarus_mem[int(inp)] = registers["Y"]
    return carry
def o69(inp,carry):
    registers["A"]+=inp+carry
    if registers["A"] > 255:
        carry=1
    else:
        carry=0
    registers["A"] %= 256
    return carry
def oE9(val, carry):
    result = registers["A"] - val - (1 - carry)
    carry = 1 if result >= 0 else 0
    registers["A"] = result & 0xFF  # Wrap to 0-255
    return carry
istr = {
      0xA9: oA9,
      0xA2: oA2,
      0xA0: oA0,
      0x8D: o8D,
      0x86: o86,
      0x84: o84,
      0x69: o69,
      0xe9: oE9,
}
icarus_pro = {
    1: (0xA9,8),
    2: (0x69,256),
}
s = time.perf_counter()
for _ in range(1000000):
    carry = 0
    for i in icarus_pro:
        op = icarus_pro[i][0]
        carry = istr[op](icarus_pro[i][1],carry) 
        tc+=1
e = time.perf_counter()
print(e-s)
p = 1/(e-s)
print(f"{(tc*p):,} cycles per second")
print(registers)
print(icarus_mem[255])
print(carry)
