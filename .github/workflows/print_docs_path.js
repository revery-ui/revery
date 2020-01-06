const path = require("path");
const cp = require("child_process");

const docsPath = cp.execSync(
  `esy @doc echo "#{self.target_dir / 'default' / '_doc' / '_html'}"`
);
const relativeDocsPath = path.relative(process.cwd(), docsPath.toString());

console.log(`::set-output name=docs_path::${relativeDocsPath}`);
