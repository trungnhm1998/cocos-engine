const ps = require('path');
const fs = require('fs');
const gulp = require('gulp');
const babelify = require("babelify");
const browserify = require('browserify');
const source = require("vinyl-source-stream");
const buffer = require('vinyl-buffer');
const chalk = require('chalk').default;
const uglify = require('gulp-uglify');
const rename = require('gulp-rename');

const engineRoot = ps.join(__dirname, '..');

(async function bundleAdapter () {
    try {
        console.time('Bundle Adapter');
        await bundleNativeAdapter();
        await bundleMinigameAdapter();
        await bundleRuntimeAdapter();
        console.timeEnd('Bundle Adapter');
        process.exit(0);
    } catch (e) {
        console.error(e);
        process.exit(1);
    }
})();

function getPlatformsFromPath (path) {
    let platforms = fs.readdirSync(path);
    platforms = platforms.filter(p => !p.startsWith('.'));
    return platforms;
}

async function bundleNativeAdapter () {
    console.log(chalk.green('\nBundling native adapter'));
    // bundle engine-adapter.js
    const engineAdapterEntry = normalizePath(ps.join(engineRoot, 'platforms/native/engine/index.js'));
    const engineAdapterOutput = normalizePath(ps.join(engineRoot, 'bin/adapter/native/engine-adapter.js'));
    await bundle(engineAdapterEntry, engineAdapterOutput);

    // bundle web-adapter.js
    const webAdapterEntry = normalizePath(ps.join(engineRoot, 'platforms/native/builtin/index.js'));
    const webAdapterOutput = normalizePath(ps.join(engineRoot, 'bin/adapter/native/web-adapter.js'));
    await bundle(webAdapterEntry, webAdapterOutput);
}

async function bundleMinigameAdapter () {
    const platformsPath = ps.join(engineRoot, 'platforms/minigame/platforms');
    const platforms = getPlatformsFromPath(platformsPath);
    console.log(chalk.green(`\nBundling minigame platform adapters, including: ${platforms}`));

    for (let platform of platforms) {
        console.log(`handle platform: ${chalk.green(platform)}`);
        // bundle engine-adapter.js
        const engineEntry = normalizePath(ps.join(engineRoot, `platforms/minigame/platforms/${platform}/wrapper/engine/index.js`));
        const engineOutput = normalizePath(ps.join(engineRoot, `bin/adapter/minigame/${platform}/engine-adapter.js`));
        await bundle(engineEntry, engineOutput);

        // bundle web-adapter.js
        let builtinEntry = normalizePath(ps.join(engineRoot, `platforms/minigame/platforms/${platform}/wrapper/builtin/index.js`));
        if (platform === 'alipay' || platform === 'xiaomi') {
            builtinEntry = normalizePath(ps.join(engineRoot, `platforms/minigame/platforms/${platform}/wrapper/builtin.js`));
        }
        const builtinOutput = normalizePath(ps.join(engineRoot, `bin/adapter/minigame/${platform}/web-adapter.js`));
        await bundle(builtinEntry, builtinOutput);
    }
}

async function bundleRuntimeAdapter () {
    const platformsPath = ps.join(engineRoot, 'platforms/runtime/platforms');
    const platforms = getPlatformsFromPath(platformsPath);
    console.log(chalk.green(`\nBundling runtime platform adapters, including: ${platforms}`));
    for (let platform of platforms) {
        console.log(`handle platform: ${chalk.green(platform)}`);
        // bundle engine-adapter.js
        const engineEntry = normalizePath(ps.join(engineRoot, `platforms/runtime/platforms/${platform}/engine/index.js`));
        const engineOutput = normalizePath(ps.join(engineRoot, `bin/adapter/runtime/${platform}/engine-adapter.js`));
        await bundle(engineEntry, engineOutput);
    }
}


function normalizePath (path) {
    return path.replace(/\\/g, '/');
}

/**
 * Create bundle task
 * @param {string} src 
 * @param {string} dst 
 * @param {boolean} uglify 
 */
function createBundleTask (src, dst) {
    let targetFileName = ps.basename(dst);
    let targetFileNameMin = ps.basename(targetFileName, '.js') + '.min.js';
    dst = ps.dirname(dst);
    let bundler =  browserify(src);
    return bundler.transform(babelify, {presets: [require('@babel/preset-env')],
        plugins: [
            require('@babel/plugin-proposal-class-properties'),
            require('@babel/plugin-proposal-export-default-from')
        ]})
        .bundle()
        .pipe(source(targetFileName))
        .pipe(buffer())
        .pipe(gulp.dest(dst))
        .pipe(rename(targetFileNameMin)).pipe(uglify()).pipe(gulp.dest(dst));
}

/**
 * Build adapters
 * @param {string} entry 
 * @param {string} output 
 */
async function bundle (entry, output) {
    await new Promise((resolve) => {
        console.log(`Generate bundle: ${chalk.green(ps.basename(output))}`);
        createBundleTask(entry, output).on('end', resolve);
    });
}