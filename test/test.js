const fs = require("fs");
const co = require("co");
const path = require("path");
var expect = require('chai').expect;

const addon = require('../build/Release/addon');

// const testDirctory = path.join(__dirname, "test");
// if (!fs.existsSync(testDirctory)) {
//     fs.mkdirSync(testDirctory);
// }
// fs.readdirSync(testDirctory).forEach(function (file) {
//     fs.unlinkSync(path.join(testDirctory, file));
// });

const jpgBuf = fs.readFileSync(path.join(__dirname, "test.jpg"));
const pngBuf = fs.readFileSync(path.join(__dirname, "test.png"));
const webpBuf = fs.readFileSync(path.join(__dirname, "test.webp"));

describe.only('TWebP Test', function () {
    it('N_WebPGetFeatures', function (done) {
        let webpInfo = addon.N_WebPGetFeatures(webpBuf, webpBuf.length);
        expect(webpInfo.width).to.be.equal(198);
        expect(webpInfo.height).to.be.equal(105);
        expect(webpInfo.has_alpha).to.be.equal(0);
        done();
    });

    it('N_WebPEncode JPEG', function (done) {
        let jpgToWebPBuffer = addon.N_WebPEncode(jpgBuf, jpgBuf.length);
        let webpInfo = addon.N_WebPGetFeatures(jpgToWebPBuffer, jpgToWebPBuffer.length);
        expect(webpInfo.width).to.be.equal(140);
        expect(webpInfo.height).to.be.equal(108);
        expect(webpInfo.has_alpha).to.be.equal(0);
        done();
    });

    it('N_WebPEncode PNG', function (done) {
        let pngToWebPBuffer = addon.N_WebPEncode(pngBuf, pngBuf.length);
        let webpInfo = addon.N_WebPGetFeatures(pngToWebPBuffer, pngToWebPBuffer.length);
        expect(webpInfo.width).to.be.equal(720);
        expect(webpInfo.height).to.be.equal(801);
        expect(webpInfo.has_alpha).to.be.equal(1);
        done();
    });

    it('N_WebPEncode Memory', function (done) {
        this.timeout(100 * 1000);
        process.stdout.write("Plase wait for memory leaking test: ");
        var before = (process.memoryUsage().rss / 1024 / 1024).toFixed(2);
        var testCount = 100;
        var processCounter = -10;
        for (let i = 0; i < testCount; i++) {
            addon.N_WebPEncode(pngBuf, pngBuf.length);
            if(i % (testCount / 10) == 0) {
                processCounter += 10;
                process.stdout.write(`${processCounter}% `);
                global.gc();
            }
        }
        global.gc();
        var after = (process.memoryUsage().rss / 1024 / 1024).toFixed(2);
        console.log(`\nbefore:${before}MB         after:${after}MB`);
        if (after - before > 10) return done(new Error("Memory leak!"));
        done();
    });

    it('N_WebPEncodeAsync PNG', function (done) {
        addon.N_WebPEncodeAsync(pngBuf, pngBuf.length, function (err, pngToWebPBuffer) {
            if (err) {
                return done(err);
            }
            let webpInfo = addon.N_WebPGetFeatures(pngToWebPBuffer, pngToWebPBuffer.length);
            expect(webpInfo.width).to.be.equal(720);
            expect(webpInfo.height).to.be.equal(801);
            expect(webpInfo.has_alpha).to.be.equal(1);
            done();
            global.gc();
        });
    });

    it('N_WebPEncodeAsync JPEG', function (done) {
        addon.N_WebPEncodeAsync(jpgBuf, jpgBuf.length, function (err, jpgToWebPBuffer) {
            if (err) {
                return done(err);
            }
            let webpInfo = addon.N_WebPGetFeatures(jpgToWebPBuffer, jpgToWebPBuffer.length);
            expect(webpInfo.width).to.be.equal(140);
            expect(webpInfo.height).to.be.equal(108);
            expect(webpInfo.has_alpha).to.be.equal(0);
            done();
        });
    });
})