const { performance } = require('perf_hooks');
const cp = require('child_process');
const fs = require('fs');

const { start, step, threads, block } = JSON.parse(fs.readFileSync(`../states/${process.argv[2]}.json`));
const miners = [];

var startTp = performance.now(), count = 0;
for (var i = 0; i < threads; i++)
	(id => {
		miners[id] = cp.spawn('./miner', [start + id * step, start + (id + 1) * step]);
		miners[id].result = [];
		miners[id].stdout.on('data', data => miners[id].result.push(data));
		miners[id].stderr.on('data', () => {
			if (++count % threads === 0) {
				var nowTp = performance.now();
				console.log(`count: ${count * 2e6}, speed: ${count * 2e6 * 86400 * 1000 / (nowTp - startTp)}/d.`);
			}
		});
	})(i);

(async () => {
	await Promise.all(miners.map(thread =>
		new Promise(resolve => thread.on('close', resolve)))
	);

	const result = miners.map(miner => Buffer.concat(miner.result)).join('');
	fs.writeFileSync('result', result);
	fs.writeFileSync(`../states/${process.argv[2]}.json`, JSON.stringify({
		start: start + block,
		step,
		threads,
		block
	}));
})();