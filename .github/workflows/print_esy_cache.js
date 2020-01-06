const fs = require("fs");
const os = require("os");
const path = require("path");

const ESY_FOLDER = process.env.ESY__PREFIX
  ? process.env.ESY__PREFIX
  : path.join(os.homedir(), ".esy");
const esy3 = fs
  .readdirSync(ESY_FOLDER)
  .filter(name => name.length > 0 && name[0] === "3")
  .sort()
  .pop();
console.log(`::set-output name=esy_cache::${path.join(ESY_FOLDER, esy3, "i")}`);
