// Simple build script to copy over files from the release folder

let cp = require("child_process");
let fs = require("fs-extra");
let os = require("os");
let path = require("path");

let playgroundRoot = __dirname;
let reveryRoot = path.join(playgroundRoot, "..");

let playgroundExampleSources = path.join(playgroundRoot, "sources");
let playgroundExampleHost = path.join(playgroundRoot, "host");

let reveryExampleSources = path.join(reveryRoot, "examples");

let getEsyPath = () => {
    let result = cp.execSync("where esy");
    let found = result.toString("utf8");

    let candidates = found.trim().split(os.EOL);
    return candidates[candidates.length - 1];
};

let esyPath = getEsyPath();
console.log("Esy path: " + esyPath);


let getBuildArtifactFolder = () => {
    let result = cp.spawnSync(esyPath, ["bash", "-c", "echo $cur__bin"], { cwd: reveryRoot });
    return result.stdout.toString("utf8").trim();
};

let artifactFolder = getBuildArtifactFolder();

console.log("Artifact folder: " + artifactFolder);

console.log(`Copying examples from ${reveryExampleSources} to ${playgroundExampleSources}...`);
fs.copySync(reveryExampleSources, playgroundExampleSources);
console.log("Examples copied.");

console.log("Copying artifacts...");
fs.copySync(artifactFolder, playgroundExampleHost);
console.log("Artifacts copied.");
