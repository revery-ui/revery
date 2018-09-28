const fs = require("fs");

const testData = new Uint8Array(5);
testData[0] = 255;
testData[1] = 0;
testData[2] = 1;
testData[3] = 2;
testData[4] = 128;

fs.writeFileSync("binary.dat", new Buffer(testData));
