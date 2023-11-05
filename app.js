const { spawn } = require('child_process');

let bindStderr = false;
if (process.argv.includes('--bind-stderr')) {
    bindStderr = true;
}

const child = spawn('./writer');

if (bindStderr) {
    child.stderr.on('data', (data) => {
        console.log(`Received from stderr: ${data}`);
    });
}

console.log(`High water mark for the stderr of the process: ${child.stderr.readableHighWaterMark} bytes`);

child.on('exit', (code) => {
    console.log(`Child process exited with code ${code}`);
});
