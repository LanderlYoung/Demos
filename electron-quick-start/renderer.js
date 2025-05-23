/**
 * This file is loaded via the <script> tag in the index.html file and will
 * be executed in the renderer process for that window. No Node.js APIs are
 * available in this process because `nodeIntegration` is turned off and
 * `contextIsolation` is turned on. Use the contextBridge API in `preload.js`
 * to expose Node.js functionality from the main process.
 */
document.getElementById("content_from_render").innerText = (() => {
  try {
    const myAddon = require("./build/Release/MyAddon.node");
    return `${__filename} addon pid:${myAddon.getpid()} js pid:${process.pid}`;
  } catch (e) {
    return `cannot load addon`;
  }
})();
