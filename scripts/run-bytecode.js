const child = require("child_process");
var receivedOutput = false;
var examples = child.spawn("esy", ["x","Examples.bc"]);

examples.stdout.pipe(process.stdout, {end: false});
examples.stderr.pipe(process.stderr, {end: false});

var counter = 0;
examples.stderr.on('data', () => {
  if (counter == 15) {
    var watcher = child.spawn("esy", ["b", "dune", "build", "--watch"]);
    watcher.stdout.pipe(process.stdout, {end: false});
    watcher.stderr.pipe(process.stderr, {end: false});
  }
  counter++;
});


