// Simple build script to copy over files from the release folder

let cp = require("child_process");
let fs = require("fs-extra");
let os = require("os");
let path = require("path");

let playgroundRoot = __dirname
let playgroundSources = path.join(playgroundRoot, "src");
let reveryRoot = path.join(playgroundRoot, "..");
let playgroundBuild = path.join(playgroundRoot, "_playground");

let nodeModulesSrc = path.join(playgroundRoot, "node_modules", "monaco-editor");
let nodeModulesDest = path.join(playgroundBuild, "monaco-editor");

let playgroundExampleSources = path.join(playgroundBuild, "sources");
let playgroundExampleHost = path.join(playgroundBuild, "host");

let reveryExampleSources = path.join(reveryRoot, "examples");

let getEsyPath = () => {
    let result
    try {
        result = cp.execSync("where esy");
    } catch (error) {
        // some operating systems (unix) use `which` instead of `where`.
        if (error.toString().indexOf('not found') != -1) {
            result = cp.execSync("which esy");
        } else {
            console.error('Unable to find `esy` is it installed?');
            throw error;
        }
    }

    let found = result.toString("utf8");
    let candidates = found.trim().split(os.EOL);
    return candidates[candidates.length - 1];
};

let getCommit = () => {
    let result = cp.execSync("git rev-parse --short HEAD");
    return result.toString("utf8").trim();
}

let getVersion = () => {
    let packageJson = fs.readFileSync(path.join(reveryRoot, "package.json")).toString("utf8");
    return JSON.parse(packageJson).version;
}

let esyPath = getEsyPath();
let commitId = getCommit();
let version = getVersion();
console.log("Esy path: " + esyPath);
console.log("Commit id: " + commitId);
console.log("Version: " + version);

let getBuildArtifactFolder = () => {
    let test = cp.spawnSync(esyPath, ["x", "echo", "get installed sources"], { cwd: playgroundRoot });
    return path.join(playgroundRoot, "_build", "install", "default", "bin");
};

let replace = (str, val, newVal) => {
    return str.split(val).join(newVal);
};

let artifactFolder = getBuildArtifactFolder();

console.log("Artifact folder: " + artifactFolder);

console.log(`Copying sources from ${playgroundSources} to ${playgroundBuild}...`);
fs.copySync(playgroundSources, playgroundBuild);
console.log("Sources copied.");

console.log(`Copying examples from ${reveryExampleSources} to ${playgroundExampleSources}...`);
fs.copySync(reveryExampleSources, playgroundExampleSources);
console.log("Examples copied.");

console.log("Copying artifacts...");
// Remove destination to prevent a 'Source and destination must not be the same' error when re-building
fs.removeSync(playgroundExampleHost);
fs.copySync(artifactFolder, playgroundExampleHost);
console.log("Artifacts copied.");

console.log("Copying node_modules...");
fs.copySync(nodeModulesSrc, nodeModulesDest);
console.log("node_modules copied.");

console.log("Replacing constants in index.html");
let indexHtmlPath = path.join(playgroundBuild, "index.html");

let indexHtml = fs.readFileSync(indexHtmlPath).toString("utf8");
indexHtml = replace(indexHtml, "{#VERSION}", version);
indexHtml = replace(indexHtml, "{#COMMIT}", commitId);

fs.writeFileSync(indexHtmlPath, indexHtml);
console.log("Done!");
