# NodeJS subprocess stderr backpressure demo
This quick demo stack provides a minimal example of the streams backpressure issue with the `stderr` stream of a child process spawned in NodeJS.

## Plain description of the problem
Suppose that a NodeJS application spawns one or more C-based applications as child processes using the `child_process.spawn()` method.  These child processes write data to `stderr` using the `fprintf(stderr)` method.  

If the NodeJS application does not bind a listener to the `child.stderr.on('data')` event, the contents of `stderr` will be buffered until they reach a maximum amount known as the [high watermark level](https://nodejs.org/api/stream.html#readablereadablehighwatermark).  Since the `stderr` is represented as a stream on the NodeJS side, this behaviour is inherited from the streams API.  In Node v20 at least, the default for this is 16kb.

In C apps using the `fprintf` method (but likely in many other stderr implementations across programming languages), once that stream buffer is full, no more contents can be written to `stderr`.  Consequently, the app will block at the `stderr` write that exhausted the parent Node app's buffer.  In essence, after writing 16kb of data, the child app hangs (blocks) indefinitely, "waiting" for a buffer  flush which will never occur.

## Solution
The solution is to simply bind an event handler to stderr such as:

``` 
    child.stderr.on('data', (data) => {
        console.log(`Received from stderr: ${data}`);
    });
```

Now the data is being 'handled' and won't accumulate.  The child process can continuously push data to `stderr` without issues.

## Demo (of the problem)
First, compile the C app running `gcc writer.c -o writer`.  Next, launch the node app using `node app.js`.  This action causes Node to start the C writer as a child process.  The C writer will write to both `stderr` and to a file.  `stderr` will eventually block once the Node buffer is full.  Since a write is done to a log file each time after a successful `stderr` write, you can see "how far along" the writes got by tailing the `./logs/log.txt` file (`tail logs/log.txt` in a new Terminal window).

On my machine configuration, the app will reach no more than 735 iterations before blocking indefinitely, since the data size of those 735 iterations is enough to fill the default buffer.

## Demo (of the solution)
Compile the C app if you haven't done so already, and then run the node app with:  `node app.js --bind-stderr`.  This will cause the Node app to add an appropriate event listener to the `stderr` stream of the child process, thus preventing the silent buffering of that `stderr` activity and the eventual blocking when that buffer becomes full.

When running in this configuration, your NodeJS will print the `stderr` being provided by the C app and it should run indefinitely without ever blocking, going well beyond the 735 iterations from before when there was no event handler attached:

```
Received from stderr: [1699165458] [14308] Wrote to stderr.
```
