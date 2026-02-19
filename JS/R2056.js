var tc = 0;
var op;
var carry = 0;
var registers = {"A": 0, "X":0,"Y": 0, "SP":0, "P": 0};
var icarus_mem = new Array(65536);
var istr = {0xA9: oA9, 0xA2: oA2, 0xA0: oA0, 0x8D: o8D, 0x86: o86, 0x84: o84, 0x69: o69};
var icarus_pro = {0: [0xA9,8],1:[0x69,256]};
function oA9(inp,carry){
  inp %=256;
  registers["A"] = inp;
  return carry;
}
function oA2(inp,carry){
  inp %=256;
  registers["X"] = inp;
  return carry;
}
function oA0(inp,carry){
  inp %=256;
  registers["Y"] = inp;
  return carry;
}
function o8D(inp,carry){
  icarus_mem[Number(inp)] = registers["A"];
  return carry;
}
function o86(inp,carry){
  icarus_mem[Number(inp)] = registers["A"];
  return carry;
}
function o84(inp,carry){
  icarus_mem[Number(inp)] = registers["Y"];
  return carry;
}
function o69(inp,carry){
  inp%=256;
  registers["A"]+=inp+carry;
  if (registers["A"]>255){
    carry = 1;
  }
  else {
    carry = 0;
  }
  return carry;
}
var start = Date.now()/1000
for (var k = 0; k < 1000*1000; k++){
for (var i in icarus_pro){
  op = icarus_pro[i][0];
  carry = istr[op](icarus_pro[i][1],carry);
  tc+=1;
}
}
var end = Date.now()/1000
var t = end-start;
console.log(1/t * 1000000,"Cycles per second")
console.log(registers);
while (true){
}
