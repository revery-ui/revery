const path = require("path");
const cp = require("child_process");

const docsPath = cp.execSync(
  `esy @ci print"`
);
const relativeDocsPath = path.relative(process.cwd(), docsPath.toString());

console.log(`::set-output name=docs_path::${relativeDocsPath}`);
